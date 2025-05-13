#include <WiFi.h>
#include <HTTPClient.h>
#include <driver/i2s.h>

const char* ssid = "wifo_id";       
const char* password = "wifi_password";
const char* serverUrl = "http://ip_address/audio";  // Flask API URL

// INMP441 Microphone Pin Configuration (ESP32-S)
#define I2S_WS  15  // Word Select (L/R Clock)
#define I2S_SD  32  // Serial Data (Data In)
#define I2S_SCK 14  // Serial Clock (Bit Clock)

void setupI2S() {
    i2s_config_t i2s_config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
        .sample_rate = 16000, 
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
        .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
        .communication_format = I2S_COMM_FORMAT_I2S,
        .intr_alloc_flags = 0,
        .dma_buf_count = 8,
        .dma_buf_len = 1024,
        .use_apll = false,
        .tx_desc_auto_clear = false
    };

    i2s_pin_config_t pin_config = {
        .bck_io_num = I2S_SCK,  
        .ws_io_num = I2S_WS,    
        .data_out_num = I2S_PIN_NO_CHANGE, 
        .data_in_num = I2S_SD    
    };

    i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
    i2s_set_pin(I2S_NUM_0, &pin_config);
}

void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nConnected to WiFi!");

    setupI2S();
}

void loop() {
    uint8_t audio_buffer[1024];
    size_t bytes_read;
    
    // Read audio data from INMP441 microphone
    i2s_read(I2S_NUM_0, audio_buffer, sizeof(audio_buffer), &bytes_read, portMAX_DELAY);

    // Send audio data to Flask server via HTTP
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        http.begin(serverUrl);
        http.addHeader("Content-Type", "audio/wav");
        int httpResponseCode = http.POST(audio_buffer, bytes_read);
        Serial.println(httpResponseCode);
        http.end();
    }

    delay(5000);  // Send audio every 5 seconds
}

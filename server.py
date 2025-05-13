from flask import Flask, request
import soundfile as sf
import numpy as np

app = Flask(__name__)

@app.route('/audio', methods=['POST'])
def receive_audio():
    audio_data = request.data  # Get raw audio bytes
    filename = "received_audio.wav"

    # Save the audio data as a WAV file
    with open(filename, "wb") as f:
        f.write(audio_data)

    return "Audio received", 200

if __name__ == '__main__':
    app.run(host="0.0.0.0", port=5000, debug=True)

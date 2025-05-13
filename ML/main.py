
def MathChallenge(n):
    num = int(n)
    if num > 1:
    

        for i in range(2, (num//2)+1):
        
            if (num % i) == 0:
                return "false"
                break
        else:
            return "true"
    else:
        return "false"

print(MathChallenge(input()))
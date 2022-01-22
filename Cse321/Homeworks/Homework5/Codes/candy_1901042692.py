# The rod cutting problem
def maxObtainableValue(rodLength, prices):
    values = [0] * (rodLength+1)

    for i in range(1, rodLength+1):
        maxValue = -9999
        for j in range(0, i):
            maxValue = max(maxValue, prices[j]+values[i-j-1])
        values[i] = maxValue

    return values[rodLength]

rodLength = 8
prices = [1, 5, 8, 9, 10, 17, 17, 20]

print("Maximum Obtainable Value: ", maxObtainableValue(rodLength, prices))
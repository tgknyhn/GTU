def cluster_a(stations, profitRates):
    size          = stations.__len__()
    start         = 0
    end           = 0
    currentProfit = 0
    maxProfit     = 0

    for i in range(size-1):
        currentProfit = profitRates[i]
        for j in range(i+1, size):
            currentProfit += profitRates[j]
            if (currentProfit > maxProfit):
                start      = i
                end        = j
                maxProfit  = currentProfit

    return stations[start:end+1]

def cluster_b(profitRates, low, high):
    if (high <= low):
        return profitRates[low]

    mid = (low + high) // 2     # I used // instead of / to make it int division 

    leftMaxProfit = -9999999    # assumed minus infinity for int 
    currentProfit = 0

    for i in range(mid, low, -1):
        currentProfit += profitRates[i]
        if (currentProfit > leftMaxProfit):
            leftMaxProfit = currentProfit

    rightMaxProfit = -9999999    # assumed minus infinity for int
    currentProfit  = 0

    for i in range(mid+1, high):
        currentProfit += profitRates[i]
        if (currentProfit > rightMaxProfit):
            rightMaxProfit = currentProfit

    leftResult  = cluster_b(profitRates, low  , mid )
    rightResult = cluster_b(profitRates, mid+1, high)

    result = max(leftResult, rightResult, (leftMaxProfit + rightMaxProfit))

    return result


stations    = ['A', 'B', 'C', 'D', 'E', 'F', 'G']
profitRates = [3, -5, 2, 11, -8, 9, -5]
low         = 0
high        = stations.__len__() - 1 

print("Part a:\t", cluster_a(stations, profitRates))
print("Part b:\t", cluster_b(profitRates, low, high))
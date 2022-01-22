def meaningful(successRates, k):
    worstRate = min(successRates)
    bestRate  = max(successRates)

    size = len(successRates)

    while (worstRate <= bestRate):
        mean = (bestRate + worstRate)//2
        countLess  = 0
        countEqual = 0

        for i in range(0, size):
            if (successRates[i] < mean):
                countLess += 1
            elif (successRates[i] == mean):
                countEqual += 1
            if (countLess >= k):
                break

        if ( (countLess < k) and (countLess + countEqual >= k) ):
            return mean
        elif (countLess >= k):
            bestRate = mean - 1
        else:
            worstRate = mean + 1

    return -1


successRates = [2, 52, 77, 33, 55, 22, 1]
k = 5

print("The first meaningful success rate:", meaningful(successRates, k))
  
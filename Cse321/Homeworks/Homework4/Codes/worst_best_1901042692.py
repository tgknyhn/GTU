def worst(successRates, worstRate):
    if len(successRates) == 1:
        if successRates[0] < worstRate:
            return successRates[0]

    elif len(successRates) > 1:
        mid = len(successRates)//2
  
        L = successRates[:mid]
        R = successRates[mid:]
  
        return min(worst(L, worstRate), worst(R, worstRate))

def best(successRates, bestRate):
    if len(successRates) == 1:
        if successRates[0] > bestRate:
            return successRates[0]

    elif len(successRates) > 1:
        mid = len(successRates)//2
  
        L = successRates[:mid]
        R = successRates[mid:]
  
        return max(best(L, bestRate), best(R, bestRate))


successRates = [2, 5, 7, 3, 55, 22, 1]
worstRate    = 999
bestRate     = 0

print("Rates : ", successRates)
print("Worst : ", worst(successRates, worstRate))
print("Best  : ", best(successRates, bestRate))
  
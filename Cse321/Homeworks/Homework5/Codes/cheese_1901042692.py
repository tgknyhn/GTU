# Fractional Knapsack Problem
def maxPrice(prices, weights, maxWeight):
    size   = len(weights)
    price  = 0
    weight = 0

    # Assuming the weights and prices lists are sorted according to decreasing price/weight ratio
    for i in range(size):
        if (weight + weights[i] <= maxWeight):
            weight += weights[i]
            price  += prices[i]
            print("Current Weight:", int(weight), "\tCurrent Price :", int(price))
        else:
            remainRatio = (maxWeight - weight) / weights[i]
            weight += remainRatio * weights[i]
            price  += remainRatio * prices[i]
            print("Current Weight:", int(weight), "\tCurrent Price :", int(price))
            break

    return price

maxWeight = 120 
weights   = [15, 25, 50, 45]
prices    = [100, 150, 250, 200]

# Ratio  = 
# 1) 100 / 15 = 6.66
# 2) 150 / 25 = 6
# 3) 250 / 50 = 5 
# 4) 200 / 30 = 4.44

print("Maximum price is:", int(maxPrice(prices, weights, maxWeight)))
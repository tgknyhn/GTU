# Kadane's Algorithm
def maxProfit(profitRates):
    # Cluster Size
    size = len(profitRates)
    # Local maximum profit
    # We will use it while calculating profits for every index dynamicaly
    local_max_profit = 0
    # Global maximum profit
    # The return value & Actually the answer
    global_max_profit = -9999 # Thought it as minus infinity
    
    for i in range(size):
        local_max_profit = max(profitRates[i], profitRates[i] + local_max_profit)
        if (local_max_profit > global_max_profit):
            global_max_profit = local_max_profit

    return global_max_profit

profitRates = [3, -5, 2, 11, -8, 9, -5]
print("Maximum profit : ", maxProfit(profitRates))

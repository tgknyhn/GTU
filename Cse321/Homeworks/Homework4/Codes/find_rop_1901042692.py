def countOfROP(infoList):
    low  = 0
    high = len(infoList) - 1 
    count = divide(infoList, low, high) 
    return count

def divide(infoList, low, high):
    if (low >= high):
        return 0
    
    mid = low + ((high - low) // 2)

    left  = divide(infoList, low   , mid)
    right = divide(infoList, mid+1 , high) 

    merge = conquer(infoList, low, mid, high)

    return left + right + merge

def conquer(infoList, low, mid, high):
    tempLeft  = infoList[low:mid+1]
    tempRight = infoList[mid+1:high+1]

    i = 0; j = 0; k = low; pairs = 0
    leftLen  = len(tempLeft)
    rightLen = len(tempRight)

    while (i < leftLen and j < rightLen):
        if (tempLeft[i] <= tempRight[j]):
            infoList[k] = tempLeft[i]
            k += 1
            i += 1
        else:
            infoList[k] = tempRight[j]
            k += 1
            j += 1

            pairs += (mid + 1) - (low + i)

    while (i < leftLen):
        infoList[k] = tempLeft[i]
        k += 1
        i += 1

    while (j < rightLen):
        infoList[k] = tempRight[j]
        k += 1
        j += 1

    return pairs


infoList = [10, 9, 8, 7, 6, 5, 4, 3, 2, 1]
print("Number of reverse ordered pairs:", countOfROP(infoList))
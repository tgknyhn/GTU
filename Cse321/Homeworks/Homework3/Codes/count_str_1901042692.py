def count_str(str, firstLetter, lastLetter):
    size  = str.__len__()
    count = 0

    for i in range(size-1):
        for j in range((i+1), size):
            if (str[i] == firstLetter and str[j] == lastLetter):
                count += 1

    return count

str   = "TXZXXJZWX"
first = "X"
last  = "Z"

print(count_str(str, first, last))
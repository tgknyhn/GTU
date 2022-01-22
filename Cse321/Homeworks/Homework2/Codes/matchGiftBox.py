def matchGiftBox(box, gift):

    # Size of gift array
    giftSize = len(gift)

    # Base Case
    if giftSize < 2:
        return box
    
    # Partitioning
    for i in range(1, giftSize): 
        if box[i] == gift[0]:                   # No comparison between gift-gift or box-box
            box[i], box[0] = box[0], box[i]     # Swapping boxes
    
    # Recursion
    left = matchGiftBox(box[0:1], gift[0:1])                    #Matches the boxes to the left of pivot
    right = matchGiftBox(box[1:giftSize], gift[1:giftSize])     #Matches the boxes to the right of pivot

    # merging
    box = left + right 
    
    return box

# Example Arrays
box     = [8,4,5,2,1,6,7,3] # Will be matched
gift    = [1,2,3,4,5,6,7,8] # According to this


# Before matching
print("\nBefore Sorting")
print("--------------")
print("Box  :\t", box)
print("Gift :\t", gift)

# Matching
box = matchGiftBox(box, gift)

# After Matching
print("\nAfter Sorting")
print("-------------")
print("Box  :\t", box)
print("Gift :\t", gift)
def cutting(n):
    if (n == 0 or n == 1):
        return 0
    else:
        return cutting((n+1)//2) + 1

n = 10
print("Length of steel wire    : ", n)
print("Required number of cuts : ", cutting(n))
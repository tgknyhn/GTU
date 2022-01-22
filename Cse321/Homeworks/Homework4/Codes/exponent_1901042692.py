def bruteForceExp(a, n):
    result = 1
    
    for i in range(0, n):
        result = result * a

    return result

def divideAndConquerExp(a, n):
    if(n == 0):
        return 1    
    elif(n == 1):
        return a    

    left  = n//2
    right = n - (n//2) 

    return divideAndConquerExp(a, left) * divideAndConquerExp(a, right)


a = 4
n = 5

print("a:", a, "\nn:", n)
print("Brute Force        :",bruteForceExp(a, n))
print("Divide and Conquer :",divideAndConquerExp(a, n))
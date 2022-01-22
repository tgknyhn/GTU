def polynomial(coefficients, x):
    result   = coefficients[0]   # if polynomial is something like f(x) = a*x^0 , then just return first coefficient 
    currentX = 1                 # x^0 = 1

    for coefficient in coefficients[1:]:
        currentX = currentX * x
        result   = result + (currentX * coefficient)

    return result

coefficients = [4, 2, 5]        # Write last coefficient first. (4 is coefficient of x^0 and 5 is coefficient of x^2)
x            = 3

# f(x) = 5*(x^2) + 2*(x^1) + 4*(x^0)
#      = 5*9     + 2*3     + 4*1
#      = 45      + 6       + 4
#      = 55 

print(polynomial(coefficients, x))
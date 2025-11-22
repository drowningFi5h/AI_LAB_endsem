from sympy import symbols, Poly, GF, invert

# def find_multiplicative_inverse():
print("Find Multiplicative Inverse of a Polynomial")

# Define the field and input symbols
x = symbols('x')
field_char = int(input("Enter the field characteristic (e.g., 2 for GF(2)): "))
modulus_coeffs = input("Enter the modulus polynomial coefficients (space-separated, e.g., '1 0 1' for x^2 + 1): ").split()
modulus_coeffs = [int(c) for c in modulus_coeffs]
modulus = Poly(modulus_coeffs, x).as_expr()

poly_coeffs = input("Enter the polynomial coefficients (space-separated): ").split()
poly_coeffs = [int(c) for c in poly_coeffs]
polynomial = Poly(poly_coeffs, x).as_expr()

# Calculate the multiplicative inverse
try:
    inverse = invert(polynomial, modulus, domain=GF(field_char))
    print(f"Multiplicative inverse of {polynomial} mod {modulus} is: {inverse}")
except ValueError as e:
    print(f"Error: {e}. No inverse exists.")

# if __name__ == "_main_":
#     find_multiplicative_inverse()
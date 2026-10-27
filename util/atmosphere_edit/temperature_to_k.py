import numpy as np
from scipy.integrate import quad
from scipy.optimize import brentq

# Physical constants (SI)
#G = 6.67430e-11          # [m^3 kg^-1 s^-2]
#M_E = 5.9722e24          # [kg]
GM_E = 3.986004356e14
R_E = 6370.0             # [km]
k_B = 1.380649e-23       # [J K^-1]
#m = 28.9647 * 1.66053906660e-27  # Mean molecular mass [kg]
M_air = 28.964420e-3 # [kg/mol], JIS W 0201
N_A = 6.02214076e23
m = M_air / N_A


def temperature(k, T0, z0, z1):
	"""
	k:  exponential coefficient [km^-1]
	T0: temperature at z0 [K]
	z0: upper altitude [km]
	z1: lower altitude [km]
	"""
	dz = z0 - z1

	def integrand(x):
		r = (R_E + z1 + x) * 1000.0  # [m]
		return np.exp(-k * x) / r**2 * 1000.0

	integral, _ = quad(integrand, 0.0, dz)

	return (
		T0 * np.exp(-k * dz)
		+ m * GM_E / k_B * integral
	)


def solve_k(T0, T1, z0, z1):
	"""Find k satisfying T(z1; k) = T1."""

	if z0 <= z1:
		raise ValueError("z0 must be greater than z1.")
	if T0 <= 0 or T1 <= 0:
		raise ValueError("Temperatures must be positive.")

	def f(k):
		return temperature(k, T0, z0, z1) - T1

	# Expand the bracket until it contains the root.
	left, right = -0.1, 0.1

	while f(left) < 0:
		left *= 2

	while f(right) > 0:
		right *= 2

	return brentq(f, left, right, xtol=1e-12)


if __name__ == "__main__":

	for dT in [0, 5, -5, 10, -10]:
		# Input parameters
		T0 = 270.7  # [K]
		T1 = 264.2 + dT  # [K]
		z0 = 50.0   # [km]
		z1 = 45.0   # [km]

		k_e = solve_k(T0, T1, z0, z1)
		k_10 = k_e / np.log(10.0)

		print(f"k (base e)  = {k_e:.12f} km^-1")
		print(f"k (base 10) = {k_10:.12f} km^-1")
		print(f"T(z1)	   = {temperature(k_e, T0, z0, z1):.8f} K\n")

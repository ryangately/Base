/*
1/22/2016 Ryan Gately

BASE conversion class

The class defines a numerical base using a set of characters to represent each value.
*/
#include "stdafx.h"	// comment for non Windows builds
#include <string>
#include <vector>

using namespace std;

class base {
private:
	unsigned int baseNum;
	vector<char> values;

public:

	// implicit values for bases between 2 and 36
	base(int n) {
		if (n > 1 && n < 37) {
			baseNum = n;
			values = vector<char>(n);

			// fill default digits
			if (n > 10) {
				values[0] = '0';

				for (int i = 1; i < 10; ++i) {
					values[i] = values[i - 1] + 1;
				}

				// for values greater than 9, use english alphabet characters
				char fill = 'A';
				for (int i = 10; i < n; ++i) {
					values[i] = fill++;
				}
			}
			else {
				values[0] = '0';

				for (int i = 1; i < n; ++i) {
					values[i] = values[i - 1] + 1;
				}
			}
		}

		// if the base is more than 36, an explicit set of values must be provided
		else {
			baseMissingValuesException e;
			throw e;
		}
	}

	// preferred constructor, base is defined by a set of char values
	base(vector<char> v) {

		// TO DO: implement safety for vectors with duplicate values
		baseNum = v.size();
		values = v;
	}

	// exception if there aren't enough values to match the base
	struct baseMissingValuesException : std::exception {
		const char* what() const noexcept { return "A set of values must be provided for the base.\n"; }
	};

	// exception if the input can't be matched to the base
	struct baseMismatchException : std::exception {
		const char* what() const noexcept { return "The input does not match the base.\n"; }
	};

	// convert a string in the base to the equivalent decimal value
	unsigned long long baseToDecimal(string input) {

		unsigned long long result = 0;
		int power = input.size() - 1;

		// convert each place value to the decimal equivalent
		for (unsigned int i = 0; i < input.size(); ++i) {

			// find the matching value in the base's values set
			for (unsigned int j = 0; j <= values.size(); ++j) {
				if (input[i] == values[j]) {

					// add the calculated value to the final result and move to the next place
					result += j * (unsigned long long) pow(baseNum, power);
					break;
				}

				// if there is no matching value, the input is bad
				if (j == values.size()) {
					baseMismatchException e;
					throw e;
				}
			}

			--power;

		}

		return result;
	}

	// overloaded base to decimal
	unsigned long long decimal(string input) {
		return baseToDecimal(input);
	}

	// convert a decimal value to the base equivalent
	string decimalToBase(unsigned long long input) {

		string result = "";
		int power = 0;
		unsigned long long val = 1;
		int digitIndex = 0;
		bool placed = false;

		// if the input is zero, return the zero value
		if (!input) {
			return values[0] + result;
		}

		// find the largest place value
		while (input > val) {
			++power;
			val *= baseNum;
		}

		// decrement one power if needed
		if (val > input) {
			--power;
			val /= baseNum;
		}

		// if the input value is in the singles
		if (!power) {
			return values[(unsigned long)input] + result;
		}

		// reduce input and convert to values
		while (power) {

			// if the input is less than the current power value
			// reduce the power
			while (input < val && power) {
				--power;
				val /= baseNum;
				result.push_back(values[0]);
				digitIndex = 0;
				placed = true;
			}

			// reduce the input by the value and add the value to the result
			while (input >= val) {
				if (placed) {
					result[result.size() - 1] = values[++digitIndex];
				}
				else {
					result.push_back(values[1]);
					digitIndex = 1;
					placed = true;
				}
				input -= val;
			}
		}

		return result;
	}

	// overloaded decimal to base conversion
	string convert(unsigned long long input) {
		return decimalToBase(input);
	}

	// add two base values together
	string sum(string a, string b) {

		long long aDec = baseToDecimal(a);
		long long bDec = baseToDecimal(b);

		long long result = aDec + bDec;

		return decimalToBase(result);
	}

	// overloaded sum of two base values
	string add(string a, string b) {
		return sum(a, b);
	}

	// multiply two base values
	string product(string a, string b) {

		unsigned long long aDec = baseToDecimal(a);
		unsigned long long bDec = baseToDecimal(b);

		unsigned long long result = aDec * bDec;

		return decimalToBase(result);
	}

	// overloaded product of two base values
	string multi(string a, string b) {
		return product(a, b);
	}
};
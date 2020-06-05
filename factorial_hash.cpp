/**
 * \brief Cellcrypt Factorial Hash Challenge
 *
 * Copyright Felipe Bolsi
 */

/**
 * 1. Approach:
 *
 * - Challenge requires simple functions. I didn't see the need for classes or
 *   anything fancy;
 * - To achieve the solution of the challenge different tasks are required, and
 *   so, breaking all these tasks into function that perform specific operations
 *   are better. And function can be used later on (DRY);
 * - Challenge is math-related. Calculate the factorial is direct and simple.
 *   The breaking the number into digits parts that took me a bit more time to
 *   figure it out.
 *
 * 2. Correctness
 *
 * - First, problems specifies and clear interval. So made sure only valid
 *   values are allowed. Tested this individually;
 * - Since code has several functions, I tested each function individually as
 *   well. First factorial, then split into digits and last the sum of digits;
 * - Testing the factorial function I was worried with the upper and lower
 *   limits. factorial of 0 must be equals 1. That is correct. But when testing
 *   upper limit I checked that values is not being corrected calculates. Number
 *   is too big! Values greater than 65 are not calculated correctly.
 *
 * 3. Optimization
 *
 * - I watched for unsigned numbers, since we are only dealing with positive
 *   numbers;
 * - For factorial result used uint64_t type, since it is a type the stores
 *   greater numbers;
 * - If memory was really really an issue, the code could be reorganized to be a
 *   single functions to avoid some variables and copies, like the std::vector;
 * - And, of course, adjust code so memory is not a problem to solve factorial
 *   for big numbers;
 *
 *  * Obs: I would break the related function on its own library. but kept it
 *         all here for ease of reading.
 */

#include <iostream>
#include <vector>

/**
 * \brief Calculates the factorial of a number
 * \param num Number to calculate factorial
 * \return Factorial of given number
 */
uint64_t factorial(uint32_t num) {
  // TODO(felipe.bolsi): figure it out how to deal with really big numbers!
  //
  // I did a quick search and there are options, like C++ classes for this
  // purpose and a method of breaking the number into an array to store its
  // parts.
  uint64_t f = 1;

  for (uint32_t i = num; i > 0; --i) {
    f *= i;
  }

  return f;
}

/**
 * \brief Splits a numbert into its digits
 * \param num Number to split
 * \return Vector of with every digit of number given number
 */
std::vector<unsigned int> split_num_into_digits(uint64_t num) {
  std::vector<uint32_t> v;

  while (num > 0) {
    unsigned int digit = num % 10;
    v.push_back(digit);
    // std::cout << "Digit " << digit << std::endl;
    num /= 10;
  }

  return v;
}

/**
 * \brief Calculates the sum of digits of a number
 * \param num Number to calculate the sum
 * \return Sum of digits of given number
 */
unsigned int sum_of_digits(uint64_t num) {
  const std::vector<uint32_t> &v = split_num_into_digits(num);

  uint32_t sum = 0;
  for (const uint32_t d : v) {
    sum += d;
  }

  return sum;
}

/**
 * \brief Entry point of Factorial Hash Challenge
 * \return 0 on success; -1 on error
 */
int main() {
  const uint32_t kUpperBound = 2000;
  std::cout << "Enter a number within range [0," << kUpperBound << "]: ";

  uint32_t x = 0;
  std::cin >> x;

  if (x > kUpperBound) {
    std::cout << "Given number (" << x << ") is out of range [0," << kUpperBound
              << "]!" << std::endl;
    return -1;
  }

  uint64_t f = factorial(x);
  // std::cout << "Factorial of " << x << " = " << f << std::endl;

  uint32_t digit_sum = sum_of_digits(f);
  std::cout << "Sum of digits of factorial of " << x << " = " << digit_sum
            << std::endl;

  return 0;
}
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
 *   upper limit I checked that values were not being corrected calculates.
 *   Number is too big! Used used boost::multiprecision::mpz_int type to solve
 *   problem.
 *
 * 3. Optimization
 *
 * - I watched for unsigned numbers, since we are only dealing with positive
 *   numbers;
 * - For factorial result used boost::multiprecision::mpz_int type, since it
 *   stores greater numbers;
 * - If memory was really really an issue, the code could be reorganized to be a
 *   single functions to avoid some variables and copies, like the std::vector;
 *
 *  * Obs: I would break the related function on its own library. but kept it
 *         all here for ease of reading.
 *
 * New changes:
 * - Since logic changed, and its a bit more complex, now it makes sense to have
 *   a class to encapsulate data e control behavior over it;
 * - Created BigNumber class;
 * - BigNumber class encapsulates std::vector that keeps track of every decimal
 *   part
 * - Implemented operator*(), only one needed now, that performs multiplication;
 * - Implemented operator<<() -> it knows how to stream BigNumber;
 * - All other operator should be implemented;
 */

#include <iostream>
#include <vector>

class BigNum {
 public:
  BigNum() : big_num_() {}

  explicit BigNum(const std::string &num) : big_num_() {
    for (uint32_t i = 0; i < num.size(); ++i) {
      std::string c(1, num[i]);
      big_num_.push_back(std::atoi(c.c_str()));
    }
  }

  BigNum operator*(uint32_t num) {
    BigNum new_big_num(*this);

    uint32_t carry = 0;

    for (uint32_t i = 0; i < new_big_num.big_num_.size(); ++i) {
      uint32_t prod = new_big_num.big_num_[i] * num + carry;

      new_big_num.big_num_[i] = prod % 10;

      carry = prod / 10;
    }

    while (carry) {
      new_big_num.big_num_.push_back(carry % 10);
      carry = carry / 10;
    }

    return new_big_num;
  }

  const std::vector<uint32_t> &big_num_raw() const { return big_num_; }

  // TODO(felipe.bolsi) add other operators!

  friend std::ostream &operator<<(std::ostream &o, const BigNum &big_num);

 private:
  std::vector<uint32_t> big_num_;
};

/**
 * \brief Stream output BigNum
 * \return Stream output BigNum
 */
std::ostream &operator<<(std::ostream &o, const BigNum &big_num) {
  if (!big_num.big_num_.empty()) {
    for (int32_t i = (big_num.big_num_.size() - 1); i >= 0; --i) {
      o << big_num.big_num_[i];
    }
  }

  return o;
}

/**
 * \brief Calculates the factorial of a number
 * \param num Number to calculate factorial
 * \return Factorial of given number
 */
BigNum factorial(uint32_t num) {
  BigNum f("1");

  for (uint32_t i = num; i > 0; --i) {
    f = f * i;
  }

  return f;
}

/**
 * \brief Calculates the sum of digits of a number
 * \param big_num Number to calculate the sum
 * \return Sum of digits of given number
 */
uint32_t sum_of_digits(const BigNum &big_num) {
  uint32_t sum = 0;
  for (const uint32_t d : big_num.big_num_raw()) {
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

  BigNum f_big_num = factorial(x);
  std::cout << "Factorial of " << x << " = " << f_big_num << std::endl;

  uint32_t digit_sum = sum_of_digits(f_big_num);
  std::cout << "Sum of digits of factorial of " << x << " = " << digit_sum
            << std::endl;

  return 0;
}

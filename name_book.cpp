/**
 * \brief Cellcrypt Name Book Challenge
 *
 * Copyright Felipe Bolsi
 */

/**
 * 1. Approach:
 * - Problems was pretty simple. But a class was a good fit so data and
 *   behaviors related to NameBook was handled by itself;
 * - If a file with names is provided object NameBook can handle the input read;
 * - If no file with names is provided on object creation, object has a default
 *   constructor and names can be added in two flavors: by file name, or name by
 *   name;
 * - NameBook self validate its consistency via method IsConsistent()
 *   following the given rule;
 *     - IsConsistent() only cares if name BEGINS with the other name;
 *     - Others cases of substring were not considered;
 *
 * 2. Correctness
 * - Checked by ways of adding names:
 *     - Default construct and after read file;
 *     - Construct with file name;
 *     - Add name by name
 * - Verify different names input to check for problems
 *     - Names beginning with other names;
 *     - Names that have a substring of other names by not at position 0;
 *     - File with no names -> is consistent since there is no name violating
 *       the consistency rule;
 *  - A good unit test was needed. But I guess it is not the main goal here.
 *
 * 3. Optimization
 * - Decided to use std::vector. It is as class with low overhead over standard
 *   arrays;
 * - But, more importantly, std::vector allow random access, and at cost O(1);
 * - Decided to check name_1 against name_2 on the same loop. This way the
 *   number of time I had to iterate on vector is reduced; Loop inside loop is
 *   always something to watch for;
 *
 * Obs: I would break class on its own .cpp and .h file. but kept it all here
 *      for ease of reading.
 *
 * New changes:
 * - method IsConsistent() is executing for last element without any need, but
 *   to avoid that, we have to check if list is empty before loop. It would be
 *   changing one IF for another; Decide to keep it the way it is;
 * - IsConsistent() was kept for comparisons, because new approach does not need
 *   it;
 * - Code was changed to verify consistency of name list when name is
 *   being added to name list;
 * - To keep track of consistency state, a new attribute 'consistent_' was
 *   created;
 * - For this version, without the need of method IsConsistent(), std::vector is
 *   not mandatory, but still a good choice since it is fast and has
 *   little overhead. No need for a double linked list as std::list
 */

#include <fstream>
#include <iostream>
#include <vector>

/**
 * \brief NameBook class
 *
 * Stores, output data and check consistency of names
 */
class NameBook {
 public:
  /**
   * \brief Default constructor
   */
  NameBook() : name_list_(), consistent_(true) {}

  /**
   * \brief Constructor by file of names
   * \param file_name File with names (one by line)
   */
  explicit NameBook(const std::string &file_name)
      : name_list_(), consistent_(true) {
    ReadNames(file_name);
  }

  /**
   * \brief Destructor
   */
  ~NameBook() = default;

  /**
   * \brief Read names feom file of names
   * \param file_name File with names (one by line)
   */
  void ReadNames(const std::string &file_name) {
    std::fstream f;
    f.open(file_name);

    std::string name{};

    while (f >> name) {
      if (!CheckNameConsistency(name)) {
        consistent_ = false;
      }
      name_list_.push_back(name);
    }

    f.close();
  }

  /**
   * \brief Get the name list
   * \return Name list
   */
  const std::vector<std::string> &name_list() const { return name_list_; }

  /**
   * \brief Add name to name list
   * \param name Name to be added to name list
   */
  void AddName(const std::string &name) {
    if (!CheckNameConsistency(name)) {
      consistent_ = false;
    }

    name_list_.push_back(name);
  }

  /**
   * \brief Remove all names from name list
   */
  void ClearNames() {
    name_list_.clear();
    consistent_ = true;
  }

  /**
   * \brief Get list name as string (one by line)
   * \return List name as string
   */
  std::string to_string() const {
    std::string names;

    for (const auto &name : name_list_) {
      names += name + "\n";
    }

    return names;
  }

  /**
   * \brief Check is name list is consistent
   *
   * If no name begins with the same sequence of letters that makes up another
   * whole name
   *
   * \return true is name list is consistent; false otherwise
   */
  bool IsConsistent() const {
    for (uint32_t i = 0; i < name_list_.size(); ++i) {
      const std::string name_1(name_list_[i]);

      for (uint32_t j = i + 1; j < name_list_.size(); ++j) {
        const std::string name_2 = (name_list_[j]);

        if (CheckIfSubString(name_1, name_2)) {
          return false;
        }
      }
    }

    return true;
  }

  /**
   * \brief Check is name list is consistent
   *
   * If no name begins with the same sequence of letters that makes up another
   * whole name
   *
   * \return true is name list is consistent; false otherwise
   */
  bool consistent() const { return consistent_; }

  friend std::ostream &operator<<(std::ostream &o, const NameBook &name_book);

 private:
  /**
   * \brief Check if string are substring of each other
   * \param name_1 First name
   * \param name_2 Second name
   * \return true if substring; false otherwise
   */
  bool CheckIfSubString(const std::string name_1,
                        const std::string name_2) const {
    // check if name_2 is a substring of name_1
    uint32_t pos_1 = name_1.find(name_2);
    if (pos_1 != std::string::npos && pos_1 == 0) {
      return true;
    }

    // check if name_1 is a substring of name_2
    uint32_t pos_2 = name_2.find(name_1);
    if (pos_2 != std::string::npos && pos_2 == 0) {
      return true;
    }

    return false;
  }

  /**
   * \brief Check name consistency against name list
   * \param name_1 First name
   * \return true is name list is consistent with given name; false otherwise
   */
  bool CheckNameConsistency(const std::string name) {
    for (const auto &name_in_l : name_list_) {
      if (CheckIfSubString(name, name_in_l)) {
        return false;
      }
    }

    return true;
  }

  std::vector<std::string> name_list_;  //!< Name list
  bool consistent_;  //!< Flag to indicate if name list is consistent
};

/**
 * \brief Stream output NameBook (one by line)
 * \return Stream output NameBook
 */
std::ostream &operator<<(std::ostream &o, const NameBook &name_book) {
  for (const auto &name : name_book.name_list_) {
    o << name << "\n";
  }

  return o;
}

/**
 * \brief Entry point of Factorial Hash Challenge
 * \return 0 on success; -1 on error
 */
int main() {
  std::cout << "Enter file name with list of name: ";

  // since it was not clear how many names have to be handled, or when to stop
  // reading input, or even if list had to be checked every time one new name is
  // being added, I decided to read it all from a file.
  std::string file_name;
  std::cin >> file_name;

  // constructed with name
  // NameBook name_book{file_name};

  // default constructed and read file
  /*
     NameBook name_book;
     name_book.ReadNames(file_name);
  */

  // default constructed and add name by name

  NameBook name_book;
  std::fstream f;
  f.open(file_name);

  std::string name{};

  while (f >> name) {
    name_book.AddName(name);
    // std::cout << "Adding " << name << "; Name book is consistent? "
    //         << (name_book.consistent() ? "true" : "false") << std::endl;
  }
  std::cout << "Name book is consistent after loop? "
            << (name_book.consistent() ? "true" : "false") << std::endl;

  f.close();

  // std::cout << "Names read: " << std::endl << name_book;

  std::cout << "Name book is consistent? "
            << (name_book.IsConsistent() ? "true" : "false") << std::endl;

  return 0;
}

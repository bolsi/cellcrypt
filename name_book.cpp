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
  NameBook() : name_list_{} {}

  /**
   * \brief Constructor by file of names
   * \param file_name File with names (one by line)
   */
  explicit NameBook(const std::string &file_name) : name_list_{} {
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
  void AddName(const std::string &name) { name_list_.push_back(name); }

  /**
   * \brief Remove all names from name list
   */
  void ClearNames() { name_list_.clear(); }

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

        // check if name_2 is a substring of name_1
        uint32_t pos_1 = name_1.find(name_2);
        if (pos_1 != std::string::npos && pos_1 == 0) {
          return false;  // inconsistent! no need to go on
        }

        // check if name_1 is a substring of name_2
        uint32_t pos_2 = name_2.find(name_1);
        if (pos_2 != std::string::npos && pos_2 == 0) {
          return false;  // inconsistent! no need to go on
        }
      }
    }

    return true;
  }

  friend std::ostream &operator<<(std::ostream &o, NameBook name_book);

 private:
  std::vector<std::string> name_list_;
};

/**
 * \brief Stream output NameBook (one by line)
 * \return Stream output NameBook
 */
std::ostream &operator<<(std::ostream &o, NameBook name_book) {
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

  // since was not clear how many names have to be handled, or when to stop
  // reading input, or even if list had to be checked every time one new name is
  // being added, I decided to read it all from a file.
  std::string file_name;
  std::cin >> file_name;

  // constructed with name
  NameBook name_book{file_name};

  // default constructed and read file
  /*
     NameBook name_book;
     name_book.ReadNames(file_name);
  */

  // default constructed and add name by name
  /*
    NameBook name_book;
    std::fstream f;
    f.open(file_name);

    std::string name{};

    while (f >> name) {
      name_book.AddName(name);
    }

    f.close();
  */

  // std::cout << "Names read: " << std::endl << name_book;

  std::cout << "Name book is consistent? "
            << (name_book.IsConsistent() ? "true" : "false") << std::endl;

  return 0;
}

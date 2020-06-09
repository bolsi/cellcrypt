/**
 * \brief Cellcrypt Name Book Challenge based on tree
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
 * - NameBook encapsulates a tree structure where each node is a letter of the
 *    name; following the given rule;
 * - Consistency only cares if name BEGINS with the other name;
 * - Others cases of substring were not considered;
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
 *  - Names with exact same size are OK
 *  - Tested node and tree alone to check if data was being corrected added, and
 *    collision detected;
 *  - A good unit test was needed. But I guess it is not the main goal here.
 *
 * 3. Optimization
 * - As requested, new data structure to store the names is now a tree;
 * - New solution really detects faster a consistency problem with names;
 * - If there are several similar or equal names memory can be saved with this
 *   approach;
 *
 * Obs: I would break class on its own .cpp and .h file. but kept it all here
 *      for ease of reading.
 *
 * Obs1: This approach is more complex and error prone. A good unit test is
 * really needed;
 *
 * Obs2: if input names batch being validated every time is small, or all names
 * are really different among themselves, all the overhead structures for every
 * letter may consume more memory;
 */

#include <fstream>
#include <iostream>

/**
 * \brief Node class
 *
 * Node stores its data and all its possible children
 */
class Node {
 public:
  /**
   * \brief Constructor by data
   */
  explicit Node(char c) : data_(c), num_children_(0) {
    for (uint32_t i = 0; i < kMaxChildren; ++i) {
      children_[i] = nullptr;
    }
  }

  /**
   * \bfief Destructor
   */
  ~Node() {
    for (uint32_t i = 0; i < kMaxChildren; ++i) {
      if (children_[i] != nullptr) {
        delete children_[i];
      }
    }
  }

  /**
   * \brief Find child node based on data
   * \param c Data
   * \return true if child found; false otherwise
   */
  bool FindChild(char c) const {
    if (children_[IndexFromChar(c)] == nullptr) {
      return true;
    } else {
      return false;
    }
  }

  /**
   * \brief Get child node based on data
   * \param c Data
   * \return Child node
   */
  Node *GetChild(char c) const { return children_[IndexFromChar(c)]; }

  /**
   * \brief Add a child node to this node (it does not check collision!)
   * \brief node Child node
   */
  void AddChild(Node *node) {
    int32_t index = IndexFromChar(node->data_);
    children_[index] = node;
    ++num_children_;
  }

  /**
   * \brief Get the number of children of the node
   * \return Number of children
   */
  uint8_t num_children() const { return num_children_; }

 private:
  /**
   * \brief Get index from char
   * \return Index
   */
  int32_t IndexFromChar(char c) const {
    return c - 97;  // 97 is the value of 'a' in ASCII
  }

  const static uint8_t kMaxChildren =
      26;      //!< Max number of children (26 letter of alphabet)
  char data_;  //!< Data char stored
  Node *children_[kMaxChildren];  //!< Array of children nodes
  uint8_t num_children_;          //!< Number of children of this node
};

enum class TreeRetCode { kOK, KCollision };

/**
 * \brief WordTree class
 *
 * Creates a tree where each letter of word is a node
 */
class WordTree {
 public:
  /**
   * \brief Default constructor
   */
  WordTree() : root_('r') {}

  /**
   * \brief Add word to tree
   * \param word Word
   * \return TreeRetCode
   */
  TreeRetCode AddWord(const std::string &word);

 private:
  /**
   * \brief Add node to tree
   * \param c Data
   * \áram base_node Base (parent) node
   * \return Node added
   */
  Node *AddNode(char c, Node *base_node);

  Node root_;  //!< Root of word tree
};

Node *WordTree::AddNode(char c, Node *base_node) {
  Node *node = base_node->GetChild(c);

  if (node == nullptr) {
    node = new Node(c);
    base_node->AddChild(node);
  } else {
  }

  return node;
}

TreeRetCode WordTree::AddWord(const std::string &word) {
  TreeRetCode ret = TreeRetCode::kOK;

  Node *base_node = &root_;

  // get letter by letter
  for (uint32_t i = 0; i < word.size(); ++i) {
    Node *node = AddNode(word[i], base_node);

    if (node->num_children() > 0 && i < word.size()) {
      ret = TreeRetCode::KCollision;
    }

    base_node = node;
  }

  return ret;
}

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
  NameBook() : t_(), consistent_(true) {}

  /**
   * \brief Constructor by file of names
   * \param file_name File with names (one by line)
   */
  explicit NameBook(const std::string &file_name) : t_(), consistent_(true) {
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
      if (t_.AddWord(name) == TreeRetCode::KCollision) {
        consistent_ = false;
      }
    }

    f.close();
  }

  /**
   * \brief Add name to name list
   * \param name Name to be added to name list
   */
  void AddName(const std::string &name) {
    if (t_.AddWord(name) == TreeRetCode::KCollision) {
      consistent_ = false;
    }
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

 private:
  WordTree t_;
  bool consistent_;  //!< Flag to indicate if name list is consistent
};

/**
 * \brief Entry point of Factorial Hash Challenge
 * \return 0 on success; -1 on error
 */
int main() {
  std::cout << "Enter file name with list of name: ";

  // since it was not clear how many names have to be handled, or when to stop
  // reading input, or even if list had to be checked every time one new name
  // is being added, I decided to read it all from a file.
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

  return 0;
}

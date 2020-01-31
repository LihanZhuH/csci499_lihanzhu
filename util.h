#ifndef UTIL_H_
#define UTIL_H_

#include <string>
#include <vector>
#include <unordered_map>

// Contain a combination of argument
class Combination {
 public:
  Combination() {};
  ~Combination() {};

  // Add a flag. 'with_parameter' indicate whether it has parameter.
  // Return true if no error occurred.
  // Return false if a flag is duplicated.
  bool AddOption(const std::string &flag, bool with_parameter);
  
  // Check if this combination contains 'flag'
  // Return a pair where 'first' is whether the flag exists
  // and 'second' is whether the flag has a parameter
  std::pair<bool, bool> HasOption(const std::string &flag) const;

 private:
  // Map flag to 'with_parameter' boolean
  std::unordered_map<std::string, bool> flag_map_;
};

// Command line argument parser
// Calling Parse() after adding all combinations
// will returns data and combination number.
class CommandParser {
 public:
  CommandParser(int argc, const char *argv[]);
  ~CommandParser() {};

  // Add a combination with a unique id
  void AddCombination(Combination *comb, int id);
  
  // Parse the arguments.
  // Return the id of the corresponding combination
  int Parse();

  // Get the parameter after the flag
  const std::string GetArgument(const std::string &flag);

 private:
  // Number of arguments
  int num_arguments_;

  // A vector that stores all arguments
  std::vector<std::string> arguments_vec_;

  // A vector of Combination pointers
  std::unordered_map<int, Combination*> combination_map_;

  // A map from flag to parameter
  std::unordered_map<std::string, std::string> argument_map_;
};

#endif  // UTIL_H_
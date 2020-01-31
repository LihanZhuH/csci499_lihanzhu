#include "util.h"

using std::string;
using std::unordered_map;
using std::pair;

// ============== Implementation of Combination ==============
bool Combination::AddOption(const string &flag, bool with_parameter) {
  if (flag_map_.count(flag) > 0) {
    return false;  // Duplicated flag
  } else {
    flag_map_.insert({flag, with_parameter});
    return true;
  }
}

pair<bool, bool> Combination::HasOption(const string &flag) const {
  if (flag_map_.count(flag) == 0) {
    return std::make_pair(false, false);
  } else {
    return std::make_pair(true, flag_map_.at(flag));
  }
}

// ============== Implementation of CommandParser ==============
// CommandParser::CommandParser(int argc, const char *argv[]) {
//   num_arguments_ = argc - 1;
//   for (int i = 1; i < argc; ++i) {
//     arguments_vec_.push_back(argv[i]);
//   }
// }

// void CommandParser::AddCombination(Combination *comb, int id) {
//   combination_map_.at(id) = comb;
// }

// int CommandParser::Parse() {
//   // Loop through all combinations
//   unordered_map<int,Combination*>::iterator it;
//   for (it = combination_map_.begin(); it != combination_map_.end; ++it) {
//     pair<bool, bool> has_option;
//     bool expect_flag = true;  // Type of the next argument (flag/parameter)

//     // Loop through all arguments
//     for (int i = 0; i < arguments_vec_.size(); ++i) {
//       has_option = it->second->HasOption(arguments_vec_[i]);
//       if (expect_flag) {
//         if (has_option.first) {
//           expect_flag = !has_option.second;
//         }
//       } else {
//         expect_flag = !expect_flag;
//       }
//     }
//   }
// }
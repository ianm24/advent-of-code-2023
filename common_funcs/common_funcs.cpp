#include <string>
#include <vector>

// Reads delimited ints from a string
std::vector<int> readIntList(std::string line) {
  const std::string digits = "0123456789";
  std::vector<int> outNums;

  int numStartIdx = line.find_first_of(digits);
  int numEndIdx = 0;

  while (numStartIdx != -1) {
    numStartIdx += numEndIdx;

    // Find end of the number
    numEndIdx = line.substr(numStartIdx).find_first_not_of(digits);
    if (numEndIdx != std::string::npos) {
      numEndIdx += numStartIdx;
    } else {
      numEndIdx = line.length();
    }

    // Add number to list
    std::string num = line.substr(numStartIdx, numEndIdx - numStartIdx);
    outNums.push_back(stoi(num));

    // std::cout << stoi(num) << std::endl;

    numStartIdx = line.substr(numEndIdx).find_first_of(digits);
  }

  return outNums;
}

// Reads delimited long longs from a string
std::vector<long long> readLongLongsList(std::string line) {
  const std::string digits = "0123456789";
  std::vector<long long> outNums;

  int numStartIdx = line.find_first_of(digits);
  int numEndIdx = 0;

  while (numStartIdx != -1) {
    numStartIdx += numEndIdx;

    // Find end of the number
    numEndIdx = line.substr(numStartIdx).find_first_not_of(digits);
    if (numEndIdx != std::string::npos) {
      numEndIdx += numStartIdx;
    } else {
      numEndIdx = line.length();
    }

    // Add number to list
    std::string num = line.substr(numStartIdx, numEndIdx - numStartIdx);
    // std::cout << "|" << num << "|" << std::endl;
    outNums.push_back(stoll(num));

    numStartIdx = line.substr(numEndIdx).find_first_of(digits);
  }

  return outNums;
}
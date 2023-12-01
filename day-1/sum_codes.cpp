// Code by Ian Chandler McDowell
// Written: 1 Dec 2023

#include <fstream>
#include <iostream>
#include <map>
#include <string>

char getFirstDigitFromString(std::string line, bool reverse) {
  std::string digitStrings[] = {"one", "two",   "three", "four", "five",
                                "six", "seven", "eight", "nine"};
  std::map<std::string, int> digitMap = {
      {"one", 1}, {"two", 2},   {"three", 3}, {"four", 4}, {"five", 5},
      {"six", 6}, {"seven", 7}, {"eight", 8}, {"nine", 9}};

  char digit = 'a';

  if (!reverse) {
    // Start at the beginning of the line and find the first digit
    for (int i = 0; i < line.length(); i++) {
      digit = line[i];
      // Check if char is digit
      if (isdigit(digit)) {
        break;
      } else {
        // If char isn't digit, check if its part of digit string
        for (int j = 0; j < (sizeof(digitStrings) / sizeof(*digitStrings));
             j++) {
          // If first char matches, and digit isnt too long, and digits equal
          if (digit == digitStrings[j][0] &&
              i + int(digitStrings[j].length()) < line.length() &&
              line.substr(i, digitStrings[j].length()) == digitStrings[j]) {
            // Set digit based on char digit conversion of int
            digit = digitMap[digitStrings[j]] + '0';
            return digit;
          }
        }
      }
    }
  } else {
    // Start at the end of the line and find the first digit
    for (int i = line.length() - 1; i >= 0; i--) {
      digit = line[i];
      // Check if char is digit
      if (isdigit(digit)) {
        break;
      } else {
        // If char isn't digit, check if its part of digit string
        for (int j = 0; j < (sizeof(digitStrings) / sizeof(*digitStrings));
             j++) {
          // If last char matches, and digit isnt too long, and digits equal
          // Ensure length is int, not size_t otherwise negative nums underflow
          if (digit == digitStrings[j].back() &&
              i - int(digitStrings[j].length() - 1) > 0) {
            if (line.substr(i - (digitStrings[j].length() - 1),
                            digitStrings[j].length()) == digitStrings[j]) {
              // Set digit based on char digit conversion of int
              digit = digitMap[digitStrings[j]] + '0';
              // std::cout << "Got it reverse: " << digit << std::endl;
              return digit;
            }
          }
        }
      }
    }
  }

  return digit;
}

int main() {
  // Get data file name
  std::string dataFileName = "../data.txt";
  std::ifstream dataFile;

  // Instantiate variables for first and last digit and sum
  std::string digits = "ab";
  int sum = 0;

  // Open data file
  dataFile.open(dataFileName);

  // Ensure data file is open
  if (dataFile.is_open()) {
    // Get each line in the file
    std::string line;
    while (std::getline(dataFile, line)) {
      // Get first and last digits
      digits[0] = getFirstDigitFromString(line, false);
      digits[1] = getFirstDigitFromString(line, true);

      // Add the concatenated digits together and reset digits
      sum += std::stoi(digits);
      digits = "ab";
    }
  }

  // Print sum of codes and wait for input to end
  std::cout << sum << std::endl;
  std::cin.ignore();

  return 0;
}
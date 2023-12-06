// Code by Ian Chandler McDowell
// Written: 2 Dec 2023

#include <fstream>
#include <iostream>
#include <map>
#include <string>

char getHelp(std::string line) {
  char digit = 'a';

  return digit;
}

int main() {
  // Get data file name
  std::string dataFileName = "../data.txt";
  std::ifstream dataFile;

  // Instantiate variables

  // Open data file
  dataFile.open(dataFileName);

  // Ensure data file is open
  if (dataFile.is_open()) {
    // Get each line in the file
    std::string line;
    while (std::getline(dataFile, line)) {
      char a = getHelp(line);
    }
  }

  // Print sum of codes and wait for input to end
  std::cout << "Output" << std::endl;
  std::cin.ignore();

  return 0;
}
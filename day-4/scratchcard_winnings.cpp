// Code by Ian Chandler McDowell
// Written: 7 Dec 2023

#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

// Gets the number of points for a card
int getCardWinnings(std::string line) {
  const std::string digits = "0123456789";
  int points = 0;

  // Separate winning numbers from player numbers
  std::string winningNums =
      line.substr(line.find(':') + 1, line.find('|') - line.find(':') - 2);
  std::string playerNums = line.substr(line.find('|') + 1);

  // Get all the winning numbers
  std::vector<int> winNumList;
  int numStartIdx = winningNums.find_first_of(digits);
  int numEndIdx = 0;

  while (numStartIdx != -1) {
    numStartIdx += numEndIdx;

    // Find end of the number
    numEndIdx = winningNums.substr(numStartIdx).find_first_not_of(digits);
    if (numEndIdx != std::string::npos) {
      numEndIdx += numStartIdx;
    } else {
      numEndIdx = winningNums.length();
    }

    // Add number to list
    std::string winningNum =
        winningNums.substr(numStartIdx, numEndIdx - numStartIdx);
    winNumList.push_back(stoi(winningNum));

    numStartIdx = winningNums.substr(numEndIdx).find_first_of(digits);
  }

  // Count how many player numbers match winning numbers
  int matchingNumCount = 0;
  numStartIdx = playerNums.find_first_of(digits);
  numEndIdx = 0;

  while (numStartIdx != -1) {
    numStartIdx += numEndIdx;

    // Find end of the number
    numEndIdx = playerNums.substr(numStartIdx).find_first_not_of(digits);
    if (numEndIdx != std::string::npos) {
      numEndIdx += numStartIdx;
    } else {
      numEndIdx = playerNums.length();
    }

    // Check if number is a winner
    int playerNum =
        stoi(playerNums.substr(numStartIdx, numEndIdx - numStartIdx));
    for (int i = 0; i < winNumList.size(); i++) {
      if (playerNum == winNumList[i]) {
        matchingNumCount++;
        break;
      }
    }

    numStartIdx = playerNums.substr(numEndIdx).find_first_of(digits);
  }

  // Calculate points based on matching numbers
  points = pow(2, matchingNumCount - 1);
  return points;
}

int main() {
  // Get data file name
  std::string dataFileName = "../data.txt";
  std::ifstream dataFile;

  // Instantiate variables
  int cardSum = 0;

  // Open data file
  dataFile.open(dataFileName);

  // Ensure data file is open
  if (dataFile.is_open()) {
    // Get each line in the file
    std::string line;
    while (std::getline(dataFile, line)) {
      cardSum += getCardWinnings(line);
    }
  }

  // Print sum of codes and wait for input to end
  std::cout << cardSum << std::endl;
  std::cin.ignore();

  return 0;
}
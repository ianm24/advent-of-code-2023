// Code by Ian Chandler McDowell
// Written: 7 Dec 2023

#include <cmath>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

// Gets the number of matches for a card
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

  // Return number of matches
  return matchingNumCount;
}

// Based on the winnings = copies rule, gets the total amount of scratchcards
int getNumScratchcards(std::map<int, int> cardWinningsMap, int numCards) {
  int numScratchcards = 0;
  std::vector<int> cardsPerID(numCards, 1);

  // Go through each card
  for (int i = 1; i < numCards + 1; i++) {
    int numWins = cardWinningsMap[i];

    // Add one card to j-1 card for each instance of a i-1 card
    for (int j = i + 1; j <= i + numWins; j++) {
      cardsPerID[j - 1] += cardsPerID[i - 1];
    }
    numScratchcards += cardsPerID[i - 1];
  }

  return numScratchcards;
}

int main() {
  // Get data file name
  std::string dataFileName = "../data.txt";
  std::ifstream dataFile;

  // Instantiate variables
  int cardSum = 0;
  std::map<int, int> cardWinningsMap;
  int cardID = 1;

  // Open data file
  dataFile.open(dataFileName);

  // Ensure data file is open
  if (dataFile.is_open()) {
    // Get each line in the file
    std::string line;
    while (std::getline(dataFile, line)) {
      // First pass calculates number of matches and points for each card
      int matchingNumCount = getCardWinnings(line);
      cardSum += pow(2, matchingNumCount - 1);
      cardWinningsMap.insert(std::pair<int, int>(cardID, matchingNumCount));
      cardID++;
    }
  }

  int numScratchcards = getNumScratchcards(cardWinningsMap, cardID - 1);

  // Print sum of codes and wait for input to end
  std::cout << "Card Points Sum:" << cardSum
            << "\nNumber of original and copied scratchcards:"
            << numScratchcards << std::endl;
  std::cin.ignore();

  return 0;
}
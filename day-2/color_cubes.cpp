// Code by Ian Chandler McDowell
// Written: 2 Dec 2023

#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

// Gets the max red, green, blue cubes from the sets
int* parseGameSets(std::vector<std::string> sets) {
  int* maxCubes = new int[3]{0, 0, 0};

  for (int i = 0; i < sets.size(); i++) {
    // Get first delimeter
    int prevSetIdx = 0;
    int nextSetIdx = sets[i].find(',');
    int currNum = 0;
    std::string currCubeCount;

    // Get all but last set
    while (nextSetIdx != std::string::npos && nextSetIdx != 0) {
      currCubeCount = sets[i].substr(prevSetIdx + 1, nextSetIdx);

      // Get the number associated
      currNum = std::stoi(currCubeCount.substr(0, currCubeCount.find(' ')));
      // std::cout << maxCubes[0] << " " << maxCubes[1] << " " << maxCubes[2]
      //           << std::endl;

      // Get the color of cube and check if max
      if (currCubeCount.find('d') != std::string::npos &&
          currNum > maxCubes[0]) {  // If red
        maxCubes[0] = currNum;
      } else if (currCubeCount.find('g') != std::string::npos &&
                 currNum > maxCubes[1]) {  // If green
        maxCubes[1] = currNum;
      } else if (currCubeCount.find('b') != std::string::npos &&
                 currNum > maxCubes[2]) {  // If blue
        maxCubes[2] = currNum;
      }

      prevSetIdx += nextSetIdx + 1;
      nextSetIdx = sets[i].substr(prevSetIdx).find(',');
    }

    // Last set
    // Get the number associated
    currCubeCount = sets[i].substr(prevSetIdx + 1);
    currNum = std::stoi(currCubeCount.substr(0, currCubeCount.find(' ')));
    // std::cout << maxCubes[0] << " " << maxCubes[1] << " " << maxCubes[2]
    //           << std::endl;

    // Get the color of cube and check if max
    if (currCubeCount.find('d') != std::string::npos &&
        currNum > maxCubes[0]) {  // If red
      maxCubes[0] = currNum;
    } else if (currCubeCount.find('g') != std::string::npos &&
               currNum > maxCubes[1]) {  // If green
      maxCubes[1] = currNum;
    } else if (currCubeCount.find('b') != std::string::npos &&
               currNum > maxCubes[2]) {  // If blue
      maxCubes[2] = currNum;
    }
  }

  return maxCubes;
}

// Gets a vector of strings, each representing one set of cubes
std::vector<std::string> getGameSets(std::string rawSets) {
  std::vector<std::string> sets;

  // Get first delimeter
  int nextSetIdx = rawSets.find(';');

  // Get all but last set
  while (nextSetIdx != std::string::npos && nextSetIdx != 0) {
    sets.push_back(rawSets.substr(0, nextSetIdx));
    rawSets = rawSets.substr(nextSetIdx + 1);

    nextSetIdx = rawSets.find(';');
  }

  // Get last set
  sets.push_back(rawSets);

  return sets;
}

// Gets the [gameNum, maxRed, maxGreen, maxBlue] for a game
int* parseGame(std::string line) {
  int* gameInfo = new int[4]{0, 0, 0, 0};

  // Get game number
  std::string gameID = line.substr(0, line.find(':'));
  gameInfo[0] = std::stoi(gameID.substr(gameID.find(' ')));

  // Get list of cube sets
  std::string rawSets = line.substr(line.find(':') + 1);
  std::vector<std::string> sets;

  sets = getGameSets(rawSets);
  int* maxCount = parseGameSets(sets);

  gameInfo[1] = maxCount[0];
  gameInfo[2] = maxCount[1];
  gameInfo[3] = maxCount[2];

  return gameInfo;
}

int main() {
  // Get data file name
  std::string dataFileName = "../data.txt";
  std::ifstream dataFile;

  // Instantiate variables
  const int maxCubes[] = {12, 13, 14};  // Red, Green, Blue cubes
  int gameSum = 0;

  // Open data file
  dataFile.open(dataFileName);

  // Ensure data file is open
  if (dataFile.is_open()) {
    // Get each line in the file
    std::string line;
    while (std::getline(dataFile, line)) {
      // Parse each game
      // std::cout << line << std::endl;
      int* currGame = parseGame(line);

      // std::cout << currGame[0] << " " << currGame[1] << " " << currGame[2]
      //           << " " << currGame[3] << std::endl;

      if (currGame[1] <= maxCubes[0] && currGame[2] <= maxCubes[1] &&
          currGame[3] <= maxCubes[2]) {
        gameSum += currGame[0];
      }
    }
  }

  // Print sum of codes and wait for input to end
  std::cout << gameSum << std::endl;
  std::cin.ignore();

  return 0;
}
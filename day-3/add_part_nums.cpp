// Code by Ian Chandler McDowell
// Written: 6-7 Dec 2023

#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

// Returns a map with every part number and its starting index in the schematic
std::map<int, std::string> getPartNumMap(std::vector<std::string> schematic) {
  std::string digits = "0123456789";

  // Schematic dimensions
  int numRows = schematic.size();
  int numCols = schematic[0].size();

  std::map<int, std::string> partNumMap;

  // Go through schematic and mark all part numbers and their locations
  for (int i = 0; i < numRows; i++) {
    std::string row = schematic[i];

    // Find the first part number in the row
    int partNumStartIdx = schematic[i].substr(0).find_first_of(digits);

    while (partNumStartIdx != std::string::npos) {
      // Find index at the end of the part number
      int partNumEndIdx = row.substr(partNumStartIdx).find_first_not_of(digits);
      if (partNumEndIdx == std::string::npos) {
        partNumEndIdx = row.length();
      } else {
        partNumEndIdx += partNumStartIdx;
      }

      // Calculate the map index and crop the part number to insert into map
      int mapIdx = numCols * i + partNumStartIdx;
      std::string partNum =
          row.substr(partNumStartIdx, partNumEndIdx - partNumStartIdx);

      partNumMap.insert(std::pair<int, std::string>(mapIdx, partNum));

      // Search for new part numbers later in the row
      partNumStartIdx = row.substr(partNumEndIdx).find_first_of(digits);
      if (partNumStartIdx != std::string::npos) {
        partNumStartIdx += partNumEndIdx;
      }
    }
  }

  return partNumMap;
}

// Checks adjacent spaces for valid part numbers.
// Returns gear ratio if applicable.
int checkAdjacentSpaces(std::vector<std::string> schematic,
                        std::map<int, std::string> &partNumMap,
                        std::vector<int> &adjPartNums, int symbolIdx) {
  // Schematic dimensions
  int numRows = schematic.size();
  int numCols = schematic[0].size();

  // Keep track of gearRatio
  int gearRatio = 0;
  int adjNumCount = 0;
  bool isGear = schematic[symbolIdx / numCols][symbolIdx % numCols] == '*';

  // Adjacent map indices are just the current index added to these
  const int *adjacentSpaces =
      new int[8]{-(numCols + 1), -numCols, -(numCols - 1), -1, 1,
                 numCols - 1,    numCols,  numCols + 1};
  const int numAdjSpaces = 8;

  // Ensure 1 symbol doesnt add the same number twice
  int prevNumAddedIdx = -1;

  // Check each adjacent space
  for (int j = 0; j < numAdjSpaces; j++) {
    int currMapIdx = symbolIdx + adjacentSpaces[j];

    // Ignore spaces outside schematic
    bool aboveTop = (currMapIdx < 0);
    bool belowBottom = (currMapIdx > (numRows * numCols) - 1);
    bool pastLeft = (symbolIdx % numCols == 0 &&
                     ((currMapIdx == symbolIdx - 1) ||
                      (currMapIdx == symbolIdx + numCols - 1) ||
                      (currMapIdx == symbolIdx - (numCols + 1))));
    bool pastRight = (symbolIdx % numCols == numCols - 1 &&
                      ((currMapIdx == symbolIdx + 1) ||
                       (currMapIdx == symbolIdx - (numCols - 1)) ||
                       (currMapIdx == symbolIdx + numCols + 1)));

    if (aboveTop || belowBottom || pastLeft || pastRight) {
      continue;
    }

    // Go through the map of part numbers
    std::string pNum = "";
    for (std::map<int, std::string>::iterator iter = partNumMap.begin();
         iter != partNumMap.end(); iter++) {
      int mapIdx = iter->first;

      if (currMapIdx >= mapIdx &&
          currMapIdx < mapIdx + (iter->second).length() &&
          mapIdx != prevNumAddedIdx) {
        // If part number is adjacent and not already marked, mark as such
        pNum = iter->second;
        adjPartNums.push_back(stoi(pNum));
        prevNumAddedIdx = mapIdx;

        // Keep track of gear ratio
        if (isGear && adjNumCount == 0) {
          gearRatio += stoi(pNum);
          adjNumCount++;
        } else if (isGear && adjNumCount == 1) {
          gearRatio *= stoi(pNum);
          adjNumCount++;
        } else {
          adjNumCount++;
        }
        break;
      }
    }
  }

  // If this is a valid gear, return its ratio for sum otherwise 0
  if (adjNumCount != 2) {
    isGear = false;
  }
  if (isGear) {
    return gearRatio;
  }
  return 0;
}

// Gets a vector containing all part numbers adjacent to a non-'.' symbol
int *getAdjacentPartNums(std::vector<std::string> schematic,
                         std::map<int, std::string> partNumMap) {
  std::string nonSpecialSymbols = "0123456789.";
  std::vector<int> adjPartNums;

  // Schematic dimensions
  int numRows = schematic.size();
  int numCols = schematic[0].size();

  int gearRatioSum = 0;
  int partNumSum = 0;

  // For each row
  for (int i = 0; i < schematic.size(); i++) {
    int prevSymbolIdx = 0;
    int symbolIdx = schematic[i].find_first_not_of(nonSpecialSymbols);
    while (symbolIdx != std::string::npos) {
      //   Get map index and search for part numbers in adjacent spaces
      int symbolMapIdx = numCols * i + symbolIdx;

      gearRatioSum +=
          checkAdjacentSpaces(schematic, partNumMap, adjPartNums, symbolMapIdx);

      // Look for next symbol in the row
      prevSymbolIdx = symbolIdx + 1;
      symbolIdx = schematic[i]
                      .substr(prevSymbolIdx)
                      .find_first_not_of(nonSpecialSymbols);

      if (symbolIdx != std::string::npos) {
        symbolIdx += prevSymbolIdx;
      }
    }
  }

  for (int i = 0; i < adjPartNums.size(); i++) {
    partNumSum += adjPartNums[i];
  }

  return new int[2]{partNumSum, gearRatioSum};
}

// Gets the sum of valid part numbers from the schematic
int *readSchematic(std::vector<std::string> schematic) {
  int partNumSum = 0;

  std::map<int, std::string> partNumMap = getPartNumMap(schematic);
  int *partNumSums = getAdjacentPartNums(schematic, partNumMap);

  return partNumSums;
}

int main() {
  // Get data file name
  std::string dataFileName = "../data.txt";
  std::ifstream dataFile;

  // Instantiate variables
  int *partNumSums = new int[2]{0, 0};
  std::vector<std::string> schematic;

  // Open data file
  dataFile.open(dataFileName);

  // Ensure data file is open
  if (dataFile.is_open()) {
    // Get each line in the file
    std::string line;

    while (std::getline(dataFile, line)) {
      schematic.push_back(line);
    }
  }

  partNumSums = readSchematic(schematic);

  // Print sum of codes and wait for input to end
  std::cout << "Sum of Symbol-Adjacent Part Numbers:" << partNumSums[0]
            << "\nSum of Valid Gear Ratios: " << partNumSums[1] << std::endl;
  std::cin.ignore();

  return 0;
}
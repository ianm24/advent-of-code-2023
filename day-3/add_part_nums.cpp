// Code by Ian Chandler McDowell
// Written: 2 Dec 2023

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

      // std::cout << "Found Number:" << mapIdx << "|" << partNum << "|"
      //           << partNumEndIdx << "|" << partNumStartIdx << "|"
      //           << row.substr(partNumEndIdx) << "|" << std::endl;
    }
  }

  return partNumMap;
}

// Checks adjacent spaces and adds valid part numbers
void checkAdjacentSpaces(std::vector<std::string> schematic,
                         std::map<int, std::string> &partNumMap,
                         std::vector<int> &adjPartNums, int symbolIdx) {
  // Schematic dimensions
  int numRows = schematic.size();
  int numCols = schematic[0].size();

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

    // std::cout << "before: " << symbolIdx << "|" << currMapIdx << std::endl;

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

    // std::cout << "Valid Adjacent Space: " << symbolIdx << "|" << currMapIdx
    //           << std::endl;

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
        // std::cout << "Found Adjacent Number:" << currMapIdx << "|" << mapIdx
        //           << "|" << pNum << "|" << std::endl;
        adjPartNums.push_back(stoi(pNum));
        prevNumAddedIdx = mapIdx;
        break;
      }
    }

    // TODO If pNum added to list, remove from map
    // if (pNum != "") {
    //  partNumMap.erase(pNum);
    //  pNum = "";
    //}
  }
}

// Gets a vector containing all part numbers adjacent to a non-'.' symbol
std::vector<int> getAdjacentPartNums(std::vector<std::string> schematic,
                                     std::map<int, std::string> partNumMap) {
  std::string nonSpecialSymbols = "0123456789.";
  std::vector<int> adjPartNums;

  // Schematic dimensions
  int numRows = schematic.size();
  int numCols = schematic[0].size();

  // For each row
  for (int i = 0; i < schematic.size(); i++) {
    int prevSymbolIdx = 0;
    int symbolIdx = schematic[i].find_first_not_of(nonSpecialSymbols);
    while (symbolIdx != std::string::npos) {
      //   Get map index and search for part numbers in adjacent spaces
      int symbolMapIdx = numCols * i + symbolIdx;
      // std::cout << "Found Symbol:" << symbolMapIdx << "|"
      //           << schematic[i][symbolIdx] << std::endl;
      checkAdjacentSpaces(schematic, partNumMap, adjPartNums, symbolMapIdx);

      // Look for next symbol in the row
      prevSymbolIdx = symbolIdx + 1;
      symbolIdx = schematic[i]
                      .substr(prevSymbolIdx)
                      .find_first_not_of(nonSpecialSymbols);
      // std::cout << "symbolIdx:" << symbolIdx << std::endl;

      if (symbolIdx != std::string::npos) {
        symbolIdx += prevSymbolIdx;
      }
    }
  }

  return adjPartNums;
}

// Gets the sum of valid part numbers from the schematic
int readSchematic(std::vector<std::string> schematic) {
  int partNumSum = 0;

  std::cout << "1: Starting number parsing" << std::endl;
  std::map<int, std::string> partNumMap = getPartNumMap(schematic);
  std::cout << "2, Starting symbol finding:" << partNumMap.size() << std::endl;
  std::vector<int> adjPartNums = getAdjacentPartNums(schematic, partNumMap);
  std::cout << "3, doing sum:" << adjPartNums.size() << std::endl;
  for (int i = 0; i < adjPartNums.size(); i++) {
    partNumSum += adjPartNums[i];
  }

  return partNumSum;
}

int main() {
  // Get data file name
  std::string dataFileName = "../data.txt";
  std::ifstream dataFile;

  // Instantiate variables
  int partSum = 0;
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

  partSum = readSchematic(schematic);

  // Print sum of codes and wait for input to end
  std::cout << partSum << std::endl;
  std::cin.ignore();

  return 0;
}
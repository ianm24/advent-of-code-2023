// Code by Ian Chandler McDowell
// Written: 8 Dec 2023

#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "../common_funcs/common_funcs.cpp"

// Returns the index mappings for a given map
std::vector<std::vector<long long>> readMap(std::vector<std::string> mapDef) {
  std::vector<std::vector<long long>> outMap;

  // std::cout << "New Map:" << std::endl;

  for (int i = 0; i < mapDef.size(); i++) {
    //  Parameters of mapping are: <dest_range_start,src_range_start,range_len>
    outMap.push_back(readLongLongsList(mapDef[i]));
  }

  return outMap;
}

// Reads the almanac and gets the seed-to-location mapping for each seed
std::map<long long, long long> readAlmanac(std::vector<std::string> almanac) {
  std::map<long long, long long> seedToLocation;

  // Get list of seeds
  std::string seedsList = almanac[0].substr(almanac[0].find(':') + 1);
  std::vector<long long> seeds = readLongLongsList(seedsList);

  // Get each map
  std::vector<std::vector<std::vector<long long>>> allMaps;
  std::vector<std::string> currMapDef;
  int mapDefStartIdx = 0;

  // Ensure that the last mapping is recorded
  almanac.push_back("");

  for (int i = 1; i < almanac.size(); i++) {
    // Empty line indicates start of new mapping
    if (almanac[i] == "") {
      // Add previous mapping to list of maps and clear the current map
      if (mapDefStartIdx != 0) {
        allMaps.push_back(readMap(currMapDef));
        currMapDef.clear();
      }
      // Move forward to the start of the map definition
      mapDefStartIdx = i + 2;
      i++;
      continue;
    } else {
      // Add the current line to the map definition
      currMapDef.push_back(almanac[i]);
    }
  }

  // Mapping parameters
  const int dest_range_start = 0;
  const int src_range_start = 1;
  const int range_len = 2;

  // Generate seed to location map
  for (int i = 0; i < seeds.size(); i++) {  // For each seed
    long long seedIdx = seeds[i];
    // Follow the mapping-path through each map (last map is location)
    for (int j = 0; j < allMaps.size(); j++) {  // For each type of map
      bool foundLocalMapping = false;

      std::vector<std::vector<long long>> currMap = allMaps[j];
      // For each map rule in the mapping
      for (int k = 0; k < currMap.size(); k++) {
        std::vector<long long> mapParams = currMap[k];

        // If the index is within the mapping range
        if (seedIdx >= mapParams[src_range_start] &&
            seedIdx < mapParams[src_range_start] + mapParams[range_len]) {
          // Gets offset from the source range start and apply to destination
          long long offset = seedIdx - mapParams[src_range_start];
          seedIdx = mapParams[dest_range_start] + offset;

          foundLocalMapping = true;
          break;
        }
      }

      // If the mapping is not defined, it is one-to-one
      if (!foundLocalMapping) {
        continue;
      }
    }
    // Insert seed to location mapping
    seedToLocation.insert(std::pair<long long, long long>(seeds[i], seedIdx));
    // std::cout << seeds[i] << "|" << seedIdx << "|" << std::endl;
  }

  // Return seed to location map
  return seedToLocation;
}

int main() {
  // Get data file name
  std::string dataFileName = "../data.txt";
  std::ifstream dataFile;

  // Instantiate variables
  std::vector<std::string> almanac;
  long long minDist = std::numeric_limits<long long>::max();

  // Open data file
  dataFile.open(dataFileName);

  // Ensure data file is open
  if (dataFile.is_open()) {
    // Get each line in the file
    std::string line;
    while (std::getline(dataFile, line)) {
      almanac.push_back(line);
    }
  }

  std::map<long long, long long> seedToLocation = readAlmanac(almanac);
  for (std::map<long long, long long>::iterator iter = seedToLocation.begin();
       iter != seedToLocation.end(); iter++) {
    long long location = iter->second;
    if (location < minDist) {
      minDist = location;
    }
  }

  // Print sum of codes and wait for input to end
  std::cout << "Closest Location of a seed: " << minDist << std::endl;
  std::cin.ignore();

  return 0;
}
// Code by Ian Chandler McDowell
// Written: 1 Dec 2023


#include <fstream>
#include <iostream>
#include <string>


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

			// Start at the beginning of the line and find the first digit
			for (int i = 0; i < line.length(); i++) {
				digits[0] = line[i];
				if (isdigit(digits[0])) {
					break;
				}
			}
			// Start at the end of the line and find the first digit
			for (int i = line.length() - 1; i >= 0; i--) {
				digits[1] = line[i];
				if (isdigit(digits[1])) {
					break;
				}
			}

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
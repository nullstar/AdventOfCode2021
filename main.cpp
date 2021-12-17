#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <functional>
#include <vector>
#include <set>
#include <map>
#include <bitset>
#include <assert.h>
#include <cctype> 



void Day01Puzzle1()
{
	std::ifstream dataStream("depthData.txt");

	std::string depthSample;
	std::getline(dataStream, depthSample);
	int prevDepth = std::stoi(depthSample);

	int numIncreases = 0;
	while(std::getline(dataStream, depthSample))
	{
		const int nextDepth = std::stoi(depthSample);
		if(nextDepth > prevDepth) ++numIncreases;
		prevDepth = nextDepth;
	}

	std::cout << "Advent of Code Day 1 Puzzle 1" << std::endl;
	std::cout << "Num depth increases = " << numIncreases << std::endl;
	std::cout << std::endl;
}



void Day01Puzzle2()
{
	std::ifstream dataStream("depthData.txt");

	int depthWindow[3] = { 0, 0, 0 };
	std::string depthSample;

	for(int i = 0; i < 3; ++i)
	{
		std::getline(dataStream, depthSample);
		const int depth = std::stoi(depthSample);
		for(int j = 0; j <= i; ++j)
			depthWindow[j] += depth;
	}

	int numIncreases = 0;
	int fullWindowIndex = 0;
	while(std::getline(dataStream, depthSample))
	{
		const int prevWindowDepth = depthWindow[fullWindowIndex];
		depthWindow[fullWindowIndex] = 0;
		fullWindowIndex = (++fullWindowIndex) % 3;

		const int depth = std::stoi(depthSample);
		depthWindow[0] += depth;
		depthWindow[1] += depth;
		depthWindow[2] += depth;

		if(depthWindow[fullWindowIndex] > prevWindowDepth)
			++numIncreases;
	}

	std::cout << "Advent of Code Day 1 Puzzle 2" << std::endl;
	std::cout << "Num depth increases = " << numIncreases << std::endl;
	std::cout << std::endl;
}



void Day02Puzzle1()
{
	std::ifstream dataStream("directionData.txt");

	int horizontalPosition = 0;
	int depthPosition = 0;

	std::string directionSample;
	while(std::getline(dataStream, directionSample))
	{
		const int moveAmount = directionSample.back() - '0';
		switch(directionSample[0])
		{
		case 'f':
			horizontalPosition += moveAmount;
			break;

		case 'u':
			depthPosition -= moveAmount;
			break;

		case 'd':
			depthPosition += moveAmount;
			break;
		}
	}

	std::cout << "Advent of Code Day 2 Puzzle 1" << std::endl;
	std::cout << "Horizontal position times depth = " << horizontalPosition * depthPosition << std::endl;
	std::cout << std::endl;
}



void Day02Puzzle2()
{
	std::ifstream dataStream("directionData.txt");

	int aim = 0;
	int horizontalPosition = 0;
	int depthPosition = 0;

	std::string directionSample;
	while(std::getline(dataStream, directionSample))
	{
		const int amount = directionSample.back() - '0';
		switch(directionSample[0])
		{
		case 'f':
			horizontalPosition += amount;
			depthPosition += amount * aim;
			break;

		case 'u':
			aim -= amount;
			break;

		case 'd':
			aim += amount;
			break;
		}
	}

	std::cout << "Advent of Code Day 2 Puzzle 2" << std::endl;
	std::cout << "Horizontal position times depth = " << horizontalPosition * depthPosition << std::endl;
	std::cout << std::endl;
}



void Day03Puzzle1()
{
	std::ifstream dataStream("diagnosticReport.txt");

	constexpr uint16_t numBits = 12;
	uint16_t numSamples = 0;
	uint16_t bitCounts[12] = {};

	std::string sample;
	while(std::getline(dataStream, sample))
	{
		++numSamples;
		for(uint16_t i = 0; i < numBits; ++i)
			if(sample[i] == '1')
				++bitCounts[i];
	}

	const uint16_t halfNumSamples = numSamples / 2;

	uint16_t gammaRate = 0;
	for(uint16_t i = 0; i < numBits; ++i)
		if(bitCounts[i] > halfNumSamples)
			gammaRate |= 1 << (numBits - i - 1);

	const uint16_t epsilonRate = ~gammaRate & 0b0000111111111111;

	std::cout << "Advent of Code Day 3 Puzzle 1" << std::endl;
	std::cout << "GammaRate * EpsilonRate = " << gammaRate * epsilonRate << std::endl;
	std::cout << std::endl;
}



void Day03Puzzle2()
{
	std::ifstream dataStream("diagnosticReport.txt");	

	constexpr uint16_t numBits = 12;

	// gather samples
	std::vector<uint16_t> samples;
	samples.reserve(1000);

	std::string sampleText;
	while(std::getline(dataStream, sampleText))
		samples.push_back(std::stoi(sampleText, 0, 2));

	// sort according to bits
	std::sort(samples.begin(), samples.end(), [](const uint16_t& a, const uint16_t& b) -> bool
	{
		return a > b;
	});

	// find rating
	const auto findRating = [&](std::function<bool(uint16_t numSamples, uint16_t samplesInRange)> compare) -> uint16_t
	{
		uint16_t startIndex = 0;
		uint16_t endIndex = (uint16_t)samples.size();
		uint16_t bitIndex = numBits - 1;

		while(startIndex + 1 != endIndex)
		{
			const uint16_t bitMask = 1 << bitIndex;
			const uint16_t samplesInRange = endIndex - startIndex;

			uint16_t splitIndex = 0;
			for(splitIndex = startIndex; splitIndex < endIndex; ++splitIndex)
				if((samples[splitIndex] & bitMask) == 0)
					break;

			const uint16_t numSetSamples = splitIndex - startIndex;
			if(compare(numSetSamples, samplesInRange))
				endIndex = splitIndex;
			else
				startIndex = splitIndex;

			--bitIndex;
		}

		return samples[startIndex];
	};

	const uint16_t oxygenRating = findRating([](uint16_t numSamples, uint16_t samplesInRange) -> bool
	{
		return numSamples >= samplesInRange - numSamples;
	});

	const uint16_t co2Rating = findRating([](uint16_t numSamples, uint16_t samplesInRange) -> bool
	{
		return numSamples < samplesInRange - numSamples;
	});
	
	std::cout << "Advent of Code Day 3 Puzzle 2" << std::endl;
	std::cout << "Life support rating = " << oxygenRating * co2Rating << std::endl;
	std::cout << std::endl;
}


void Day04()
{
	std::ifstream dataStream("bingo.txt");

	// get bingo numbers
	std::string dataLine;
	std::getline(dataStream, dataLine);

	std::stringstream numberStream(dataLine);
	std::string numberData;
	std::vector<int> numbers;
	while(std::getline(numberStream, numberData, ','))
		numbers.push_back(std::stoi(numberData));

	// get bingo grids
	constexpr int gridSize = 5;

	struct GridEntry
	{
		GridEntry(int col, int row) : m_col(col), m_row(row) {}
		int m_col = -1;
		int m_row = -1;
		bool m_marked = false;
	};

	class Grid
	{
	public:
		void AddEntry(int number, int col, int row)
		{
			m_entries.insert({number, GridEntry(col, row)});
		}

		bool MarkEntry(int number)
		{
			auto entryIter = m_entries.find(number);
			if(entryIter == m_entries.end())
				return false;

			GridEntry& entry = entryIter->second;
			entry.m_marked = true;
			++m_numMarkedInCol[entry.m_col];
			++m_numMarkedInRow[entry.m_row];
			return m_numMarkedInCol[entry.m_col] == gridSize
				|| m_numMarkedInRow[entry.m_row] == gridSize;
		}

		int SumUnmarked() const
		{
			int sum = 0;
			for(const auto& pair : m_entries)
				if(!pair.second.m_marked)
					sum += pair.first;
			return sum;
		}

	private:
		std::map<int, GridEntry> m_entries;
		int m_numMarkedInCol[gridSize] = {};
		int m_numMarkedInRow[gridSize] = {};
	};

	std::vector<Grid> grids;
	while(std::getline(dataStream, dataLine))
	{
		grids.push_back(Grid());
		Grid& grid = grids.back();

		for(int col = 0; col < gridSize; ++col)
		{
			std::getline(dataStream, dataLine);

			// remove leading spaces
			while(dataLine.front() == ' ')
				dataLine.erase(0, 1);

			// remove double spaces
			int doubleSpaceIndex = dataLine.find("  ");
			while(doubleSpaceIndex != std::string::npos)
			{
				dataLine.replace(doubleSpaceIndex, 2, " ");
				doubleSpaceIndex = dataLine.find("  ");
			}

			numberStream = std::stringstream(dataLine);

			for(int row = 0; row < gridSize; ++row)
			{
				std::getline(numberStream, numberData, ' ');
				grid.AddEntry(std::stoi(numberData), col, row);
			}
		}
	}

	// play bingo
	bool firstWinnerFound = false;
	for(int number : numbers)
	{
		for(int gridIndex = 0; gridIndex < grids.size(); ++gridIndex)
		{
			Grid& grid = grids[gridIndex];
			if(grid.MarkEntry(number))
			{
				if(!firstWinnerFound)
				{
					// winner winner chicken dinner
					firstWinnerFound = true;
					const int unmarkedSum = grid.SumUnmarked();
					const int finalScore = number * unmarkedSum;

					std::cout << "Advent of Code Day 4 Puzzle 1" << std::endl;
					std::cout << "Final score = " << finalScore << std::endl;
					std::cout << std::endl;
				}

				if(grids.size() == 1)
				{
					// last winner
					const int unmarkedSum = grid.SumUnmarked();
					const int finalScore = number * unmarkedSum;

					std::cout << "Advent of Code Day 4 Puzzle 2" << std::endl;
					std::cout << "Final score = " << finalScore << std::endl;
					std::cout << std::endl;

					return;
				}

				grids[gridIndex] = grids.back();
				grids.pop_back();
				--gridIndex;
			}
		}
	}
}


void Day05()
{
	std::ifstream dataStream("hydrothermalVents.txt");

	// get lines
	struct Point
	{
		int m_x = -1;
		int m_y = -1;
	};

	struct Line
	{
		Point m_start;
		Point m_end;

		bool isVertical() const { return m_start.m_x == m_end.m_x; }
		bool isHorizontal() const { return m_start.m_y == m_end.m_y; }
	};

	std::vector<Line> verticalLines;
	std::vector<Line> horizontaLines;
	std::vector<Line> diagonalLines;

	std::string dataLine;
	while(std::getline(dataStream, dataLine))
	{
		Line line;
		std::stringstream inputStream(dataLine);
		std::string inputData;

		std::getline(inputStream, inputData, ',');
		line.m_start.m_x = std::stoi(inputData);
		std::getline(inputStream, inputData, ' ');
		line.m_start.m_y = std::stoi(inputData);
		std::getline(inputStream, inputData, ' ');
		std::getline(inputStream, inputData, ',');
		line.m_end.m_x = std::stoi(inputData);
		std::getline(inputStream, inputData, ' ');
		line.m_end.m_y = std::stoi(inputData);

		if(line.isVertical())
			verticalLines.push_back(line);
		else if(line.isHorizontal())
			horizontaLines.push_back(line);
		else
			diagonalLines.push_back(line);
	}

	// fill out grid to determine overlaps of horizontal and vertical lines
	constexpr int gridSize = 1000;
	constexpr int numGridSpaces = gridSize * gridSize;
	int* pGrid = new int[numGridSpaces];
	memset(pGrid, 0, sizeof(int) * numGridSpaces);

	for(const Line& line : verticalLines)
	{
		const int dir = line.m_end.m_y > line.m_start.m_y ? 1 : -1;
		for(int y = line.m_start.m_y; y != line.m_end.m_y + dir; y += dir)
			++pGrid[line.m_start.m_x + y * gridSize];

	}

	for(const Line& line : horizontaLines)
	{
		const int dir = line.m_end.m_x > line.m_start.m_x ? 1 : -1;
		for(int x = line.m_start.m_x; x != line.m_end.m_x + dir; x += dir)
			++pGrid[x + line.m_start.m_y * gridSize];
	}

	// count overlaps
	int numOverlaps = 0;
	for(int i = 0; i < numGridSpaces; ++i)
		if(pGrid[i] > 1)
			++numOverlaps;

	std::cout << "Advent of Code Day 5 Puzzle 1" << std::endl;
	std::cout << "Num overlaps = " << numOverlaps << std::endl;
	std::cout << std::endl;

	// add in diagonal lines
	for(const Line& line : diagonalLines)
	{
		const int xDir = line.m_end.m_x > line.m_start.m_x ? 1 : -1;
		const int yDir = line.m_end.m_y > line.m_start.m_y ? 1 : -1;
		const int length = abs(line.m_end.m_x - line.m_start.m_x) + 1;
		for(int i = 0; i < length; ++i)
			++pGrid[line.m_start.m_x + i * xDir + (line.m_start.m_y + i * yDir) * gridSize];
	}

	// count overlaps
	numOverlaps = 0;
	for(int i = 0; i < numGridSpaces; ++i)
		if(pGrid[i] > 1)
			++numOverlaps;

	std::cout << "Advent of Code Day 5 Puzzle 2" << std::endl;
	std::cout << "Num overlaps = " << numOverlaps << std::endl;
	std::cout << std::endl;

	// cleanup
	delete []pGrid;
}


void Day06()
{
	std::ifstream dataStream("lanternFish.txt");
	std::string dataLine;
	std::getline(dataStream, dataLine);

	// count initial population
	std::stringstream stageStream(dataLine);
	std::string stageData;
	uint64_t initNumFishAtStage[9] = {};
	while(std::getline(stageStream, stageData, ','))
	{
		const uint64_t stage = std::stoi(stageData);
		++initNumFishAtStage[stage];
	}

	// simulate fish growth
	const auto simulateFishGrowth = [&](uint64_t numDays) -> uint64_t
	{
		constexpr uint64_t matureCycleLength = 9;
		constexpr uint64_t birthCycleLength = 7;

		uint64_t numFishAtStage[matureCycleLength] = {};
		memcpy(numFishAtStage, initNumFishAtStage, sizeof(uint64_t) * matureCycleLength);

		for(uint64_t i = 0; i < numDays; ++i)
		{
			const uint64_t numFishToBirth = numFishAtStage[0];
			for(uint64_t j = 1; j < matureCycleLength; ++j)
				numFishAtStage[j - 1] = numFishAtStage[j];

			numFishAtStage[matureCycleLength - 1] = numFishToBirth;
			numFishAtStage[birthCycleLength - 1] += numFishToBirth;
		}

		// count number of fish
		uint64_t fishCount = 0;
		for(uint64_t i = 0; i < matureCycleLength; ++i)
			fishCount += numFishAtStage[i];

		return fishCount;
	};

	std::cout << "Advent of Code Day 6 Puzzle 1" << std::endl;
	std::cout << "Num fish = " << simulateFishGrowth(80) << std::endl;
	std::cout << std::endl;

	std::cout << "Advent of Code Day 6 Puzzle 2" << std::endl;
	std::cout << "Num fish = " << simulateFishGrowth(256) << std::endl;
	std::cout << std::endl;
}


void Day07()
{
	std::ifstream dataStream("crabs.txt");
	std::string dataLine;
	std::getline(dataStream, dataLine);

	// get crab positions
	std::stringstream posStream(dataLine);
	std::string posData;
	std::vector<int> positions;
	while(std::getline(posStream, posData, ','))
		positions.push_back(std::stoi(posData));

	// determine min and max pos
	int minPos = INT16_MAX;
	int maxPos = 0;
	for(int pos : positions)
	{
		minPos = std::min(minPos, pos);
		maxPos = std::max(maxPos, pos);
	}

	// function to calculte fuel cost of alignment position for puzzle 1
	const auto calculateFuelCost1 = [&](int alignPos) -> int
	{
		int cost = 0;
		for(int pos : positions)
			cost += abs(pos - alignPos);
		return cost;
	};

	// function to calculte fuel cost of alignment position for puzzle 2
	const auto calculateFuelCost2 = [&](int alignPos) -> int
	{
		int cost = 0;
		for(int pos : positions)
		{
			int diff = abs(pos - alignPos);
			for(int i = 1; i <= diff; ++i)
				cost += i;
		}
		return cost;
	};

	// function to find the saddle point
	const auto findSaddlePosFuel = [](int minPos, int maxPos, std::function<int(int alignPos)> fuelCost) -> int
	{
		while(maxPos > minPos)
		{
			int midPos = (maxPos + minPos) / 2;
			int midPosScore = fuelCost(midPos);
			int midPosPlusScore = fuelCost(midPos + 1);
		
			if(midPosPlusScore < midPosScore)
				minPos = std::max(midPos, minPos + 1);
			else
				maxPos = std::min(midPos, maxPos - 1);
		}

		return fuelCost(maxPos);
	};

	std::cout << "Advent of Code Day 7 Puzzle 1" << std::endl;
	std::cout << "Fuel cost = " << findSaddlePosFuel(minPos, maxPos, calculateFuelCost1) << std::endl;
	std::cout << std::endl;

	std::cout << "Advent of Code Day 7 Puzzle 2" << std::endl;
	std::cout << "Fuel cost = " << findSaddlePosFuel(minPos, maxPos, calculateFuelCost2) << std::endl;
	std::cout << std::endl;
}


void Day08()
{
	std::ifstream dataStream("sevenSegmentData.txt");

	// read in display entries
	constexpr int numSignalPatterns = 10;
	constexpr int numOutputDigits = 4;
	constexpr int numSegments = 7;
	constexpr int numSingleDigits = 10;

	struct DisplayEntry
	{
		std::bitset<8> m_signalPatterns[numSignalPatterns] = {};
		std::bitset<8> m_outputDigits[numOutputDigits] = {};
	};
	std::vector<DisplayEntry> displayEntries;

	std::string dataLine;
	while(std::getline(dataStream, dataLine))
	{
		std::stringstream signalStream(dataLine);
		displayEntries.push_back(DisplayEntry());
		DisplayEntry& entry = displayEntries.back();

		for(int i = 0; i < numSignalPatterns; ++i)
		{
			std::string signalData;
			std::getline(signalStream, signalData, ' ');
			for(int j = 0; j < (int)signalData.size(); ++j)
				entry.m_signalPatterns[i].set(signalData[j] - 'a');
		}

		signalStream.get();
		signalStream.get();

		for(int i = 0; i < numOutputDigits; ++i)
		{
			std::string signalData;
			std::getline(signalStream, signalData, ' ');
			for(int j = 0; j < (int)signalData.size(); ++j)
				entry.m_outputDigits[i].set(signalData[j] - 'a');
		}
	}

	// count number of 1 (2 segments), 4 (4 segments), 7 (3 segments), and 8 (7 segments) digits in output values
	int uniqueDigitCount = 0;
	for(const DisplayEntry& entry : displayEntries)
	{
		for(int i = 0; i < numOutputDigits; ++i)
		{
			const int numSegments = entry.m_outputDigits[i].count();
			if(numSegments == 2 || numSegments == 3 || numSegments == 4 || numSegments == 7)
				++uniqueDigitCount;
		}
	}

	std::cout << "Advent of Code Day 8 Puzzle 1" << std::endl;
	std::cout << "Unique digit count = " << uniqueDigitCount << std::endl;
	std::cout << std::endl;

	// deduce mappings
	const auto addUnique = [](std::vector<std::bitset<8>>& possibleDigitCodes, std::bitset<8> code) -> void
	{
		for(const std::bitset<8>& possibleCode : possibleDigitCodes)
			if(possibleCode == code)
				return;
		possibleDigitCodes.push_back(code);
	};

	const auto removeUnique = [](std::vector<std::bitset<8>>& possibleDigitCodes, std::bitset<8> code) -> void
	{
		for(int i = 0; i < possibleDigitCodes.size(); ++i)
		{
			if(possibleDigitCodes[i] == code)
			{
				possibleDigitCodes[i] = possibleDigitCodes.back();
				possibleDigitCodes.pop_back();
			}
		}
	};

	const auto compilePossibleDigitCodes = [&](std::vector<std::bitset<8>>* pPossibleDigitCodes, const std::bitset<8>* pCodes, int numCodes) -> void
	{
		for(int i = 0; i < numCodes; ++i)
		{
			const std::bitset<8>& code = pCodes[i];
			const int numSegments = code.count();
			switch(numSegments)
			{
			case 2:
				addUnique(pPossibleDigitCodes[1], code);
				break;

			case 3:
				addUnique(pPossibleDigitCodes[7], code);
				break;

			case 4:
				addUnique(pPossibleDigitCodes[4], code);
				break;

			case 5:
				addUnique(pPossibleDigitCodes[2], code);
				addUnique(pPossibleDigitCodes[3], code);
				addUnique(pPossibleDigitCodes[5], code);
				break;

			case 6:
				addUnique(pPossibleDigitCodes[0], code);
				addUnique(pPossibleDigitCodes[6], code);
				addUnique(pPossibleDigitCodes[9], code);
				break;

			case 7:
				addUnique(pPossibleDigitCodes[8], code);
				break;
			}
		}
	};

	int outputValueSum = 0;
	for(const DisplayEntry& entry : displayEntries)
	{
		std::vector<std::bitset<8>> possibleDigitCodes[numSingleDigits];
		compilePossibleDigitCodes(possibleDigitCodes, entry.m_signalPatterns, numSignalPatterns);
		compilePossibleDigitCodes(possibleDigitCodes, entry.m_outputDigits, numOutputDigits);

		// can differentiate 3 from 2 and 5 by comapring to 1 or 7
		if(possibleDigitCodes[3].size())
		{
			const std::bitset<8> code1or7 = possibleDigitCodes[1].size() ? possibleDigitCodes[1][0] : possibleDigitCodes[7].size() ? possibleDigitCodes[7][0] : 0;
			if(code1or7 != 0)
			{
				bool found = false;
				for(int i = 0; i < (int)possibleDigitCodes[3].size(); ++i)
				{
					const std::bitset<8> code3 = possibleDigitCodes[3][i];
					if((code3 & code1or7) == code1or7)
					{
						possibleDigitCodes[3].clear();
						possibleDigitCodes[3].push_back(code3);
						removeUnique(possibleDigitCodes[2], code3);
						removeUnique(possibleDigitCodes[5], code3);
						found = true;
						break;
					}
				}

				if(!found)
					possibleDigitCodes[3].clear();
			}
		}

		// can differentiate 9 from 0 and 6 by comparing to 3
		if(possibleDigitCodes[9].size() && possibleDigitCodes[3].size())
		{
			const std::bitset<8> code3 = possibleDigitCodes[3][0];

			bool found = false;
			for(int i = 0; i < (int)possibleDigitCodes[9].size(); ++i)
			{
				const std::bitset<8> code9 = possibleDigitCodes[9][i];
				if((code9 & code3) == code3)
				{
					possibleDigitCodes[9].clear();
					possibleDigitCodes[9].push_back(code9);
					removeUnique(possibleDigitCodes[0], code9);
					removeUnique(possibleDigitCodes[6], code9);
					found = true;
					break;
				}
			}

			if(!found)
				possibleDigitCodes[9].clear();
		}

		// can differentiate 5 from 2 comparing to 9
		if(possibleDigitCodes[2].size() && possibleDigitCodes[9].size())
		{
			const std::bitset<8> code9 = possibleDigitCodes[9][0];

			bool found = false;
			for(int i = 0; i < (int)possibleDigitCodes[5].size(); ++i)
			{
				const std::bitset<8> code5 = possibleDigitCodes[5][i];
				if((code5 & code9) == code5)
				{
					possibleDigitCodes[5].clear();
					possibleDigitCodes[5].push_back(code5);
					removeUnique(possibleDigitCodes[2], code5);
					found = true;
					break;
				}
			}

			if(!found)
				possibleDigitCodes[5].clear();
		}

		// can differentiate 6 from 0 by comparing to 5
		// otherwise can differentiate 0 from 6 by comparing to 7
		if(possibleDigitCodes[6].size() && possibleDigitCodes[5].size())
		{
			const std::bitset<8> code5 = possibleDigitCodes[5][0];

			bool found = false;
			for(int i = 0; i < (int)possibleDigitCodes[6].size(); ++i)
			{
				const std::bitset<8> code6 = possibleDigitCodes[6][i];
				if((code6 & code5) == code5)
				{
					possibleDigitCodes[6].clear();
					possibleDigitCodes[6].push_back(code6);
					removeUnique(possibleDigitCodes[0], code6);
					found = true;
					break;
				}
			}

			if(!found)
				possibleDigitCodes[6].clear();
		}
		else if(possibleDigitCodes[0].size() && possibleDigitCodes[7].size())
		{
			const std::bitset<8> code7 = possibleDigitCodes[7][0];

			bool found = false;
			for(int i = 0; i < (int)possibleDigitCodes[0].size(); ++i)
			{
				const std::bitset<8> code0 = possibleDigitCodes[0][i];
				if((code0 & code7) == code7)
				{
					possibleDigitCodes[0].clear();
					possibleDigitCodes[0].push_back(code0);
					removeUnique(possibleDigitCodes[5], code0);
					found = true;
					break;
				}
			}

			if(!found)
				possibleDigitCodes[0].clear();
		}

		// decode output values
		int outputDigits[numOutputDigits] = {};
		for(int i = 0; i < numOutputDigits; ++i)
		{
			std::bitset<8> outputCode = entry.m_outputDigits[i];
			for(int j = 0; j < numSingleDigits; ++j)
			{
				if(possibleDigitCodes[j].size() && possibleDigitCodes[j][0] == outputCode)
				{
					outputDigits[i] = j;
					break;
				}
			}
		}

		outputValueSum += outputDigits[0] * 1000;
		outputValueSum += outputDigits[1] * 100;
		outputValueSum += outputDigits[2] * 10;
		outputValueSum += outputDigits[3];
	}

	std::cout << "Advent of Code Day 8 Puzzle 2" << std::endl;
	std::cout << "Output value sum = " << outputValueSum << std::endl;
	std::cout << std::endl;
}


void Day09()
{
	std::ifstream dataStream("lavaTubes.txt");

	// get depth data
	std::vector<int> depthData;
	int gridWidth = 0;
	int gridHeight = 0;

	std::string dataLine;
	while(std::getline(dataStream, dataLine))
	{
		gridWidth = dataLine.size();
		++gridHeight;
		for(int i = 0; i < gridWidth; ++i)
			depthData.push_back(dataLine[i] - '0');
	}

	// depth access
	const auto getGridIndex = [&](int x, int y) -> int
	{
		return x + y * gridWidth;
	};

	const auto getCoords = [&](int gridIndex, int& x, int& y) -> void
	{
		y = gridIndex / gridWidth;
		x = gridIndex % gridWidth;
	};

	const auto getDepth = [&](int x, int y) -> int
	{
		return depthData[getGridIndex(x, y)];
	};

	// find risk level and basins
	int riskLevel = 0;
	std::vector<int> basinSizes;
	const int lastX = gridWidth - 1;
	const int lastY = gridHeight - 1;

	const auto calculateBasinSize = [&](int xLowest, int yLowest) -> void
	{
		std::vector<int> visited;
		std::vector<int> unvisited;
		unvisited.push_back(getGridIndex(xLowest, yLowest));

		const auto canVisit = [&](int gridIndex) -> bool
		{
			if(depthData[gridIndex] == 9)
				return false;

			for(int visitedGridIndex : visited)
				if(visitedGridIndex == gridIndex)
					return false;

			for(int unvisitedGridIndex : unvisited)
				if(unvisitedGridIndex == gridIndex)
					return false;

			return true;
		};

		while(unvisited.size())
		{
			const int gridIndex = unvisited.back();
			unvisited.pop_back();
			visited.push_back(gridIndex);

			int startX, startY;
			getCoords(gridIndex, startX, startY);

			if(startY > 0)
			{
				const int nextGridIndex = getGridIndex(startX, startY - 1);
				if(canVisit(nextGridIndex))
					unvisited.push_back(nextGridIndex);
			}

			if(startY < lastY)
			{
				const int nextGridIndex = getGridIndex(startX, startY + 1);
				if(canVisit(nextGridIndex))
					unvisited.push_back(nextGridIndex);
			}

			if(startX > 0)
			{
				const int nextGridIndex = getGridIndex(startX - 1, startY);
				if(canVisit(nextGridIndex))
					unvisited.push_back(nextGridIndex);
			}

			if(startX < lastX)
			{
				const int nextGridIndex = getGridIndex(startX + 1, startY);
				if(canVisit(nextGridIndex))
					unvisited.push_back(nextGridIndex);
			}
		}

		basinSizes.push_back(visited.size());
	};

	// visit corners
	const int tlCornerDepth = getDepth(0, 0);
	if(tlCornerDepth < getDepth(1, 0) && tlCornerDepth < getDepth(0, 1)) 
	{ 
		riskLevel += 1 + tlCornerDepth; 
		calculateBasinSize(0, 0);
	}

	const int trCornerDepth = getDepth(lastX, 0);
	if(trCornerDepth < getDepth(lastX - 1, 0) && trCornerDepth < getDepth(lastX, 1)) 
	{ 
		riskLevel += 1 + trCornerDepth; 
		calculateBasinSize(lastX, 0);
	}

	const int blCornerDepth = getDepth(0, lastY);
	if(blCornerDepth < getDepth(1, lastY) && blCornerDepth < getDepth(0, lastY - 1)) 
	{ 
		riskLevel += 1 + blCornerDepth; 
		calculateBasinSize(0, lastY);
	}

	const int brCornerDepth = getDepth(lastX, lastY);
	if(brCornerDepth < getDepth(lastX - 1, lastY) && brCornerDepth < getDepth(lastX, lastY - 1))
	{ 
		riskLevel += 1 + brCornerDepth; 
		calculateBasinSize(lastX, lastY);
	}

	// visit edges
	for(int x = 1; x < lastX; ++x)
	{
		const int tDepth = getDepth(x, 0);
		if(tDepth < getDepth(x - 1, 0) && tDepth < getDepth(x + 1, 0) && tDepth < getDepth(x, 1)) 
		{ 
			riskLevel += 1 + tDepth; 
			calculateBasinSize(x, 0);
		}

		const int bDepth = getDepth(x, lastY);
		if(bDepth < getDepth(x - 1, lastY) && bDepth < getDepth(x + 1, lastY) && bDepth < getDepth(x, lastY - 1)) 
		{ 
			riskLevel += 1 + bDepth; 
			calculateBasinSize(x, lastY);
		}
	}

	for(int y = 1; y < lastY; ++y)
	{
		const int lDepth = getDepth(0, y);
		if(lDepth < getDepth(0, y - 1) && lDepth < getDepth(0, y + 1) && lDepth < getDepth(1, y)) 
		{
			riskLevel += 1 + lDepth; 
			calculateBasinSize(0, y);
		}

		const int rDepth = getDepth(lastX, y);
		if(rDepth < getDepth(lastX, y - 1) && rDepth < getDepth(lastX, y + 1) && rDepth < getDepth(lastX - 1, y)) 
		{ 
			riskLevel += 1 + rDepth; 
			calculateBasinSize(lastX, y);
		}
	}

	// visit central locations
	for(int x = 1; x < lastX; ++x)
	{
		for(int y = 1; y < lastY; ++y)
		{
			const int depth = getDepth(x, y);
			const int uDepth = getDepth(x, y - 1);
			const int dDepth = getDepth(x, y + 1);
			const int lDepth = getDepth(x - 1, y);
			const int rDepth = getDepth(x + 1, y);

			if(depth < uDepth
				&& depth < dDepth
				&& depth < lDepth
				&& depth < rDepth)
			{
				riskLevel += 1 + depth;
				calculateBasinSize(x, y);
			}
		}
	}

	// find largest 3 basins
	std::sort(basinSizes.begin(), basinSizes.end(), [](const int& a, const int& b) -> bool
	{
		return a > b;
	});

	int largestBasinProduct = basinSizes[0] * basinSizes[1] * basinSizes[2];

	// output
	std::cout << "Advent of Code Day 9 Puzzle 1" << std::endl;
	std::cout << "Risk level = " << riskLevel << std::endl;
	std::cout << std::endl;

	std::cout << "Advent of Code Day 9 Puzzle 2" << std::endl;
	std::cout << "Largett Basin size product = " << largestBasinProduct << std::endl;
	std::cout << std::endl;
}


void Day10()
{
	std::ifstream dataStream("syntax.txt");

	int syntaxErrorScore = 0;
	std::vector<uint64_t> autoCompleteScores;

	std::string dataLine;
	while(std::getline(dataStream, dataLine))
	{
		std::vector<char> parsedSyntax;
		bool errorFound = false;

		// parse line and search for syntax errors
		for(char ch : dataLine)
		{
			switch(ch)
			{
			case '(':
			case '[':
			case '{':
			case '<':
				parsedSyntax.push_back(ch);
				break;

			case ')':
				if(parsedSyntax.size() > 0 && parsedSyntax.back() == '(') { parsedSyntax.pop_back(); }
				else { errorFound = true; syntaxErrorScore += 3; }
				break;

			case ']':
				if(parsedSyntax.size() > 0 && parsedSyntax.back() == '[') { parsedSyntax.pop_back(); }
				else { errorFound = true; syntaxErrorScore += 57; }
				break;

			case '}':
				if(parsedSyntax.size() > 0 && parsedSyntax.back() == '{') { parsedSyntax.pop_back(); }
				else { errorFound = true; syntaxErrorScore += 1197; }
				break;

			case '>':
				if(parsedSyntax.size() > 0 && parsedSyntax.back() == '<') { parsedSyntax.pop_back(); }
				else { errorFound = true; syntaxErrorScore += 25137; }
				break;
			}

			if(errorFound)
				break;
		}

		// if no errors found then determine autocomplete score
		if(!errorFound)
		{
			uint64_t autoCompleteScore = 0;
			while(parsedSyntax.size())
			{
				char ch = parsedSyntax.back();
				parsedSyntax.pop_back();

				autoCompleteScore *= 5;
				switch(ch)
				{
				case '(': autoCompleteScore += 1; break;
				case '[': autoCompleteScore += 2; break;
				case '{': autoCompleteScore += 3; break;
				case '<': autoCompleteScore += 4; break;
				}
			}
			autoCompleteScores.push_back(autoCompleteScore);
		}
	}

	// find middle auto complete score
	std::sort(autoCompleteScores.begin(), autoCompleteScores.end());
	const uint64_t midAutoCompleteScore = autoCompleteScores[autoCompleteScores.size() / 2];

	std::cout << "Advent of Code Day 10 Puzzle 1" << std::endl;
	std::cout << "Syntax error score = " << syntaxErrorScore << std::endl;
	std::cout << std::endl;

	std::cout << "Advent of Code Day 10 Puzzle 2" << std::endl;
	std::cout << "Auto complete score = " << midAutoCompleteScore << std::endl;
	std::cout << std::endl;
}


void Day11()
{
	std::ifstream dataStream("dumboOctopus.txt");

	constexpr int gridSize = 10;
	constexpr int numOctopi = gridSize * gridSize;
	int energyLevels[numOctopi] = {};

	for(int x = 0; x < gridSize; ++x)
	{
		std::string dataLine;
		std::getline(dataStream, dataLine);
		for(int y = 0; y < gridSize; ++y)
			energyLevels[x + y * gridSize] = dataLine[y] - '0';
	}

	constexpr int flashEnergy = 10;
	uint64_t numFlashesAfter100Steps = 0;
	uint64_t numSteps = 0;
	bool syncronisedFlash = false;

	while(!syncronisedFlash)
	{
		++numSteps;
		std::vector<int> flashIndices;

		// increase energy levels of all octopi by 1
		for(int i = 0; i < numOctopi; ++i)
			if(++energyLevels[i] == flashEnergy)
				flashIndices.push_back(i);

		// resolve flashes
		while(flashIndices.size())
		{
			if(numSteps <= 100)
				++numFlashesAfter100Steps;

			const int flashIndex = flashIndices.back();
			flashIndices.pop_back();
			const int flashCoordX = flashIndex % gridSize;
			const int flashCoordY = flashIndex / gridSize;

			for(int x = std::max(flashCoordX - 1, 0); x <= std::min(flashCoordX + 1, gridSize - 1); ++x)
			{
				for(int y = std::max(flashCoordY - 1, 0); y <= std::min(flashCoordY + 1, gridSize - 1); ++y)
				{
					const int effectedIndex = x + y * gridSize;
					if(++energyLevels[effectedIndex] == flashEnergy)
						flashIndices.push_back(effectedIndex);
				}
			}
		}

		// reset energy levels of flashed octopi and check for syncronised flash
		syncronisedFlash = true;
		for(int i = 0; i < numOctopi; ++i)
		{
			if(energyLevels[i] >= flashEnergy)
				energyLevels[i] = 0;
			else
				syncronisedFlash = false;
		}
	}

	std::cout << "Advent of Code Day 11 Puzzle 1" << std::endl;
	std::cout << "Num flashes after 100 steps = " << numFlashesAfter100Steps << std::endl;
	std::cout << std::endl;

	std::cout << "Advent of Code Day 11 Puzzle 2" << std::endl;
	std::cout << "Num steps for syncronised flash = " << numSteps << std::endl;
	std::cout << std::endl;
}


struct ICaveNode
{
	virtual void Visit() = 0;
	virtual void Unvisit() = 0;
	virtual bool CanVisit() const = 0;
	std::string m_name;
	std::vector<ICaveNode*> m_connectedNodes;
};

struct StartCave : public ICaveNode
{
	virtual void Visit() override { }
	virtual void Unvisit() override { }
	virtual bool CanVisit() const override { return false; }
};

struct EndCave : public ICaveNode
{
	virtual void Visit() override { ++m_numVisits; }
	virtual void Unvisit() override { }
	virtual bool CanVisit() const override { return true; }
	int m_numVisits = 0;
};

struct SmallCave : public ICaveNode
{
	virtual void Visit() override 
	{ 
		if(++m_numVisits == 2)
			s_hasDoubleVisit = true;
	}

	virtual void Unvisit() override 
	{ 
		if(--m_numVisits == 1) 
			s_hasDoubleVisit = false; 
	}

	virtual bool CanVisit() const override
	{
		if(s_allowDoubleVisits)
		{
			const int visitCap = s_hasDoubleVisit ? 1 : 2;
			return m_numVisits < visitCap;
		}
		return m_numVisits < 1;
	}

	int m_numVisits = 0;
	static bool s_allowDoubleVisits;
	static int s_hasDoubleVisit;
};
bool SmallCave::s_allowDoubleVisits = false;
int SmallCave::s_hasDoubleVisit = false;

struct LargeCave : public ICaveNode
{
	virtual void Visit() override { }
	virtual void Unvisit() override { }
	virtual bool CanVisit() const override { return true; }
};

void Day12()
{
	std::ifstream dataStream("caves.txt");

	// parse cave nodes
	std::map<std::string, ICaveNode*> caveNodes;
	StartCave* pStartCave = nullptr;
	EndCave* pEndCave = nullptr;

	const auto getOrCreateCaveNode = [&](const std::string& name) -> ICaveNode*
	{
		const auto caveIter = caveNodes.find(name);
		if(caveIter != caveNodes.end())
			return caveIter->second;

		ICaveNode* pCaveNode = nullptr;
		if(name == "start") { pStartCave = new StartCave(); pCaveNode = pStartCave; }
		else if(name == "end") { pEndCave = new EndCave(); pCaveNode = pEndCave; }
		else if(std::isupper(name[0])) { pCaveNode = new LargeCave(); }
		else { pCaveNode = new SmallCave(); }

		pCaveNode->m_name = name;
		caveNodes[name] = pCaveNode;
		return pCaveNode;
	};

	std::string dataLine;
	while(std::getline(dataStream, dataLine))
	{
		std::stringstream caveStream(dataLine);
		std::string caveName1;
		std::string caveName2;
		std::getline(caveStream, caveName1, '-');
		std::getline(caveStream, caveName2, '-');

		ICaveNode* pCave1 = getOrCreateCaveNode(caveName1);
		ICaveNode* pCave2 = getOrCreateCaveNode(caveName2);

		pCave1->m_connectedNodes.push_back(pCave2);
		pCave2->m_connectedNodes.push_back(pCave1);
	}

	// we don't want outgoing connections from the end cave since this is the end of the path
	pEndCave->m_connectedNodes.clear();

	// find paths for puzzle 1
	std::function<void(ICaveNode*)> findPath;
	findPath = [&](ICaveNode* pCave) -> void
	{
		pCave->Visit();
		for(ICaveNode* pNextCave : pCave->m_connectedNodes)
			if(pNextCave->CanVisit())
				findPath(pNextCave);
		pCave->Unvisit();
	};

	findPath(pStartCave);
	const int numPaths1 = pEndCave->m_numVisits;

	std::cout << "Advent of Code Day 12 Puzzle 1" << std::endl;
	std::cout << "Num paths = " << numPaths1 << std::endl;
	std::cout << std::endl;

	// find paths for puzzle 2
	pEndCave->m_numVisits = 0;
	SmallCave::s_allowDoubleVisits = true;

	findPath(pStartCave);
	const int numPaths2 = pEndCave->m_numVisits;

	std::cout << "Advent of Code Day 12 Puzzle 2" << std::endl;
	std::cout << "Num paths2 = " << numPaths2 << std::endl;
	std::cout << std::endl;

	// cleanup
	for(auto caveNodeEntry : caveNodes)
		delete caveNodeEntry.second;
}


void Day13()
{
	std::ifstream dataStream("transparent.txt");

	// index and coord conversion funcs
	constexpr int32_t split = 2000;
	
	const auto getIndex = [split](int32_t x, int32_t y) -> int32_t
	{
		return x + y * split;
	};

	const auto getCoords = [split](int32_t index, int32_t& x, int32_t& y) -> void
	{
		x = index % split;
		y = index / split;
	};

	// get points
	std::set<int32_t> points;
	std::string dataLine;
	while(std::getline(dataStream, dataLine))
	{
		if(dataLine.length() == 0)
			break;

		std::stringstream inputStream(dataLine);
		std::string inputData1;
		std::string inputData2;
		std::getline(inputStream, inputData1, ',');
		std::getline(inputStream, inputData2, ',');
		const int32_t x = std::stoi(inputData1);
		const int32_t y = std::stoi(inputData2);
		assert(x < split);
		assert(y < split);
		points.insert(getIndex(x, y));
	}

	// folding function
	const auto fold = [&]() -> void
	{
		std::stringstream inputStream(dataLine);
		std::string inputData;
		std::getline(inputStream, inputData, ' ');
		std::getline(inputStream, inputData, ' ');

		std::getline(inputStream, inputData, '=');
		const int32_t coordIndex = inputData[0] == 'x' ? 0 : 1;

		std::getline(inputStream, inputData, '=');
		const int32_t plane = std::stoi(inputData);

		std::set<int32_t> newPoints;

		int32_t coords[2] = {};
		for(auto pointIter = points.begin(), last = points.end(); pointIter != last;)
		{
			getCoords(*pointIter, coords[0], coords[1]);
			if(coords[coordIndex] > plane)
			{
				coords[coordIndex] = 2 * plane - coords[coordIndex];
				newPoints.insert(getIndex(coords[0], coords[1]));
				pointIter = points.erase(pointIter);
			}
			else
			{
				++pointIter;
			}
		}

		points.insert(newPoints.begin(), newPoints.end());
	};

	// do first fold
	std::getline(dataStream, dataLine);
	fold();

	std::cout << "Advent of Code Day 13 Puzzle 1" << std::endl;
	std::cout << "Visible dots after first fold = " << points.size() << std::endl;
	std::cout << std::endl;

	// complete folds and output results
	while(std::getline(dataStream, dataLine))
		fold();

	std::cout << "Advent of Code Day 13 Puzzle 2" << std::endl;

	int32_t lastX = 0;
	int32_t lastY = 0;
	for(int32_t point : points)
	{
		int32_t x, y;
		getCoords(point, x, y);

		while(y > lastY)
		{
			++lastY = y;
			lastX = 0;
			std::cout << std::endl;
		}

		while(x > lastX + 1)
		{
			++lastX;
			std::cout << ".";
		}

		lastX = x;
		std::cout << "#";
	}

	std::cout << std::endl;
	std::cout << std::endl;
}


void Day14()
{
	std::ifstream dataStream("polymerization.txt");

	// get polymer
	std::string polymer;
	std::getline(dataStream, polymer);

	// split polymer into pair count map
	struct ElementPair
	{
		ElementPair(char left, char right) : m_leftElement(left), m_rightElement(right) {}
		char m_leftElement;
		char m_rightElement;

		bool operator<(const ElementPair& other) const
		{
			return (m_leftElement != other.m_leftElement) ? m_leftElement < other.m_leftElement : m_rightElement < other.m_rightElement;
		}
	};

	std::map<ElementPair, uint64_t> polymerPairCounts;
	for(uint64_t i = 1; i < polymer.size(); ++i)
	{
		ElementPair elementPair(polymer[i - 1], polymer[i]);
		polymerPairCounts.try_emplace(elementPair, 0);
		++polymerPairCounts[elementPair];
	}

	// get element counts
	uint64_t elementCounts[26] = {};
	for(char element : polymer)
		++elementCounts[element - 'A'];

	// get pair insertions
	struct PairInsertion
	{
		PairInsertion(char left, char right, char insert)
			: m_element(insert)
			, m_inputPair(left, right)
			, m_outputPairLeft(left, insert)
			, m_outputPairRight(insert, right)
		{
		}

		char m_element;
		ElementPair m_inputPair;
		ElementPair m_outputPairLeft;
		ElementPair m_outputPairRight;
	};
	std::vector<PairInsertion> pairInsertions;

	std::string pairInsertionData;
	std::getline(dataStream, pairInsertionData);
	while(std::getline(dataStream, pairInsertionData))
		pairInsertions.push_back(PairInsertion(pairInsertionData[0], pairInsertionData[1], pairInsertionData[6]));

	// insertion function
	const auto insertElements = [&]() -> void
	{
		std::map<ElementPair, uint64_t> prevPolymerPairCounts = polymerPairCounts;
		for(uint64_t pairIndex = 0; pairIndex < pairInsertions.size(); ++pairIndex)
		{
			const PairInsertion& pair = pairInsertions[pairIndex];
			if(prevPolymerPairCounts.find(pair.m_inputPair) != prevPolymerPairCounts.end())
			{
				const uint64_t numPairsInPolymer = prevPolymerPairCounts[pair.m_inputPair];
				polymerPairCounts[pair.m_inputPair] -= numPairsInPolymer;
				if(polymerPairCounts[pair.m_inputPair] == 0)
					polymerPairCounts.erase(pair.m_inputPair);

				polymerPairCounts.try_emplace(pair.m_outputPairLeft, 0);
				polymerPairCounts[pair.m_outputPairLeft] += numPairsInPolymer;

				polymerPairCounts.try_emplace(pair.m_outputPairRight, 0);
				polymerPairCounts[pair.m_outputPairRight] += numPairsInPolymer;

				elementCounts[pair.m_element - 'A'] += numPairsInPolymer;
			}
		}
	};

	// find most common and least common elements function
	const auto findMostAndLeastElementCount = [&](uint64_t& mostCount, uint64_t& leastCount) -> void
	{
		mostCount = 0;
		leastCount = UINT64_MAX;

		for(int i = 0; i < 26; ++i)
		{
			if(elementCounts[i] != 0)
			{
				leastCount = std::min(leastCount, elementCounts[i]);
				mostCount = std::max(mostCount, elementCounts[i]);
			}
		}
	};

	// apply insertions for 10 steps
	for(uint64_t step = 0; step < 10; ++step)
		insertElements();

	uint64_t mostCount10, leastCount10;
	findMostAndLeastElementCount(mostCount10, leastCount10);

	std::cout << "Advent of Code Day 14 Puzzle 1" << std::endl;
	std::cout << "Difference between most and least element after 10 steps = " << mostCount10 - leastCount10 << std::endl;
	std::cout << std::endl;

	// apply remaining 30 steps of insertions
	for(uint64_t step = 0; step < 30; ++step)
		insertElements();

	uint64_t mostCount40, leastCount40;
	findMostAndLeastElementCount(mostCount40, leastCount40);

	std::cout << "Advent of Code Day 14 Puzzle 2" << std::endl;
	std::cout << "Difference between most and least element after 40 steps = " << mostCount40 - leastCount40 << std::endl;
	std::cout << std::endl;
}


void Day15()
{
	std::ifstream dataStream("chitons.txt");

	// get risk grid
	std::vector<int> riskGrid;

	std::string dataLine;
	while(std::getline(dataStream, dataLine))
		for(char ch : dataLine)
			riskGrid.push_back(ch - '0');

	const int riskGridWidth = dataLine.size();
	const int riskGridHeight = riskGrid.size() / riskGridWidth;

	// calculate least risk path using dijkstra's algorithm
	const auto calculateLeastRisk = [](const std::vector<int>& riskGrid, int riskGridWidth, int riskGridHeight) -> int
	{
		const int startIndex = 0;
		const int endIndex = riskGrid.size() - 1;

		const auto coordToIndex = [riskGridWidth](int x, int y) -> int
		{
			return x + y * riskGridWidth;
		};

		const auto indexToCoord = [riskGridWidth](int index, int& x, int& y) -> void
		{
			x = index % riskGridWidth;
			y = index / riskGridWidth;
		};

		struct Node
		{
			int m_totalRisk = INT32_MAX;
			bool m_visited = false;
			bool m_toVisit = false;
		};

		std::vector<Node> nodes;
		nodes.insert(nodes.begin(), riskGrid.size(), Node());

		nodes[startIndex].m_totalRisk = 0;

		std::vector<Node*> toVisitNodes;
		toVisitNodes.push_back(&nodes[startIndex]);
		nodes[startIndex].m_toVisit = true;
	
		while(toVisitNodes.size())
		{
			// find node to visit with lowest risk
			Node* pLowestRiskNode = toVisitNodes[0];
			int lowestRiskUnvisitedIndex = 0;
			for(int i = 1; i < toVisitNodes.size(); ++i)
			{
				if(toVisitNodes[i]->m_totalRisk < pLowestRiskNode->m_totalRisk)
				{
					pLowestRiskNode = toVisitNodes[i];
					lowestRiskUnvisitedIndex = i;
				}
			}

			// visit node
			toVisitNodes[lowestRiskUnvisitedIndex] = toVisitNodes.back();
			toVisitNodes.pop_back();
			pLowestRiskNode->m_visited = true;
			pLowestRiskNode->m_toVisit = false;

			// return total risk if this is the end node
			const int lowestRiskGridIndex = pLowestRiskNode - nodes.data();
			if(lowestRiskGridIndex == endIndex)
				return pLowestRiskNode->m_totalRisk;

			// update path cost to adjacent nodes
			const auto updateAdjacentPath = [&](int ax, int ay, Node* pCurrentNode) -> void
			{
				if(ax < 0 || ax >= riskGridWidth) return;
				if(ay < 0 || ay >= riskGridHeight) return;
				int aGridIndex = coordToIndex(ax, ay);

				Node& adjacentNode = nodes[aGridIndex];
				if(adjacentNode.m_visited == false)
				{
					adjacentNode.m_totalRisk = std::min(adjacentNode.m_totalRisk, pCurrentNode->m_totalRisk + riskGrid[aGridIndex]);
					if(adjacentNode.m_toVisit == false)
					{
						toVisitNodes.push_back(&adjacentNode);
						adjacentNode.m_toVisit = true;
					}
				}
			};

			int x, y;
			indexToCoord(lowestRiskGridIndex, x, y);
			updateAdjacentPath(x - 1, y, pLowestRiskNode);
			updateAdjacentPath(x + 1, y, pLowestRiskNode);
			updateAdjacentPath(x, y - 1, pLowestRiskNode);
			updateAdjacentPath(x, y + 1, pLowestRiskNode);
		}

		return -1;
	};

	const int leastRisk = calculateLeastRisk(riskGrid, riskGridWidth, riskGridHeight);

	std::cout << "Advent of Code Day 15 Puzzle 1" << std::endl;
	std::cout << "Lowest risk path = " << leastRisk << std::endl;
	std::cout << std::endl;

	// expand risk grid
	std::vector<int> expandedRiskGrid;
	expandedRiskGrid.resize(riskGrid.size() * 25);
	const int expandedRiskGridWidth = riskGridWidth * 5;
	const int expandedRiskGridHeight = riskGridHeight * 5;

	for(int y = 0; y < riskGridHeight; ++y)
	{
		for(int x = 0; x < riskGridWidth; ++x)
		{
			const int origRisk = riskGrid[x + y * riskGridWidth];
			for(int my = 0; my < 5; ++my)
			{
				for(int mx = 0; mx < 5; ++mx)
				{
					const int expandedIndex = x + riskGridWidth * mx + (y + riskGridHeight * my) * expandedRiskGridWidth;
					expandedRiskGrid[expandedIndex] = ((origRisk + mx + my - 1) % 9) + 1;
				}
			}
		}
	}

	const int leastRiskExpanded = calculateLeastRisk(expandedRiskGrid, expandedRiskGridWidth, expandedRiskGridHeight);

	std::cout << "Advent of Code Day 15 Puzzle 2" << std::endl;
	std::cout << "Lowest risk path = " << leastRiskExpanded << std::endl;
	std::cout << std::endl;
}


void Day16()
{
	std::ifstream hexDataStream("packetDecoder.txt");
	std::string hexDataLine;
	std::getline(hexDataStream, hexDataLine);

	// convert hex data to binary data
	std::string binaryData;
	binaryData.reserve(hexDataLine.size() * 4);

	for(char ch : hexDataLine)
	{
		std::bitset<4> bits(std::isdigit(ch) ? ch - '0' : ch - 'A' + 10);
		binaryData += bits.to_string();
	}

	// parse functions
	enum class E_PacketType : uint8_t
	{
		Sum,
		Product,
		Minimum,
		Maximum,
		LiteralValue,
		GreaterThan,
		LessThan,
		EqualTo,

		MAX
	};

	uint64_t readHead = 0;
	const auto readData = [&](uint64_t size) -> std::string
	{
		std::string data = binaryData.substr(readHead, size);
		readHead += size;
		return data;
	};

	const auto readPacketVersion = [&]() -> uint8_t
	{
		const std::string packetVersionData = readData(3);
		const uint8_t packetVersion = std::stoi(packetVersionData, nullptr, 2);
		return packetVersion;
	};

	const auto readPacketType = [&]() -> E_PacketType
	{
		const std::string packetTypeIdData = readData(3);
		const uint8_t packetTypeId = std::stoi(packetTypeIdData, nullptr, 2);
		return (E_PacketType)packetTypeId;
	};

	const auto readLiteralValue = [&]() -> uint64_t
	{
		std::string valueBinary;
		bool readDigits = true;
		while(readDigits)
		{
			readDigits = readData(1)[0] == '1';
			valueBinary += readData(4);
		}

		const uint64_t value = std::stoll(valueBinary, nullptr, 2);
		return value;
	};

	// operator functions
	const auto sumOperator = [](uint64_t lhs, uint64_t rhs) -> uint64_t
	{
		return lhs + rhs;
	};

	const auto productOperator = [](uint64_t lhs, uint64_t rhs) -> uint64_t
	{
		return lhs * rhs;
	};

	const auto minimumOperator = [](uint64_t lhs, uint64_t rhs) -> uint64_t
	{
		return lhs < rhs ? lhs : rhs;
	};

	const auto maximumOperator = [](uint64_t lhs, uint64_t rhs) -> uint64_t
	{
		return lhs > rhs ? lhs : rhs;
	};

	const auto greaterThanOperator = [](uint64_t lhs, uint64_t rhs) -> uint64_t
	{
		return lhs > rhs ? 1 : 0;
	};

	const auto lessThanOperator = [](uint64_t lhs, uint64_t rhs) -> uint64_t
	{
		return lhs < rhs ? 1 : 0;
	};

	const auto equalToOperator = [](uint64_t lhs, uint64_t rhs) -> uint64_t
	{
		return lhs == rhs ? 1 : 0;
	};

	const auto nullOperator = [](uint64_t lhs, uint64_t rhs) -> uint64_t
	{
		assert(false);
		return 0;
	};

	std::function<uint64_t(uint64_t, uint64_t)> operatorFunctions[(uint8_t)E_PacketType::MAX] = {
		sumOperator,
		productOperator,
		minimumOperator,
		maximumOperator,
		nullOperator,
		greaterThanOperator,
		lessThanOperator,
		equalToOperator };

	// read packet function
	std::function<uint64_t(uint64_t&)> readPacket;
	readPacket = [&](uint64_t& versionNumberTotal) -> uint64_t
	{
		const uint8_t packetVersion = readPacketVersion();
		versionNumberTotal += packetVersion;

		const E_PacketType packetType = readPacketType();
		switch(packetType)
		{
		case E_PacketType::LiteralValue:
			return readLiteralValue();

		default:
			if(readData(1)[0] == '0')
			{
				const uint64_t numBitsInSubpackets = std::stoi(readData(15), nullptr, 2);
				uint64_t startRead = readHead;
				uint64_t lhs = readPacket(versionNumberTotal);
				while(readHead - startRead < numBitsInSubpackets)
				{
					const uint64_t rhs = readPacket(versionNumberTotal);
					lhs = operatorFunctions[(uint8_t)packetType](lhs, rhs);
				}
				return lhs;
			}
			else
			{
				const uint64_t numSubpackets = std::stoi(readData(11), nullptr, 2);
				uint64_t lhs = readPacket(versionNumberTotal);
				for(uint64_t i = 1; i < numSubpackets; ++i)
				{
					const uint64_t rhs = readPacket(versionNumberTotal);
					lhs = operatorFunctions[(uint8_t)packetType](lhs, rhs);
				}
				return lhs;
			}
			break;
		}
	};

	// parse packets
	uint64_t versionNumberTotal = 0;
	uint64_t packetValue = readPacket(versionNumberTotal);

	std::cout << "Advent of Code Day 16 Puzzle 1" << std::endl;
	std::cout << "Version Number Total = " << versionNumberTotal << std::endl;
	std::cout << std::endl;

	std::cout << "Advent of Code Day 16 Puzzle 2" << std::endl;
	std::cout << "Packet value = " << packetValue << std::endl;
	std::cout << std::endl;
}


void Day17()
{
	constexpr int targetMinX = 81;
	constexpr int targetMaxX = 129;
	constexpr int targetMinY = -150;
	constexpr int targetMaxY = -108;

	// The probe is garaunteed to hit y = 0 with the negative of its launch speed on the way down.
	// How large can we make this speed and place ourselves in the target?
	// This will be the distance to the lowest target Y minus one for added gravity
	const int maxLaunchSpeedY = -targetMinY - 1;

	// what is the highest y we can reach
	int highestYPos = 0;
	int ySpeed = maxLaunchSpeedY;
	while(ySpeed > 0)
	{
		highestYPos += ySpeed;
		--ySpeed;
	}

	std::cout << "Advent of Code Day 17 Puzzle 1" << std::endl;
	std::cout << "Highest Y Pos = " << highestYPos << std::endl;
	std::cout << std::endl;

	// the mininum launch y speed takes us to the furthest edge of the target in one step
	const int minLaunchSpeedY = targetMinY;

	// the maximum x launch speed takes us to the furthest target x within one step
	int maxLaunchSpeedX = targetMaxX;

	// calculate number of valid launches
	int numValidLanuches = 0;
	for(int launchX = 1; launchX <= maxLaunchSpeedX; ++launchX)
	{
		for(int launchY = minLaunchSpeedY; launchY <= maxLaunchSpeedY; ++launchY)
		{
			int xPos = 0;
			int yPos = 0;
			int xSpeed = launchX;
			int ySpeed = launchY;

			while(xPos < targetMaxX && yPos > targetMinY)
			{
				xPos += xSpeed;
				yPos += ySpeed;

				if(xPos >= targetMinX && xPos <= targetMaxX
					&& yPos >= targetMinY && yPos <= targetMaxY)
				{
					++numValidLanuches;
					break;
				}

				xSpeed = std::max(xSpeed - 1, 0);
				--ySpeed;
			}
		}
	}

	std::cout << "Advent of Code Day 17 Puzzle 2" << std::endl;
	std::cout << "Num valid launches = " << numValidLanuches << std::endl;
	std::cout << std::endl;
}



int main()
{
	/*
	Day01Puzzle1();
	Day01Puzzle2();
	Day02Puzzle1();
	Day02Puzzle2();
	Day03Puzzle1();
	Day03Puzzle2();
	Day04();
	Day05();
	Day06();
	Day07();
	Day08();
	Day09();
	Day10();
	Day11();
	Day12();
	Day13();
	Day14();
	Day15();
	Day16();
	*/
	Day17();
}

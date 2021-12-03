#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <vector>



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
	{
		uint16_t sample = 0;
		for(uint16_t i = 0; i < numBits; ++i)
			if(sampleText[i] == '1')
				sample |= 1 << (numBits - i - 1);
		samples.push_back(sample);
	}

	// sort according to bits
	std::sort(samples.begin(), samples.end(), [numBits](const uint16_t& a, const uint16_t& b) -> bool
	{
		for(uint16_t i = 0; i < numBits; ++i)
		{
			const uint16_t bitMask = 1 << (numBits - i - 1);
			const bool aSet = a & bitMask;
			const bool bSet = b & bitMask;
			
			if(aSet != bSet)
				return aSet;
		}
		return true;
	});

	// find oxygen rating
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
		if(numSetSamples >= samplesInRange - numSetSamples)
			endIndex = splitIndex;
		else
			startIndex = splitIndex;

		--bitIndex;
	}

	const uint16_t oxygenRating = samples[startIndex];

	// find co2 rating
	startIndex = 0;
	endIndex = (uint16_t)samples.size();
	bitIndex = numBits - 1;

	while(startIndex + 1 != endIndex)
	{
		const uint16_t bitMask = 1 << bitIndex;
		const uint16_t samplesInRange = endIndex - startIndex;

		uint16_t splitIndex = 0;
		for(splitIndex = startIndex; splitIndex < endIndex; ++splitIndex)
			if((samples[splitIndex] & bitMask) == 0)
				break;

		const uint16_t numSetSamples = splitIndex - startIndex;
		if(numSetSamples < samplesInRange - numSetSamples)
			endIndex = splitIndex;
		else
			startIndex = splitIndex;

		--bitIndex;
	}

	const uint16_t co2Rating = samples[startIndex];
	
	std::cout << "Advent of Code Day 3 Puzzle 2" << std::endl;
	std::cout << "Life support rating = " << oxygenRating * co2Rating << std::endl;
	std::cout << std::endl;
}



int main()
{
	Day01Puzzle1();
	Day01Puzzle2();
	Day02Puzzle1();
	Day02Puzzle2();
	Day03Puzzle1();
	Day03Puzzle2();
}

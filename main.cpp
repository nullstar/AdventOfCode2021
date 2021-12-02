#include <string>
#include <fstream>
#include <iostream>



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



int main()
{
	Day01Puzzle1();
	Day01Puzzle2();
	Day02Puzzle1();
	Day02Puzzle2();
}
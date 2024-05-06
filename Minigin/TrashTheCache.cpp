#include "TrashTheCache.h"
#include <chrono>

dae::TrashTheCache::TrashTheCache()
{
}

dae::TrashTheCache::~TrashTheCache()
{
}

std::vector<float> dae::TrashTheCache::Exercise1(const int samples,const int amountOfSteps)
{
	const int arrSize = 10000000;
	int* arr = new int[arrSize];

	std::vector<float> totalTimes;
	std::vector<float> averageTimes;

	for (int i = 0; i < samples; i++)
	{
		for (int stepsize = 1; stepsize <= pow(2,amountOfSteps); stepsize *= 2)
		{
			auto start = std::chrono::high_resolution_clock::now();
			for (int i2 = 0; i2 < arrSize; i2 += stepsize)
			{
				arr[i2] *= 2;
			}
			auto end = std::chrono::high_resolution_clock::now();
			auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
			totalTimes.emplace_back(static_cast<float>(duration));
		}

		averageTimes.emplace_back(0.0f);
	}
	
	for (int i = 0; i < static_cast<int>(averageTimes.size()); i++)
	{
		averageTimes[i % samples] += totalTimes[i];
	}
	return averageTimes;
}

std::vector<float> dae::TrashTheCache::Exercise2(const int samples, const int amountOfSteps)
{
	const int arrSize = 10000000;
	GameObject3D* arr = new GameObject3D[arrSize];

	std::vector<float> totalTimes;
	std::vector<float> averageTimes;

	for (int i = 0; i < samples; i++)
	{
		for (int stepsize = 1; stepsize <= pow(2, amountOfSteps); stepsize *= 2)
		{
			auto start = std::chrono::high_resolution_clock::now();
			for (int i2 = 0; i2 < arrSize; i2 += stepsize)
			{
				arr[i2].id *= 2;
			}
			auto end = std::chrono::high_resolution_clock::now();
			auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
			totalTimes.emplace_back(static_cast<float>(duration));
		}

		averageTimes.emplace_back(0.0f);
	}

	for (int i = 0; i < static_cast<int>(averageTimes.size()); i++)
	{
		averageTimes[i % samples] += totalTimes[i];
	}
	return averageTimes;
}

std::vector<float> dae::TrashTheCache::Exercise2Alt(const int samples, const int amountOfSteps)
{
	const int arrSize = 10000000;
	GameObject3DAlt* arr = new GameObject3DAlt[arrSize];

	std::vector<float> totalTimes;
	std::vector<float> averageTimes;

	for (int i = 0; i < samples; i++)
	{
		for (int stepsize = 1; stepsize <= pow(2, amountOfSteps); stepsize *= 2)
		{
			auto start = std::chrono::high_resolution_clock::now();
			for (int i2 = 0; i2 < arrSize; i2 += stepsize)
			{
				arr[i2].id *= 2;
			}
			auto end = std::chrono::high_resolution_clock::now();
			auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
			totalTimes.emplace_back(static_cast<float>(duration));
		}

		averageTimes.emplace_back(0.0f);
	}

	for (int i = 0; i < static_cast<int>(averageTimes.size()); i++)
	{
		averageTimes[i % samples] += totalTimes[i];
	}
	return averageTimes;
}


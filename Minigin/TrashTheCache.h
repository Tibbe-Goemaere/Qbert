#pragma once
#include <vector>

namespace dae
{
	

	struct transform {
		float matrix[16] = {
			1,0,0,0,
			0,1,0,0,
			0,0,1,0,
			0,0,0,1
		};
	};

	class GameObject3D
	{
	public:
		transform local;
		int id;
	};

	class GameObject3DAlt
	{
	public:
		transform* local;
		int id;
	};

	class TrashTheCache final
	{
	public:
		TrashTheCache();
		~TrashTheCache();
		TrashTheCache(const TrashTheCache& other) = delete;
		TrashTheCache(TrashTheCache&& other) = delete;
		TrashTheCache& operator=(const TrashTheCache& other) = delete;
		TrashTheCache& operator=(TrashTheCache&& other) = delete;

		static std::vector<float> Exercise1(const int samples, const int amountOfSteps = 10);
		static std::vector<float> Exercise2(const int samples, const int amountOfSteps = 10);
		static std::vector<float> Exercise2Alt(const int samples, const int amountOfSteps = 10);
	private:


	};
}
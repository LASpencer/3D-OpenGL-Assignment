#pragma once
#include <vector>
#include <algorithm>

namespace las {
	template <typename T> inline void removeEraseFromVector(std::vector<T>& vec, T entry)
	{
		vec.erase(std::remove(vec.begin(), vec.end(), entry), vec.end());
	}
}
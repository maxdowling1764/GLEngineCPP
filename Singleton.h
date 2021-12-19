#pragma once
template <typename T>
class Singleton
{
	static T* instance;

	static T* getInstance()
	{
		if (instance == nullptr)
		{
			instance = new T;
		}
		return instance;
	}
};
#include "Singleton.hpp"

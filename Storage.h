#pragma once
#include <iostream>
#include<string>
using namespace std;

template <typename T>
class Storage
{
	int capacity;
	int countStorage;
	T* data;

public:
	Storage()
	{
		capacity = 100;
		countStorage = 0;
		data = new T[capacity];
	}

	Storage(int size)
	{
		capacity = size;
		countStorage = 0;
		data = new T[capacity];
	}
	~Storage()
	{
		delete[] data;
	}

	Storage(const Storage& ) = delete;

	Storage& operator=(const Storage& ) = delete;

	void add(const T& item)
	{
		ensureCapacity();
		data[countStorage] = item;
		countStorage++;
	}

	void ensureCapacity()
	{
		if (countStorage == capacity)
		{
			capacity *= 2;
			T* newData = new T[capacity];
			for (int i = 0; i < countStorage; i++)
			{
				newData[i] = data[i];
			}
			delete[] data;
			data = newData;
		}
	}

	void removeById(string id)
	{
		int indexRemovingIs = -1;

		for (int i = 0; i < countStorage; i++)
		{
			if (data[i].getID() == id)
			{
				indexRemovingIs = i;
				break;
			}
		}

		if (indexRemovingIs != -1)
		{
			for (int i = indexRemovingIs; i < countStorage - 1; i++)
			{
				data[i] = data[i + 1];
			}
			countStorage--;
			cout << "Item with ID \"" << id << "\" is successfully removed!" << endl;
		}

		else
		{
			cout << "Failed to remove item with ID \"" << id << "\". Because no such ID exist!" << endl;
		}
	}

	T* findById(string id) const
	{
		for (int i = 0; i < countStorage; i++)
		{
			if (data[i].getID() == id)
			{
				return &data[i];
			}
		}
		return nullptr;
	}

	T* getAT(int index) const
	{
		if (index >= 0 && index < countStorage)
		{
			return&data[index];
		}
		return nullptr;
	}

	int getCount() const
	{
		return countStorage;
	}
};
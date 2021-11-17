#pragma once
#include <vector>

using namespace std;

class CompManager
{
private:
	static int _size;
	static int genNewTypeID() { return _size++; }
	template<typename T, typename T2, typename... TRest>
	static vector<bool>& getKey(vector<bool>& key)
	{
		key[getTypeID<T>()] = true;
		return getKey<T2, TRest...>(key);
	}
	template<typename T>
	static vector<bool>& getKey(vector<bool>& key)
	{
		key[getTypeID<T>()] = true;
		return key;
	}
public:
	static int size() { return _size; }
	template<typename T> 
	static int getTypeID()
	{
		static int typeID = genNewTypeID();
		return typeID;
	}
	template<typename T, typename T2, typename... TRest>
	static vector<bool> getKey()
	{
		vector<bool> key(_size);
		key[getTypeID<T>()] = true;
		return getKey<T2, TRest...>(key);
	}
	template<typename T>
	static vector<bool> getKey()
	{
		vector<bool> key(_size);
		key[getTypeID<T>()] = true;
		return key;
	}
};
typedef CompManager CmpMngr;

struct Component
{
private:

public:
	virtual Component* clone() { return new Component(); }
};
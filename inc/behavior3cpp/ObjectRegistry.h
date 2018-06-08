#ifndef MANGOS_OBJECTREGISTRY_H
#define MANGOS_OBJECTREGISTRY_H

#include <string>
#include <unordered_map>

template<class T, class Key = std::string>
class ObjectRegistry
{
public:
	typedef std::unordered_map<Key, T*> RegistryMapType;
	ObjectRegistry(){}
	~ObjectRegistry()
	{
		for (typename RegistryMapType::iterator it = i_registeredObjects.begin(); it != i_registeredObjects.end(); ++it)
		{
			delete it->second;
		}
		i_registeredObjects.clear();
	}

	const T* GetRegistryItem(Key key) const
	{
		typename RegistryMapType::const_iterator it = i_registeredObjects.find(key);
		return it == i_registeredObjects.end() ? nullptr : it->second;
	}

	bool InsertItem(T* obj, Key key, bool override = false)
	{
		typename RegistryMapType::iterator it = i_registeredObjects.find(key);
		if (it != i_registeredObjects.end())
		{
			if (!override)
				return false;
			delete it->second;
			i_registeredObjects.erase(it);
		}
		i_registeredObjects[key] = obj;
		return true;
	}

	void RemoveItem(Key key, bool delete_obj = true)
	{
		typename RegistryMapType::iterator it = i_registeredObjects.find(key);
		if (it != i_registeredObjects.end())
		{
			if (delete_obj)
				delete it->second;
			i_registeredObjects.erase(it);
		}
	}

	bool HasItem(Key key) const
	{
		return i_registeredObjects.find(key) != i_registeredObjects.end();
	}

private:
	RegistryMapType i_registeredObjects;
};

#endif
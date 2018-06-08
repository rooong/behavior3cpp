#ifndef MANGOS_FACTORY_HOLDER
#define MANGOS_FACTORY_HOLDER

#include "ObjectRegistry.h"

template<class T, class Key = std::string>
class FactoryHolder
{
public:
	typedef ObjectRegistry<FactoryHolder< T, Key>, Key> FactoryHolderRegistry;

	static FactoryHolderRegistry& getInstance(){
		static FactoryHolderRegistry r;
		return r;
	}

	FactoryHolder(Key k) :i_key(k){}
	virtual ~FactoryHolder(){}
	inline Key key() const { return i_key; }

	void RegisterSelf(bool override = true) { getInstance().InsertItem(this, i_key, override); }
	void DeregisterSelf() { getInstance().RemoveItem(i_key, false); }
	void Register(FactoryHolderRegistry& r, bool override = true) { r.InsertItem(this, i_key, override); }
	void Deregister(FactoryHolderRegistry& r) { r.RemoveItem(i_key, false); }
	virtual T* Create(void* data1 = nullptr, void* data2 = nullptr) const = 0;

private:
	Key i_key;
};

#endif
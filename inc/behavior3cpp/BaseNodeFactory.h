#ifndef BASE_NODE_FACTORY_H
#define BASE_NODE_FACTORY_H

#include "b3.hpp"
#include "core/BaseNode.hpp"
#include "FactoryHolder.h"

NS_B3_BEGIN

typedef FactoryHolder<BaseNode> BaseNodeCreator;
typedef FactoryHolder<BaseNode>::FactoryHolderRegistry BaseNodeRegistry;

template<class REAL_NODE>
struct BaseNodeFactory : public FactoryHolder < BaseNode >
{
	BaseNodeFactory(const char* node_name) : FactoryHolder < BaseNode >(node_name){}
	BaseNode* Create(void* data, void*) const
	{
		b3::Value* properties = (b3::Value*)(data);
		return new REAL_NODE(properties);
	}
};

template<class T>
void registerNode(BaseNodeRegistry& r, const char* node_name){
	if (node_name == nullptr)
		return;
	(new BaseNodeFactory<T>(node_name))->Register(r);
}

NS_B3_END

#endif
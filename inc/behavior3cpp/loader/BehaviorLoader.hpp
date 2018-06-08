//
//  BehaviorLoader.hpp
//  behavior3cpp
//
//  Created by luozhipeng on 16/1/6.
//  Copyright © 2016年 luozhipeng. All rights reserved.
//

#ifndef BehaviorLoader_hpp
#define BehaviorLoader_hpp

#include "core/BehaviorTree.hpp"
#include "core/Value.hpp"
#include "core/BaseNode.hpp"
#include "BaseNodeFactory.h"

NS_B3_BEGIN

class BehaviorLoader{

public:
	BehaviorLoader();
public:
	virtual bool loadFromJson(BehaviorTree* tree, const std::string& jsonFile) = 0;
	template<class T>
	void registerNodeToLoader(const char* node_name)
	{
		registerNode<T>(r, node_name);
	}
protected:
    virtual BaseNode* createNodeByName(const std::string& name, Value* properties);
	void registerBaseNode();

private:
	BaseNodeRegistry r;
};

NS_B3_END

#endif /* BehaviorLoader_hpp */

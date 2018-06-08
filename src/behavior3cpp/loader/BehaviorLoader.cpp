//
//  BehaviorLoader.cpp
//  behavior3cpp
//
//  Created by luozhipeng on 16/1/6.
//  Copyright © 2016年 luozhipeng. All rights reserved.
//

#include "loader/BehaviorLoader.hpp"
#include "composites/Sequence.hpp"
#include "composites/Priority.hpp"
#include "composites/Parallel.hpp"
#include "composites/MemSequence.hpp"
#include "composites/MemPriority.hpp"
#include "composites/MemRanPriority.hpp"
#include "actions/Succeeder.hpp"
#include "actions/Failer.hpp"
#include "actions/Runner.hpp"
#include "actions/Error.hpp"
#include "actions/Wait.hpp"
#include "decorators/Inverter.hpp"
#include "decorators/Limiter.hpp"
#include "decorators/MaxTime.hpp"
#include "decorators/Repeater.hpp"
#include "decorators/RanRepeater.hpp"
#include "decorators/RepeatUntilFailure.hpp"
#include "decorators/RepeatUntilSuccess.hpp"
#include <assert.h>

NS_B3_BEGIN

BehaviorLoader::BehaviorLoader()
{
	registerBaseNode();
}

BaseNode* BehaviorLoader::createNodeByName(const std::string& name, Value* properties){
    BaseNode* ret = nullptr;

	const BaseNodeCreator* node_registry = r.GetRegistryItem(name);
	if (node_registry == nullptr)
	{
		return nullptr;
	}
	ret = node_registry->Create((void*)properties);
    return ret;
}

void BehaviorLoader::registerBaseNode()
{
	registerNode<Sequence>(r, "Sequence");
	registerNode<Priority>(r, "Priority");
	registerNode<Parallel>(r, "Parallel");
	registerNode<MemSequence>(r, "MemSequence");
	registerNode<MemPriority>(r, "MemPriority");
	registerNode<MemRanPriority>(r, "MemRanPriority");
	registerNode<Succeeder>(r, "Succeeder");
	registerNode<Failer>(r, "Failer");
	registerNode<Runner>(r, "Runner");
	registerNode<Error>(r, "Error");
	registerNode<Wait>(r, "Wait");
	registerNode<Inverter>(r, "Inverter");
	registerNode<Limiter>(r, "Limiter");
	registerNode<MaxTime>(r, "MaxTime");
	registerNode<Repeater>(r, "Repeater");
	registerNode<RanRepeater>(r, "RanRepeater");
	registerNode<RepeatUntilFailure>(r, "RepeatUntilFailure");
	registerNode<RepeatUntilSuccess>(r, "RepeatUntilSuccess");
}

NS_B3_END
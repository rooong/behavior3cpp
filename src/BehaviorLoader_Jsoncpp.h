#ifndef BEHAVIOR_LOADER_JSONCPP_H
#define BEHAVIOR_LOADER_JSONCPP_H

#include "behavior3cpp/b3.hpp"
#include "behavior3cpp/loader/BehaviorLoader.hpp"
#include "json/json.h"

class BehaviorLoader_Jsoncpp : public b3::BehaviorLoader
{
public:
	virtual bool loadFromJson(b3::BehaviorTree* tree, const std::string& jsonFile);
	virtual b3::BaseNode* createNodeByName(const std::string& name, Json::Value& v);
};

#endif
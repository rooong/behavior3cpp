#ifndef BEHAVIOR_LOADER_RAPIDJSON_H
#define BEHAVIOR_LOADER_RAPIDJSON_H

#include "behavior3cpp/b3.hpp"
#include "behavior3cpp/loader/BehaviorLoader.hpp"
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"

class BehaviorLoader_Rapidjson : public b3::BehaviorLoader
{
public:
	virtual bool loadFromJson(b3::BehaviorTree* tree, const std::string& jsonFile);
	virtual b3::BaseNode* createNodeByName(const std::string& name, rapidjson::Value& v);
};

#endif
#include "BehaviorLoader_Jsoncpp.h"
#include "behavior3cpp/core/Composite.hpp"
#include "behavior3cpp/core/Decorator.hpp"
#include <iostream>
#include <fstream>

#pragma comment(lib, "../lib/jsoncpp.lib")

using namespace b3;

b3::BaseNode* BehaviorLoader_Jsoncpp::createNodeByName(const std::string& name, Json::Value& v)
{
	b3::Value properties;
	properties.setType(b3::Value::Type::MAP);
	if (v.isMember("properties"))
	{
		Json::Value& js_properties = v["properties"];
		if (js_properties.isObject())
		{
			for (auto it = js_properties.begin(); it != js_properties.end(); ++it)
			{
				b3::Value v;
				switch (it->type()) {
				case Json::nullValue:
					break;
				case Json::booleanValue:
					v.setBool(it->asBool());
					break;
				case Json::arrayValue:
					break;
				case Json::objectValue:
					break;
				case Json::realValue:
					v.setFloat(it->asDouble());
					break;
				case Json::stringValue:
					v.setString(it->asString());
					break;
				case Json::uintValue:
				case Json::intValue:
					v.setInt(it->asInt());
					break;
				default:
					break;
				}
				if (!v.isNull())
				{
					std::string key = it.name();
					properties.asMap().insert(std::make_pair(key, v));
				}
			}
		}
	}
	return b3::BehaviorLoader::createNodeByName(name, &properties);
}

bool BehaviorLoader_Jsoncpp::loadFromJson(b3::BehaviorTree* tree, const std::string& jsonFile)
{
	//1.load jsonFile
	Json::Value root;
	Json::Reader reader;
	std::ifstream ifs(jsonFile.c_str());//open file example.json
	if (!reader.parse(ifs, root)){
		return false;
	}
	auto& nodes = root["nodes"];
	std::map<std::string, BaseNode*> nodelist;
	for (auto iter = nodes.begin(); iter != nodes.end(); ++iter)
	{
		auto& key = (iter).key();
		auto& node = *iter;
		std::string nodeName = node["name"].asCString();
		BaseNode* btNode = createNodeByName(nodeName, node);
		if (btNode)
		{
			btNode->setId(node["id"].asCString());
			btNode->setTitle(node["title"].asCString());
			btNode->setDescription(node["description"].asCString());
			nodelist[btNode->getId()] = btNode;
		}
	}
	for (auto iter = nodes.begin(); iter != nodes.end(); ++iter)
	{
		auto& node = *iter;
		auto it = nodelist.find(node["id"].asString());
		if (it == nodelist.end())
			continue;
		BaseNode* btNode = it->second;
		if (btNode == nullptr)
		{
			continue;
		}
		if (btNode->getCategory() == COMPOSITE && node.isMember("children"))
		{
			auto& childs = node["children"];
			b3::Composite* comp = (b3::Composite*)btNode;
			for (size_t i = 0; i < childs.size(); i++)
			{
				auto& child = childs[i];
				auto it = nodelist.find(child.asString());
				if (it == nodelist.end())
					continue;
				comp->pushChild(it->second);
			}
		}
		if (btNode->getCategory() == DECORATOR && node.isMember("child"))
		{
			auto& child = node["child"];
			if (child != Json::Value::null)
			{
				b3::Decorator* dec = (b3::Decorator*)btNode;
				auto it = nodelist.find(child.asString());
				if (it != nodelist.end())
				{
					dec->setChild(it->second);
				}
			}
		}
	}
	std::string root_id = root["root"].asString();
	tree->setRoot(nodelist[root_id]);
	return true;
}

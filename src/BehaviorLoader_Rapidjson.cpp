#include "BehaviorLoader_Rapidjson.h"
#include "rapidjson/reader.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/filereadstream.h"

#include "behavior3cpp/core/Composite.hpp"
#include "behavior3cpp/core/Decorator.hpp"

using namespace b3;

b3::BaseNode* BehaviorLoader_Rapidjson::createNodeByName(const std::string& name, rapidjson::Value& v)
{
	b3::Value properties;
	properties.setType(b3::Value::Type::MAP);
	if (v.HasMember("properties"))
	{
		rapidjson::Value& js_properties = v["properties"];
		for (auto it = js_properties.MemberBegin(); it != js_properties.MemberEnd(); ++it)
		{
			b3::Value v;
			rapidjson::Value& pv = it->value;
			switch (it->value.GetType()) {
			case rapidjson::kNullType:
				break;
			case rapidjson::kFalseType:
			case rapidjson::kTrueType:
				v.setBool(pv.GetBool());
				break;
			case rapidjson::kObjectType:
				break;
			case rapidjson::kArrayType:
				break;
			case rapidjson::kStringType:
				v.setString(pv.GetString());
				break;
			case rapidjson::kNumberType:
				v.setInt(pv.GetInt());
				break;
			default:
				break;
			}
			if (!v.isNull())
			{
				std::string key = it->name.GetString();
				properties.asMap().insert(std::make_pair(key, v));
			}
		}
	}
	return b3::BehaviorLoader::createNodeByName(name, &properties);
}

bool BehaviorLoader_Rapidjson::loadFromJson(b3::BehaviorTree* tree, const std::string& jsonFile)
{
	//1.load jsonFile
	rapidjson::Document newDoc;
	FILE* fp = fopen(jsonFile.c_str(), "rb"); // 非 Windows 平台使用 "r"
	char readBuffer[65536];
	rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
	newDoc.ParseStream(is);
	fclose(fp);
	//判断解析从流中读取的字符串是否有错误
	if (newDoc.HasParseError()) {
		//std::cout << "Json Parse error:" << newDoc.GetParseError(); //打印错误编号
		return false;
	}
	auto& nodes = newDoc["nodes"];
	std::map<std::string, b3::BaseNode*> nodelist;
	for (auto iter = nodes.MemberBegin(); iter != nodes.MemberEnd(); ++iter)
	{
		std::string nodeid = iter->name.GetString();
		auto& node = iter->value;
		std::string nodeName = node["name"].GetString();
		b3::BaseNode* btNode = createNodeByName(nodeName, node);
		if (btNode)
		{
			btNode->setId(node["id"].GetString());
			btNode->setTitle(node["title"].GetString());
			btNode->setDescription(node["description"].GetString());
			nodelist[btNode->getId()] = btNode;
		}
	}

	//Connect the nodes
	for (auto iter = nodes.MemberBegin(); iter != nodes.MemberEnd(); ++iter)
	{
		auto& node = iter->value;
		auto it = nodelist.find(node["id"].GetString());
		if (it == nodelist.end())
			continue;
		b3::BaseNode* btNode = it->second;
		if (btNode == nullptr)
		{
			continue;
		}
		if (btNode->getCategory() == COMPOSITE && node.HasMember("children"))
		{
			b3::Composite* comp = (b3::Composite*)btNode;
			auto& childs = node["children"];
			for (size_t i = 0; i < childs.Size(); i++)
			{
				auto& child = childs[i];
				auto it = nodelist.find(child.GetString());
				if (it == nodelist.end())
					continue;
				comp->pushChild(it->second);
			}
			
		}
		if (btNode->getCategory() == DECORATOR && node.HasMember("child"))
		{
			b3::Decorator* dec = (b3::Decorator*)btNode;
			auto& child = node["child"];
			auto it = nodelist.find(child.GetString());
			if (it != nodelist.end())
			{
				dec->setChild(it->second);
			}
		}
	}
	std::string root_id = newDoc["root"].GetString();
	tree->setRoot(nodelist[root_id]);
	return true;
}

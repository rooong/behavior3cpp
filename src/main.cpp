#include <stdio.h>
#include "behavior3cpp/b3.hpp"
#include "BehaviorLoader_Jsoncpp.h"
#include "BehaviorLoader_Rapidjson.h"
#include "behavior3cpp/core/Action.hpp"
#pragma comment(lib, "../lib/behavior3cpp.lib")
#include <windows.h>

using namespace b3;

class TestLog : public b3::Action
{
public:
	TestLog();
	TestLog(Value* properties);
	virtual ~TestLog();

protected:
	virtual State tick(Tick* tick) override;

private:
	std::string info;
};

TestLog::TestLog()
{
	m_name = "Log";
}

TestLog::TestLog(Value* properties)
{
	m_name = "Log";
	if (properties->asMap().empty())
		return;
	auto& v = properties->asMap()["info"];
	info = v.asString();
}

TestLog::~TestLog()
{

}

State TestLog::tick(Tick *tick){
	printf("logtest:%s\n", this->info.c_str());
	return b3::SUCCESS;
}

std::string GetCurrentPath()
{
	char szPath[MAX_PATH] = { 0 };
	GetModuleFileNameA(NULL, szPath, MAX_PATH);
	std::string curPath = szPath;
	curPath = curPath.substr(0, curPath.rfind('\\') + 1);
	return curPath;
}

int main()
{
	BehaviorLoader_Jsoncpp loader;
	BehaviorLoader_Rapidjson loader1;
	//自定义节点注册
	loader.registerNodeToLoader<TestLog>("Log");
	loader1.registerNodeToLoader<TestLog>("Log");
	BehaviorTree bt;
	BehaviorTree bt1;
	//载入
	char szPath[MAX_PATH] = { 0 };
	GetModuleFileNameA(NULL, szPath, MAX_PATH);
	std::string curPath = GetCurrentPath();
	std::string filePath = curPath + "tree.json";
	loader.loadFromJson(&bt, filePath);
	loader1.loadFromJson(&bt1, filePath);
	//输入板
	Blackboard* board = new Blackboard();
	//循环每一帧
	for (int i = 0; i < 5; ++i){
		bt.tick(nullptr, board, i);
	}
	system("pause");
	//循环每一帧
	for (int i = 0; i < 5; ++i){
		bt1.tick(nullptr, board, i);
	}
	system("pause");
	return 0;
}
#pragma once

#include "Service.h"
#include "SocketWorker.h"

class TaskMonkey:
	public Service::Monkey
{
public:
	TaskMonkey();
	~TaskMonkey();

	virtual Json::Value doGet(Json::Value props, SocketWorker& socketWorker);
};

#pragma once

#include <stdhdr.h>

class CJob
{
public:
	CJob();
	virtual ~CJob();
	virtual void Run() = 0;
};

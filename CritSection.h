#pragma once
#include <Windows.h>

class CritSection
{
public:
	CritSection();
	~CritSection();
	void enter();
	void exit();

private:
	CRITICAL_SECTION CriticalSection;
};


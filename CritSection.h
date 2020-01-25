#pragma once
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


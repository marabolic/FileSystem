#include "CritSection.h"
#include <Windows.h>

CritSection::CritSection() {
	InitializeCriticalSection(&CriticalSection);
}

CritSection::~CritSection() {
	DeleteCriticalSection(&CriticalSection);
}

void CritSection::enter() {
	EnterCriticalSection(&CriticalSection);
}
void CritSection::exit() {
	LeaveCriticalSection(&CriticalSection);
}

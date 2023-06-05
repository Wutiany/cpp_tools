#pragma once
#include <Windows.h>
#include <thread>
#include <mutex>
#include <iostream>
#pragma comment (lib, "Advapi32.lib")
#pragma comment (lib, "user32.lib")

bool rebootSystem();

struct TIME
{
	WORD wYear;
	WORD wMonth;
	WORD wDay;
	WORD wDayofWeek;
	WORD wHour;
	WORD wMinute;
	WORD wSecond;
	WORD wMilliseconds;
};

class Reboot_win
{
public:
	Reboot_win();
	bool rebootSystem();
	bool timedShutDownForYear(TIME*);
	bool timedShutDownEveryday(TIME*);
	void printCurrentTime();

private:
	bool timing(TIME*);
	SYSTEMTIME now;
	TIME timer;

};


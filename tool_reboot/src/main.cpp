#include <iostream>
#include "../include/reboot_win.h"


int main() {
	struct TIME timer;
	timer.wHour = WORD(22);
	timer.wMinute = WORD(38);
	Reboot_win rebootTimer;
	rebootTimer.timedShutDownEveryday(&timer);
	return 0;
}
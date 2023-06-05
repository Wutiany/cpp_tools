#include "../include/reboot_win.h"

bool rebootSystem() {
	HANDLE hToken;
	TOKEN_PRIVILEGES tkp;

	// 获取进程标识
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) {
		return false;
	}

	// 获取关机特权的LUID
	// SE_SHUTDOWN 关闭本地系统的权限
	LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);
	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	// 获取进程的关机权限
	AdjustTokenPrivileges(hToken, false, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0);
	if (GetLastError() != ERROR_SUCCESS) {
		return false;
	}

	// 强制重启计算机
	//ExitWindowsEx(EWX_SHUTDOWN | EWX_FORCE, 0);  // 关机
	if (!ExitWindowsEx(EWX_REBOOT | EWX_FORCE, 0)) {
		// 如果使用EWX_REBOOT,所调用的进程必须有SE_SHUTDOWN_NAME权限，所以开始就获取进程句柄，然后赋值权限了
		return false;
	}
	return false;
}

// 从后向前理解，ExitWindowsEx是让windows系统重启或者关机的api，但是关机的EWX_REBOOT参数的使用是需要进程获取权限的
// 进而有了上面的给进程进行附加权限  AdjustTokenPrivileges，将token优先级权限赋给进程的句柄,但是赋值需要程序的句柄，就有了下一步的获取程序句柄
// LookupPrivilegeValue是给Token优先级进行赋权限的，为后续给进行句柄赋值做准备
// 最先就是获取程序句柄， OpenProcessToken将获取的当前进程的句柄赋值给事先定义的句柄
// 通过GetCurrentProcess来获取当前进程的句柄

Reboot_win::Reboot_win() {
	GetLocalTime(&now);
}


bool Reboot_win::rebootSystem() {
	HANDLE hToken;
	TOKEN_PRIVILEGES tkp;

	// 获取进程标识
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) {
		return false;
	}

	// 获取关机特权的LUID
	// SE_SHUTDOWN 关闭本地系统的权限
	LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);
	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	// 获取进程的关机权限
	AdjustTokenPrivileges(hToken, false, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0);
	if (GetLastError() != ERROR_SUCCESS) {
		return false;
	}

	// 强制重启计算机
	//ExitWindowsEx(EWX_REBOOT | EWX_FORCE, 0);  // 关机
	if (!ExitWindowsEx(EWX_SHUTDOWN | EWX_FORCE, 0)) {
		// 如果使用EWX_REBOOT,所调用的进程必须有SE_SHUTDOWN_NAME权限，所以开始就获取进程句柄，然后赋值权限了
		return false;
	}
	return false;
}

bool Reboot_win::timing(TIME* time) {
	if (time->wYear == 0 || time->wMonth == 0 || time->wDay == 0) {
		return false;
	}
	timer.wYear = time->wYear;
	timer.wMonth = time->wMonth;
	timer.wDay = time->wDay;
	timer.wDayofWeek = time->wDayofWeek;
	timer.wHour = time->wHour;
	timer.wMinute = time->wMinute;
	timer.wSecond = time->wSecond;
	timer.wMilliseconds = time->wMilliseconds;
	return true;
}

bool Reboot_win::timedShutDownForYear(TIME* time) {
	timing(time);
	GetLocalTime(&now);
	if (now.wYear >= timer.wYear && now.wMonth >= timer.wMonth && now.wDay >= timer.wDay &&
		now.wHour >= timer.wHour && now.wMinute > timer.wMinute) return false;
	std::thread t([this]() {
		while (now.wYear != timer.wYear && now.wMonth != timer.wMonth && now.wDay != timer.wDay &&
			now.wHour != timer.wHour && now.wMinute != timer.wMinute) GetLocalTime(&now);
		rebootSystem();
		});
	t.join();
	return true;
}

bool Reboot_win::timedShutDownEveryday(TIME* time) {
	timing(time);

	std::thread t([this]() {
		while (now.wHour != timer.wHour || now.wMinute != timer.wMinute) {
			GetLocalTime(&now);
			//std::cout << "current time:" << now.wHour << ":" << now.wMinute << ":" << now.wSecond << std::endl;
			//std::cout << "timer time:" << timer.wHour << ":" << timer.wMinute << ":" << timer.wSecond << std::endl;
		}
		rebootSystem();
		});
	t.join();
	return true;
}
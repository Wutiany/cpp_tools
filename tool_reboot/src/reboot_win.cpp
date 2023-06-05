#include "../include/reboot_win.h"

bool rebootSystem() {
	HANDLE hToken;
	TOKEN_PRIVILEGES tkp;

	// ��ȡ���̱�ʶ
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) {
		return false;
	}

	// ��ȡ�ػ���Ȩ��LUID
	// SE_SHUTDOWN �رձ���ϵͳ��Ȩ��
	LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);
	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	// ��ȡ���̵Ĺػ�Ȩ��
	AdjustTokenPrivileges(hToken, false, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0);
	if (GetLastError() != ERROR_SUCCESS) {
		return false;
	}

	// ǿ�����������
	//ExitWindowsEx(EWX_SHUTDOWN | EWX_FORCE, 0);  // �ػ�
	if (!ExitWindowsEx(EWX_REBOOT | EWX_FORCE, 0)) {
		// ���ʹ��EWX_REBOOT,�����õĽ��̱�����SE_SHUTDOWN_NAMEȨ�ޣ����Կ�ʼ�ͻ�ȡ���̾����Ȼ��ֵȨ����
		return false;
	}
	return false;
}

// �Ӻ���ǰ��⣬ExitWindowsEx����windowsϵͳ�������߹ػ���api�����ǹػ���EWX_REBOOT������ʹ������Ҫ���̻�ȡȨ�޵�
// ������������ĸ����̽��и���Ȩ��  AdjustTokenPrivileges����token���ȼ�Ȩ�޸������̵ľ��,���Ǹ�ֵ��Ҫ����ľ������������һ���Ļ�ȡ������
// LookupPrivilegeValue�Ǹ�Token���ȼ����и�Ȩ�޵ģ�Ϊ���������о����ֵ��׼��
// ���Ⱦ��ǻ�ȡ�������� OpenProcessToken����ȡ�ĵ�ǰ���̵ľ����ֵ�����ȶ���ľ��
// ͨ��GetCurrentProcess����ȡ��ǰ���̵ľ��

Reboot_win::Reboot_win() {
	GetLocalTime(&now);
}


bool Reboot_win::rebootSystem() {
	HANDLE hToken;
	TOKEN_PRIVILEGES tkp;

	// ��ȡ���̱�ʶ
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) {
		return false;
	}

	// ��ȡ�ػ���Ȩ��LUID
	// SE_SHUTDOWN �رձ���ϵͳ��Ȩ��
	LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);
	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	// ��ȡ���̵Ĺػ�Ȩ��
	AdjustTokenPrivileges(hToken, false, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0);
	if (GetLastError() != ERROR_SUCCESS) {
		return false;
	}

	// ǿ�����������
	//ExitWindowsEx(EWX_REBOOT | EWX_FORCE, 0);  // �ػ�
	if (!ExitWindowsEx(EWX_SHUTDOWN | EWX_FORCE, 0)) {
		// ���ʹ��EWX_REBOOT,�����õĽ��̱�����SE_SHUTDOWN_NAMEȨ�ޣ����Կ�ʼ�ͻ�ȡ���̾����Ȼ��ֵȨ����
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
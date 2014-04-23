#include "AppDelegate.h"
#include "GameScene.h"
#include "WelcomeScene.h"
#include "proj.win32\MyKeyboardControl.h"

USING_NS_CC;

static void PVRFrameEnableControlWindow(bool bEnable);

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
	MyKeyboardControl::release();
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    CCDirector* pDirector = CCDirector::sharedDirector();
    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();

    pDirector->setOpenGLView(pEGLView);
	
    // turn on display FPS
    pDirector->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
	CCScene *pScene = WelcomeScene::create();
	//HelloWorld::create();
	//WelcomeScene::create();使用这行来启用开场动画

    // run
    pDirector->runWithScene(pScene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    CCDirector::sharedDirector()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    CCDirector::sharedDirector()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}

int AppDelegate::run() {
	PVRFrameEnableControlWindow(false);

	// Main message loop:
	MSG msg;
	LARGE_INTEGER nFreq;
	LARGE_INTEGER nLast;
	LARGE_INTEGER nNow;

	QueryPerformanceFrequency(&nFreq);
	QueryPerformanceCounter(&nLast);

	// Initialize instance and cocos2d.
	if (!applicationDidFinishLaunching())
	{
		return 0;
	}

	CCEGLView* pMainWnd = CCEGLView::sharedOpenGLView();
	pMainWnd->centerWindow();
	ShowWindow(pMainWnd->getHWnd(), SW_SHOW);

	while (1)
	{
		if (!PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// Get current time tick.
			QueryPerformanceCounter(&nNow);

			// If it's the time to draw next frame, draw it, else sleep a while.
			if (nNow.QuadPart - nLast.QuadPart > m_nAnimationInterval.QuadPart)
			{
				nLast.QuadPart = nNow.QuadPart;
				CCDirector::sharedDirector()->mainLoop();
				if (MyKeyboardControl::getInstance()->isActive()) {
					MyKeyboardControl::getInstance()->checkKeyState();
				}
			}
			else
			{
				Sleep(0);
			}
			continue;
		}

		if (WM_QUIT == msg.message)
		{
			// Quit message loop.
			break;
		}

		// Deal with windows message.
		if (!m_hAccelTable || !TranslateAccelerator(msg.hwnd, m_hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}

static void PVRFrameEnableControlWindow(bool bEnable)
{
	HKEY hKey = 0;

	// Open PVRFrame control key, if not exist create it.
	if (ERROR_SUCCESS != RegCreateKeyExW(HKEY_CURRENT_USER,
		L"Software\\Imagination Technologies\\PVRVFRame\\STARTUP\\",
		0,
		0,
		REG_OPTION_NON_VOLATILE,
		KEY_ALL_ACCESS,
		0,
		&hKey,
		NULL))
	{
		return;
	}

	const WCHAR* wszValue = L"hide_gui";
	const WCHAR* wszNewData = (bEnable) ? L"NO" : L"YES";
	WCHAR wszOldData[256] = { 0 };
	DWORD   dwSize = sizeof(wszOldData);
	LSTATUS status = RegQueryValueExW(hKey, wszValue, 0, NULL, (LPBYTE)wszOldData, &dwSize);
	if (ERROR_FILE_NOT_FOUND == status              // the key not exist
		|| (ERROR_SUCCESS == status                 // or the hide_gui value is exist
		&& 0 != wcscmp(wszNewData, wszOldData)))    // but new data and old data not equal
	{
		dwSize = sizeof(WCHAR)* (wcslen(wszNewData) + 1);
		RegSetValueEx(hKey, wszValue, 0, REG_SZ, (const BYTE *)wszNewData, dwSize);
	}

	RegCloseKey(hKey);
}
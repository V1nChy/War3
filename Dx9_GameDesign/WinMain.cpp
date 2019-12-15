#include <windows.h>
#include "MyApp.h"

const int ClientWidth = GetSystemMetrics(SM_CXSCREEN);
const int ClientHeight = GetSystemMetrics(SM_CYSCREEN);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPWSTR cmdLine, int cmdShow)
{
	MyApp demo;

	if(SUCCEEDED(demo.Create(hInstance, ClientWidth, ClientHeight,"MyEngine By:VinChy")))
		demo.Run();

	return 0;
}

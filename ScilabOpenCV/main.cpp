#include "../wxOpenCVMain/ImageInfo.h"


#ifdef _USRDLL
#include "dllHeader.h"
extern "C" {
DLLSCILABOCV_EXPORTS  void PrepAcq(double *tps,int *indPeriph,int *err);
}
#else
#define DLLSCILABOCV_EXPORTS
//PortAudio carte(10);
#endif

GateWayOCV gOpenCV;


#ifdef _USRDLL
// DllScilab.cpp : définit les fonctions exportées pour l'application DLL.
//

#include <math.h>
#include <windows.h>

// Global variable 
 
HMODULE hinst; 

BOOL InitApplication(HINSTANCE hinstance); 
BOOL InitInstance(HINSTANCE hinstance, int nCmdShow); 

DLLSCILABOCV_EXPORTS  void PrepAcq(double *tps,int *indPeriph,int *err)
{
    int index=28;
	*err = 2 * gOpenCV.Lecture(index);
}


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
 int nCmdShow=0;
 hinst=hModule;
    if (!InitApplication(hModule)) 
        return true; 
 
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}


BOOL InitApplication(HINSTANCE hinstance) 
{ 
    WNDCLASSEX wcx; 
 
    // Fill in the window class structure with parameters 
    // that describe the main window. 
 
    wcx.cbSize = sizeof(wcx);          // size of structure 
    wcx.style = CS_HREDRAW | 
        CS_VREDRAW;                    // redraw if size changes 
    wcx.lpfnWndProc = (WNDPROC)DllMain;     // points to window procedure 
    wcx.cbClsExtra = 0;                // no extra class memory 
    wcx.cbWndExtra = 0;                // no extra window memory 
    wcx.hInstance = hinstance;         // handle to instance 
    wcx.hIcon = LoadIcon(NULL, 
        IDI_APPLICATION);              // predefined app. icon 
    wcx.hCursor = LoadCursor(NULL, 
        IDC_ARROW);                    // predefined arrow 
    wcx.hbrBackground = NULL;                  // white background brush 
    wcx.lpszMenuName =  (LPCSTR)"MainMenu";    // name of menu resource 
    wcx.lpszClassName = (LPCSTR)"MainWClass";  // name of window class 
    wcx.hIconSm = NULL; 
 
    // Register the window class. 
 
    return RegisterClassEx(&wcx); 
} 
 
BOOL InitInstance(HINSTANCE hinstance, int nCmdShow) 
{ 
    HWND hwnd; 
 
    // Save the application-instance handle. 
 
    hinst = hinstance; 
 
    // Create the main window. 
 
    hwnd = CreateWindow( 
        (LPCSTR)"MainWClass",        // name of window class 
        (LPCSTR)"Sample",            // title-bar string 
        WS_OVERLAPPEDWINDOW, // top-level window 
        CW_USEDEFAULT,       // default horizontal position 
        CW_USEDEFAULT,       // default vertical position 
        CW_USEDEFAULT,       // default width 
        CW_USEDEFAULT,       // default height 
        (HWND) NULL,         // no owner window 
        (HMENU) NULL,        // use class menu 
        hinstance,           // handle to application instance 
        (LPVOID) NULL);      // no window-creation data 
 
    if (!hwnd) 
        return FALSE; 
 
    // Show the window and send a WM_PAINT message to the window 
    // procedure. 
 
    ShowWindow(hwnd, nCmdShow); 
    UpdateWindow(hwnd); 
    return TRUE; 
 
} 





#endif

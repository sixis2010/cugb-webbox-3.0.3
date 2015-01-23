#include "Window.h" 
 

int APIENTRY _tWinMain(HINSTANCE hInstance,  HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{ 
	Window::GetSharedInstance().Run();
	
	return 0;
}
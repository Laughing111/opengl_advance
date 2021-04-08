#include "ggl.h"
#include "scene.h"
#include "utils.h"
#pragma comment(lib,"glew32.lib")
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"winmm.lib")

unsigned char* LoadFileContent(const char* path, int& filesize)
{
	unsigned char* fileContent = nullptr;
	filesize = 0;
	FILE* pFile = fopen(path, "rb");
	if (pFile) 
	{
		fseek(pFile, 0, SEEK_END);
		int nLen = ftell(pFile);
		if (nLen > 0)
		{
			rewind(pFile);
			fileContent = new unsigned char[nLen + 1];
			fread(fileContent, sizeof(unsigned char), nLen, pFile);
			fileContent[nLen] = '\0';
			filesize = nLen;
		}
		fclose(pFile);
	}
	return fileContent;
}

float GetFrameTime()
{
	static unsigned long lastTime = 0, timeSinceComputerStart = 0;
	timeSinceComputerStart = timeGetTime();
	unsigned long frameTime = lastTime == 0 ? 0 : timeSinceComputerStart - lastTime;
	lastTime = timeSinceComputerStart;
	return float(frameTime) / 1000.f;
}

LRESULT CALLBACK GLWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_CLOSE:						//只处理关闭操作
			PostQuitMessage(0);
			return 0;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

/// <summary>
/// window窗口绘制
/// </summary>
/// <param name="hInstance">当前这次绘制的窗口实例</param>
/// <param name="hPrevInstance">上一次绘制的窗口实例</param>
/// <param name="lpCmdLine">命令行参数</param>
/// <param name="nShowCmd">如何显示窗口参数</param>
/// <returns></returns>
INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
																									//1.注册窗口
	WNDCLASSEX wndclass;
	wndclass.cbClsExtra = 0;															//窗口类型需要的额外空间为0   窗口空间的原大小为100kb
	wndclass.cbSize = sizeof(WNDCLASSEX);								//该窗口对象 实际占据的内存大小
	wndclass.cbWndExtra = 0;														//该窗口对象的额外内存
	wndclass.hbrBackground = NULL;											//不使用windows GDI绘图 直接设置为空（使用opengl绘图
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);			//设置窗口光标样式
	wndclass.hIcon = NULL;															//窗口exe程序的icon
	wndclass.hIconSm = NULL;														//窗口exe程序运行时显示的小icon
	wndclass.hInstance = hInstance;												//当前窗口实例
	wndclass.lpfnWndProc = GLWindowProc;								//用户操作事件
	wndclass.lpszClassName = L"GLWindow";								//窗口名称
	wndclass.lpszMenuName = NULL;											//菜单名称
	wndclass.style = CS_VREDRAW | CS_HREDRAW;						//窗口样式为垂直+水平
	ATOM atom = RegisterClassExW(&wndclass);						//传入类型指针，注册完毕
	if (!atom) {																				//检测是否注册失败，失败则弹窗提醒
		MessageBox(NULL, L"Notice", L"Error", MB_OK);
		return 0;
	}

	RECT rect;
	rect.left = 0;
	rect.right = 800;
	rect.top = 0;
	rect.bottom = 600;
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, NULL);
	int windowWidth = rect.right - rect.left;
	int windowHeight = rect.bottom - rect.top;
																								   //2.创建窗口
	HWND hwnd = CreateWindowEx(NULL, L"GLWindow", L"OpenGL Window", WS_OVERLAPPEDWINDOW,
		100, 100, windowWidth, windowHeight,
		NULL, NULL, hInstance, NULL);

	//1.设置窗口像素格式
	HDC  dc = GetDC(hwnd);
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nVersion = 1;
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.cColorBits = 32;					//颜色缓冲区 每一个像素占用32bit R,G,B,A 各8bit
	pfd.cDepthBits = 24;				//深度缓冲区每一个像素占用24bit  表示的是一个float
	pfd.cStencilBits = 8;					//模板缓冲区每一个像素占用8bit
	pfd.iPixelType = PFD_TYPE_RGBA;			//设置像素类型
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | LPD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER ;  //绘制到窗口 | 使用opengl | 使用双帧缓冲区
	int pixelFormat = ChoosePixelFormat(dc, &pfd);			//声明选择的像素格式 会返回像素格式的ID
	SetPixelFormat(dc, pixelFormat, &pfd);

	//2.创建opengl渲染上下文的句柄
	HGLRC rc = wglCreateContext(dc);   
	wglMakeCurrent(dc, rc);					//生效上下文

	//初始化opengl的高级api
	glewInit();
	//初始化场景
	Init();
	SetViewPortSize(800.f,600.f);

	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);
	MSG msg;
	while (true)
	{
		if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		//逐帧绘制
		Draw();
		//双缓冲区交换
		SwapBuffers(dc);
	}

	
	return 0;
}
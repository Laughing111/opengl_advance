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
		case WM_CLOSE:						//ֻ����رղ���
			PostQuitMessage(0);
			return 0;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

/// <summary>
/// window���ڻ���
/// </summary>
/// <param name="hInstance">��ǰ��λ��ƵĴ���ʵ��</param>
/// <param name="hPrevInstance">��һ�λ��ƵĴ���ʵ��</param>
/// <param name="lpCmdLine">�����в���</param>
/// <param name="nShowCmd">�����ʾ���ڲ���</param>
/// <returns></returns>
INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
																									//1.ע�ᴰ��
	WNDCLASSEX wndclass;
	wndclass.cbClsExtra = 0;															//����������Ҫ�Ķ���ռ�Ϊ0   ���ڿռ��ԭ��СΪ100kb
	wndclass.cbSize = sizeof(WNDCLASSEX);								//�ô��ڶ��� ʵ��ռ�ݵ��ڴ��С
	wndclass.cbWndExtra = 0;														//�ô��ڶ���Ķ����ڴ�
	wndclass.hbrBackground = NULL;											//��ʹ��windows GDI��ͼ ֱ������Ϊ�գ�ʹ��opengl��ͼ
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);			//���ô��ڹ����ʽ
	wndclass.hIcon = NULL;															//����exe�����icon
	wndclass.hIconSm = NULL;														//����exe��������ʱ��ʾ��Сicon
	wndclass.hInstance = hInstance;												//��ǰ����ʵ��
	wndclass.lpfnWndProc = GLWindowProc;								//�û������¼�
	wndclass.lpszClassName = L"GLWindow";								//��������
	wndclass.lpszMenuName = NULL;											//�˵�����
	wndclass.style = CS_VREDRAW | CS_HREDRAW;						//������ʽΪ��ֱ+ˮƽ
	ATOM atom = RegisterClassExW(&wndclass);						//��������ָ�룬ע�����
	if (!atom) {																				//����Ƿ�ע��ʧ�ܣ�ʧ���򵯴�����
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
																								   //2.��������
	HWND hwnd = CreateWindowEx(NULL, L"GLWindow", L"OpenGL Window", WS_OVERLAPPEDWINDOW,
		100, 100, windowWidth, windowHeight,
		NULL, NULL, hInstance, NULL);

	//1.���ô������ظ�ʽ
	HDC  dc = GetDC(hwnd);
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nVersion = 1;
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.cColorBits = 32;					//��ɫ������ ÿһ������ռ��32bit R,G,B,A ��8bit
	pfd.cDepthBits = 24;				//��Ȼ�����ÿһ������ռ��24bit  ��ʾ����һ��float
	pfd.cStencilBits = 8;					//ģ�建����ÿһ������ռ��8bit
	pfd.iPixelType = PFD_TYPE_RGBA;			//������������
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | LPD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER ;  //���Ƶ����� | ʹ��opengl | ʹ��˫֡������
	int pixelFormat = ChoosePixelFormat(dc, &pfd);			//����ѡ������ظ�ʽ �᷵�����ظ�ʽ��ID
	SetPixelFormat(dc, pixelFormat, &pfd);

	//2.����opengl��Ⱦ�����ĵľ��
	HGLRC rc = wglCreateContext(dc);   
	wglMakeCurrent(dc, rc);					//��Ч������

	//��ʼ��opengl�ĸ߼�api
	glewInit();
	//��ʼ������
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
		//��֡����
		Draw();
		//˫����������
		SwapBuffers(dc);
	}

	
	return 0;
}
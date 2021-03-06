// wx_autochat.cpp: 定义控制台应用程序的入口点。
//
// MMyKey.cpp : 定义控制台应用程序的入口点。
//


#include "stdafx.h"
#include <iostream>
#include "Afx.h"
#include "Afxwin.h"
using namespace std;


//#define MAPVK_VK_TO_VSC 0
BYTE scan_code(DWORD pKey)
{
	const DWORD result = MapVirtualKey(pKey, MAPVK_VK_TO_VSC);
	return static_cast<BYTE>(result);
}
void press_key(DWORD pKey)
{
	keybd_event(static_cast<BYTE>(pKey), scan_code(pKey), 0, 0);
}
void release_key(DWORD pKey)
{
	keybd_event(static_cast<BYTE>(pKey), scan_code(pKey), KEYEVENTF_KEYUP, 0);
}
bool isInvalid = true;
// for testing
//#define PRESS(x) press_key(x); std::cout << "Press: " #x << std::endl
//#define RELEASE(x) release_key(x); std::cout << "Release: " #x << std::end
#define PRESS(x) press_key(x);
#define RELEASE(x) release_key(x);
// test


//HHOOK g_ms_hook = 0;
HHOOK g_kb_hook = 0;




LRESULT CALLBACK kb_proc(int code, WPARAM w, LPARAM l)
{

	//printf((w==WM_KEYDOWN)?"按下%c\n":"抬起%c\n",((PKBDLLHOOKSTRUCT)l)->vkCode);

	//F1结束'p'真的是F1
	char key = ((PKBDLLHOOKSTRUCT)l)->vkCode;
	if (key == 39) {
		isInvalid = false;
		cout << "启动转换" << endl;
	}
	return CallNextHookEx(g_kb_hook, code, w, l);
}
/*LRESULT CALLBACK ms_proc (int code, WPARAM w, LPARAM l)
{
if(w == WM_LBUTTONDOWN)
printf("按下左键\n");
else if(w == WM_LBUTTONUP)
printf("抬起左键\n");
else
printf("x:%d\ty:%d\n",((PMSLLHOOKSTRUCT)l)->pt.x,((PMSLLHOOKSTRUCT)l)->pt.y);
return CallNextHookEx (g_ms_hook, code, w, l);
}*/
DWORD WINAPI mySwitch(PVOID pvParam) {
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		Sleep(10);
	}
	/*while(1) {
	cout << "myswitch" << endl;
	Sleep(50);
	}*/
	return 0;
}

void SetClipBoard(const char* str)
{
	OpenClipboard(NULL);//打开剪切板  
	EmptyClipboard();//清空剪切板  
	HANDLE hHandle = GlobalAlloc(GMEM_FIXED, 1000);//分配内存  
	char* pData = (char*)GlobalLock(hHandle);//锁定内存，返回申请内存的首地址  
	strcpy((char *)pData, (char *)str);
	SetClipboardData(CF_TEXT, hHandle);//设置剪切板数据  
	GlobalUnlock(hHandle);//解除锁定  
	CloseClipboard();//关闭剪切板  
}

string Utf8ToGbk(const std::string& strUtf8)//传入的strUtf8是UTF-8编码
{
	//UTF-8转unicode
	int len = MultiByteToWideChar(CP_UTF8, 0, strUtf8.c_str(), -1, NULL, 0);
	wchar_t * strUnicode = new wchar_t[len];//len = 2
	wmemset(strUnicode, 0, len);
	MultiByteToWideChar(CP_UTF8, 0, strUtf8.c_str(), -1, strUnicode, len);

	//unicode转gbk
	len = WideCharToMultiByte(CP_ACP, 0, strUnicode, -1, NULL, 0, NULL, NULL);
	char *strGbk = new char[len];//len=3 本来为2，但是char*后面自动加上了\0
	memset(strGbk, 0, len);
	WideCharToMultiByte(CP_ACP, 0, strUnicode, -1, strGbk, len, NULL, NULL);

	std::string strTemp(strGbk);//此时的strTemp是GBK编码
	delete[]strUnicode;
	delete[]strGbk;
	strUnicode = NULL;
	strGbk = NULL;
	return strTemp;
}

CString ScreenShot(void);
DWORD WINAPI anjian(PVOID pvParam) {
	string headstr = string("元旦到来之际，祝");
	string teststr = string("在2018年平安幸福，快乐健康，心想事成！元旦快乐！——控制工程师小王");

	while (1)
	{
		if (!isInvalid) {
			CString bmpName = ScreenShot();
			CString jpgName = bmpName;
			char cmd1Char[200] = "magick E:\\vs_studio\\wx_autochat\\x64\\Debug\\ScreenShot\\wx_name.bmp E:\\vs_studio\\wx_autochat\\x64\\Debug\\ScreenShot\\wx_name.jpg";
			char cmd3Char[200] = "magick convert -resize 832x56 E:\\vs_studio\\wx_autochat\\x64\\Debug\\ScreenShot\\wx_name.jpg E:\\vs_studio\\wx_autochat\\x64\\Debug\\ScreenShot\\wx2.jpg";
			char cmd2Char[200] = "tesseract E:\\vs_studio\\wx_autochat\\x64\\Debug\\ScreenShot\\wx2.jpg E:\\vs_studio\\wx_autochat\\x64\\Debug\\ScreenShot\\1.txt -l chi_sim";
			system(cmd1Char);
			system(cmd3Char);
			system(cmd2Char);

			// 读取1.txt
			CFile file;
			CString txtFile = CString("E:\\vs_studio\\wx_autochat\\x64\\Debug\\ScreenShot\\1.txt.txt");
			file.Open((LPCTSTR)txtFile, CFile::modeRead);
			char tmp_str[200];
			int len = file.Read((void *)tmp_str, 200);
			char name_str[100];
			int i, j = 0;
			for (i = 0; i < len; i++)
			{
				if (tmp_str[i] == 0x20)
					continue;
				if (tmp_str[i] == 10)
				{
					name_str[j] = 0;
					break;
				}
				name_str[j++] = tmp_str[i];
			}
			string tmp = string(name_str);
			string t = Utf8ToGbk(tmp);
			printf("%s\n", t.data());
			file.Close();
			//jpgName.SetAt(jpgName.GetLength() - 2, 'j');
			//jpgName.SetAt(jpgName.GetLength() - 1, 'p');
			//jpgName.SetAt(jpgName.GetLength() - 0, 'g');
			//CString bmp2jpgcmd = CString("magick")+" "+ bmpName+" " + jpgName;
			//char *ch = (char *)(bmp2jpgcmd.GetBuffer(0));
			//system(ch);
			//bmp2jpgcmd.ReleaseBuffer();
			SetClipBoard((headstr+t+teststr).data());

			PRESS(17);
			PRESS(86);
			Sleep(10);
			RELEASE(17);
			RELEASE(86);

			Sleep(50); // wait for a second
			PRESS(18);
			PRESS(83);
			Sleep(10);
			RELEASE(18);
			RELEASE(83);
			Sleep(10);
			isInvalid = TRUE;
		}
	//	Sleep(1000);
	}
}

wstring GetAppPathW()
{
	wchar_t szExePath[MAX_PATH] = { 0 };
	GetModuleFileNameW(NULL, szExePath, MAX_PATH);
	wchar_t *pstr = wcsrchr(szExePath, '\\');
	memset(pstr + 1, 0, 2);
	wstring strAppPath(szExePath);
	return strAppPath;
}

CString ScreenShot(void)
{
	CWnd * pDesktop = CWnd::FromHandle(GetDesktopWindow());
	CDC *pdeskdc = pDesktop->GetDC();
	CRect re(330,23,550,51);
	//获取窗口的大小
//	pDesktop->GetClientRect(&re);
	CBitmap bmp;
	bmp.CreateCompatibleBitmap(pdeskdc, re.Width(), re.Height());
	//创建一个兼容的内存画板
	CDC memorydc;
	memorydc.CreateCompatibleDC(pdeskdc);
	//选中画笔
	CBitmap *pold = memorydc.SelectObject(&bmp);
	//绘制图像
	memorydc.BitBlt(0, 0, re.Width(), re.Height(), pdeskdc, re.left, re.top, SRCCOPY);
	//获取鼠标位置，然后添加鼠标图像
	CPoint po;
	GetCursorPos(&po);
	HICON hinco = (HICON)GetCursor();
	memorydc.DrawIcon(po.x - 10, po.y - 10, hinco);
	//选中原来的画笔
	memorydc.SelectObject(pold);
	BITMAP bit;
	bmp.GetBitmap(&bit);
	//定义 图像大小（单位：byte）
	DWORD size = bit.bmWidthBytes * bit.bmHeight;
	LPSTR lpdata = (LPSTR)GlobalAlloc(GPTR, size);
	//后面是创建一个bmp文件的必须文件头
	BITMAPINFOHEADER pbitinfo;
	pbitinfo.biBitCount = 24;
	pbitinfo.biClrImportant = 0;
	pbitinfo.biCompression = BI_RGB;
	pbitinfo.biHeight = bit.bmHeight;
	pbitinfo.biPlanes = 1;
	pbitinfo.biSize = sizeof(BITMAPINFOHEADER);
	
	pbitinfo.biWidth = bit.bmWidth;
	pbitinfo.biSizeImage = (bit.bmWidth*pbitinfo.biBitCount+31)/32*4* bit.bmHeight;
	pbitinfo.biXPelsPerMeter = 0;
	pbitinfo.biYPelsPerMeter = 0;
	GetDIBits(pdeskdc->m_hDC, bmp, 0, pbitinfo.biHeight, lpdata, (BITMAPINFO*)
		&pbitinfo, DIB_RGB_COLORS);
	BITMAPFILEHEADER bfh;
	bfh.bfReserved1 = bfh.bfReserved2 = 0;
	bfh.bfType = ((WORD)('M' << 8) | 'B');
	bfh.bfSize = size + 54;
	bfh.bfOffBits = 54;
	//写入文件
	CFile file;
	CString strFileName(GetAppPathW().c_str());
	strFileName += _T("ScreenShot\\");
	CreateDirectory((LPCTSTR)strFileName, NULL);
	strFileName += "wx_name";
	strFileName += _T(".bmp");
	if (file.Open((LPCTSTR)strFileName, CFile::modeCreate | CFile::modeWrite))
	{
		file.Write(&bfh, sizeof(BITMAPFILEHEADER));
		file.Write(&pbitinfo, sizeof(BITMAPINFOHEADER));
		file.Write(lpdata, size);
		file.Close();
	}
	GlobalFree(lpdata);
	return strFileName;
}

int _tmain(int argc, _TCHAR* argv[])
{


	g_kb_hook = SetWindowsHookEx(WH_KEYBOARD_LL, kb_proc, GetModuleHandle(NULL), 0);
	//g_ms_hook = SetWindowsHookEx (WH_MOUSE_LL, ms_proc,GetModuleHandle(NULL),0);
	if (g_kb_hook == NULL)
	{
		printf("安装钩子出错\n");
		return 0;
	}
	else {
		cout << "/*******************************->按键启动****************************/" << endl;
	}
	/*HANDLE hThread1 = CreateThread(NULL,       //默认安全级别
	0,          //默认栈大小
	mySwitch,//线程函数
	NULL,       //函数没有参数
	0,          //创建后直接运行
	NULL);      //线程标识，不需要
	*/
	HANDLE hThread2 = CreateThread(NULL,       //默认安全级别  
		0,          //默认栈大小  
		anjian,//线程函数   
		NULL,       //函数没有参数  
		0,          //创建后直接运行  
		NULL);      //线程标识，不需要  
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		Sleep(10);
	}


	UnhookWindowsHookEx(g_kb_hook);
	//CloseHandle(hThread1);
	CloseHandle(hThread2);
	return 0;
}

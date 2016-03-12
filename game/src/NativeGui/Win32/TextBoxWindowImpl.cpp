#include "NativeGui/Win32/TextBoxWindowImpl.hpp"

#ifdef OS_WINDOWS

#include <codecvt>
#include <iostream>
#include <locale>

#include "Richedit.h"

#define ID_EDITCHILD 100

namespace nativegui
{
namespace impl
{

bool TextBoxWindowImpl::classInitialized = false;

namespace
{

std::wstring stringToWide(const std::string& str)
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    return converter.from_bytes(str);
}

std::string wideToString(const std::wstring& wstr)
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    return converter.to_bytes(wstr);
}

}

TextBoxWindowImpl::TextBoxWindowImpl(const std::string& initialText, const std::string& title) :
	m_window(nullptr),
	m_editControl(nullptr),
	m_initialText(stringToWide(initialText))
{
	HINSTANCE hInstance = (HINSTANCE)GetModuleHandle(NULL);

	if(!classInitialized)
	{
		LoadLibrary(TEXT("Riched20.dll"));

		WNDCLASSEX wcex;

	    wcex.cbSize = sizeof(WNDCLASSEX);
	    wcex.style          = CS_HREDRAW | CS_VREDRAW;
	    wcex.lpfnWndProc    = &TextBoxWindowImpl::WndProc;
	    wcex.cbClsExtra     = 0;
	    wcex.cbWndExtra     = 0;
	    wcex.hInstance      = hInstance;
	    wcex.hIcon          = nullptr;
	    wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
	    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
	    wcex.lpszMenuName   = NULL;
	    wcex.lpszClassName  = _T("yapgtextboxwindow");
	    wcex.hIconSm        = nullptr;

	    if (!RegisterClassEx(&wcex))
	    {
	        MessageBox(NULL,
	            _T("Call to RegisterClassEx failed!"),
	            _T("YAPG"),
	            NULL);

			//TODO: Throw runtime_error
	    }

		classInitialized = true;
	}

	// The parameters to CreateWindow explained:
    // szWindowClass: the name of the application
    // szTitle: the text that appears in the title bar
    // WS_OVERLAPPEDWINDOW: the type of window to create
    // CW_USEDEFAULT, CW_USEDEFAULT: initial position (x, y)
    // 500, 100: initial size (width, length)
    // NULL: the parent of this window
    // NULL: this application does not have a menu bar
    // hInstance: the first parameter from WinMain
    // NULL: not used in this application
    m_window = CreateWindow(
        _T("yapgtextboxwindow"),
        stringToWide(title).c_str(),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        500, 100,
        NULL,
        NULL,
        hInstance,
        (LPVOID)this
    );

    if (!m_window)
    {
        MessageBox(NULL,
            _T("Call to CreateWindow failed!"),
            _T("YAPG"),
            NULL);

		//TODO: Throw runtime_error
    }
}

int TextBoxWindowImpl::run()
{
	// The parameters to ShowWindow explained:
    // hWnd: the value returned from CreateWindow
    // nCmdShow: the fourth parameter from WinMain
    ShowWindow(m_window,
        SW_SHOW);
    UpdateWindow(m_window);

    // Main message loop:
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int) msg.wParam;
}

std::string TextBoxWindowImpl::getText() const
{

}

LRESULT CALLBACK TextBoxWindowImpl::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	TextBoxWindowImpl* instanceImpl = reinterpret_cast<TextBoxWindowImpl*>(
		GetWindowLongPtr(
			hWnd,
			GWLP_USERDATA
		));

    switch (message)
    {
		case WM_CREATE:
			//Called when window is created (before all other events)
			//lParam is the pointer to the data given to lpParam of
			//CreateWindow function

			//Set the user data pointer to the "this" pointer (stored in lParam)
			SetWindowLongPtr(
				hWnd,
				GWLP_USERDATA,
				(LONG_PTR)((CREATESTRUCT*)(lParam))->lpCreateParams
			);

			instanceImpl = (TextBoxWindowImpl*)
				GetWindowLongPtr(
					hWnd,
					GWLP_USERDATA
				);

		   	instanceImpl->m_editControl = CreateWindowEx(
							   0, RICHEDIT_CLASS/*L"EDIT"*/,   // predefined class
							   NULL,         // no window title
							   WS_CHILD | WS_VISIBLE | WS_VSCROLL |
							   ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL,
							   0, 0, 0, 0,   // set size in WM_SIZE message
							   hWnd,         // parent window
							   (HMENU) ID_EDITCHILD,   // edit control ID
							   (HINSTANCE) GetWindowLong(hWnd, GWL_HINSTANCE),
							   NULL);        // pointer not needed

		    // Add text to the window.
		   	SendMessage(instanceImpl->m_editControl, WM_SETTEXT, 0, (LPARAM) instanceImpl->m_initialText.c_str());

		    return 0;
	   case WM_SETFOCUS:
           SetFocus(instanceImpl->m_editControl);
           return 0;

       case WM_SIZE:
           // Make the edit control the size of the window's client area.

           MoveWindow(instanceImpl->m_editControl,
                      0, 0,                  // starting x- and y-coordinates
                      LOWORD(lParam),        // width of client area
                      HIWORD(lParam),        // height of client area
                      TRUE);                 // repaint window
           return 0;

	    case WM_DESTROY:
	        PostQuitMessage(0);
	        break;
	    default:
	        return DefWindowProc(hWnd, message, wParam, lParam);
	        break;
    }

    return 0;
}

}
}

#endif

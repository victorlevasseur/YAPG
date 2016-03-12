#ifndef YAPG_GAME_NATIVEGUI_WIN32_TEXTBOXWINDOWIMPL_H
#define YAPG_GAME_NATIVEGUI_WIN32_TEXTBOXWINDOWIMPL_H
#ifdef OS_WINDOWS

#include <string>

#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>

namespace nativegui
{
namespace impl
{

class TextBoxWindowImpl
{
public:
	TextBoxWindowImpl(const std::string& initialText, const std::string& title);

	int run();

	std::string getText() const;

private:
	static bool classInitialized;

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	HWND m_window;
	HWND m_editControl;

	std::wstring m_initialText;
	
	std::string m_text;
};

}
}

#endif
#endif

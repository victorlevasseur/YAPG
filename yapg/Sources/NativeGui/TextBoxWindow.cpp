#include "NativeGui/TextBoxWindow.hpp"

#ifdef OS_LINUX
#include "NativeGui/Linux/TextBoxWindowImpl.hpp"
#endif
#ifdef OS_WINDOWS
#include "NativeGui/Win32/TextBoxWindowImpl.hpp"
#endif

namespace yapg
{

TextBoxWindow::TextBoxWindow(const std::string& initialText, const std::string& title) :
	m_impl(std::make_unique<impl::TextBoxWindowImpl>(initialText, title))
{

}

TextBoxWindow::~TextBoxWindow()
{

}

int TextBoxWindow::run()
{
	return m_impl->run();
}

std::string TextBoxWindow::getText() const
{
	return m_impl->getText();
}

}

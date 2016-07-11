#ifndef YAPG_GAME_NATIVEGUI_TEXTBOXWINDOW_H
#define YAPG_GAME_NATIVEGUI_TEXTBOXWINDOW_H

#include <memory>
#include <string>

namespace yapg{ namespace impl{ class TextBoxWindowImpl; } }

namespace yapg
{

class TextBoxWindow
{
public:
	TextBoxWindow(const std::string& initialText, const std::string& title);
	~TextBoxWindow();

	int run();

	std::string getText() const;

private:
	std::unique_ptr<impl::TextBoxWindowImpl> m_impl;
};

}

#endif

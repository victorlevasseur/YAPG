#ifndef YAPG_GAME_NATIVEGUI_LINUX_TEXTBOXWINDOWIMPL_H
#define YAPG_GAME_NATIVEGUI_LINUX_TEXTBOXWINDOWIMPL_H
#ifdef OS_LINUX

#include <string>

#include <gtk/gtk.h>

namespace yapg
{
namespace impl
{

class TextBoxWindowImpl
{
public:
	TextBoxWindowImpl(const std::string& initialText, const std::string& title);

    ~TextBoxWindowImpl();

	int run();

	std::string getText() const;

private:
    std::string m_initialText;
    std::string m_title;

    std::string m_text;
};

}
}

#endif
#endif

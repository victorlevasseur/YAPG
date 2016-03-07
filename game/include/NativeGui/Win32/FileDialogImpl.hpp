#ifndef YAPG_GAME_NATIVEGUI_WIN32_FILEDIALOGIMPL_H
#define YAPG_GAME_NATIVEGUI_WIN32_FILEDIALOGIMPL_H

#ifdef OS_WINDOWS

#include <windows.h>

#include "NativeGui/FileDialog.hpp"

namespace nativegui
{
namespace impl
{

class FileDialogImpl
{
public:
    FileDialogImpl(const std::string& title, FileDialog::Action action);
    ~FileDialogImpl();

    bool run();

    std::string getFilename() const;

private:
    OPENFILENAME *m_fileDialogStruct;
    FileDialog::Action m_action;
};

}
}

#endif

#endif

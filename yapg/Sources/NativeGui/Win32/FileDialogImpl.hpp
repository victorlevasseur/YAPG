#ifndef YAPG_GAME_NATIVEGUI_WIN32_FILEDIALOGIMPL_H
#define YAPG_GAME_NATIVEGUI_WIN32_FILEDIALOGIMPL_H

#ifdef OS_WINDOWS

#include <windows.h>

#include "NativeGui/FileDialog.hpp"

namespace yapg
{
namespace impl
{

class FileDialogImpl
{
public:
    FileDialogImpl(
        const std::string& title,
        FileDialog::Action action,
        const std::vector<FileDialog::Filter>& filters = std::vector<FileDialog::Filter>(),
        std::size_t selectedFilter = 0
        );
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

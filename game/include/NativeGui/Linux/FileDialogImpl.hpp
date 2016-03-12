#ifndef YAPG_GAME_NATIVEGUI_LINUX_FILEDIALOGIMPL_H
#define YAPG_GAME_NATIVEGUI_LINUX_FILEDIALOGIMPL_H

#ifdef OS_LINUX

#include "NativeGui/FileDialog.hpp"

#include <gtk/gtk.h>

namespace nativegui
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
    std::string m_title;
    FileDialog::Action m_action;
    std::vector<FileDialog::Filter> m_filters;
    std::size_t m_selectedFilter;

    std::string m_selectedFileName;
};

}
}

#endif

#endif

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
    GtkWidget* m_fileDialog;
    std::string m_selectedFileName;
};

}
}

#endif

#endif

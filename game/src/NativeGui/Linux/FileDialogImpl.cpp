#ifdef OS_LINUX

#include "NativeGui/Linux/FileDialogImpl.hpp"

#include <glib.h>
#include <glib/gi18n.h>

namespace nativegui
{
namespace impl
{

FileDialogImpl::FileDialogImpl(const std::string& title, FileDialog::Action action) :
    m_fileDialog(nullptr),
    m_selectedFileName()
{
    m_fileDialog = gtk_file_chooser_dialog_new(
        title.c_str(),
        nullptr,
        action == FileDialog::Open ? GTK_FILE_CHOOSER_ACTION_OPEN : GTK_FILE_CHOOSER_ACTION_SAVE,
        _("_Cancel"),
        GTK_RESPONSE_CANCEL,
        _("_Open"),
        GTK_RESPONSE_ACCEPT,
        nullptr
    );

    gtk_window_set_keep_above(GTK_WINDOW(m_fileDialog), TRUE);
}

FileDialogImpl::~FileDialogImpl()
{
    gtk_widget_destroy(m_fileDialog);
}

bool FileDialogImpl::run()
{
    gint result = gtk_dialog_run(GTK_DIALOG(m_fileDialog));

    if(result == GTK_RESPONSE_ACCEPT)
    {
        char* filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(m_fileDialog));
        m_selectedFileName = std::string(filename);

        g_free(filename);
    }

    gtk_window_close(GTK_WINDOW(m_fileDialog));

    while (gtk_events_pending())
        gtk_main_iteration_do(false);

    return result == GTK_RESPONSE_ACCEPT;
}

std::string FileDialogImpl::getFilename() const
{
    return m_selectedFileName;
}


}
}

#endif

#ifdef OS_LINUX

#include "NativeGui/Linux/FileDialogImpl.hpp"

#include <glib.h>
#include <glib/gi18n.h>

namespace nativegui
{
namespace impl
{

FileDialogImpl::FileDialogImpl(
    const std::string& title,
    FileDialog::Action action,
    const std::vector<FileDialog::Filter>& filters,
    std::size_t selectedFilter
    ) :
    m_title(title),
    m_action(action),
    m_filters(filters),
    m_selectedFilter(selectedFilter),
    m_selectedFileName()
{

}

FileDialogImpl::~FileDialogImpl()
{

}

bool FileDialogImpl::run()
{
    //Init the dialog
    GtkWidget* fileDialog = gtk_file_chooser_dialog_new(
        m_title.c_str(),
        nullptr,
        m_action == FileDialog::Open ? GTK_FILE_CHOOSER_ACTION_OPEN : GTK_FILE_CHOOSER_ACTION_SAVE,
        _("_Cancel"),
        GTK_RESPONSE_CANCEL,
        m_action == FileDialog::Open ? _("_Open") : _("Save"),
        GTK_RESPONSE_ACCEPT,
        nullptr
    );

    //Init the filters
    for(auto it = m_filters.cbegin(); it != m_filters.cend(); ++it)
    {
        GtkFileFilter* filter = gtk_file_filter_new();
        gtk_file_filter_set_name(filter, it->name.c_str());

        for(auto it2 = it->patterns.cbegin(); it2 != it->patterns.cend(); ++it2)
            gtk_file_filter_add_pattern(filter, (*it2).c_str());

        gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(fileDialog), filter);
    }

    //Set the default filter
    GSList* filtersList = gtk_file_chooser_list_filters(GTK_FILE_CHOOSER(fileDialog));
    if(m_selectedFilter < g_slist_length(filtersList))
        gtk_file_chooser_set_filter(GTK_FILE_CHOOSER(fileDialog), GTK_FILE_FILTER(g_slist_nth_data(filtersList, m_selectedFilter)));

    g_slist_free(filtersList);

    gtk_window_set_keep_above(GTK_WINDOW(fileDialog), TRUE);

    //Run it
    gint result = gtk_dialog_run(GTK_DIALOG(fileDialog));

    if(result == GTK_RESPONSE_ACCEPT)
    {
        char* filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(fileDialog));
        m_selectedFileName = std::string(filename);

        g_free(filename);
    }

    gtk_window_close(GTK_WINDOW(fileDialog));
    gtk_widget_destroy(fileDialog);

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

#include "NativeGui/Linux/TextBoxWindowImpl.hpp"

#ifdef OS_LINUX

namespace nativegui
{
namespace impl
{

TextBoxWindowImpl::TextBoxWindowImpl(const std::string& initialText, const std::string& title) :
    m_initialText(initialText),
    m_title(title),
    m_text()
{

}

TextBoxWindowImpl::~TextBoxWindowImpl()
{

}

int TextBoxWindowImpl::run()
{
    GtkDialog* dialog = GTK_DIALOG(gtk_dialog_new_with_buttons(
        m_title.c_str(),
        nullptr,
        GTK_DIALOG_MODAL,
        "Close",
        GTK_RESPONSE_ACCEPT,
        nullptr
    ));

    gtk_window_set_keep_above(GTK_WINDOW(dialog), true);

    GtkTextView* textView = GTK_TEXT_VIEW(gtk_text_view_new());

    gtk_text_buffer_set_text(
        gtk_text_view_get_buffer(textView),
        m_initialText.c_str(),
        -1
    );

    gtk_box_pack_start(
        GTK_BOX(gtk_dialog_get_content_area(dialog)),
        GTK_WIDGET(textView),
        true,
        true,
        0
    );

    gtk_widget_set_size_request(
        GTK_WIDGET(dialog),
        400,
        300
    );

    gtk_widget_show_all(GTK_WIDGET(dialog));

    gint result = gtk_dialog_run(GTK_DIALOG(dialog));

    //Set the stored text
    GtkTextIter startIter;
    GtkTextIter endIter;

    gtk_text_buffer_get_start_iter(
        gtk_text_view_get_buffer(textView),
        &startIter
    );
    gtk_text_buffer_get_end_iter(
        gtk_text_view_get_buffer(textView),
        &endIter
    );

    m_text = std::string(
        gtk_text_buffer_get_text(
            gtk_text_view_get_buffer(textView),
            &startIter,
            &endIter,
            false
        )
    );

    //Close the window
    gtk_window_close(GTK_WINDOW(dialog));
    gtk_widget_destroy(GTK_WIDGET(dialog));

    while (gtk_events_pending())
        gtk_main_iteration_do(false);

    return result == GTK_RESPONSE_ACCEPT ? 0 : 1;
}

std::string TextBoxWindowImpl::getText() const
{
    return m_text;
}

}
}

#endif

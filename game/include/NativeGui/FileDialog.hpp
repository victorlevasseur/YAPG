#ifndef YAPG_GAME_NATIVEGUI_FILEDIALOG_H
#define YAPG_GAME_NATIVEGUI_FILEDIALOG_H

#include <memory>
#include <string>

namespace nativegui{ namespace impl { class FileDialogImpl; } }

namespace nativegui
{

class FileDialog
{
public:
    enum Action
    {
        Open,
        Save
    };

    FileDialog(const std::string& title, Action action);

    ~FileDialog();

    bool run();

    const std::string& getFilename() const;

private:
    std::unique_ptr<impl::FileDialogImpl> m_impl;
};

}

#endif

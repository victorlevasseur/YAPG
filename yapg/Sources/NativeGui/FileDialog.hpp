#ifndef YAPG_GAME_NATIVEGUI_FILEDIALOG_H
#define YAPG_GAME_NATIVEGUI_FILEDIALOG_H

#include <memory>
#include <string>
#include <vector>

namespace yapg{ namespace impl { class FileDialogImpl; } }

namespace yapg
{

class FileDialog
{
public:
    struct Filter
    {
        std::string name;
        std::vector<std::string> patterns;
    };

    enum Action
    {
        Open,
        Save
    };

    FileDialog(
        const std::string& title,
        Action action,
        const std::vector<Filter>& filters = std::vector<Filter>(),
        std::size_t selectedFilter = 0
        );

    ~FileDialog();

    bool run();

    std::string getFilename() const;

private:
    std::unique_ptr<impl::FileDialogImpl> m_impl;
};

}

#endif

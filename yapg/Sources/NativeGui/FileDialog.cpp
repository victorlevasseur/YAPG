#include "NativeGui/FileDialog.hpp"

#ifdef OS_LINUX
#include "NativeGui/Linux/FileDialogImpl.hpp"
#endif
#ifdef OS_WINDOWS
#include "NativeGui/Win32/FileDialogImpl.hpp"
#endif

namespace yapg
{

FileDialog::FileDialog(
    const std::string& title,
    Action action,
    const std::vector<Filter>& filters,
    std::size_t selectedFilter
    ) :
    m_impl(std::make_unique<impl::FileDialogImpl>(title, action, filters, selectedFilter))
{

}

FileDialog::~FileDialog()
{

}

bool FileDialog::run()
{
    return m_impl->run();
}

std::string FileDialog::getFilename() const
{
    return m_impl->getFilename();
}

}

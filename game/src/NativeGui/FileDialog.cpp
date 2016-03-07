#include "NativeGui/FileDialog.hpp"

#ifdef OS_LINUX
#include "NativeGui/Linux/FileDialogImpl.hpp"
#endif
#ifdef OS_WINDOWS
#include "NativeGui/Win32/FileDialogImpl.hpp"
#endif

namespace nativegui
{

FileDialog::FileDialog(const std::string& title, Action action) :
    m_impl(std::make_unique<impl::FileDialogImpl>(title, action))
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

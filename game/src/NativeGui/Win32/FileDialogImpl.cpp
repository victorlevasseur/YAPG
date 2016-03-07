#ifdef OS_WINDOWS

#include "NativeGui/Win32/FileDialogImpl.hpp"

#include <codecvt>
#include <locale>

namespace nativegui
{
namespace impl
{

FileDialogImpl::FileDialogImpl(const std::string& title, FileDialog::Action action) :
    m_fileDialogStruct(new OPENFILENAME()),
    m_action(action)
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::wstring wideTitle = converter.from_bytes(title);

    ZeroMemory(m_fileDialogStruct, sizeof(*m_fileDialogStruct));

    m_fileDialogStruct->lStructSize = sizeof(*m_fileDialogStruct);
    m_fileDialogStruct->hwndOwner = nullptr;
    m_fileDialogStruct->lpstrDefExt = nullptr;
    m_fileDialogStruct->lpstrFile = new TCHAR[MAX_PATH];
    m_fileDialogStruct->lpstrFile[0] = '\0';
    m_fileDialogStruct->nMaxFile = MAX_PATH;
    m_fileDialogStruct->lpstrFilter = nullptr;
    m_fileDialogStruct->nFilterIndex = 0;
    m_fileDialogStruct->lpstrInitialDir = nullptr;
    m_fileDialogStruct->lpstrTitle = wideTitle.c_str();
    m_fileDialogStruct->Flags =
        m_action == FileDialog::Open ? OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST : 0;
}

FileDialogImpl::~FileDialogImpl()
{
    delete m_fileDialogStruct;
}

bool FileDialogImpl::run()
{
    if(m_action == FileDialog::Open)
        return GetOpenFileName(m_fileDialogStruct) != 0;
    else if(m_action == FileDialog::Save)
        return GetSaveFileName(m_fileDialogStruct) != 0;

    return false;
}

std::string FileDialogImpl::getFilename() const
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::string fileName = converter.to_bytes(std::wstring(m_fileDialogStruct->lpstrFile));

    return fileName;
}


}
}

#endif

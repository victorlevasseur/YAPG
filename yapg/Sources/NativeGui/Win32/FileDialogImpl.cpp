#ifdef OS_WINDOWS

#include "NativeGui/Win32/FileDialogImpl.hpp"

#include <cstring>
#include <cwchar>
#include <codecvt>
#include <locale>

namespace yapg
{
namespace impl
{

namespace
{

std::wstring stringToWide(const std::string& str)
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    return converter.from_bytes(str);
}

std::string wideToString(const std::wstring& wstr)
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    return converter.to_bytes(wstr);
}

std::size_t calculateBufferSize(const std::vector<FileDialog::Filter>& filters)
{
    std::size_t bufferSize = 0u;
    for(auto filterIt = filters.cbegin(); filterIt != filters.cend(); ++filterIt)
    {
        bufferSize += stringToWide(filterIt->name).size(); //The filter's name
        bufferSize += 1; // '\0'

        for(auto patternIt = filterIt->patterns.cbegin(); patternIt != filterIt->patterns.cend(); ++patternIt)
        {
            bufferSize += stringToWide(*patternIt).size(); //The pattern
            bufferSize += 1; //';' or '\0' for the last pattern
        }

        if(filterIt->patterns.size() == 0)
            bufferSize += 1;
    }

    bufferSize += 1; //For the last '\0'

    return bufferSize;
}

}

FileDialogImpl::FileDialogImpl(
    const std::string& title,
    FileDialog::Action action,
    const std::vector<FileDialog::Filter>& filters,
    std::size_t selectedFilter
    ) :
    m_fileDialogStruct(new OPENFILENAME()),
    m_action(action)
{
    std::wstring wideTitle = stringToWide(title);

    ZeroMemory(m_fileDialogStruct, sizeof(*m_fileDialogStruct));

    m_fileDialogStruct->lStructSize = sizeof(*m_fileDialogStruct); //Some strange WinAPI init...
    m_fileDialogStruct->hwndOwner = nullptr;
    m_fileDialogStruct->lpstrDefExt = nullptr;

    m_fileDialogStruct->lpstrFile = new TCHAR[MAX_PATH];
    m_fileDialogStruct->lpstrFile[0] = '\0';
    m_fileDialogStruct->nMaxFile = MAX_PATH;

    //Init the filters
    if(filters.size() > 0)
    {
        //Calculate the needed buffer size
        std::size_t filterBufferSize = calculateBufferSize(filters);

        //Construct the filters string
        TCHAR* filtersStr = (TCHAR*)malloc(filterBufferSize * sizeof(wchar_t));
        TCHAR* currentPos = filtersStr;
        for(auto filterIt = filters.cbegin(); filterIt != filters.cend(); ++filterIt)
        {
            wmemcpy(currentPos, stringToWide(filterIt->name).c_str(), stringToWide(filterIt->name).size());
            currentPos += stringToWide(filterIt->name).size();
            wmemcpy(currentPos, L"\0", 1);
            currentPos += 1;

            for(auto patternIt = filterIt->patterns.cbegin(); patternIt != filterIt->patterns.cend(); ++patternIt)
            {
                wmemcpy(currentPos, stringToWide(*patternIt).c_str(), stringToWide(*patternIt).size());
                currentPos += stringToWide(*patternIt).size();

                wmemcpy(currentPos, L";", 1);
                currentPos += 1;
            }

            wmemcpy(filterIt->patterns.size() > 0 ? (currentPos - 1) : currentPos, L"\0", 1);
            if(filterIt->patterns.size() == 0)
                currentPos += 1;
        }

        wmemcpy(currentPos, L"\0", 1);

        m_fileDialogStruct->lpstrFilter = filtersStr;
    }
    else
    {
        m_fileDialogStruct->lpstrFilter = nullptr;
    }

    m_fileDialogStruct->nFilterIndex = selectedFilter + 1;

    m_fileDialogStruct->lpstrInitialDir = nullptr;
    m_fileDialogStruct->lpstrTitle = wideTitle.c_str(); //The window title

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
    return wideToString(std::wstring(m_fileDialogStruct->lpstrFile));
}


}
}

#endif

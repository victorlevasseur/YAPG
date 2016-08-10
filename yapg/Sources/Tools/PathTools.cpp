#include "Tools/PathTools.hpp"

namespace fs = boost::filesystem;

namespace yapg
{

fs::path getHomePath()
{
    char const* home = getenv("HOME");
    if (home || ((home = getenv("USERPROFILE"))))
    {
        return std::string(home);
    }
    else
    {
        char const *hdrive = getenv("HOMEDRIVE");
        char const *hpath = getenv("HOMEPATH");
        return std::string(hdrive) + std::string(hpath);
    }
}

}

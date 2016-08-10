#ifndef YAPG_GAME_TEMPLATE_PACKAGE_H
#define YAPG_GAME_TEMPLATE_PACKAGE_H

#include <exception>
#include <string>

#include <boost/filesystem.hpp>

namespace yapg{ class LuaState; }

namespace yapg
{

class Package
{
public:

    class BadPackagePath : public std::runtime_error
    {
        //Reuse the std::runtime_error ctor
        using std::runtime_error::runtime_error;
    };

    class BadPackageInfo : public std::runtime_error
    {
        //Reuse the std::runtime_error ctor
        using std::runtime_error::runtime_error;
    };

    /**
     * Declare a package at packagePath and load its information (from the
     * PackageInfo.xml file located in packagePath).
     *
     * If an error happens, the ctor throws an exception:
     *  - Package::BadPackagePath if the path doesn't exists
     *  - Package::BadPackageInfo if the folder contains an invalid
     *    PackageInfo.xml file (or no PackageInfo.xml file)
     */
    Package(boost::filesystem::path packagePath);

    std::string getName() const;
    std::string getFriendlyName() const;
    boost::filesystem::path getAssetsPath() const;

    void loadTemplatesIntoLua(LuaState& luaState) const;

private:
    boost::filesystem::path m_fullPath; ///< Fullpath to the package folder
                                        ///(useful to locate the assets)

    std::string m_friendlyName;
};

bool operator<(const Package& lhs, const Package& rhs);
bool operator==(const Package& lhs, const Package& rhs);

}

#endif

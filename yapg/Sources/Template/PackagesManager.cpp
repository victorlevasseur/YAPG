#include "Template/PackagesManager.hpp"

#include <iostream>

#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

namespace yapg
{

std::unique_ptr<PackagesManager> PackagesManager::instance = std::unique_ptr<PackagesManager>(nullptr);

void PackagesManager::init(std::initializer_list<boost::filesystem::path> packagesPaths, const boost::filesystem::path& downloadPath)
{
    instance = std::unique_ptr<PackagesManager>(new PackagesManager(packagesPaths, downloadPath));
}

Package PackagesManager::getLocalPackage(const std::string& packageName) const
{
    cleanFuturePackages();

    //If the package is being downloaded, it may have its folder created.
    //However, we still need to ignore it to avoid incomplete packages.
    if(m_alreadyRequestedPackages.count(packageName) > 0)
        throw Package::BadPackagePath(packageName + " is not avaible locally!");

    //Search for the package in the directories
    for(const auto& packagesPath : m_packagesPaths)
    {
        try
        {
            return Package(fs::canonical(fs::path(packageName), packagesPath));
        }
        catch(...) {} //Ignore the package exceptions
    }

    //Not found locally!
    throw Package::BadPackagePath(packageName + " is not avaible locally!");
}

std::set<Package> PackagesManager::getLocalPackages() const
{
    std::set<Package> localPackages;

    //Each packages paths
    for(const auto& packagesPath : m_packagesPaths)
    {
        if(fs::exists(packagesPath))
        {
            //List all packages
            for(fs::directory_entry& packageEntry : fs::directory_iterator(packagesPath))
            {
                fs::path packagePath = packageEntry.path();
                if(fs::is_directory(packagePath))
                {
                    //This is a folder, so it is be a package.
                    std::string packageName = fs::relative(packagePath, packagesPath).string();
                    try
                    {
                        localPackages.insert(getLocalPackage(packageName)); //Note: getLocalPackage will throw if the package is being currently downloaded.
                    }
                    catch(...) {} //Ignore if the package is invalid or downloading !
                }
            }
        }
        else
        {
            std::cout << "[Packages/Warning] \"" << packagesPath.string() << "\" doesn't exists!" << std::endl;
        }
    }

    return localPackages;
}

std::shared_future<Package> PackagesManager::getPackage(const std::string& packageName) const
{
    cleanFuturePackages();

    if(m_alreadyRequestedPackages.count(packageName) == 0)
    {
        m_alreadyRequestedPackages[packageName] =
            std::async([&]() -> Package
            {
                //TODO: Download if needed
                for(const auto& packagesPath : m_packagesPaths)
                {
                    try
                    {
                        return Package(fs::canonical(fs::path(packageName), packagesPath));
                    }
                    catch(...) {} //Ignore the package exceptions
                }

                throw Package::BadPackagePath(packageName + " is not avaible locally!");
            })
            .share();
    }

    return m_alreadyRequestedPackages.at(packageName);
}

PackagesManager::PackagesManager(std::initializer_list<boost::filesystem::path> packagesPaths, const boost::filesystem::path& downloadPath) :
    m_packagesPaths(packagesPaths),
    m_downloadPath(downloadPath),
    m_alreadyRequestedPackages()
{
    std::cout << "[Packages] PackagesManager loaded." << std::endl;
}

void PackagesManager::cleanFuturePackages() const
{
    for(auto it = m_alreadyRequestedPackages.begin(); it != m_alreadyRequestedPackages.end(); )
    {
        if(it->second.valid())
            it = m_alreadyRequestedPackages.erase(it);
        else
            ++it;
    }
}

}

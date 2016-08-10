#ifndef YAPG_GAME_TEMPLATE_PACKAGESMANAGER
#define YAPG_GAME_TEMPLATE_PACKAGESMANAGER

#include <future>
#include <initializer_list>
#include <map>
#include <memory>
#include <set>
#include <string>

#include <boost/filesystem.hpp>

#include "Template/Package.hpp"

namespace yapg
{

class PackagesManager
{
public:
    static void init(std::initializer_list<boost::filesystem::path> packagesPaths, const boost::filesystem::path& downloadPath);
    static PackagesManager& get() { return *instance; }

    Package getLocalPackage(const std::string& packageName) const;
    std::set<Package> getLocalPackages() const;

    std::shared_future<Package> getPackage(const std::string& packageName) const;

protected:
    PackagesManager(std::initializer_list<boost::filesystem::path> packagesPaths, const boost::filesystem::path& downloadPath);

private:
    /**
     * Remove the successfully loaded/downloaded packages from m_alreadyRequestedPackages
     */
    void cleanFuturePackages() const;

    std::vector<boost::filesystem::path> m_packagesPaths;
    boost::filesystem::path m_downloadPath;

    mutable std::map<std::string, std::shared_future<Package>> m_alreadyRequestedPackages;

    static std::unique_ptr<PackagesManager> instance;
};

}

#endif

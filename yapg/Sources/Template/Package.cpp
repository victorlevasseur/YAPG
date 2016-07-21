#include "Template/Package.hpp"

#include "Lua/LuaState.hpp"
#include "Settings/tinyxml2.h"
#include "Tools/IterateTools.hpp"

namespace fs = boost::filesystem;
namespace tx2 = tinyxml2;

namespace yapg
{

Package::Package(fs::path packagePath) :
    m_fullPath(),
    m_friendlyName()
{
    try
    {
        m_fullPath = fs::canonical(packagePath);
    }
    catch(...)
    {
        throw BadPackagePath(packagePath.string() + " is not a valid path for a package!");
    }

    if(fs::exists(m_fullPath) && fs::is_directory(m_fullPath))
    {
        //Test if it's a package folder
        tx2::XMLDocument packageMetadata;
        std::string packageInfoFilePath = m_fullPath.string() + "/PackageInfo.xml";
        if(packageMetadata.LoadFile(packageInfoFilePath.data()) != tx2::XML_NO_ERROR)
        {
            throw BadPackageInfo("No PackageInfo.xml in \"" + m_fullPath.string() + "\"!");
        }

        tx2::XMLElement* rootElem = packageMetadata.RootElement();
        if(strcmp(rootElem->Name(), "yapg_package") != 0)
        {
            throw BadPackageInfo("Invalid PackageInfo.xml in \"" + m_fullPath.string() + "\"!");
        }

        tx2::XMLElement* packageFriendlyElem = rootElem->FirstChildElement("friendly_name");
        if(packageFriendlyElem->GetText() == nullptr)
        {
            throw BadPackageInfo("Invalid PackageInfo.xml in \"" + m_fullPath.string() + "\"!");
        }
        m_friendlyName = std::string(packageFriendlyElem->GetText());
        std::cout << "[Packages/Note] Package " << getName() << " loaded." << std::endl;
    }
    else
    {
        throw BadPackagePath(packagePath.string() + " is not a valid path for a package!");
    }
}

std::string Package::getName() const
{
    return m_fullPath.filename().string();
}

std::string Package::getFriendlyName() const
{
    return m_friendlyName;
}

fs::path Package::getAssetsPath() const
{
    return fs::absolute(fs::path("assets"), m_fullPath);
}

void Package::loadTemplatesIntoLua(LuaState& luaState) const
{
    //Lists the templates of the package
    std::vector<std::reference_wrapper<EntityTemplate>> loadedTemplates;

    for(fs::directory_entry& templateFileEntry : fs::directory_iterator(m_fullPath))
    {
        fs::path templateFilePath = templateFileEntry.path();
        if(fs::is_regular_file(templateFilePath) && templateFilePath.extension().string() == ".lua")
        {
            try
            {
                luaState.getState().script_file(templateFilePath.string());

                loadedTemplates.push_back(
                    std::ref(
                        luaState.declareTemplate(
                            EntityTemplate(luaState.getState().get<sol::table>("entity_template"), getName(), getAssetsPath().string())
                        )
                    )
                );

                std::cout << "[Lua/Note] Loaded template from " << templateFilePath << "." << std::endl;
            }
            catch(const sol::error& e)
            {
                std::cout << "[Lua/Warning] Can't load \"" << templateFilePath << "\" because it contains an error :" << std::endl;
                std::cout << e.what() << std::endl;
            }
        }
    }

    //Apply inheritance to all of them
    iterate_tools::safeForEach(
        loadedTemplates,
        [&luaState](EntityTemplate& entityTemplate) -> bool
        {
            try
            {
                entityTemplate.applyInheritance(luaState);
            }
            catch(const std::out_of_range& e)
            {
                //The template didn't find its base template, remove it from the templates
                std::cout << "[Lua/Warning] Can't find the base template of \"" << entityTemplate.getName() << "\", the template inheritance will be ignored !" << std::endl;
                return false;
            }

            return true;
        }
    );
}

}

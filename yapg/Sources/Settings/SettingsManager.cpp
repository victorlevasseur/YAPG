#include "Settings/SettingsManager.hpp"

#include <cstring>
#include <exception>
#include <iostream>

#include "Settings/KeySettings.hpp"
#include "Settings/tinyxml2.h"

namespace tx2 = tinyxml2;

namespace yapg
{

SettingsManager::SettingsManager(const std::string& configFileName) :
    m_configFileName(configFileName),
    m_keySettings(std::make_unique<KeySettings>())
{
    std::cout << "[Settings/Note] Loading settings..." << std::endl;

    //Load the settings
    tx2::XMLDocument configXML;
    if(configXML.LoadFile(m_configFileName.data()) != tx2::XML_NO_ERROR)
        throw std::runtime_error("[SettingsManager/Error] Ill-formed configuration file (parsing error) !");

    tx2::XMLElement* rootElem = configXML.RootElement();
    if(strcmp(rootElem->Name(), "yapg_settings") != 0)
        throw std::runtime_error("[SettingsManager/Error] Ill-formed configuration file (not a yapg config file) !");

    //Load the key settings
    tx2::XMLElement* keySettingsElem = rootElem->FirstChildElement("keys_settings");
    if(keySettingsElem)
    {
        m_keySettings->loadFromXml(keySettingsElem);
    }
    else
    {
        std::cout << "[Settings/Warning] Can't find the keyboard settings !" << std::endl;
    }

    std::cout << "[Settings/Note] Settings loaded." << std::endl;
}

SettingsManager::~SettingsManager()
{
    std::cout << "[Settings/Note] Saving settings..." << std::endl;

    //Save the settings
    tx2::XMLDocument configXML;
    configXML.InsertEndChild(configXML.NewElement("yapg_settings"));

    tx2::XMLElement* keySettingsElem = configXML.NewElement("keys_settings");
    configXML.RootElement()->InsertEndChild(keySettingsElem);
    m_keySettings->saveToXml(&configXML, keySettingsElem);

    configXML.SaveFile(m_configFileName.data());

    std::cout << "[Settings/Note] Settings saved." << std::endl;
}

KeySettings& SettingsManager::getKeySettings()
{
    return *m_keySettings;
}

const KeySettings& SettingsManager::getKeySettings() const
{
    return *m_keySettings;
}

}

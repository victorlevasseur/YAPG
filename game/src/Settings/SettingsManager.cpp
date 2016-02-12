#include "Settings/SettingsManager.hpp"

#include <iostream>

#include "Settings/KeySettings.hpp"
#include "Settings/tinyxml2.h"

namespace tx2 = tinyxml2;

namespace settings
{

SettingsManager::SettingsManager(const std::string& configFileName) :
    m_configFileName(configFileName),
    m_keySettings(std::make_unique<KeySettings>())
{
    std::cout << "[Settings/Note] Loading settings..." << std::endl;

    //Load the settings
    tx2::XMLDocument configXML;
    configXML.LoadFile(m_configFileName.data());

    tx2::XMLElement* rootElem = configXML.RootElement();

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
    //Save the settings
    tx2::XMLDocument configXML;
    //TODO: Save the settings
    configXML.SaveFile(m_configFileName.data());
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

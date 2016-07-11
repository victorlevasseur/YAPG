#ifndef YAPG_GAME_SETTINGS_SETTINGSMANAGER_H
#define YAPG_GAME_SETTINGS_SETTINGSMANAGER_H

#include <map>
#include <memory>
#include <string>

namespace tinyxml2{ class XMLDocument; }
namespace settings{ class KeySettings; }

namespace settings
{

class SettingsManager
{
public:
    SettingsManager(const std::string& configFileName);
    ~SettingsManager();

    KeySettings& getKeySettings();
    const KeySettings& getKeySettings() const;

private:
    std::string m_configFileName;
    std::unique_ptr<KeySettings> m_keySettings;
};

}

#endif

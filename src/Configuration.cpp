#include "../include/Configuration.hpp"

#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

std::map<std::string, float> Configuration::floatValues;
std::map<std::string, std::string> Configuration::stringValues;
std::map<std::string, int> Configuration::keyBinds;

using json = nlohmann::json;

void Configuration::loadConfiguration(const std::string& resourceBasePath) {
    setValue("ResourceBasePath", resourceBasePath);

    std::ifstream stream;
    // load Settings.json
    stream.open(resourceBasePath + "/Misc/Settings.json");
    if (!stream.good()) {
        std::cout << "Failed to open Settings.json" << std::endl;
        exit(1);
    }

    json root;
    stream >> root;
    stream.close();
    floatValues = root.get<std::map<std::string, float>>();

    // load Keybinds.json
    stream.open(resourceBasePath + "/Misc/Keybinds.json");
    if (!stream.good()) {
        std::cout << "Failed to open Keybinds.json" << std::endl;
        exit(1);
    }

    stream >> root;
    stream.close();
    keyBinds = root.get<std::map<std::string, int>>();
}

float Configuration::getFloatValue(const std::string &location)
{
    return floatValues[location];
}

const std::string& Configuration::getStringValue(const std::string &location)
{
    return stringValues[location];
}

void Configuration::setValue(const std::string& location, float value)
{
    floatValues[location] = value;
}

void Configuration::setValue(const std::string& location, std::string value)
{
    stringValues[location] = value;
}

int Configuration::getAssociatedKey(const std::string &location)
{
    return keyBinds[location];
}

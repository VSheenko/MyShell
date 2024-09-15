//
// Created by vovan on 15.09.2024.
//

#include "XmlWorker.h"
#include <iostream>
#include <unordered_map>

XmlWorker::XmlWorker(const std::string& s_path) {
    bool flag = SetAppsettingsPath(s_path);
    xml::XMLError er = xml::XML_ERROR_EMPTY_DOCUMENT;

    if (flag)
        er = config.LoadFile(file_path.string().c_str());
    else
        std::cerr << "Invalid path to configuration file" << std::endl;


    if (er != xml::XML_SUCCESS) {
        std::cerr << "Default settings will be used" << std::endl;

        std::unordered_map<std::string, std::string> settings {
                {"user_name", "ivan"},
                {"pc_name", "MainPC"},
                {"archive_path", "filesystem.zip"},
                {"log_path", "logs/"},
                {"start_script_path", "-"}
        };


        xml::XMLElement* p_settings_list = config.NewElement("settings");

        for (const auto& [tag, value] : settings) {
            xml::XMLElement* p_list_element = config.NewElement(tag.c_str());
            p_list_element->SetText(value.c_str());

            p_settings_list->InsertFirstChild(p_list_element);
        }

        config.InsertFirstChild(p_settings_list);
        config.InsertFirstChild(config.NewDeclaration());

        er = config.SaveFile("appsettings.xml");

        if (er != xml::XML_SUCCESS)
            std::cerr << "Error saving appsettings.xml file" << std::endl;
    }
}

bool XmlWorker::SetAppsettingsPath(const std::string& s_path) {
    fs::path temp_path = s_path;

    if (exists(temp_path) && temp_path.extension() == ".xml") {
        file_path = temp_path;
        return true;
    }
    return false;
}

std::string XmlWorker::GetValue(const std::string &) {
    return std::string();
}

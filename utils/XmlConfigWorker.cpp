#include "XmlConfigWorker.h"
#include <iostream>
#include <unordered_map>
#include <algorithm>
XmlConfigWorker::XmlConfigWorker(const std::string& s_path) {
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
                {"pc_name", "host"},
                {"archive_path", "archive.zip"},
                {"log_path", "logs/"},
                {"commands_path", "commands/"}
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

bool XmlConfigWorker::SetAppsettingsPath(const std::string& s_path) {
    fs::path temp_path = s_path;

    if (exists(temp_path) && temp_path.extension() == ".xml") {
        file_path = temp_path;
        return true;
    }
    return false;
}

std::vector<std::string> XmlConfigWorker::GetValues(const std::string& s_path) {
    std::vector<std::string> vec_path = utils::Split(s_path, "/");
    std::vector<std::string> res;

    if (vec_path.size() <= 1) {
        return {};
    }

    if(config.RootElement()->Name() != vec_path[0])
        return {};
    vec_path.erase(vec_path.begin());

    std::vector<xml::XMLElement*> cur_elems = GetElements(config.RootElement(), vec_path[0]);
    vec_path.erase(vec_path.begin());

    for (int i = 0; i < vec_path.size(); i++) {
        std::string s_tag = vec_path[i];

        std::vector<xml::XMLElement*> temp;
        std::copy(cur_elems.begin(), cur_elems.end(), std::back_inserter(temp));
        cur_elems.clear();

        for (const auto& elem: temp) {
            for (const auto& for_add : GetElements(elem, s_tag))
                cur_elems.push_back(for_add);
        }
    }

    for (const auto& elem : cur_elems)
        if (elem->ChildElementCount() == 0)
            res.push_back(elem->GetText());


    return res;
}

std::vector<xml::XMLElement*> XmlConfigWorker::GetElements(xml::XMLElement* root, std::string& s_tag) {
    xml::XMLElement* cur = root->FirstChildElement();
    std::vector<xml::XMLElement*> res;

    while (cur != nullptr) {
        std::string cur_tag = cur->Name();
        if (cur_tag == s_tag)
            res.push_back(cur);

        cur = cur->NextSiblingElement();
    }

    return res;
}



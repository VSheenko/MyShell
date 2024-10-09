#include "Shell.h"


Shell::Shell(const std::string& s_config_path) {
    XmlConfigWorker xml_worker(s_config_path);

    logger = new Logger(xml_worker.GetValues("settings/log_path")[0]);
    logger->Log(LogLevel::INFO, "The shell has been created");

    SetUserName(xml_worker.GetValues("settings/user_name")[0]);
    SetPcName(xml_worker.GetValues("settings/pc_name")[0]);
    SetArchivePath(xml_worker.GetValues("settings/archive_path")[0]);


    InitCommands(xml_worker.GetValues("settings/commands_path")[0]);

    sharedData = new SharedData();
    sharedData->CreateFileMap();
}



void Shell::SetUserName(const std::string& s_name) {
    const std::regex r("^[a-z_][a-z0-9_-]{0,31}$");

    if (!std::regex_match(s_name, r)) {
        logger->Log(LogLevel::WARNING, "Incorrect user name format: " + s_name);
        std::cerr << "Incorrect user name format\n";

        if (cur_user.empty()){
            std::cerr << "The name ivan will be used" << '\n';
            cur_user = "ivan";
        }

    } else {
        cur_user = s_name;
        logger->Log(LogLevel::INFO, "User name has been set: " + s_name);
    }
}

void Shell::SetPcName(const std::string& s_name) {
    const std::regex r("^[a-zA-Z]([a-zA-Z0-9-]{0,61}[a-zA-Z0-9])?$");

    if(!std::regex_match(s_name, r)) {
        logger->Log(LogLevel::WARNING, "Incorrect pc name format: " + s_name);
        std::cerr << "Incorrect pc name format\n";

        if (pc_name.empty()){
            std::cerr << "The name MainPc will be used" << '\n';
            pc_name = "MainPc";
        }
    } else {
        logger->Log(LogLevel::INFO, "Pc name has been set: " + s_name);
        pc_name = s_name;
    }
}

void Shell::SetCurPath(std::string s_path) {
    ArchiveZipWorker zip_worker(archive_path.string());

    std::string abs_path = cur_path_in_archive + "/" + s_path;
    abs_path = ArchiveZipWorker::NormalizeVirtualPath(abs_path);

    std::string r_path = ArchiveZipWorker::NormalizeVirtualPath(s_path);

    if (abs_path.empty()) {
        cur_path_in_archive = abs_path; // root
    } else if (zip_worker.FolderExist(abs_path)) {
        cur_path_in_archive = abs_path;
    } else if (zip_worker.FolderExist(r_path)) {
        cur_path_in_archive = r_path;
    } else {
        logger->Log(LogLevel::WARNING, "No such directory for changing path " + s_path);
        std::cerr << "No such directory" << std::endl;
        return;
    }

    logger->Log(LogLevel::INFO, "Path has been changed " + s_path);
}

int Shell::ExecShell() {
    std::string s;

    PrintSystemInvitation();
    logger->Log(LogLevel::INFO, "Shell has been opened");

    while (std::getline(std::cin, s)) {
        if (s.empty()) {
            PrintSystemInvitation();
            continue;
        }

        if (s == EXIT_COMMAND)
            break;

        std::vector<std::string> comm_split = utils::Split(s, " ");

        if (command_links.count(comm_split[0])) {
            logger->Log(LogLevel::INFO, "Command has been found: " + comm_split[0] + " " + s);
            size_t pos = s.find(" ");
            s.erase(0, pos + 1);

            // Мда...
            ExecCommand(command_links[comm_split[0]].string() + " " +
                        (comm_split.size() != 1 ? s : ""));
        } else if (comm_split[0] == "cd" && comm_split.size() >= 2) {
            SetCurPath(comm_split[1]);
        } else {
            logger->Log(LogLevel::WARNING, "Command not found: " + comm_split[0]);
            std::cout << comm_split[0] << ": command not found" << '\n';
        }

        PrintSystemInvitation();
    }

    logger->Log(LogLevel::INFO, "Shell has been closed");
    return 0;
}

bool Shell::InitCommands(const std::string& commands_root_folder) {
    bool er_flag = fs::exists(commands_root_folder);

    if (!er_flag) {
        logger->Log(LogLevel::ERR, "No such folder: " + commands_root_folder);

        std::cerr << "No such folder" << '\n';
        return false;
    }

    fs::path command_path;
    for (auto& p : fs::directory_iterator(commands_root_folder)) {
        command_path = p;
        if (command_path.extension() == ".exe")
            command_links[command_path.filename().replace_extension("").string()] = command_path;
    }

    logger->Log(LogLevel::INFO, "Commands have been initialized");
    return true;
}

void Shell::ExecCommand(const std::string& args) {
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    // Обнуление структур
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    si.dwFlags |= STARTF_USESTDHANDLES;  // Указываем, что будем передавать стандартные дескрипторы
    si.hStdInput = GetStdHandle(STD_INPUT_HANDLE);   // Наследуем стандартный ввод
    si.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE); // Наследуем стандартный вывод
    si.hStdError = GetStdHandle(STD_ERROR_HANDLE);   // Наследуем стандартный поток ошибок

    sharedData->UpdateFields(archive_path.string(), cur_path_in_archive);
    sharedData->SerializeSharedData();

    // Запуск нового процесса
    if (CreateProcess(
            nullptr,              // имя исполняемого файла
            (LPSTR)args.data(),  // Командная строка для запуска программы
            nullptr,              // Дескриптор безопасности процесса
            nullptr,              // Дескриптор безопасности потока
            TRUE,                 // Наследование дескрипторов
            0,                    // Флаги создания (можно добавить CREATE_NEW_CONSOLE для новой консоли)
            nullptr,              // Среда
            nullptr,              // Текущий каталог
            &si,                  // Указатель на STARTUPINFO
            &pi                   // Указатель на PROCESS_INFORMATION
    )) {
        // Ожидание завершения процесса
        WaitForSingleObject(pi.hProcess, INFINITE);
        logger->Log(LogLevel::INFO, "Command has been executed");

        sharedData->DeserializeSharedData();
        cur_path_in_archive = sharedData->s_path_in_archive;

        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    } else {
        logger->Log(LogLevel::ERR, "Error while executing command");
        std::cerr << "Error!" << std::endl;
    }
}

void Shell::PrintSystemInvitation() {
    std::string s_path = cur_path_in_archive;
    std::replace(s_path.begin(), s_path.end(), '\\', '/');

    std::cout << "[" << cur_user << "@" << pc_name << " /" << s_path << "]$ ";
}

void Shell::SetArchivePath(const std::string &s_path) {
    fs::path dest_path = s_path;

    if (fs::exists(dest_path) && dest_path.extension() == ".zip") {
        archive_path = dest_path;
        logger->Log(LogLevel::INFO, "Archive path has been set");
    } else {
        logger->Log(LogLevel::WARNING, "Wrong archive path " + s_path);
        std::cerr << "Wrong path\n Default archive will be used" << '\n';
        archive_path = fs::current_path() / "archive.zip";
    }
}

Shell::~Shell() {
    delete logger;
    delete sharedData;
}



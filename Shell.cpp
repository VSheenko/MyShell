#include "Shell.h"


Shell::Shell(const std::string& s_config_path) {
    XmlConfigWorker xml_worker(s_config_path);

    SetUserName(xml_worker.GetValues("settings/user_name")[0]);
    SetPcName(xml_worker.GetValues("settings/pc_name")[0]);
    SetArchivePath(xml_worker.GetValues("settings/archive_path")[0]);

    logger = new Logger(xml_worker.GetValues("settings/log_path")[0]);

    InitCommands();
}



void Shell::SetUserName(const std::string& s_name) {
    const std::regex r("^[a-z_][a-z0-9_-]{0,31}$");

    if (!std::regex_match(s_name, r)) {
        std::cerr << "Incorrect user name format\n";

        if (cur_user.empty()){
            std::cerr << "The name ivan will be used" << '\n';
            cur_user = "ivan";
        }
    } else {
        cur_user = s_name;
    }
}

void Shell::SetPcName(const std::string& s_name) {
    const std::regex r("^[a-zA-Z]([a-zA-Z0-9-]{0,61}[a-zA-Z0-9])?$");

    if(!std::regex_match(s_name, r)) {
        std::cerr << "Incorrect pc name format\n";

        if (pc_name.empty()){
            std::cerr << "The name MainPc will be used" << '\n';
            pc_name = "MainPc";
        }
    } else {
        pc_name = s_name;
    }
}

void Shell::SetCurPath(std::string s_path) {
    ArchiveZipWorker zip_worker(archive_path.string());

    if (s_path[s_path.size() -1] == '/') {
        s_path.erase(s_path.size() - 1, 1);
    }

    fs::path abs_path = cur_path_in_archive / s_path;
    abs_path = ArchiveZipWorker::NormalizeVirtualPath(abs_path);

    fs::path r_path = s_path;
    r_path = ArchiveZipWorker::NormalizeVirtualPath(r_path);

    if (abs_path.empty()) {
        cur_path_in_archive = abs_path; // root
    } else if (zip_worker.FolderExist(abs_path.string())) {
        cur_path_in_archive = abs_path;
    } else if (zip_worker.FolderExist(r_path.string())) {
        cur_path_in_archive = r_path;
    } else {
        std::cerr << "No such directory" << std::endl;
    }
}

int Shell::ExecShell() {
    std::string s;

    PrintSystemInvitation();
    while (std::getline(std::cin, s)) {
        if (s.empty()) {
            PrintSystemInvitation();
            continue;
        }

        if (s == EXIT_COMMAND)
            break;

        std::vector<std::string> comm_split = UtilsMini::Split(s, " ");

        if (command_links.count(comm_split[0])) {
            size_t pos = s.find(" ");
            s.erase(0, pos + 1);

            // Мда...
            ExecCommand(command_links[comm_split[0]].string() + " /" + cur_path_in_archive.string() + " " +
                        (comm_split.size() != 1 ? s : ""));
        } else if (comm_split[0] == "cd" && comm_split.size() >= 2) {
            SetCurPath(comm_split[1]);
        } else {
            std::cout << comm_split[0] << ": command not found" << '\n';
        }

        PrintSystemInvitation();
    }
    return 0;
}

bool Shell::InitCommands(const std::string& commands_root_folder) {
    bool er_flag = fs::exists(commands_root_folder);

    if (!er_flag) {
        std::cerr << "No such folder" << '\n';
        return false;
    }

    fs::path command_path;
    for (auto& p : fs::directory_iterator(commands_root_folder)) {
        command_path = p;
        if (command_path.extension() == ".exe")
            command_links[command_path.filename().replace_extension("").string()] = command_path;
    }

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

        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    } else {
        std::cerr << "Error!" << std::endl;
    }
}

void Shell::PrintSystemInvitation() {
    std::string s_path = cur_path_in_archive.string();
    std::replace(s_path.begin(), s_path.end(), '\\', '/');

    std::cout << "[" << cur_user << "@" << pc_name << " /" << s_path << "]$ ";
}

void Shell::SetArchivePath(const std::string &s_path) {
    fs::path dest_path = s_path;

    if (fs::exists(dest_path) && dest_path.extension() == ".zip") {
        archive_path = dest_path;
    } else {
        std::cerr << "Wrong path\n Default archive will be used" << '\n';
        archive_path = fs::current_path() / "archive.zip";
    }
}

Shell::~Shell() {
    delete logger;
}

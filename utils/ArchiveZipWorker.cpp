#include <algorithm>
#include "ArchiveZipWorker.h"

bool ArchiveZipWorker::AddFile(std::string s_path) {
    const char* data = "";

    bool flag;
}

void ArchiveZipWorker::ChangeDirectory(std::string) {

}

bool ArchiveZipWorker::GetFileData(std::string s_path, std::vector<char>& buffer) {
    mz_zip_archive archive;
    bool er_flag = ZipReaderInit(archive);

    if (!er_flag)
        return false;

    std::replace(s_path.begin(), s_path.end(), '\\', '/');

    int file_index = mz_zip_reader_locate_file(&archive, s_path.c_str(), nullptr, 0);

    if (file_index < 0) {
        mz_zip_reader_end(&archive);
        return false;
    }

    mz_zip_archive_file_stat file_stat;
    er_flag = mz_zip_reader_file_stat(&archive, file_index, &file_stat);
    if (!er_flag) {
        mz_zip_reader_end(&archive);
        return false;
    }

    buffer.resize(file_stat.m_uncomp_size);
    er_flag = mz_zip_reader_extract_to_mem(&archive, file_index, buffer.data(), buffer.size(), 0);
    if (!er_flag){
        std::cerr << "Error writing file to buffer" << '\n';
        mz_zip_reader_end(&archive);
        return false;
    }

    mz_zip_reader_end(&archive);
    return true;
}

ArchiveZipWorker::ArchiveZipWorker(const std::string& s_path) {
    fs::path dest_path = s_path;

    if (fs::exists(dest_path) && dest_path.extension() == ".zip") {
        archive_path = dest_path;
    } else {
        std::cerr << "Wrong path\n. Default archive will be used" << '\n';
        archive_path = fs::current_path() / "archive.zip";
    }
}

bool ArchiveZipWorker::GetAllFilesNameInFolder(std::string s_path, std::vector<std::string>& res_buffer) {
    fs::path dest_path = s_path;
    bool er_flag = FolderExist(dest_path.string());

    if (!er_flag)
        return false;

    mz_zip_archive archive;
    er_flag = ZipReaderInit(archive);

    if (!er_flag)
        return false;

    mz_uint num_files = mz_zip_reader_get_num_files(&archive);

    for (mz_uint i = 0; i < num_files; i++) {
        mz_zip_archive_file_stat file_stat;
        er_flag = mz_zip_reader_file_stat(&archive, i, &file_stat);

        if (!er_flag) {
            std::cerr << "Error reading file" << '\n';
            mz_zip_reader_end(&archive);
            return false;
        }

        std::string path_name = std::string(file_stat.m_filename);
        std::vector<std::string> tokens_1 = utils::Split(path_name, "/");
        std::vector<std::string> tokens_2 = utils::Split(dest_path.string(), "/");

        bool flag = (tokens_1.size() - 1) == tokens_2.size();
        for (int ind = 0; ind < tokens_2.size(); ind++)
            flag = flag & (tokens_1[ind] == tokens_2[ind]);

        if (flag)
            res_buffer.push_back(file_stat.m_is_directory ? tokens_1[tokens_1.size()-1] + "/" : tokens_1[tokens_1.size()-1]);
    }

    mz_zip_reader_end(&archive);
    return true;
}

bool ArchiveZipWorker::FolderExist(const std::string& s_path) {
    fs::path dest_path =  (s_path + "/");

    if (dest_path == "/")
        return true;

    mz_zip_archive archive;
    bool er_flag = ZipReaderInit(archive);

    if (!er_flag)
        return false;

    std::string s = dest_path.string();
    std::replace(s.begin(), s.end(), '\\', '/');

    bool res = FindPath(archive, s);
    mz_zip_reader_end(&archive);
    return res;
}

bool ArchiveZipWorker::ZipReaderInit(mz_zip_archive &archive) {
    memset(&archive, 0, sizeof(archive));

    bool er_flag = mz_zip_reader_init_file(&archive, archive_path.string().c_str(), 0);

    if (!er_flag) {
        std::cerr << "Error init zip file" << '\n';
        mz_zip_reader_end(&archive);
    }

    return er_flag;
}

bool ArchiveZipWorker::FindPath(mz_zip_archive& archive, const std::string &s_path) {
    mz_uint files_num = mz_zip_reader_get_num_files(&archive);

    for (mz_uint i = 0; i < files_num; i++) {
        mz_zip_archive_file_stat file_stat;
        bool er_flag = mz_zip_reader_file_stat(&archive, i, &file_stat);

        if (!er_flag) {
            std::cerr << "Error reading file" << '\n';
            return false;
        }

        std::string name_path = file_stat.m_filename;
        if (s_path == name_path)
            return true;
    }

    return false;
}

bool ArchiveZipWorker::FileExist(const std::string &s_path) {
    fs::path dest_path = s_path;

    mz_zip_archive archive;
    bool er_flag = ZipReaderInit(archive);

    if (!er_flag)
        return false;

    std::string s = dest_path.string();
    std::replace(s.begin(), s.end(), '\\', '/');

    bool res = FindPath(archive, s);
    mz_zip_reader_end(&archive);
    return res;
}

fs::path ArchiveZipWorker::NormalizeVirtualPath(const fs::path& temp_path) {
    std::vector<fs::path> parts;

    for (const auto& part : temp_path) {
        if (part == ".")
            continue;
        else if (part == "..") {
            if (!parts.empty())
                parts.pop_back();
        } else {
            parts.push_back(part);
        }
    }

    fs::path res;

    for (const auto& part : parts)
        res /= part;

    return res;
}



std::string ArchiveZipWorker::GetAbsPath(std::string inp_path, std::string cur_path_in_archive) {
    if (inp_path[inp_path.size() - 1] == '/') {
        inp_path.erase(inp_path.size() - 1, 1);
    }

    fs::path abs_path = fs::path(cur_path_in_archive) / fs::path(inp_path);
    abs_path = ArchiveZipWorker::NormalizeVirtualPath(abs_path);

    fs::path r_path = inp_path;
    r_path = ArchiveZipWorker::NormalizeVirtualPath(r_path);

    if (abs_path.empty()) {
        return abs_path.string(); // root
    } else if (FolderExist(abs_path.string())) {
        return abs_path.string();
    } else if (FolderExist(r_path.string())) {
        return r_path.string();
    } else {
        return "No such file or directory";
    }
}

std::string ArchiveZipWorker::GetAbsPathFile(std::string inp_path, std::string cur_path_in_archive) {
    fs::path abs_path = fs::path(cur_path_in_archive) / fs::path(inp_path);
    abs_path = ArchiveZipWorker::NormalizeVirtualPath(abs_path);
    std::string s_abs_path = abs_path.string();

    fs::path r_path = inp_path;
    r_path = ArchiveZipWorker::NormalizeVirtualPath(r_path);
    std::string s_r_path = r_path.string();

    if (s_abs_path[s_abs_path.size() - 1] == '/')
        s_abs_path.erase(s_abs_path.size() - 1, 1);
    if (s_r_path[s_r_path.size() - 1] == '/')
        s_r_path.erase(s_r_path.size() - 1, 1);


    if (FileExist(abs_path.string())) {
        return abs_path.string();
    } else if (FileExist(r_path.string())) {
        return r_path.string();
    } else {
        return "No such file or directory";
    }
}


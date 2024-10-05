#include <algorithm>
#include "ArchiveZipWorker.h"


bool ArchiveZipWorker::GetFileData(std::string s_path, std::vector<char>& buffer) {
    mz_zip_archive archive;
    ZipReaderInit(archive);

    std::replace(s_path.begin(), s_path.end(), '\\', '/');

    int file_index = mz_zip_reader_locate_file(&archive, s_path.c_str(), nullptr, 0);

    if (file_index < 0) {
        mz_zip_reader_end(&archive);
        return false;
    }

    mz_zip_archive_file_stat file_stat;
    bool er_flag = mz_zip_reader_file_stat(&archive, file_index, &file_stat);

    if (!er_flag) {
        mz_zip_reader_end(&archive);
        return false;
    }

    buffer.resize(file_stat.m_uncomp_size);
    er_flag = mz_zip_reader_extract_to_mem(&archive, file_index, buffer.data(), buffer.size(), 0);
    if (!er_flag){
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
        std::cerr << "Wrong path......\nDefault archive will be used" << '\n';
        archive_path = fs::current_path() / "archive.zip";
    }
}

bool ArchiveZipWorker::GetAllFilesNameInFolder(std::string s_path, std::vector<std::string>& res_buffer) {
    bool er_flag = FolderExist(s_path);

    if (!er_flag)
        return false;

    mz_zip_archive archive;

    ZipReaderInit(archive);


    mz_uint num_files = mz_zip_reader_get_num_files(&archive);

    res_buffer.clear();
    for (mz_uint i = 0; i < num_files; i++) {
        mz_zip_archive_file_stat file_stat;
        er_flag = mz_zip_reader_file_stat(&archive, i, &file_stat);

        if (!er_flag) {
            mz_zip_reader_end(&archive);
            continue;
        }

        std::string path_name = std::string(file_stat.m_filename);
        std::vector<std::string> tokens_1 = utils::Split(path_name, "/");
        std::vector<std::string> tokens_2 = utils::Split(s_path, "/");

        bool flag = (tokens_1.size() - 1) == tokens_2.size();
        for (int ind = 0; ind < tokens_2.size(); ind++)
            flag = flag & (tokens_1[ind] == tokens_2[ind]);

        if (flag)
            res_buffer.push_back(file_stat.m_is_directory ? tokens_1[tokens_1.size()-1] + "/" : tokens_1[tokens_1.size()-1]);
    }

    mz_zip_reader_end(&archive);
    return true;
}

bool ArchiveZipWorker::FolderExist(std::string s_path) {
    if (s_path.empty() || s_path == "/")
        return true;

    GetUnixPath(s_path);
    s_path += s_path[s_path.size() - 1] != '/' ? "/" : "";

    return FindPath(s_path);
}

void ArchiveZipWorker::ZipReaderInit(mz_zip_archive &archive) {
    memset(&archive, 0, sizeof(archive));

    bool er_flag = mz_zip_reader_init_file(&archive, archive_path.string().c_str(), 0);

    if (!er_flag)
        throw std::runtime_error("Error init zip file");
}

bool ArchiveZipWorker::FindPath(const std::string &s_path) {
    mz_zip_archive archive;

    ZipReaderInit(archive);

    mz_uint files_num = mz_zip_reader_get_num_files(&archive);

    for (mz_uint i = 0; i < files_num; i++) {
        mz_zip_archive_file_stat file_stat;

        if (!mz_zip_reader_file_stat(&archive, i, &file_stat))
            continue;

        if (s_path == file_stat.m_filename) {
            mz_zip_reader_end(&archive);
            return true;
        }
    }

    mz_zip_reader_end(&archive);
    return false;
}

bool ArchiveZipWorker::FileExist(std::string s_path) {
    if (s_path.empty())
        return false;

    GetUnixPath(s_path);
    if (s_path[s_path.size() - 1] == '/')
        s_path.erase(s_path.size() - 1, 1);

    return FindPath(s_path);
}

std::string ArchiveZipWorker::NormalizeVirtualPath(const std::string& temp_path) {
    std::string s = GetUnixPath(temp_path);
    std::vector<std::string> parts = utils::Split(s, "/");
    std::vector<std::string> res_parts;

    for (const auto& part : parts) {
        if (part == "." || part.empty())
            continue;
        else if (part == "..") {
            if (!res_parts.empty())
                res_parts.pop_back();
        } else {
            res_parts.push_back(part);
        }
    }

    std::string s_path_res = !res_parts.empty() ? res_parts[0] : "";

    for (int i = 1; i < res_parts.size(); i++)
        s_path_res += "/" + res_parts[i];

    return s_path_res;
}



bool ArchiveZipWorker::GetAbsPathDir(std::string& result_path, std::string inp_path, std::string cur_path_in_archive) {
    if (inp_path[inp_path.size() - 1] == '/') {
        inp_path.erase(inp_path.size() - 1, 1);
    }

    std::string abs_path = cur_path_in_archive + "/" + inp_path;
    abs_path = ArchiveZipWorker::NormalizeVirtualPath(abs_path);

    std::string r_path = ArchiveZipWorker::NormalizeVirtualPath(inp_path);

    if (abs_path.empty()) {
        result_path = abs_path; // root
    } else if (FolderExist(abs_path)) {
        result_path = abs_path;
    } else if (FolderExist(r_path)) {
        result_path = r_path;
    } else {
        return false;
    }

    return true;
}

bool ArchiveZipWorker::GetAbsPathFile(std::string& result_path, std::string inp_path, std::string cur_path_in_archive) {
    std::string abs_path = cur_path_in_archive + "/" + std::string(inp_path);
    abs_path = ArchiveZipWorker::NormalizeVirtualPath(abs_path);

    std::string r_path = ArchiveZipWorker::NormalizeVirtualPath(inp_path);

    if (FileExist(abs_path)) {
        result_path = abs_path;
    } else if (FileExist(r_path)) {
        result_path = r_path;
    } else {
        return false;
    }

    return true;
}

std::string ArchiveZipWorker::GetUnixPath(std::string s) {
    std::string toReplace = "\\";
    std::string replacement = "/";

    size_t pos = 0;
    while ((pos = s.find(toReplace, pos)) != std::string::npos) {
        s.replace(pos, toReplace.length(), replacement);
        pos += replacement.length();  // сдвиг позиции для поиска следующих вхождений
    }

    return s;
}


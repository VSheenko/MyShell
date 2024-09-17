#include "ArchiveZipWorker.h"

bool ArchiveZipWorker::AddFile(std::string s_path) {
    const char* data = "";

    bool flag;
}

void ArchiveZipWorker::ChangeDirectory(std::string) {

}

bool ArchiveZipWorker::GetFileData(const std::string& s_path, std::vector<unsigned char>& buffer) {
    fs::path dest_path = cur_directory / fs::path(s_path);

    mz_zip_archive archive;
    memset(&archive, 0, sizeof(archive));
    bool er_flag;

    er_flag = mz_zip_reader_init_file(&archive, archive_path.string().c_str(), 0);

    if (!er_flag) {
        std::cerr << "Error init zip file" << '\n';
        mz_zip_reader_end(&archive);
        return false;
    }

    int file_index = mz_zip_reader_locate_file(&archive, dest_path.string().c_str(), nullptr, 0);

    if (file_index < 0) {
        std::cerr << "No such file or directory" << '\n';
        mz_zip_reader_end(&archive);
        return false;
    }

    mz_zip_archive_file_stat file_stat;
    er_flag = mz_zip_reader_file_stat(&archive, file_index, &file_stat);
    if (!er_flag) {
        std::cerr << "Error reading file" << '\n';
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

ArchiveZipWorker::ArchiveZipWorker(std::string s_path) {
    fs::path desc_path = s_path;

    if (fs::exists(desc_path) && desc_path.extension() == ".zip") {
        archive_path = desc_path;
    } else {
        std::cerr << "Wrong path\n. Default archive will be used" << '\n';
        archive_path = fs::current_path() / "archive.zip";
    }

    cur_directory = "";
}

std::vector<std::string> ArchiveZipWorker::Split(std::string s, const std::string separator) {
    std::vector<std::string> tokens;
    size_t pos = 0;
    std::string token;

    while ((pos = s.find(separator)) != std::string::npos) {
        token = s.substr(0, pos);

        if (!token.empty())
            tokens.push_back(token);

        s.erase(0, pos + separator.length());
    }

    if (!s.empty())
        tokens.push_back(s);

    return tokens;
}

//TODO Сделать проверку существования такой директории и файла, если указан файл
bool ArchiveZipWorker::GetAllFilesNameInFolder(std::string s_path, std::vector<std::string>& res_buffer) {
    fs::path dest_path = cur_directory / fs::path(s_path);
    mz_zip_archive archive;
    memset(&archive, 0, sizeof(archive));
    bool er_flag;

    er_flag = mz_zip_reader_init_file(&archive, archive_path.string().c_str(), 0);

    if (!er_flag) {
        std::cerr << "Error init zip file" << '\n';
        mz_zip_reader_end(&archive);
        return {};
    }

    mz_uint num_files = mz_zip_reader_get_num_files(&archive);

    for (mz_uint i = 0; i < num_files; i++) {
        mz_zip_archive_file_stat file_stat;
        er_flag = mz_zip_reader_file_stat(&archive, i, &file_stat);

        if (!er_flag) {
            std::cerr << "Error reading file" << '\n';
            mz_zip_reader_end(&archive);
            return false;
        }

        std::string name = std::string(file_stat.m_filename);
        std::vector<std::string> tokens_1 = Split(name, "/");
        std::vector<std::string> tokens_2 = Split(dest_path.string(), "/");

        bool flag = (tokens_1.size() - 1) == tokens_2.size();
        for (int ind = 0; ind < tokens_2.size(); ind++)
            flag = flag & (tokens_1[ind] == tokens_2[ind]);

        if (flag)
            res_buffer.push_back(file_stat.m_is_directory ? tokens_1[tokens_1.size()-1] + "/" : tokens_1[tokens_1.size()-1]);
    }

    mz_zip_reader_end(&archive);
    return true;
}



// Description: Test for ArchiveZipWorker class.
// Include: utils/ArchiveZipWorker.h
// Test: ArchiveZipWorkerTest

#include "../../utils/ArchiveZipWorker.h"
#include "gtest/gtest.h"

TEST(ArchiveZipWorkerTest, NormalizeVirtualPath) {
    std::string s_path = "C:\\Users\\VSheenko\\Desktop\\test\\test.zip";
    std::string res = ArchiveZipWorker::NormalizeVirtualPath(s_path);

    ASSERT_EQ("C:/Users/VSheenko/Desktop/test/test.zip", res);

    s_path = "\\VSheenko/Desktop\\test\\test.zip\\";
    res = ArchiveZipWorker::NormalizeVirtualPath(s_path);

    ASSERT_EQ("VSheenko/Desktop/test/test.zip", res);

    s_path = "Users\\VSheenko\\Desktop\\test\\test.zip/";
    res = ArchiveZipWorker::NormalizeVirtualPath(s_path);

    ASSERT_EQ("Users/VSheenko/Desktop/test/test.zip", res);

    s_path = "";
    res = ArchiveZipWorker::NormalizeVirtualPath(s_path);

    ASSERT_EQ("", res);

    s_path = "/";
    res = ArchiveZipWorker::NormalizeVirtualPath(s_path);

    ASSERT_EQ("", res);

    s_path = "Test\\../Test\\./Test\\";
    res = ArchiveZipWorker::NormalizeVirtualPath(s_path);

    ASSERT_EQ("Test/Test", res);

    s_path = "../Test1//Test2//Test3\\..\\..\\Test4\\";
    res = ArchiveZipWorker::NormalizeVirtualPath(s_path);

    ASSERT_EQ("Test1/Test4", res);
}


TEST(ArchiveZipWorkerTest, GetAllFilesNameInFolder) {
    ArchiveZipWorker zip_worker("../../test/ArchiveZipWorkerTest/TestArchive.zip");

    std::vector<std::string> received_res;
    std::vector<std::string> expect_res = {"folder_1_1/", "folder_1_2/", "file_1_1.txt", "file_1_2"};

    bool flag = zip_worker.GetAllFilesNameInFolder("", received_res);

    ASSERT_EQ(true, flag);
    ASSERT_EQ(expect_res.size(), received_res.size());
    for (int i = 0; i < expect_res.size(); i++) {
        ASSERT_TRUE(std::find(expect_res.begin(), expect_res.end(), (received_res[i])) != expect_res.end());
    }

    expect_res.clear();
    flag = zip_worker.GetAllFilesNameInFolder("not_exist_folder", received_res);

    ASSERT_EQ(false, flag);

    expect_res = {"folder_2_1_empty/", "file_2_3_empty", "file_2_2_empty.txt", "file_2_1.bat"};

    flag = zip_worker.GetAllFilesNameInFolder("folder_1_1/", received_res);

    ASSERT_EQ(true, flag);
    ASSERT_EQ(expect_res.size(), received_res.size());
    for (int i = 0; i < expect_res.size(); i++) {
        ASSERT_TRUE(std::find(expect_res.begin(), expect_res.end(), (received_res[i])) != expect_res.end());
    }
}


TEST(ArchiveZipWorkerTest, WrongArchivePath) {
    ASSERT_THROW(ArchiveZipWorker zip_worker("wrong_path"), std::invalid_argument);
    ASSERT_THROW(ArchiveZipWorker zip_worker("../../test/ArchiveZipWorker/test_other_format.7z"), std::invalid_argument);
    ASSERT_THROW(ArchiveZipWorker zip_worker(""), std::invalid_argument);
}


TEST(ArchiveZipWorkerTest, GetAbsPathFile) {
    ArchiveZipWorker zip_worker("../../test/ArchiveZipWorkerTest/TestArchive.zip");
    std::string res;

    ASSERT_EQ(false, zip_worker.GetAbsPathFile(res, "file_1_1", ""));

    ASSERT_EQ(false, zip_worker.GetAbsPathFile(res, "file_2_1.bat", ""));

    ASSERT_EQ(true, zip_worker.GetAbsPathFile(res, "file_1_1.txt", ""));
    ASSERT_EQ("file_1_1.txt", res);

    ASSERT_EQ(true, zip_worker.GetAbsPathFile(res, "file_2_1.bat", "folder_1_1/"));
    ASSERT_EQ("folder_1_1/file_2_1.bat", res);

    ASSERT_EQ(false, zip_worker.GetAbsPathFile(res, "folder_1_1", ""));

    ASSERT_EQ(true, zip_worker.GetAbsPathFile(res,
                                              "folder_3_1\\folder_4_1\\file_5_1.txt",
                                              "folder_1_2\\folder_2_1"));
    ASSERT_EQ("folder_1_2/folder_2_1/folder_3_1/folder_4_1/file_5_1.txt", res);

    ASSERT_EQ(true, zip_worker.GetAbsPathFile(res,
                                              "folder_1_2/folder_2_1/folder_3_1\\folder_4_1\\file_5_1.txt",
                                              "folder_1_2/folder_2_1"));
    ASSERT_EQ("folder_1_2/folder_2_1/folder_3_1/folder_4_1/file_5_1.txt", res);

    ASSERT_EQ(false, zip_worker.GetAbsPathFile(res, "", ""));
}


TEST(ArchiveZipWorkerTest, GetAbsPathDir) {
    ArchiveZipWorker zip_worker("../../test/ArchiveZipWorkerTest/TestArchive.zip");
    std::string res;

    ASSERT_EQ(true, zip_worker.GetAbsPathDir(res, "folder_1_1", ""));
    ASSERT_EQ("folder_1_1", res);

    ASSERT_EQ(true, zip_worker.GetAbsPathDir(res, "folder_1_1", "folder_1_1"));
    ASSERT_EQ("folder_1_1", res);

    ASSERT_EQ(true, zip_worker.GetAbsPathDir(res, "folder_1_1", "folder_1_1/"));
    ASSERT_EQ("folder_1_1", res);

    ASSERT_TRUE(zip_worker.GetAbsPathDir(res, "", "")); // root
    ASSERT_EQ("", res);

    ASSERT_FALSE(zip_worker.GetAbsPathDir(res, "folder", ""));
    ASSERT_FALSE(zip_worker.GetAbsPathDir(res, "folder_2_1_empty", ""));

    ASSERT_TRUE(zip_worker.GetAbsPathDir(res, "folder_2_1_empty", "folder_1_1"));
    ASSERT_EQ("folder_1_1/folder_2_1_empty", res);

    ASSERT_TRUE(zip_worker.GetAbsPathDir(res, "folder_1_2\\folder_2_1/folder_3_1//folder_4_1", "folder_1_2/folder_2_1"));
    ASSERT_EQ("folder_1_2/folder_2_1/folder_3_1/folder_4_1", res);
}

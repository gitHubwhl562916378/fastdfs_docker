/*
 * @Author: your name
 * @Date: 2021-01-16 02:57:00
 * @LastEditTime: 2021-01-16 07:14:07
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /docker_hub/fastdfs_docker/package/client_demo/main.cpp
 */
#include <iostream>
#include <fstream>
#include <memory>
#include <iterator>
#include "FastDFSClient.h"

void TestUploadFile(FastDFSClient &fdfs_client)
{
    std::string file_name = "../test.jpg";
    std::string remote_file_name;
    int code;
#if 1
    code = fdfs_client.UpLoadFile(file_name.data(), remote_file_name);
#else
	std::ifstream file_handle(file_name, std::ios::in | std::ios::binary | std::ios::ate);
	int file_size = file_handle.tellg();
	file_handle.seekg(0, std::ios::beg);
	std::shared_ptr<char> file_buffer(new char[file_size], [](const char *src){delete src;});
	file_handle.read(file_buffer.get(), file_size);
	file_handle.close();

    code = fdfs_client.UpLoadFile(file_buffer.get(), nullptr, file_size, remote_file_name);
#endif

    if(code != 0)
    {
        return;
    }
    std::cout << remote_file_name << std::endl;
}

void TestDeleteFile(FastDFSClient &fdfs_client)
{
   int code = fdfs_client.DeleteFile("group1/M00/00/03/CgoBWGACgSuAQ30lAAOFGCk51bE5739758");
   if(code != 0)
   {
       return;
   }
}

void TestDownloadFile(FastDFSClient &fdfs_client)
{
    int code;
#if 0
    code = fdfs_client.DownLoadFile("group1/M00/00/03/CgoBWGACgZyALY7SAAOFGCk51bE275.jpg", "download.jpg");
#else
    char *buffer;
    int64_t file_size;
    code = fdfs_client.DownLoadFile("group1/M00/00/03/CgoBWGACgZyALY7SAAOFGCk51bE275.jpg", &buffer, file_size);
    std::copy(buffer, buffer + file_size, std::ostream_iterator<char>(std::cout));
    std::cout << std::endl << "file size: " << file_size << std::endl;

    delete buffer;
#endif
    if(code != 0)
    {
        return;
    }
}

int main(int argc, char **argv)
{
    FastDFSClient fdfs_client("../../fdfs/client.conf");
    // TestUploadFile(fdfs_client);
    // TestDeleteFile(fdfs_client);
    TestDownloadFile(fdfs_client);
    
    return 0;
}
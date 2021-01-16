/*
 * @Author: your name
 * @Date: 2021-01-16 03:01:46
 * @LastEditTime: 2021-01-16 07:13:05
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /docker_hub/fastdfs_docker/package/client_demo/FastDFSClient.h
 */
#pragma once
#include <string>
#include <fdfs_client.h>
#include <fastcommon/logger.h>

class FastDFSClient
{
public:
    FastDFSClient(const std::string &cli_conf);
    ~FastDFSClient();

    /**
     * @description: 使用文件内容上传文件
     * @param {file_content} 文件内容首地址
     * @param {file_ext_name} 文件名后綴, 可以传nullptr
     * @param {file_size} 文件大小
     * @param {storage_server_ip} storage_server的ip地址，不指明的话程序自己查询
     * @param {storage_server_port} storage_server的端口，不指明的话程序自己查询
     * @return {remote_file_name} 上传成功后，返回的文件路径
     * @return {int} 0 成功， 否则失败
     */    
    int UpLoadFile(const char *file_content, const char *file_ext_name, int file_size, 
            std::string &remote_file_name, const std::string &storage_server_ip = std::string(), const int storage_server_port = 0);
    /**
     * @description: 使用文件命上传文件
     * @param {file_name} 文件名，相对路径名和绝对路径名都可以
     * @param {storage_server_ip} storage_server的ip地址，不指明的话程序自己查询
     * @param {storage_server_port} storage_server的端口，不指明的话程序自己查询
     * @return {remote_file_name} 上传成功后，返回的文件路径
     * @return {int} 0 成功， 否则失败
     */    
    int UpLoadFile(const std::string &file_name, std::string &remote_file_name, 
            const std::string &storage_server_ip = std::string(), const int storage_server_port = 0);
    /**
     * @description: 删除文件
     * @param {file_path} 文件路径，如:group1/M00/00/03/CgoBWGACgSuAQ30lAAOFGCk51bE5739758
     * @return {int} 0 成功， 否则失败
     */    
    int DeleteFile(const std::string &file_path);
    /**
     * @description: 下载文件到本地
     * @param {file_path} 文件路径，如:group1/M00/00/03/CgoBWGACgSuAQ30lAAOFGCk51bE5739758
     * @param {dest_path} 目标路径，如:download.jpg, 绝对路径也可以
     * @return {int} 0 成功， 否则失败
     */    
    int DownLoadFile(const std::string &file_path, char *dest_path);
    /**
     * @description: 下载文件到内存
     * @param {file_path} 文件路径，如:group1/M00/00/03/CgoBWGACgSuAQ30lAAOFGCk51bE5739758
     * @return {buffer} 返回文件缓存区首地址, 需要自己手动释放
     * @return {file_size} 返回文件大小
     * @return {int} 0 成功， 否则失败
     */ 
    int DownLoadFile(const std::string &file_path, char **buffer, int64_t &file_size);
};
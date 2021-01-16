/*
 * @Author: your name
 * @Date: 2021-01-16 03:01:54
 * @LastEditTime: 2021-01-16 07:06:42
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /docker_hub/fastdfs_docker/package/client_demo/FastDFSClient.cpp
 */
#include <assert.h>
#include "fastdfs_client.h"

FastDFSClient::FastDFSClient(const std::string &cli_conf)
{
	log_init();
	g_log_context.log_level = LOG_ERR;
	ignore_signal_pipe();

    assert(fdfs_client_init(cli_conf.data()) == 0);
}

FastDFSClient::~FastDFSClient()
{
	fdfs_client_destroy();
}

int FastDFSClient::UpLoadFile(const char *file_content, const char *file_ext_name, int file_size, 
    std::string &remote_file_name, const std::string &storage_server_ip, const int storage_server_port)
{
	ConnectionInfo *p_tracker_server;
    	p_tracker_server = tracker_get_connection();
	if (p_tracker_server == NULL)
	{
		return errno != 0 ? errno : ECONNREFUSED;
	}

	ConnectionInfo storage_server;
	char group_name[FDFS_GROUP_NAME_MAX_LEN + 1];
	int store_path_index;
    if(!storage_server_ip.empty() && storage_server_port)
    {
        storage_server.sock = -1;
        ::strcpy(storage_server.ip_addr, storage_server_ip.data());
		storage_server.port = storage_server_port;
    }else if(int code = tracker_query_storage_store(p_tracker_server, &storage_server, group_name, &store_path_index); code != 0){
        return code;
    }

	char file_id[128];
	int result = storage_upload_by_filebuff1(p_tracker_server, \
		&storage_server, store_path_index, \
		file_content, file_size, file_ext_name, \
		NULL, 0, \
		group_name, file_id);
    remote_file_name.assign(file_id);

	if (result != 0)
	{
		fprintf(stderr, "upload file fail, " \
			"error no: %d, error info: %s\n", \
			result, STRERROR(result));
	}

	tracker_close_connection_ex(p_tracker_server, true);

	return  result;
}

int FastDFSClient::UpLoadFile(const std::string &file_name, std::string &remote_file_name, 
        const std::string &storage_server_ip, const int storage_server_port)
{
    ConnectionInfo *p_tracker_server;
    p_tracker_server = tracker_get_connection();
	if (p_tracker_server == NULL)
	{
		return errno != 0 ? errno : ECONNREFUSED;
	}

	ConnectionInfo storage_server;
	char group_name[FDFS_GROUP_NAME_MAX_LEN + 1];
	int store_path_index;
    if(!storage_server_ip.empty() && storage_server_port)
    {
        storage_server.sock = -1;
        ::strcpy(storage_server.ip_addr, storage_server_ip.data());
		storage_server.port = storage_server_port;
    }else if(int code = tracker_query_storage_store(p_tracker_server, &storage_server, group_name, &store_path_index); code != 0){
        return code;
    }

	char file_id[128];
	int result = storage_upload_by_filename1(p_tracker_server, \
			&storage_server, store_path_index, \
			file_name.data(), NULL, \
			NULL, 0, group_name, file_id);
    remote_file_name.assign(file_id);

	if (result != 0)
	{
		fprintf(stderr, "upload file fail, " \
			"error no: %d, error info: %s\n", \
			result, STRERROR(result));
	}

	tracker_close_connection_ex(p_tracker_server, true);

	return  result;
}

int FastDFSClient::DeleteFile(const std::string &file_path)
{
    auto p_tracker_server = tracker_get_connection();
	if (p_tracker_server == NULL)
	{
		return errno != 0 ? errno : ECONNREFUSED;
	}

	if (int result=storage_delete_file1(p_tracker_server, nullptr, file_path.data()); result != 0)
	{
		printf("delete file fail, " \
			"error no: %d, error info: %s\n", \
			result, STRERROR(result));
	}

	tracker_close_connection_ex(p_tracker_server, true);
}

int FastDFSClient::DownLoadFile(const std::string &file_path, char *dest_path)
{
	auto p_tracker_server = tracker_get_connection();
	if (p_tracker_server == NULL)
	{
		return errno != 0 ? errno : ECONNREFUSED;
	}

	int file_offset = 0;
	int download_bytes = 0;
    int64_t file_size;
	int result = storage_do_download_file1(p_tracker_server, nullptr, \
			FDFS_DOWNLOAD_TO_FILE, file_path.data(), &dest_path, &file_size);
	if (result != 0)
	{
		printf("download file fail, " \
			"error no: %d, error info: %s\n", \
			result, STRERROR(result));
	}

	tracker_close_connection_ex(p_tracker_server, true);
}

int FastDFSClient::DownLoadFile(const std::string &file_path, char **buffer, int64_t &file_size)
{
	auto p_tracker_server = tracker_get_connection();
	if (p_tracker_server == NULL)
	{
		return errno != 0 ? errno : ECONNREFUSED;
	}

	int file_offset = 0;
	int download_bytes = 0;
	int result = storage_do_download_file1(p_tracker_server, nullptr, \
			FDFS_DOWNLOAD_TO_BUFF, file_path.data(), buffer, &file_size);
	if (result != 0)
	{
		printf("download file fail, " \
			"error no: %d, error info: %s\n", \
			result, STRERROR(result));
	}

	tracker_close_connection_ex(p_tracker_server, true);
}
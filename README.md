# FastDFS文件服务器快速布署脚本
使用Dockerfile快带布署fastdfs文件服务器，支持集群和单结点

## 使用方法
1. `cd fastdfs_docker`
2. `docker build -f Dockerfile -t 你的镜像名:你的Tag .`

## 布署方法
### 单机
1. 将package目录放到目标服务器
2. 将fastdfs.sh中的IMAGE_NAME改为你编译出来的镜像
3. `sh fastdfs.sh start`

### 集群
1. 将package目录放到每台服务器上
2. 根据分配好的ip先启动tracker服务，tracker服务没啥配置的路径已经在脚本中映射好了。只需要在每台tracker机器上执行`sh fastdfs_multi.sh start`
3. 启动storage服务,在storage目标机器上先为fdfs/storage.conf添加tracker地址和配置当前这台机器是哪个组, 如下面 [不懂点这里](https://cloud.tencent.com/developer/article/1333007)
```
    tracker_server=192.168.1.210:22122
    tracker_server=192.168.1.209:22122
    group_name=group1
```
在fdfs/mod_fastdfs.conf添加tracker集群地址, 和组信息如
```
tracker_server=192.168.1.209:22122
tracker_server=192.168.1.210:22122
group_name=group2                        #当前storage所属的组名
group_count = 2                    #组的数量，示例中共两组：group1、group2

[group1]
group_name=group1
storage_server_port=23000
store_path_count=1
store_path0=/home/fastdfs

[group2]
group_name=group2
storage_server_port=23000
store_path_count=1
store_path0=/home/fastdfs
```
然后将fastdfs_multi.sh中的EXCUTEABLE赋值为storage, 执行`sh fastdfs_multi.sh start`。这样就把这台机器添加到group1去了，nginx也启动好了。其它机器同样操作
4. 对外提供服务的nginx, 在对外服务器的nginx.conf中配置类似下面
```
user  root;
worker_processes  1;
events {
    worker_connections  1024;
    use epoll;
}
http {
    include       mime.types;
    default_type  application/octet-stream;
    #log_format  main  '$remote_addr - $remote_user [$time_local] "$request" '
    #                  '$status $body_bytes_sent "$http_referer" '
    #                  '"$http_user_agent" "$http_x_forwarded_for"';
    #access_log  logs/access.log  main;
    sendfile       on;
    tcp_nopush     on;
    keepalive_timeout  65;
    #gzip on;

    #设置 group1 的服务器
    upstream fdfs_group1 {
         server 192.168.1.205:8888 weight=1 max_fails=2 fail_timeout=30s;
         server 192.168.1.206:8888 weight=1 max_fails=2 fail_timeout=30s;
    }

    #设置 group2 的服务器
    upstream fdfs_group2 {
         server 192.168.1.207:8888 weight=1 max_fails=2 fail_timeout=30s;
         server 192.168.1.208:8888 weight=1 max_fails=2 fail_timeout=30s;
    }

    server {
        listen       80;
        server_name  localhost;
        #charset koi8-r;
        #access_log  logs/host.access.log  main;

        #设置 group 的负载均衡参数
        location /group1/M00 {
            proxy_set_header Host $host;
            proxy_set_header X-Real-IP $remote_addr;
            proxy_pass http://fdfs_group1;
        }

        location /group2/M00 {
            proxy_set_header Host $host;
            proxy_set_header X-Real-IP $remote_addr;
            proxy_pass http://fdfs_group2;
        }

        #error_page  404              /404.html;
        # redirect server error pages to the static page /50x.html
        #
        error_page   500 502 503 504  /50x.html;
        location = /50x.html {
            root html; 
        }
    } 
}
```
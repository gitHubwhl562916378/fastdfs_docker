FROM centos:7

# 添加启动脚本
ADD storage.sh /usr/local/bin/
ADD tracker_storage.sh /usr/local/bin/
ADD tracker.sh /usr/local/bin/

# Run
RUN yum install git gcc gcc-c++ make automake autoconf libtool pcre pcre-devel zlib zlib-devel openssl-devel wget vim initscripts -y \
  &&    cd /usr/local/src  \
  &&    git clone https://github.com/happyfish100/libfastcommon.git --depth 1        \
  &&    git clone https://github.com/happyfish100/fastdfs.git --depth 1    \
  &&    git clone https://github.com/happyfish100/fastdfs-nginx-module.git --depth 1   \
  &&    wget http://nginx.org/download/nginx-1.15.4.tar.gz    \
  &&    tar -zxvf nginx-1.15.4.tar.gz    \
  &&    mkdir /home/fastdfs   \
  &&    cd /usr/local/src/  \
  &&    cd libfastcommon/   \
  &&    ./make.sh && ./make.sh install  \
  &&    cd ../  \
  &&    cd fastdfs/   \
  &&    ./make.sh && ./make.sh install  \
  &&    cd ../  \
  &&    cd nginx-1.15.4/  \
  &&    ./configure --add-module=/usr/local/src/fastdfs-nginx-module/src/   \
  &&    make && make install  \
  &&    rm /usr/local/src/* -r

RUN cd /usr/local/bin \
  &&  chmod +x tracker.sh && mv tracker.sh tracker \
  &&  chmod +x storage.sh && mv storage.sh storage \
  &&  chmod +x tracker_storage.sh && mv tracker_storage.sh tracker_storage \
# 保证VOLUME /usr/local/nginx/conf/nginx.conf这个文件不会报错，存在会报已存在文件
  &&  rm /usr/local/nginx/conf/nginx.conf

# export config storage
VOLUME ["/etc/fdfs", "/home/fastdfs", "/usr/local/nginx/conf/nginx.conf", "/usr/local/nginx/html", "/usr/local/nginx/logs"]

EXPOSE 22122 23000 8888 8080
# ENTRYPOINT ["/home/fastdfs.sh"]

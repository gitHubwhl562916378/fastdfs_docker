IMAGE_NAME=192.168.2.100:5000/common/fastdfs:latest
#tracker_storage 为单机布署,启动storage、tracker、nginx 
#tracker 只启动tracker服务
#storage 只启动storage、nginx服务
#默认启动单机布署
EXCUTEABLE=tracker
CONTAINER_NAME=fastdfs

FastdfsConfig=$PWD/fdfs
NginxConfig=$PWD/nginx/conf/nginx.conf
NginxHtmlRoot=$PWD/nginx/html
NginxLogs=$PWD/nginx/logs
FastDataPath=$PWD/store_path

start(){
    set -x
    docker run --net=host --name=$CONTAINER_NAME -d --restart=always --cap-add=SYS_PTRACE --security-opt seccomp=unconfined --privileged=true \
    -v $FastdfsConfig:/etc/fdfs \
    -v $NginxConfig:/usr/local/nginx/conf/nginx.conf \
    -v $NginxHtmlRoot:/usr/local/nginx/html \
    -v $NginxLogs:/usr/local/nginx/logs \
    -v $FastDataPath:/home/fastdfs \
    $IMAGE_NAME \
    $EXCUTEABLE
}

stop(){
    docker stop $CONTAINER_NAME &&\
    docker rm $CONTAINER_NAME
}

case "$1" in
  start)
        start
        ;;
  stop)
        stop
        ;;
  restart)
        stop
        start
        ;;
  *)
        echo "Usage: $0 {start|stop|restart}"
        exit 1
esac
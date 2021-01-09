#!/bin/bash

echo "start storage"
fdfs_storaged /etc/fdfs/storage.conf start

echo "start nginx"
/usr/local/nginx/sbin/nginx 

sleep 1

tail -f /home/fastdfs/logs/storaged.log
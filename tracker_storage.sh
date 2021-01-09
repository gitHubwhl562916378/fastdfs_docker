#!/bin/bash

echo "start trackerd"
fdfs_trackerd /etc/fdfs/tracker.conf start

echo "start storage"
fdfs_storaged /etc/fdfs/storage.conf start

echo "start nginx"
/usr/local/nginx/sbin/nginx

tail -f /dev/null
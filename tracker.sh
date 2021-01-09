#!/bin/bash

echo "start trackerd"
fdfs_trackerd /etc/fdfs/tracker.conf start

sleep 1

tail -f /home/fastdfs/logs/trackerd.log
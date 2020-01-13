# ftpdownload
最开始用的curlftpfs直接挂载ftp服务器目录，虽然操作简单了，但是会有一些问题：
* 偶尔挂载目录不能访问或者执行`df -h`时shell卡死
* 如果有同一个文件需要多次读取的话，每次读取其实相当于把文件再次下载下来

所以还是老老实实写代码把服务器文件下载到本地吧。

# Example Usage
*project path is `/root/ftpdownload`*
* build  
    ```bash
    $ cd /root/ftpdownload
    $ mkdir build
    $ cd build
    $ cmake -DCMAKE_INSTALL_PREFIX=/usr/local/ ..
    $ make
    $ sudo make install
    ```
* conf

    ```bash
    $ cat /root/ftpdownload/conf/example.conf
    #数据源
    src_dir=ftp://[ip]/[dir]/
    #数据存放路径
    dst_dir=[dir]
    #ftp用户名和密码
    user_pwd=[user]:[password]
    #下载间隔时间，单位是秒
    sleep_time=300
    #等待ftp服务器应答的时间，单位是秒
    low_speed_time=30
    #日志目录
    log_dir=[dir]    
    ```
* execute  
    ```bash
    $ ftpdownload /root/ftpdownload/conf/example.conf
    ```

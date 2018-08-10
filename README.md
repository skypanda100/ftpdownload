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
    $ cmake ..
    $ make
    ```
* conf

    ```bash
    $ cat /root/ftpdownload/conf/example.conf
    src_dir=ftp://[ip]/[dir]/
    dst_dir=[dir]
    user_pwd=[user]:[password]
    sleep_time=300
    ```
* execute  
    ```bash
    $ cd /root/ftpdownload/build
    $ ./ftpdownload /root/ftpdownload/conf/example.conf
    ```

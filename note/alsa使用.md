# alsa 简介

ALSA 是 Advanced Linux Sound Architecture的缩写，高级 Linux 声音架构的简称，它在 Linux 操作系统上提供了音频和 MIDI（Musical Instrument DigitalInterface，音乐设备数字化接口）的支持外部开源的库，如果要在 ARM 处理器下使用需要进行移植。

## 移植步骤(了解)

[alsa-lib-1.0.22.tar.bz2](./alsa-lib-1.0.22.tar.bz2)的安装：

- 第一步：将alsa-lib-1.0.22.tar.bz2压缩包放到家目录
- 第二步： 解压alsa-lib-1.0.22.tar.bz2文件： 
```shell
tar  -xvf  alsa-lib-1.0.22.tar.bz2
```
- 第三步：在家目录下创建alsa_arm目录：
```shell
sudo mkdir /usr/local/alsa
# 使用超级用户权限需要输入虚拟机的密码
```
- 第四步：进入alsa-lib-1.0.22目录：
```shell
cd  alsa-lib-1.0.22
```
- 第五步：执行配置命令：
```shell
./configure  --prefix=/usr/local/alsa --host=arm-linux --disable-python
```
第六步：执行编译命令：make
第七步：执行安装命令：make install

第二个库的安装：
- 第一步：将alsa-utils-1.0.22.tar.bz2压缩包放到家目录
- 第二步： 解压alsa-utils-1.0.22.tar.bz2文件： tar  -xvf  alsa-utils-1.0.22.tar.bz2
- 第三步：进入alsa-utils-1.0.22目录：cd  alsa-utils-1.0.22
- 第四步：执行配置命令：
```shell
./configure --prefix=/usr/local/alsa  --host=arm-linux   --with-alsa-prefix=/usr/local/alsa/lib/    --with-alsa-inc-prefix=/usr/local/alsa/include/   --disable-alsamixer   --disable-xmlto
```
- 第五步：执行编译命令：make
- 第六步：执行安装命令：make install
- 第七步：进入/usr/local目录：cd  /usr/local
- 第八步：打包压缩alsa目录：tar -czvf alsa.tar.gz alsa/
- 第九步：将压缩好的alsa.tar.gz文件拷贝到共享目录：cp alsa.tar.gz  /mnt/hgfs/share/ 


## 安装到开发板 (重点)
经过以上步骤，则/usr/local/alsa中的文件即为我们已经编译安装好的alsa库
[alsa.tar.gz](./alsa.tar.gz)

第一步：在开发板中解压alsa.tar.gz文件：`tar  -xvf  alsa.tar.gz`
第二步：将alsa/bin中arecord和aplay命令拷贝到开发板/bin中：`cp  alsa/bin/arecord  alsa/bin/aplay  /bin`
第三步：创建一个alsa目录 `mkdir  /usr/local/alsa/share  -p`
第四步：将alsa/share/alsa复制到开发板的/usr/local/alsa/share 中：`cp /alsa/share/alsa  /usr/local/alsa/share -r`
第五步：将/alsa/lib/libasound.so.2拷贝到开发板中/lib中：`cp  /alsa/lib/libasound.so.2  /lib`


## 使用alsa录音
录音命令：arecord -d3 -c1 -r16000 -twav  -fS16_LE   1.wav
 命令解析：
        -d：录音时长（duration） 秒
        -c：音轨（channels）
        -r：采样频率（rate）  人的耳朵听到的声音频率范围20Hz---20000Hz
        -t：封装格式（type）
        -f：量化位数（format）16bit  小端
        1.wav：录音后生成的音频文件名
        
播放录音命令：aplay  1.wav

## 开发板配置网络
1. 编辑开发板的配置文件
```shell
vi /etc/profile 
```
在配置文件
```shell
ifconfig eth0 up
#下方加
ifconfig eth0 10.11.53.3
```
配置完毕`esc :wq` 退出

2. 重启开发板后, 输入`ifconfig`看一下网卡ip是否配置成功，接下来ping一下
```shell
# 这里的ip 10.11.53.200 是教师机
ping 10.11.53.200
PING 10.11.53.200 (10.11.53.200): 56 data bytes
64 bytes from 10.11.53.200: seq=0 ttl=128 time=1.456 ms
64 bytes from 10.11.53.200: seq=1 ttl=128 time=0.723 ms
64 bytes from 10.11.53.200: seq=2 ttl=128 time=0.793 ms
64 bytes from 10.11.53.200: seq=3 ttl=128 time=0.774 ms
64 bytes from 10.11.53.200: seq=4 ttl=128 time=0.722 ms
64 bytes from 10.11.53.200: seq=5 ttl=128 time=0.708 ms
64 bytes from 10.11.53.200: seq=6 ttl=128 time=0.735 ms
```

## nfsd 开发板网络资源共享

1. 解压 [nfsd.zip](./nfsd.zip) 
2. 运行 nfssrv-64.exe
3. 我已经配置好了，你需要右键路径点击`Edit`
4. 只需要改Folder，选择你想共享的文件夹给开发板就好了
5. 给6818开发板创建一个挂载文件夹（只需要创建一次以后就一直有了）
```shell
mkdir /mnt/nfs
```
6. 在开发板能ping通主机的前提下，输入以下指令将pc文件挂在到开发板
```shell
cd /
mount -o nolock 电脑ip:/share  /mnt/nfs
```
7. 进入共享文件夹，开发板就可以操作电脑的文件了
```shell
cd /mnt/nfs
```

       Please note:
       Accessing the server from clients requires firewall rules.
       The installer has NOT touched your firewall rules.
       Please verify and run firewall.bat from installation
       folder or setup your individual rules for the server.


haneWIN NFS Server Updates

1.2.68 / in exported name allowed
       bug in language translation, fixed
1.2.67 bug in command line argument fixed
1.2.66 error message on empty path changed
       fixes access from VLC without path
1.2.64 directory scan improved
1.2.63 handling of broken tcp connections improved
       single character names enabled
1.2.62 max. datasize up to 262144 allowed
1.2.60 parameter for max # of open dir handles added
1.2.59 bug in tcp mountd fixed
1.2.58 new option -i32 to limit inode sizes to 32 bits
       solves problems with client software that can not handle
       windows 64 bit file id as inode number
       bug in upper/lower case handling, fixed
       performance improved
1.2.57 manifest for long path names
       handling of junctions (with local inodes) broken, fixed
1.2.56 problem with Win10 nfsclient behaviour fixed
1.2.55 false virus alert in 1.2.54 fixed
       attribute HIDDEN ignored on volumes
1.2.54 character-set selection moved to exports
       enables operation of utf-8 and ANSI clients based on exports entry
       automatic switching to ANSI for win10 clients
1.2.53 compatibility with Win10 nfsclient added
1.2.52 handling of case sensivity improved
       socket/fifo support added
       bug on UDF DVD fixed
       attributes on mkdir incorrect, fixed
       nfs2 link/symlink broken since 1.2.50, fixed
1.2.51 for files on NTFS:
       support for up to 10 variants of name that differ in case only
       link size on reparse points set to correct size
1.2.50 new option to save attributes/uid/gid on NTFS
       handling of invalid filename characters on Windows improved
------
1.2.46 bug in GUI for exports fixed
1.2.45 limit of simultanous directory lists increased to 64
1.2.43 exports option for substitution of a colon added
1.2.42 bug fixes
1.2.41 option for context menu entry for folders added
       drop support "folder to exports view" added
       service gui can run without elevation
       stale file handle bug from 1.2.40, fixed
1.2.40 new gui, exports update without restart
       UTF-8 set as default
------ 
1.2.33 bug (from changes in 1.2.32) in long dir lists, fixed
1.2.32 support for file-id use with junctions to other file-systems
1.2.31 new option for use of reparse point symbolic links
       option for exporting SMB folders removed, set to enabled
       support for \\?\ and volumeGUID in exports added
1.2.30 NetworkLockManager added
       file-id as inode for NTFS on win7 and higher
       gui modified
------
1.2.29 bug from new hardlink implementation fixed
1.2.28 hardlink implementation improved
       permission checking improved
1.2.27 inode handling improved
1.2.26 recv timeout configuration
1.2.25 bug in lookup on cached file, fixed
1.2.24 readdir problem fixed
1.2.22 font problem fixed
1.2.21 mount count problem with name,alldirs options, fixed
1.2.20 eof flag on nfs3 read not handled correct, fixed 
1.2.19 attribute mapping improved
1.2.18 support for create exclusive added
1.2.16 simple character mapping added
1.2.15 attribute ctime wrong, fixed
1.2.13 pmapd updated
1.2.12 locking problem in file handle cache, fixed
1.2.11 file closed on write with attr file sync, fixed
1.2.10 option -ignorecase added
1.2.8  dump of large exports crashed server, fixed
1.2.7  edit exports fixed
1.2.6  umask extended to owner permissions 
1.2.5  bug in x64 rpc code fixed 
1.2.4  languages implementation replaced, dll -> ini, an Editor or TransTool
       from PDF Creator could be used for setting up new translations
1.2.3  option to force NFS3 sync write added
1.2.2  native 64 bit versions for nfs server and portmapper
       inode handling modified
       link count of subdirectories made optional
1.2    edit exports modified

1.1.76 -exec option added
1.1.75 problem with quoted path in exports fixed
1.1.74 icon display on win 7 x64 fixed
1.1.73 bug on remote drives using UNC path fixed
1.1.72 unlink of readonly files fixed
1.1.70 operation without portmapper registration added 
1.1.69 operation with standby/hibernate mode active, fixed
1.1.68 link count for subdirectories fixed
1.1.67 volume sizes > 2 TB incorrectly reported, fixed
1.1.66 bug on received fragemented UDP packets fixed
1.1.65 UTF-8 support added
1.1.64 bug in UNC path handling, fixed
1.1.62 nfs_read and nfs_write operations could return wrong byte count
       (on cache overflow), fixed
1.1.61 portmapper CALLIT function fixed
1.1.60 spaces in path names not saved in mounttab, fixed
1.1.59 duplicate entries from mounttab avoided
1.1.58 retry if winsock send failed with WSAINTR, added
1.1.57 bug in -range option, fixed
1.1.56 rename of directories broken from modifications of 1.1.55, fixed
1.1.55 access using -maproot:0 incorrect, fixed
1.1.54 server using 100% cpu, fixed
1.1.53 reset option for inode-table added
1.1.52 server temporary not responding due to name resolving, improved
1.1.51 combination of -name and -alldirs options failed, fixed
1.1.50 support for quoted drive\\path specifiactions
1.1.49 UNC notation for network shares supported
1.1.48 German language resources moved to DLL
1.1.47 write performance improved
1.1.46 problem with reception of fragmented packets under tcp, fixed
       bug in extending inode table, fixed
1.1.44 options added
1.1.43 inode table checks added
1.1.42 DST time shift for files on NTFS volumes, fixed
1.1.41 bug in pmap_dump fixed, caused wrong rpcinfo output
1.1.40 mount tcp/udp binding related to nfs tcp/udp binding
1.1.39 bug from code changes in 1.1.34 in nfs_write operation, fixed
1.1.38 / allowed in share names
1.1.36 serving of tcp connections incorrect, fixed
1.1.35 setattr(nfs3) modified for Linux
1.1.34 exports reload without unmount/mount of unmodified entries
       exports -alldirs, -umask options added
       bug fixes
1.1.33 inode handling in inode.nfs improved
1.1.32 more file handle checks
1.1.31 "nfsd -install -portmap" installs NFS server with built-in portmapper
1.1.30 invalid file handle could cause NFS server crash fixed
1.1.29 bug increasing write size >8192 fixed
1.1.28 bug in setuid bit handling fixed
1.1.27 exported name from option -name not displayed in showmount, fixed
1.1.26 windows executable added
1.1.25 soft/hard link options added to applet
1.1.24 undefined blocksize variable, fixed
1.1.23 use of Win 2000/XP NTFS hard links implemented
1.1.22 bug in attributes check of readlink, fixed
1.1.21 READDIRPLUS operation implemented
1.1.20
1.1.19 handling of attributes improved
1.1.18 use of SYSTEM attribute improved
1.1.17 handling of mode bits on create_file improved
1.1.16 export of directories containing spaces enabled
1.1.15 name option for exported directories added
1.1.14 handling of repeated packets improved
1.1.13 bug (from 1.1.10) in rpc handling, fixed
1.1.12 bugs in NFS-3 protocol handling, fixed
1.1.11
1.1.10
1.1.9  bug in "-range" with odd number of addresses, fixed
1.1.8  bug in modes of special devices fixed
1.1.7  bug in semented tcp packets fixed
       bug in section locking fixed
1.1.6  Bug on long directories (missing entries) fixed
       NFS-3 bug fixed
1.1.5  Option for async write with NFS-2 
1.1.4  bug in NFS-3 on create file fixed
1.1.3  symbolic link recognition on Linux clients fixed
1.1.2  built-in Portmapper for Windows 9x/ME added
1.1.1  Portmapper modified to avoid "getmaps" error message on
       Linux clients. (Linux portmap client reads beyond end of packet!!!)
       possible server lock up on directrory reads, fixed
1.1    NFS 3 protocol implemented

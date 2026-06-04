
REM example file for creating firewall rules 
REM Adding firewall rules requires ADMINISTRATOR rights

REM extract path
set dir=%~dp0%

REM uncomment next line for XP
REM netsh firewall add allowedprogram "%dir%nfssrv.exe" "haneWIN NFS Server" enable subnet

REM for Win7 and higher versions
netsh advfirewall firewall add rule name="haneWIN NFS Server" dir=in program="%dir%nfssrv.exe" action=allow remoteip=localsubnet

pause

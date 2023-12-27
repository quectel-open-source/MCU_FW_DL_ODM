口在xml 中配置的

0.重启系统；
QWinExplorer.exe -s l


1. transfer file to module

QWinExplorer.exe -f filename [-p:][path]

filename: file to be transfer to module.
[p][path]:optional, indiacte the path which file to be locate.
	  Default path:/datatx


2. list path in module

QWinExplorer.exe -l: path

path: directory name in module which will be show.


3. remove file

QWinExplorer.exe -d: filename

filename: file in module which to be delete.


eg:

QWinExplorer.exe -f: E:\project\EFS_Tool\QEfs\123.txt
QWinExplorer.exe -f: E:\project\EFS_Tool\QEfs\123.txt -p: /datatx
QWinExplorer.exe -d: /datatx/123.txt 
QWinExplorer.exe -l: /datatx
QWinExplorer.exe -s: 1

4.log.txt
Save the operation log in an append manner.

5.config.xml
isconfig: Whether to use the configuration file, configure the port,0:not,1:yes
dmport:dm port,dmport:com*

6.V1.3 support reboot. 
 
Cmd: QWinExplorer.exe -s 1

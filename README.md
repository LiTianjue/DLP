# DLP system


TODO

1. 为bro 脚本写一个处理函数，格式化日志消息结构
2. 启动脚本同时启动bro　和　dlp-syslogd
3. 启动脚本兼具启动和停止的功能(start stop restart)
4. 启动脚本应该还要支持启动守护进程的功能

1. bro　脚本支持格式化消息(json)
2. dlp-syslogd支持多进程
3. 编写和读取主配置文件（包括读取过滤的端口号，日志服务器地址 unix-socket）


不在计划中但是将来可能要做的
* 实现正则匹配多种条件复合匹配
* 提高匹配效率

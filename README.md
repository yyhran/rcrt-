# 一个简单的 C++ 运行时库 rcrt
《程序员的自我修养》书中最后章节的内容，修改了部分代码使其能在 x86_64 linux 以及新版 gcc 下运行，Windows 版本未测试
+ 支持简单的系统调用，如：`open`，`read`，`write`，`close`
+ 简单的字符串操作，如：`strcmp`，`strcpy`，`strlen`
+ 支持 new/delete 操作
+ 支持全局对象的构造与析构
+ string 和 iostream 的简单实现

# 使用方法
1. 执行 `bash make.sh` 一键运行
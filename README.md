# 极简 C++ 运行时库 rcrt
仅支持 x86_64 linux，Windows 未测试
+ 支持简单的系统调用，如：`open`，`read`，`write`，`close`
+ 简单的字符串操作，如：`strcmp`，`strcpy`，`strlen`
+ 支持 new/delete 操作
+ string 和 iostream 的简单实现

# 使用方法
1. 运行 `./make.sh` 一键测试
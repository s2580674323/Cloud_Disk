#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdlib.h>
#include <string.h>

typedef unsigned int uint;

#define STR_MAX_SIZE 30

#define SYSTEM_ERROR "system error"

#define REGISTER_OK "register ok"
#define REGISTER_FAILED "register failed : name exists"
#define LOGIN_OK "login successfully"
#define LOGIN_FAILED "login faild : name or pwd is error or relogin"
#define SEARCH_USR_NO "no such person"
#define SEARCH_USR_ONLINE "user online"
#define SEARCH_USR_OFFLINE "user offline"
#define ADD_FRIEND_EXISTED "friend existed"
#define ADD_FRIEND_OFFLINE "added user offline"
#define DELETE_FRIEND_OK "delete friend ok"
#define USER_CURRENT_DIR_NOT_EXIST "user current dir not exist"
#define NEW_DIR_EXISTED "new dir existed"
#define CREATE_NEW_DIR_OK "create new dir ok"
#define DELETE_DIR_OK "delete the dir ok"
#define DELETE_DIR_FAILED "delete the dir failed"
#define DELETE_DIR_ERROR "please choose directory not file"
#define RENAME_DIR_OK "rename directory ok"
#define RENAME_DIR_FAILED "rename directory failed"
#define ENTER_DIR_OK "enter next dir ok"
#define ENTER_DIR_FAILED "enter next dir failed"
#define UPLOAD_FILE_OK "upload file ok"
#define UPLOAD_FILE_FAILED "upload file failed: Extra byte"
#define DELETE_FILE_OK "delete the file ok"
#define DELETE_FILE_FAILED "delete the file failed"
#define DOWNLOAD_FILE_OPEN_FILE_FAILED "download file: open file failed"
#define DOWNLOAD_FILE_READ_FILE_ERROR "download file:read file error"
#define DOWNLOAD_FILE_OK "download file ok"
#define DOWNLOAD_FLLE_START "download file start"
#define SHARE_FILE_PARH_ERROR "share file path error"
#define SHARE_FILE_OK "share file ok"
#define SHARE_FILE_AGREE "your friend agree to receive the file"
#define SHARE_FILE_REFUSE "your friend refuse to receive the file"
#define MOVE_FILE_OK "move file ok"
#define MOVE_FILE_FIALED "move file failed"
#define MOVE_FILE_ERROR "it is not a file"



enum ENUM_MSG_TYPE // 消息类型是无符号整形，即占32位
{
    ENUM_MSG_TYPE_MIN = 0,

    ENUM_MSG_TYPE_REGISTER_REQUEST,     // 注册请求
    ENUM_MSG_TYPE_REGISTER_RESPOND,      // 注册回复

    ENUM_MSG_TYPE_LOGIN_REQUEST,        // 登录请求
    ENUM_MSG_TYPE_LOGIN_RESPOND,        // 登录回复

    ENUM_MSG_TYPE_ALL_ONLINE_REQUEST,   // 所有在线用户请求
    ENUM_MSG_TYPE_ALL_ONLINE_RESPOND,   // 所有在线用户回复

    ENUM_MSG_TYPE_SEARCH_USER_REQUEST,   // 搜索用户请求
    ENUM_MSG_TYPE_SEARCH_USER_RESPOND,   // 搜索用户回复

    ENUM_MSG_TYPE_ADD_FRIEND_REQUEST,   // 添加好友请求
    ENUM_MSG_TYPE_ADD_FRIEND_RESPOND,   // 添加好友回复

    ENUM_MSG_TYPE_ADD_FRIEND_AGREE,   // 同意添加好友的回复
    ENUM_MSG_TYPE_ADD_FRIEND_REFUSE,   // 拒绝添加好友的回复

    ENUM_MSG_TYPE_FLUSH_FRIEND_REQUEST,   // 刷新好友的请求
    ENUM_MSG_TYPE_FLUSH_FRIEND_RESPOND,   // 刷新好友的回复

    ENUM_MSG_TYPE_DELETE_FRIEND_REQUEST,   // 删除好友的请求
    ENUM_MSG_TYPE_DELETE_FRIEND_RESPOND,   // 删除好友的回复

    ENUM_MSG_TYPE_PRIVATE_CHAT_REQUEST,   // 好友私聊的请求
    ENUM_MSG_TYPE_PRIVATE_CHAT_RESPOND,   // 好友私聊的回复

    ENUM_MSG_TYPE_GROUP_CHAT_REQUEST,   // 群聊的请求
    ENUM_MSG_TYPE_GROUP_CHAT_RESPOND,   // 群聊的回复

    ENUM_MSG_TYPE_CREATE_DIR_REQUEST,   // 创建文件夹的请求
    ENUM_MSG_TYPE_CREATE_DIR_RESPOND,   // 创建文件夹的回复

    ENUM_MSG_TYPE_FLUSH_DIR_AND_FILE_REQUEST,   // 刷新文件夹/文件的请求
    ENUM_MSG_TYPE_FLUSH_DIR_AND_FILE_RESPOND,   // 刷新文件夹/文件的回复

    ENUM_MSG_TYPE_DELETE_DIR_REQUEST,   // 删除文件夹的请求
    ENUM_MSG_TYPE_DELETE_DIR_RESPOND,   // 删除文件夹的回复

    ENUM_MSG_TYPE_RENAME_DIR_OR_FILE_REQUEST,   // 重命名文件夹的请求
    ENUM_MSG_TYPE_RENAME_DIR_OR_FILE_RESPOND,   // 重命名文件夹的回复

    ENUM_MSG_TYPE_ENTER_DIR_REQUEST,   // 进入文件夹的请求
    ENUM_MSG_TYPE_ENTER_DIR_RESPOND,   // 进入文件夹的回复

    ENUM_MSG_TYPE_UPLOAD_FILE_REQUEST,   // 上传文件的请求
    ENUM_MSG_TYPE_UPLOAD_FILE_RESPOND,   // 上传文件的回复

    ENUM_MSG_TYPE_DELETE_FILE_REQUEST,   // 删除普通文件的请求
    ENUM_MSG_TYPE_DELETE_FILE_RESPOND,   // 删除普通文件的回复

    ENUM_MSG_TYPE_DOWNLOAD_FILE_REQUEST,   // 下载文件的请求
    ENUM_MSG_TYPE_DOWNLOAD_FILE_RESPOND,   // 下载文件的回复

    ENUM_MSG_TYPE_SHARE_FILE_REQUEST,   // 分享文件的请求
    ENUM_MSG_TYPE_SHARE_FILE_RESPOND,   // 分享文件的回复
    ENUM_MSG_TYPE_SHARE_FILE_AGREE,    // 同意接收分享的文件
    ENUM_MSG_TYPE_SHARE_FILE_REFUSE,   // 拒绝接收分享的文件

    ENUM_MSG_TYPE_MOVE_FILE_REQUEST,   // 移动文件的请求
    ENUM_MSG_TYPE_MOVE_FILE_RESPOND,   // 移动文件的回复


    ENUM_MSG_TYPE_TEST_DELAY_REQUEST,   // 测试服务器与客户端延迟的请求
    ENUM_MSG_TYPE_TEST_DELAY_RESPOND,   //测试服务器与客户端延迟的回复

//    ENUM_MSG_TYPE_REQUEST,
//    ENUM_MSG_TYPE_RESPOND,

    ENUM_MSG_TYPE_MAX = 0xffffffff
};

struct PDU {
    uint uiPDULen;   // 32字节，总的协议数据单元的大小
    uint uiMsgType;  // 32字节，消息类型
    char cData[64];  // 64字节，文件名，用户名，密码等
    uint uiMsgLen;   // 32字节，实际消息长度

    int cMsg[];      // 实际消息，作为变长结构体的动态部分
};
struct FileInfo{
    char cName[32];
    int iFileType;
};

PDU *mkPDU(uint uiMsgLen);

#endif // PROTOCOL_H

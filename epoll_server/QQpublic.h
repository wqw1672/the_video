#ifndef __QQ_PUBLIC_H
#define __QQ_PUBLIC_H

#include <sys/types.h>
#include <sys/ipc.h>

#define KEY_FIFO_SEND   ftok(".", 1)
#define KEY_FIFO_RECV   ftok(".", 2)



typedef struct packet_header_st
{
	char srcAddr[6]; //预留
	char dstAddr[6]; //预留
	int funid; // 功能号
	int optid; // 操作码：请求和应答
	int usrlenth;// 包体的长度
	int syn; // 判断包头是否正确
	int fd; //用于前后端消息通信

}packet_header_t;


typedef struct  packet_all_st
{
	packet_header_t head;
	char data[1024-sizeof(packet_header_t)]; // ??
	//unsigned len; //要发送的数据有效长度

}packet_all_st;

typedef struct pack_back
{
	int syn;
}Pack_Back;

//登陆包0x01
typedef struct
{
	char user[10];//用户名
	char pwd[10];//密码
}requesrt_login;



//登陆应答包
typedef struct
{
	char usr[10];//
	int login_reply;//登陆结果
}reply_login;


//0x02
//频道分类应答包
typedef struct
{
	int classes_count;//频道分类数量
	int classes_id;//频道分类ID
	int classes_name[10];//频道分类名
}reply_classify;

//0x03
//视频类型分类应答包
typedef struct
{
    int classes_count;      //频道分类数量
    char classes_name[20];  //视频名称
    int video_id;           //视频号
    int arrayvideoid[10];   //视频数组
}reply_type;

typedef struct 
{
    int type;
}client_type;

//0x04
//地区分类应答包
typedef struct
{
	int area_count;//视频地区分类
	int classes_id;//视频频道分类ID
	int area_id;//视频地区分类ID
	char area_name[10];//视频地区分类名称
}reply_area;

//0x05
//视频列表
typedef struct 
{
        char VideoName[20];//视频名
     
}reply_list;

//0x06
//视频点播功能
typedef struct 
{
	int state;
	char updata_flowid[32];//
}reply_play;

typedef struct 
{
 int ret;
}re_heart;

typedef struct message
{
	char usr[10];
	char mess[100];
}MESSAGE;
//0x07
typedef struct VideoRecord   //上传影片播放时长请求包
{
        char user_name[10];  //用户姓名
        int video_id;        //视频ID
        float video_seek;    //播放时长
}VideoRecord_t;
typedef struct res_VideoRecord   //影片播放时长应答包
{
        int results;            //上传结果，0-上传成功，1-上传失败
}res_VideoRecord_t;
//0x09
typedef struct PlayHistory    //播放历史请求包
{
        char user_name[10];         //用户姓名
        int video_id;        //视频ID
}PlayHistory_t;

typedef struct res_PlayHistory    //播放历史应答包
{
        char user_name[10];      //用户姓名
        int video_id;        //视频ID
        float video_seek;    //播放时长
}res_PlayHistory_t;
//0x04
typedef struct VideoArea    //视频地区请求包
{
    int type_channel;        //视频频道类型
    int type_Areaid;        //视频地区ID    其中0:全部;1:大陆;2:香港;3:美国;4:韩国;5:英国;6:日报;
}VideoArea_t;
//0x05
typedef struct VideoType    //视频类型请求包
{
    int type_channel;        //视频频道类型
    int type_Areaid;         //视频地区ID
    int type_Videoid;        //视频类型ID 其中0:全部;1:喜剧;2:爱情;3:动作;4:犯罪;5:枪战;6:科幻;
    int type_time;            //time  1.2019  2.2018  3.2017  4.2016   5.2015   6.other time 
}VideoType_t;
#endif /* __QQ_PUBLIC_H */

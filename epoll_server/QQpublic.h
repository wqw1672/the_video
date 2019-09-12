#ifndef __QQ_PUBLIC_H
#define __QQ_PUBLIC_H

#include <sys/types.h>
#include <sys/ipc.h>

#define KEY_FIFO_SEND   ftok(".", 1)
#define KEY_FIFO_RECV   ftok(".", 2)



typedef struct packet_header_st
{
	char srcAddr[6]; //Ԥ��
	char dstAddr[6]; //Ԥ��
	int funid; // ���ܺ�
	int optid; // �����룺�����Ӧ��
	int usrlenth;// ����ĳ���
	int syn; // �жϰ�ͷ�Ƿ���ȷ
	int fd; //����ǰ�����Ϣͨ��

}packet_header_t;


typedef struct  packet_all_st
{
	packet_header_t head;
	char data[1024-sizeof(packet_header_t)]; // ??
	//unsigned len; //Ҫ���͵�������Ч����

}packet_all_st;

typedef struct pack_back
{
	int syn;
}Pack_Back;

//��½��0x01
typedef struct
{
	char user[10];//�û���
	char pwd[10];//����
}requesrt_login;



//��½Ӧ���
typedef struct
{
	char usr[10];//
	int login_reply;//��½���
}reply_login;


//0x02
//Ƶ������Ӧ���
typedef struct
{
	int classes_count;//Ƶ����������
	int classes_id;//Ƶ������ID
	int classes_name[10];//Ƶ��������
}reply_classify;

//0x03
//��Ƶ���ͷ���Ӧ���
typedef struct
{
    int classes_count;      //Ƶ����������
    char classes_name[20];  //��Ƶ����
    int video_id;           //��Ƶ��
    int arrayvideoid[10];   //��Ƶ����
}reply_type;

typedef struct 
{
    int type;
}client_type;

//0x04
//��������Ӧ���
typedef struct
{
	int area_count;//��Ƶ��������
	int classes_id;//��ƵƵ������ID
	int area_id;//��Ƶ��������ID
	char area_name[10];//��Ƶ������������
}reply_area;

//0x05
//��Ƶ�б�
typedef struct 
{
        char VideoName[20];//��Ƶ��
     
}reply_list;

//0x06
//��Ƶ�㲥����
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
typedef struct VideoRecord   //�ϴ�ӰƬ����ʱ�������
{
        char user_name[10];  //�û�����
        int video_id;        //��ƵID
        float video_seek;    //����ʱ��
}VideoRecord_t;
typedef struct res_VideoRecord   //ӰƬ����ʱ��Ӧ���
{
        int results;            //�ϴ������0-�ϴ��ɹ���1-�ϴ�ʧ��
}res_VideoRecord_t;
//0x09
typedef struct PlayHistory    //������ʷ�����
{
        char user_name[10];         //�û�����
        int video_id;        //��ƵID
}PlayHistory_t;

typedef struct res_PlayHistory    //������ʷӦ���
{
        char user_name[10];      //�û�����
        int video_id;        //��ƵID
        float video_seek;    //����ʱ��
}res_PlayHistory_t;
//0x04
typedef struct VideoArea    //��Ƶ���������
{
    int type_channel;        //��ƵƵ������
    int type_Areaid;        //��Ƶ����ID    ����0:ȫ��;1:��½;2:���;3:����;4:����;5:Ӣ��;6:�ձ�;
}VideoArea_t;
//0x05
typedef struct VideoType    //��Ƶ���������
{
    int type_channel;        //��ƵƵ������
    int type_Areaid;         //��Ƶ����ID
    int type_Videoid;        //��Ƶ����ID ����0:ȫ��;1:ϲ��;2:����;3:����;4:����;5:ǹս;6:�ƻ�;
    int type_time;            //time  1.2019  2.2018  3.2017  4.2016   5.2015   6.other time 
}VideoType_t;
#endif /* __QQ_PUBLIC_H */

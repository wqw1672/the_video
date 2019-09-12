/*************************************************************************
	> File Name: CSDB.cpp
	> Author: 
	> Mail: 
	> Created Time: 2018年10月29日 星期一 13时13分47秒
 ************************************************************************/

#include "CSinDB.h"
#include "QQpublic.h"
#include<vector>
#include<iostream>
#include<stdlib.h>
#include<sstream>
#include<sqlite3.h>

using namespace std;

CSinDB * CSinDB::pSinDB = NULL;
sqlite3 *CSinDB::db = NULL;

//因为一个程序只用一个数据库，所以可以写死
CSinDB::CSinDB(const char *acName, sqlite3 **d)
{
    int iResult = sqlite3_open(acName, d);
    if (iResult == SQLITE_OK)
    {
        printf("open successs\n");
    }
    else
    {
        printf("THE  open  error%s",sqlite3_errmsg(db));
    }
}

CSinDB::~CSinDB()
{
    sqlite3_close(CSinDB::db);
}


//让其只获得一个实例
CSinDB *CSinDB::GetSinDB()
{
    if (CSinDB::pSinDB == NULL)
    {
        CSinDB::pSinDB = new CSinDB("./VedioPlay.db",&CSinDB::db);
    }
    //	printf("get singledb");
    return CSinDB::pSinDB;
}

//回调函数，用来获取所有视频信息
//sqlite3的回调函数 
//sqlite 每查到一条记录，
//就调用一次这个回调 int LoadMyInfo(void* para,intn_column,char** column_value,char** column_name);
/*
* 参数
* para是你在sqlite3_exec 里传入的void*参数通过para参数，你可以传入一些特殊的指针（比如类指针、结构指针）， 
	然后在这里面强制转换成对应的类型（这里面是void*类型，必须强制转换成你的类型才可用）。
* n_column是这一条记录有多少个字段(即这条记录有多少列) 
* char** column_value 是个关键值，查出来的数据都保存在这里，它实际上是个1维数组（不要以为是2维数组）， 
  每一个元素都是一个char*值，是一个字段内容（用字符串来表示，以\0结尾）  
*  char** column_name 跟column_value是对应的，表示这个字段的字段名称
*/
/*
void get_video(void *para, int count, char **value, char **name)
{
				int i = 0;
				vector<ReplyList> *vec = (vector<ReplyList> *)para;
        ReplyList video;
        video.VideoId = atoi(value[i++]);
        *video.VideoName = *value[i++];
        *video.VideoAddress = *value[i++];
        video.PlayCount = atoi(value[i++]);
        *video.VideoImage = *value[i++];
        video.RegionId = atoi(value[i++]);
        video.ChannelId = atoi(value[i++]);
        video.TypeIds[0] = atoi(value[i++]);
        video.VideoTime = atoi(value[i++]);
        vec->push_back(video);
   //     qDebug() << "存入";
}
*/

//获取视频数据
/*参数
*sql:数据库执行语句
*get_video:回调函数，每查到一条数据，就执行一次回调函数
*video:此指针是传入回调函数内的
*/


int CSinDB::DoOpera(const char *sql, sqlite3_callback get_video, void *video)
{
    return sqlite3_exec(db,sql,get_video,video,&errmsg);
}

int CSinDB::Find_thename(const char *sql, char ***presult, int *nrow, int *ncolumn)
{
      int index=sqlite3_get_table(db, sql, presult, nrow,  ncolumn,&errmsg);

     return index;
}
//获取错误
char *CSinDB::GetErrmsg()
{
    return errmsg;
}

//用户数据修改
/*
*参数：
*pwd:用户密码
*name:用户名
*/

void CSinDB::user_update(char * pwd,char *name)
{
		char sql[100];
		sprintf(sql,"UPDATE user set password = '%s' where account = '%s'",pwd,name);
		sqlite3_exec(CSinDB::pSinDB->db,sql,NULL,NULL,&errmsg);
}

//用户数据删除
/*参数
*用户ID
*/

void CSinDB::user_delete(int id)
{
		char sql[100];
		sprintf(sql,"DELETE from user where usr_id = %d",id);
		sqlite3_exec(CSinDB::pSinDB->db,sql,NULL,NULL,&errmsg);
}

//用户数据添加
/*参数
*id:用户ID
*pwd：用户密码
*name:用户名
*/

void CSinDB::user_add(int id,char *pwd,char *name)
{
		char sql[100];
		sprintf(sql,"insert into user values(%d,'%s','%s')",id,name,pwd);
		sqlite3_exec(CSinDB::pSinDB->db,sql,NULL,NULL,&errmsg);
}

/*
void CSinDB::user_inquire(char *name ,char *pwd)
{
    char sql[100];
    sprintf(sql,"select * from user where account = '%s' and password = '%s'",name,pwd);
    sqlite3_exec(CSinDB::pSinDB->db,sql,NULL,NULL,&errmsg);
}
*/
/*一下为个人记录增删改查*/

//个人记录修改
/*
*参数：
*percent:视频进度百分比
*user_id:用户id
*video_id:视频id
*/

void CSinDB::recording_update(int percent, int user_id, int video_id)
{
		char sql[100];
		sprintf(sql,"UPDATE recording set percent = %d where user_id = %d and video_id = %d",percent,user_id,video_id);
		sqlite3_exec(CSinDB::pSinDB->db,sql,NULL,NULL,&errmsg);
}

//个人记录删除
/*
*参数
*id:视频ID
*/

void CSinDB::recording_delete(int id)
{
		char sql[100];
		sprintf(sql,"DELETE from recording where video_id = %d",id);
		sqlite3_exec(CSinDB::pSinDB->db,sql,NULL,NULL,&errmsg);
}

//用户数据添加
/*
*参数
*percent:视频百分比
*user_id:用户ID
*video_id:视频ID
*/

void CSinDB::recording_add(int percent, int user_id, int video_id)
{
		char sql[100];
		sprintf(sql,"insert into recording values(%d,%d,%d)",user_id,video_id,percent);
		sqlite3_exec(CSinDB::pSinDB->db,sql,NULL,NULL,&errmsg);
}



/*************************************************************************
	> File Name: CSDB.h
	> Author: 
	> Mail: 
	> Created Time: 2018年10月29日 星期一 13时19分48秒
 ************************************************************************/

#ifndef _csinDB_h_
#define _csinDB_h_

#include <sqlite3.h>
#include <stdio.h>

//数据库单利
class CSinDB
{
public:
    static CSinDB *GetSinDB();//获取实例
    int DoOpera(const char *sql,sqlite3_callback CallBack, void *);//
    int Find_thename(const char *sql, char ***presult, int *nrow, int *ncolumn);
    char *GetErrmsg();//获取错误
    
    /*用户数据操作*/
    
    void user_update(char * pwd,char *name);//用户数据修改
    void user_delete(int id);//删除数据
    void user_add(int id,char *pwd,char *name);//用户数据添加;
    void user_inquire(char *name ,char *pwd);//账号密码匹配
    
    void get_video(void *video, int count, char **value, char **name);//回调函数，获取视频
    
    /*个人记录操作*/
    
    void recording_update(int percent, int user_id, int video_id);
    void recording_delete(int id);
    void recording_add(int percent, int user_id, int video_id);
protected:
private:
    CSinDB(const char *,sqlite3 **db);
    ~CSinDB();
    char *errmsg;

    static CSinDB *pSinDB;//实例
    static sqlite3 *db;//数据库文件句柄
};




#endif

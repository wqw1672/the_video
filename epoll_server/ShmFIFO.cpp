#include <sys/types.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include "ShmFIFO.h"

#define ERR_EXIT(m) \
    do { \
            perror(m); \
            fprintf(stderr, "Err: (%s:%s:%d)", __FILE__, __func__, __LINE__); \
            exit(EXIT_FAILURE); \
    } while(0)


typedef struct shmhead_st
{
        int shmid;			// �����ڴ�ID

	unsigned int blksize;		// ���С
	unsigned int blocks;		// �ܿ���
	unsigned int rd_index;		// ������
	unsigned int wr_index;		// д����

        //������ڹ����ڴ��ڲ�����
        sem_t sem_mutex;	// ���������õ��ź���
        sem_t sem_full;		// �������ƹ����ڴ��Ƿ������ź���
        sem_t sem_empty;	// �������ƹ����ڴ��Ƿ�յ��ź���

}shmhead_t;

CShmFIFO::CShmFIFO(int key, int blksize, int blocks)
{
	this->open(key, blksize, blocks);
}

CShmFIFO::CShmFIFO()
{
	m_shmhead = NULL;
	m_payload = NULL;
	m_open = false;
}

CShmFIFO::~CShmFIFO()
{
	this->close();
}
 
//����ͷ��ַ
bool CShmFIFO::init(int key, int blksize, int blocks)
{
	int shmid = 0;

	//1. �鿴�Ƿ��Ѿ����ڹ����ڴ棬�������ɾ���ɵ�
	shmid = shmget((key_t)key, 0, 0);	
	if (shmid != -1)									
	{
		shmctl(shmid, IPC_RMID, NULL); 	//	ɾ���Ѿ����ڵĹ����ڴ�
	}
		
	//2. ���������ڴ�
	shmid = shmget((key_t)key, sizeof(shmhead_t) + blksize*blocks, 0666 | IPC_CREAT | IPC_EXCL);
	if(shmid == -1)
	{
		ERR_EXIT("shmget");
	}
	printf("Create shmid=%d size=%u \n", shmid, sizeof(shmhead_t) + blksize*blocks);
	
	//3.���ӹ����ڴ�
	m_shmhead = shmat(shmid, (void*)0, 0);					//���ӹ����ڴ�
	if(m_shmhead == (void*)-1)						
	{
		ERR_EXIT("shmat");
	}
	memset(m_shmhead, 0, sizeof(shmhead_t) + blksize*blocks);		//��ʼ��
	
	//4. ��ʼ�������ڴ���Ϣ
	shmhead_t * pHead = (shmhead_t *)(m_shmhead);
	pHead->shmid	= shmid;				//�����ڴ�shmid
	pHead->blksize	= blksize;			//������Ϣд��
	pHead->blocks	= blocks;				//д��ÿ���С
	pHead->rd_index = 0;					//һ��ʼλ�ö��ǵ�һ��
	pHead->wr_index = 0;					//
	sem_init(&pHead->sem_mutex, 1, 1);	// ��һ��1��ʾ���Կ���̹����ڶ���1��ʾ��ʼֵ
	sem_init(&pHead->sem_empty, 1, 0);	// ��һ��1��ʾ���Կ���̹����ڶ���0��ʾ��ʼֵ
	sem_init(&pHead->sem_full, 1, blocks);// ��һ��1��ʾ���Կ���̹����ڶ���blocks��ʾ��ʼֵ
	
	//5. �����ƹ����ڴ����Ϣ
	m_payload = (char *)(pHead + 1);	//ʵ�ʸ�����ʼλ��
	m_open = true;

	return true;
}

void CShmFIFO::destroy()
{
	shmhead_t *pHead = (shmhead_t *)m_shmhead;
	int shmid = pHead->shmid;

	//ɾ���ź���
	sem_destroy (&pHead->sem_full);
	sem_destroy (&pHead->sem_empty);
	sem_destroy (&pHead->sem_mutex);
	shmdt(m_shmhead); //�����ڴ�����
	
	//���ٹ����ڴ�
	if(shmctl(shmid, IPC_RMID, 0) == -1)		//ɾ�������ڴ�
	{
		printf("Delete shmid=%d \n", shmid);
		ERR_EXIT("shmctl rm");
	}

	m_shmhead = NULL;
	m_payload = NULL;
	m_open = false;
}

void CShmFIFO::Destroy(int key)
{
	int shmid = 0;

	//1. �鿴�Ƿ��Ѿ����ڹ����ڴ棬�������ɾ���ɵ�
	shmid = shmget((key_t)key, 0, 0);	
	if (shmid != -1)									
	{
		printf("Delete shmid=%d \n", shmid);
		shmctl(shmid, IPC_RMID, NULL); 	//	ɾ���Ѿ����ڵĹ����ڴ�
	}	
}

//����ͷ��ַ
bool CShmFIFO::open(int key, int blksize, int blocks)
{
	int shmid;

	this->close();

	//1. �鿴�Ƿ��Ѿ����ڹ����ڴ棬�������ɾ���ɵ�
	shmid = shmget((key_t)key, 0, 0);
	if (shmid == -1)									
	{
		return this->init(key, blksize, blocks);
	}

	//2.���ӹ����ڴ�
	m_shmhead = shmat(shmid, (void*)0, 0);					//���ӹ����ڴ�
	if(m_shmhead == (void*)-1)						
	{
		ERR_EXIT("shmat");
	}

	//3. �����ƹ����ڴ����Ϣ
	m_payload = (char *)((shmhead_t *)m_shmhead + 1);	//ʵ�ʸ�����ʼλ��
	m_open = true;

	return true;
}

void CShmFIFO::close(void)
{
	if(m_open)
	{
		shmdt(m_shmhead); //�����ڴ�����
		m_shmhead = NULL;
		m_payload = NULL;
		m_open = false;
	}
}

void CShmFIFO::write(const void *buf)
{	
	
	shmhead_t *pHead = (shmhead_t *)m_shmhead;

	sem_wait(&pHead->sem_full);				//�Ƿ�����Դд��	����д��Դ-1
	sem_wait(&pHead->sem_mutex);				//�Ƿ���������д��		
	//printf("write to shm[%d] index %d \n", pHead->shmid, pHead->wr_index);
	//memset(m_payload + (pHead->wr_index) * (pHead->blksize), 0, pHead->blksize);
	
	memcpy(m_payload + (pHead->wr_index) * (pHead->blksize), buf, pHead->blksize);
	pHead->wr_index = (pHead->wr_index+1) % (pHead->blocks);	//дλ��ƫ��		
	sem_post(&pHead->sem_mutex);				//�������	
	sem_post(&pHead->sem_empty);				//���ö���Դ+1
}

void CShmFIFO::read(void *buf)
{	
	shmhead_t *pHead = (shmhead_t *)m_shmhead;

	sem_wait(&pHead->sem_empty);				//���д��Դ�Ƿ����		
	//sem_wait(&pHead->sem_mutex);	
	//memset(buf, 0, sizeof(buf));
	printf("read from shm[%d] index %d \n", pHead->shmid, pHead->rd_index);
	memcpy(buf, m_payload + (pHead->rd_index) * (pHead->blksize), pHead->blksize);
	//memset(m_payload + (pHead->rd_index) * (pHead->blksize), 0, pHead->blksize);
	//��λ��ƫ��
	pHead->rd_index = (pHead->rd_index+1) % (pHead->blocks);	
	//sem_post(&pHead->sem_mutex);				//�������
	sem_post(&pHead->sem_full);					//���ӿ�д��Դ	
}


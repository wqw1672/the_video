#ifndef _SHM_FIFO_H_
#define _SHM_FIFO_H_

#include <iostream>

class CShmFIFO 
{
public:
	CShmFIFO(int key, int blksize, int blocks);
	CShmFIFO();
	virtual ~CShmFIFO();

	//����������
	bool init(int key, int blksize, int blocks);
	void destroy(void);
	static void Destroy(int key); //��̬ɾ�������ڴ淽��

	// �򿪺͹ر�
	bool open(int key, int blksize, int blocks);
	void close(void);

	//��ȡ�ʹ洢
	void write(const void *buf);
	void read(void *buf);


protected:
	//���̿�����Ϣ��
	bool m_open;
	void *m_shmhead;		// �����ڴ�ͷ��ָ��
	char *m_payload;			// ��Ч���ص���ʼ��ַ
};


#endif /* _SHM_FIFO_H_ */

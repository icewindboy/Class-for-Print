// SerialPort.h: interface for the CSerialPort class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SERIALPORT_H__B7189E10_6AF6_4C42_9F29_DBB34CB825AD__INCLUDED_)
#define AFX_SERIALPORT_H__B7189E10_6AF6_4C42_9F29_DBB34CB825AD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class __declspec(dllexport)  CSerialPort  
{
public:
	CSerialPort();
	virtual ~CSerialPort();
public:
	//���ô��ں�
	void	setPortName(const int port);
	//���ò�����
	void	setBaud( UINT baud );
	//�򿪴���
	BOOL	InitPort( UINT portnr = 2, UINT baud = 1152, char parity = 'N', UINT databits = 8, UINT stopsbits = 1 );
	//��������
	int	WriteToPort(unsigned  char *writeData,int WriteSize );
	//��ȡ����
	int	ReceiveChar( char *readBuff, int ReadSize );
	//�رմ���
	void	ClosePort();
private:
	HANDLE	m_hComm;   //���ھ��
	COMMTIMEOUTS	m_CommTimeouts;//��ʱʱ��
	DCB	m_dcb;//���ò����� ֹͣλ ���ں���.....�����ڴ����豸�Ĳ�������
	int	m_port;  //���ں�
UINT	m_baud;  //������

};

#endif // !defined(AFX_SERIALPORT_H__B7189E10_6AF6_4C42_9F29_DBB34CB825AD__INCLUDED_)

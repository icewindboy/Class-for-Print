// SerialPort.cpp: implementation of the CSerialPort class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SerialPort.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSerialPort::CSerialPort()  //���캯���г�ʼ��
{
	m_hComm = NULL;
	m_port = 1;
	m_baud = 9600;
}
CSerialPort::~CSerialPort()//�������ͷ�
{
	if (m_hComm != NULL)
	{
		CloseHandle(m_hComm);
		m_hComm = NULL;
	}
}
/**********************************************************************
* �������ƣ� setPortName
* ���������� ���ô��ں�
* ��������� port Ҫ���õĴ��ں�
* ��������� ��
* �� �� ֵ�� ��
* ����˵���� 
***********************************************************************/
void CSerialPort::setPortName( const int port)//���ô��ں�
{
    m_port = port;
}
/**********************************************************************
* �������ƣ� setBaud
* ���������� ���ô��ڲ�����
* ��������� baud Ҫ�豸�Ĳ�����
* ��������� ��
* �� �� ֵ�� ��
* ����˵���� 
***********************************************************************/
void CSerialPort::setBaud( UINT baud )//���ò�����
{
	m_baud = baud;
}

/**********************************************************************
* �������ƣ� ClosePort
* ���������� �رմ���
* ��������� ��
* ��������� ��
* �� �� ֵ�� ��
* ����˵���� 
***********************************************************************/
void CSerialPort::ClosePort()//�رմ���
{
	if (m_hComm != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hComm);
	}
	m_hComm = INVALID_HANDLE_VALUE;
}
/**********************************************************************
* �������ƣ� InitPort
* ���������� ��ʼ������
* ��������� portnr ���ںţ�baud �����ʣ�parity ���ԣ�databits ����λ��
                stopsbits ֹͣλ
* ��������� ��
* �� �� ֵ�� �ɹ����� TRUE��ʧ�ܷ��� FALSE
* ����˵���� 
***********************************************************************/
BOOL CSerialPort::InitPort( UINT portnr, UINT baud, char parity, UINT databits, UINT stopsbits )//�򿪴���
{
	//assert(portnr > 0 && portnr < 50);
	char *szPort = new char[50]; // ���洮�ں�
	char *szBaud = new char[50]; // ���洮�����ò���
	baud = m_baud;
	portnr = m_port;
	sprintf(szPort, "COM%d", portnr);
	sprintf(szBaud, "baud=%d parity=%c data=%d stop=%d", baud, parity, databits, stopsbits);
	if (m_hComm != NULL)
	{
		CloseHandle(m_hComm);
		m_hComm = NULL;
	}
	m_hComm = CreateFile(szPort,	// communication port string (COMX)
		GENERIC_READ | GENERIC_WRITE,	// read/write types
		NULL,	// comm devices must be opened with exclusive access
		NULL,	// no security attributes
		OPEN_EXISTING,	// comm devices must use OPEN_EXISTING
		FILE_ATTRIBUTE_NORMAL,	// Async I/O
		NULL);	// template must be 0 for comm devices
	if (m_hComm == INVALID_HANDLE_VALUE)
	{
		// port not found
		delete [] szPort;
		delete [] szBaud;
		return FALSE;
	}
	// set the timeout values
	memset(&m_CommTimeouts, 0, sizeof(m_CommTimeouts));
	memset(&m_dcb, 0, sizeof(m_dcb));
	m_CommTimeouts.ReadIntervalTimeout = 1000;
	m_CommTimeouts.ReadTotalTimeoutMultiplier = 1000;
	m_CommTimeouts.ReadTotalTimeoutConstant = 1000;
	m_CommTimeouts.WriteTotalTimeoutMultiplier = 1000;
	m_CommTimeouts.WriteTotalTimeoutConstant = 1000;
	// configure
	// ���ô���
	if (SetCommTimeouts(m_hComm, &m_CommTimeouts))
	{	  
		if (GetCommState(m_hComm, &m_dcb))
		{
			if (BuildCommDCB(szBaud, &m_dcb))
			{
				if (SetCommState(m_hComm, &m_dcb))
					; // normal operation... continue
				else
					return FALSE;//ProcessErrorMessage("SetCommState()");
			}
			else
				return FALSE;//ProcessErrorMessage("BuildCommDCB()");
		}
		else
			return FALSE;//ProcessErrorMessage("GetCommState()");
	}
	else
		return FALSE;//ProcessErrorMessage("SetCommTimeouts()");

	delete [] szPort;
	delete [] szBaud;
	// flush the port
	// �崮�ڻ���
	PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
	return TRUE;
}

/**********************************************************************
* �������ƣ� WriteToPort
* ���������� �򴮿�д����
* ��������� *writeData ָ��Ҫ���͵����ݣ�WriteSize ΪҪ���͵����ݴ�С
* ��������� ��
* �� �� ֵ�� �ɹ����ط��͵��ֽ�����ʧ�ܷ��� -1
* ����˵���� 
***********************************************************************/
int	CSerialPort::WriteToPort( unsigned  char *writeData, int WriteSize )//��������
{
	BOOL bResult = TRUE;
	DWORD BytesSent = 0;
	// Clear buffer
	PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
	bResult = WriteFile(m_hComm, writeData, WriteSize, &BytesSent, NULL);
	if (!bResult)  
	{
		DWORD dwError = GetLastError();
		switch (dwError)
		{
		case ERROR_IO_PENDING:
			{
				// continue to GetOverlappedResults()
				BytesSent = 0;
				//bWrite = FALSE;
				return -1;
				break;
			}
		default:
			{
				// all other error codes
				//port->ProcessErrorMessage("WriteFile()");
				//return -1;
			}
		}
	} 
	// end if(bWrite)
	//Verify that the data size send equals what we tried to send
	if (BytesSent != (DWORD)WriteSize)	// Length of message to send)
	{
		//TRACE("WARNING: WriteFile() error.. Bytes Sent: %d; Message Length: %d\n", BytesSent, strlen((char*)port->m_szWriteBuffer));
		return -1;
	}
	return BytesSent;
}
//----------------
/**********************************************************************
* �������ƣ� ReceiveChar
* ���������� �Ӵ��ڽ�������
* ��������� *readBuff ָ��������ݻ��棬ReadSize ΪҪ���յ��ֽ���
* ��������� ��
* �� �� ֵ�� �ɹ����ؽ��յ��ֽ�����ʧ�ܷ��� -1
* ����˵���� 
***********************************************************************/
int	CSerialPort::ReceiveChar( char *readBuff, int ReadSize )//��������
{
	BOOL  bRead = TRUE; 
	BOOL  bResult = TRUE;
	DWORD dwError = 0;
	DWORD BytesRead = 0;
	bResult = ReadFile(m_hComm, readBuff, ReadSize, &BytesRead, NULL);
	if (!bResult)  
	{ 
		switch (dwError = GetLastError()) 
		{ 
		case ERROR_IO_PENDING:
			{ 
				// asynchronous i/o is still in progress 
				// Proceed on to GetOverlappedResults();
				//bRead = FALSE;
				return -1;
				break;
			}
		default:
			{
				// Another error has occured.  Process this error.
				//port->ProcessErrorMessage("ReadFile()");
				break;
			} 
		}
	}
	if (BytesRead != (DWORD)ReadSize)
	{
		PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR);
		return -1;
	}
	return BytesRead;
}

#ifndef __DLL_PRINT_H__
#define __DLL_PRINT_H__
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
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
class __declspec(dllexport)  CPrint : public CSerialPort  
{
public:
    int MW_SetPrinterAgruments(unsigned char n, unsigned char m);
    int MW_SetHanZiAsTwiceHeightAndWidth(unsigned char n);
    int MW_SetHanZiLeftAndRightSpace(unsigned char ucLeft, unsigned char ucRight);
    int MW_ShowHanZiUndline(unsigned char n);
    int MW_SetHanZiMode(unsigned char n);
    int MW_SetBarcodeWidth(unsigned char n);
    int MW_PrintBarcode(unsigned char ucMode, char *pcData);
    int MW_SetBarcodeHeight(unsigned char n);
    int MW_SetHriFont(unsigned char n);
    int MW_SetPrintWidth(unsigned int n);
    int MW_SetPrintLeftMargin(unsigned int n);
    int MW_SetHriLocation(unsigned char n);
    int MW_SelectBlackAndWhiteReverse(unsigned char n);
    int MW_SelectFontSize(unsigned char n);
    int MW_PrintFlashBitmap(unsigned char n);
    int MW_DownloadBitmapToFlash(char *pcBitmapAddr);
    int MW_SelectAlignMode(unsigned char n);
    int MW_SetRefPrintPosition(unsigned int iPost);
    int MW_SelectAsciiFont(unsigned char n);
    int MW_SetBoldMode(unsigned char n);
    int MW_InitPrint(void);
    int MW_SetLineSpace(unsigned char n);
    int MW_SetDefaultLineSpace(void);
    int MW_SetUndlineMode(unsigned char ucMode);
    int MW_SetAbsPrintPosition(unsigned int iPost);
    int MW_SelectPrintMode(unsigned char ucMode);
    int MW_PrintString(char * pcString);
    int MW_SetCharRightSpace(unsigned char n);
    int MW_PrintBitmap(char * pcPicAddr);
    unsigned char MW_RealTimeStatus(unsigned char n);
    void MW_LF(void);
    CPrint();
    virtual ~CPrint();
    
private:
    int GetPrintStatus();
};

#endif
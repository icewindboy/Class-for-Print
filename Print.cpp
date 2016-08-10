// Print.cpp: implementation of the CPrint class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Print.h"
#include <string>
using namespace std;


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPrint::CPrint()
{

}

CPrint::~CPrint()
{

}
/**********************************************************************
* �������ƣ� MW_LF
* ���������� ����
* ��������� // �������˵��������ÿ�����������á�ȡֵ˵�����������ϵ
* ��������� // �����������˵����
* �� �� ֵ�� // ��������ֵ��˵�� 
* ����˵���� // ����˵��
***********************************************************************/
void CPrint::MW_LF()
{
	unsigned char cWriteBuf[10];
	cWriteBuf[0] = 0x0A;
	WriteToPort(cWriteBuf, 1);
}

/**********************************************************************
* �������ƣ� MW_LF
* ���������� ��ѯ��ӡ��ʵʱ״̬ 
* ��������� n: ָ����Ҫ���͵Ĵ�ӡ��״̬    (1<= n <=4 ) 
				n = 1: ���ʹ�ӡ��״̬
				n = 2: �����ѻ�״̬
				n = 3: ���ʹ���״̬
				n = 4: ����ֽ������״̬ 
* ��������� // �����������˵����
* �� �� ֵ�� ��������Ĳ�ͬ��ӡ��״̬���в�ͬ�Ĵ������
			��n = 1������ӡ��״̬
				λ	0/1		ʮ��������		ʮ������	����
				0	0		00				0			�̶�Ϊ0
				1	1		02				2			�̶�Ϊ1
				2	0		00				0			һ��������Ǯ���
					1		04				4			����Ǯ�䶼�ر�
				3	0		00				0			����
					1		08				8			�ѻ� 
				4	1		10				16			�̶�Ϊ1
				5/6	-		--				--			δ���� 
				7	0		00				0			�̶�Ϊ 0
			��n = 2 �����ѻ�״̬
				λ	0/1		ʮ��������		ʮ������	����
				0	0		00				0			�̶�Ϊ0
				1	1		02				2			�̶�Ϊ1
				2	0		00				0			�ϸǹ�
					1		04				4			�ϸǿ�
				3	0		00				0			δ����ֽ��
					1		08				8			���¸�ֽ��
				4	1		10				16			�̶�Ϊ1
				5	0		00				0			��ӡ����ȱֽ
					1		20				32			��ӡ��ȱֽ
				6	0		00				0			û�г������
					1		40				64			�д������
				7	0		00				0			�̶�Ϊ 0
			��n = 3 ��������״̬
				λ	0/1		ʮ��������		ʮ������	����
				0	0		00				0			�̶�Ϊ0
				1	1		02				2			�̶�Ϊ1
				2	-		--				-			δ����
				3	0		00				0			�е��޴���
					1		08				8			�е��д���
				4	1		10				16			�̶�Ϊ1
				5	0		00				0			�޲��ɻָ�����
					1		20				32			�в��ɻָ�����
				6	0		00				0			��ӡͷ�¶Ⱥ͵�ѹ����
					1		40				64			��ӡͷ�¶Ȼ��ѹ������Χ
				7	0		00				0			�̶�Ϊ 0
			��n = 4 ��������ֽ״̬
				λ	0/1		ʮ��������		ʮ������	����
				0	0		00				0			�̶�Ϊ0
				1	1		02				2			�̶�Ϊ1
				2/3	0		00				0			��ֽ
					1		04				4			ֽ����
				4	1		10				16			�̶�Ϊ1
				5/6	0		00				0			��ֽ
					1		20				32			ֽ��
				7	0		00				0			�̶�Ϊ 0
* ����˵���� // ����˵��
***********************************************************************/
unsigned char CPrint::MW_RealTimeStatus(unsigned char n)
{
	unsigned char cWriteBuf[3];
	char cReadBuf[1];
	if((n > 4)||(n < 1))
		return -1;
	
	cWriteBuf[0] = 0x10;
	cWriteBuf[1] = 0x04;
	cWriteBuf[2] = n;
	WriteToPort(cWriteBuf, 3);
	ReceiveChar(cReadBuf,1);
	return cReadBuf[0];
}

/**********************************************************************
* �������ƣ� MW_PrintBitmap
* ���������� ��ӡλͼ
* ��������� *pcPicAddr ��Ҫ��ӡ��ͼƬ��ַ��	
* ��������� ��
* �� �� ֵ�� 0 ������-1 ��ʾ����
* ����˵������ַ�е� \ ��Ҫ����ת�壬�� D:\\VC_Project\\Demo\\��è.bmp
***********************************************************************/
int CPrint::MW_PrintBitmap(char *pcPicAddr)
{
 
	FILE * pFile;  
    long lSize;  
    unsigned char * pucBuffer;  
    size_t result;  
      
    /* ��Ҫһ��char��©�ض��������ļ���ֻ�ܲ��ö����Ʒ�ʽ�� */   
    pFile = fopen ((char*)pcPicAddr, "rb" );  
    //pFile = fopen ("D:\\VC_Project\\Demo\\��è.bmp", "rb" );  
    if (pFile==NULL)  
    {  
        fputs ("File error",stderr);  
        return -1;
    }  
  
    /* ��ȡ�ļ���С */  
    fseek (pFile , 0 , SEEK_END);  
    lSize = ftell (pFile);  
    rewind (pFile);  
  
    /* �����ڴ�洢�����ļ� */   
    pucBuffer = (unsigned char*) malloc (sizeof(unsigned char)*lSize);  
    if (pucBuffer == NULL)  
    {  
        fputs ("Memory error",stderr);   
        return -1;
    }  
	
    /* ���ļ�������buffer�� */  
    result = fread (pucBuffer,1,lSize,pFile);  
    if (result != (size_t)lSize)  
    {  
        fputs ("Reading error",stderr);  
         return -1;
    }  

	/////////////////////////////////////////////////////////////////////////
	BITMAPFILEHEADER *ptBitmap = (BITMAPFILEHEADER *)pucBuffer;
	BITMAPINFOHEADER *ptBitmapH = (BITMAPINFOHEADER *)&pucBuffer[14];
	int iWidth;
	int iHeight;
	int iBMPBpp;
	int iLineWidthAlign;
	int iLineWidthReal;
	int iCurBlue;
	int iCurLine;
	unsigned char ucaData[10] = {0};
	unsigned char *pucSrc; // ָ��ͼƬ����

	//�����ַ��м��Ϊ 0 ����
    unsigned char data[] = { 0x1B, 0x33, 0x00 };
 	WriteToPort(data, sizeof(data));

	//ESC * m nL nH d1��dk   ѡ��λͼģʽ
	// ESC * m nL nH
	unsigned char escBmp[] = { 0x1B, 0x2A, 0x00, 0x00, 0x00 };


	/* ��ͬģʽ������ƣ�ÿ�й̶����ص�Ϊ 384
			0x00: 8 �㵥�ܶȣ�192 ���λͼ
			0x01: 8 ��˫�ܶȣ�384 ���λͼ 
			0x32: 24 �㵥�ܶȣ�192 ���λͼ 
			0x33: 24 ��˫�ܶȣ�384 ���λͼ 
	*/
	escBmp[2] = 0x21;
	
	//nL, nH
	escBmp[3] = (unsigned char)(ptBitmapH->biWidth % 256);
	escBmp[4] = (unsigned char)(ptBitmapH->biWidth / 256);
	
	iWidth = ptBitmapH->biWidth;
	iHeight = ptBitmapH->biHeight;
	iBMPBpp = ptBitmapH->biBitCount;
	
	iLineWidthReal = iWidth * iBMPBpp / 8;
	iLineWidthAlign = (iLineWidthReal + 3) & ~0x3;   // �� 4 ȡ�� 
	
	

	pucSrc = pucBuffer + ptBitmap->bfOffBits; // ָ��ͼƬ����
    //pucSrc = pucSrc + (iHeight - 1) * iLineWidthAlign; 

//  // 8 �㵥�ܶ�
// 	for (int y = 0; y < (iHeight / 8 + 1); y++)// ѭ���飬ÿ 8 ��һ�� + 1 �������һ��
// 	{	
// 		WriteToPort(escBmp, 5);
// 		for (int x = 0; x < iWidth; x++)// ѭ���У�ÿ��һ�ֽ�
// 		{
// 			ucaData[0] = 0x00;
// 			for (int z = 0; z < 8; z++) // ѭ��ÿ�ֽ�
// 			{
// 				//��ǰ���ص���(�� 0 ��ʼ) = (z * iWidth + x + y * 8 * iWidth);
// 				iCurPiexl = (z * iWidth + x + y * 8 * iWidth);
// 
// 				// �ж��ǻ��ڵ�ǰλͼ�� 
// 				if (iCurPiexl < iWidth * iHeight)
// 				{
// 					// iCurPiexl / iWidth = ������ 
// 					// iCurPiexl % iWidth = ������ = x
// 					// ���� 4 �ֽڶ���������λ�� = ������ * �������п� + ������ * 3��24 λ�
// 					iCurBlue = iCurPiexl / iWidth * iLineWidthAlign + ( (iWidth - x - 1) * 3); // (iWidth - x - 1) ����
// 					if (pucSrc[iCurBlue] != 0xFF ) // 0x00 0x00 0x00 ��ʾ��ɫ��0xFF 0xFF 0xFF ��ʾ��ɫ
// 					{
// 						ucaData[0] |= (1 << (7-z));
// 					}
// 				}	
// 			}
// 			WriteToPort(ucaData, 1);
// 			printf("0x%X ", ucaData[0]);
// 		}
// 		MW_LF();
// 	}

//	for (int y = 0; y < (iHeight / 24 + 1); y++)// ѭ���飬ÿ 24 ��һ�� + 1 �������һ��
    // ͼƬ��ת��������Ҳ�� ���� 0 �� (iHeight / 24 + 1) ���  (iHeight / 24 + 1) ��0
	for (int y = (iHeight / 24); y >=0 ; y--)// ѭ���飬ÿ 24 ��һ�� + 1 �������һ��
	{	
		WriteToPort(escBmp, 5);
		for (int x = 0; x < iWidth; x++)// ѭ���У�ÿ��3�ֽ�
		{
			ucaData[0] = 0x00;
			ucaData[1] = 0x00;
			ucaData[2] = 0x00;
			for (int z = 0; z < 24; z++) // ѭ��ÿ���ֽ�λ
			{
				//��ǰ���ص���(�� 0 ��ʼ) = �ڼ�λ * ÿ�������� + ���е�������� + �ڼ��� * ������ * ÿ�������� =(z * iWidth + x + y * 8 * iWidth);
				//iCurPiexl = (z * iWidth + x + y * 24 * iWidth);

				// ������ = ��ǰ�ڼ�λ + ��ǰ�� * ���Ա����
				iCurLine = z + y * 24;


				// �ж��ǻ��ڵ�ǰλͼ�� 
				if (iCurLine < iHeight)
				{
					// iCurPiexl / iWidth = ������ = ��ǰ�ڼ�λ + ��ǰ�� * ���Ա����
					// iCurPiexl % iWidth = ������ = x

					// ���� 4 �ֽڶ���������λ�� = ������ * �������п� + ������ * 3��24 λ�
					iCurBlue = iCurLine * iLineWidthAlign + ( x * 3); // (iWidth - x - 1) �����ݵߵ�
					if (pucSrc[iCurBlue] != 0xFF ) // 0x00 0x00 0x00 ��ʾ��ɫ��0xFF 0xFF 0xFF ��ʾ��ɫ
					{
//                         if ( z < 8)
//                         {
//                             ucaData[0] |= (1 << (7-z));
//                         }
//                         else
//                             if ( (z >= 8) && (z < 16) )
//                         {
//                             ucaData[1] |= (1 << (15-z));
//                         }
//                         else
//                             if((z >= 16) && (z < 24))
//                         {
//                             ucaData[2] |= (1 << (23-z));
//                         }

                        // ͼƬ��ת, ԭ���� ucaData[0-2] = 0 - 24 λ������Ҫ��� ucaData[0-2] = 24-0
                        if ( z < 8)
                        {
                            ucaData[2] |= (1 << (z));
                        }
                        else
                            if ( (z >= 8) && (z < 16) )
                        {
                            ucaData[1] |= (1 << (z - 8));
                        }
                        else
                            if((z >= 16) && (z < 24))
                        {
                            ucaData[0] |= (1 << (z - 16));
                        }
						
						
					}
				}	
			}
                       
			WriteToPort(ucaData, 3);
		}
		MW_LF();
	}


	/////////////////////////////////////////////////////////////////////////
    /* ������ʾ���ر��ļ����ͷ��ڴ� */  
    fclose (pFile);  
    free (pucBuffer);  
   return 0; 
	

}

/**********************************************************************
* �������ƣ� MW_SetCharRightSpace
* ���������� ���ô�ӡ�ַ��ұ߾�
* ��������� n ��ʾ�ұ߾�Ϊ [ n * �����ƶ���λ�������ƶ���λ]Ӣ��
* ��������� ��
* �� �� ֵ�� ��
* ����˵����
***********************************************************************/
int CPrint::MW_SetCharRightSpace(unsigned char n)
{
	if (n <= 0 || n >=255 )
		return -1;

	unsigned char cWriteBuf[10];
	cWriteBuf[0] = 0x1B;
	cWriteBuf[1] = 0x20;
	cWriteBuf[2] = n;
	WriteToPort(cWriteBuf, 3);
	return 0;
}	


/**********************************************************************
* �������ƣ� MW_PrintString
* ���������� ��Ҫ��ӡ���ַ���
* ��������� *pcString ������Ҫ��ӡ���ַ�������	
* ��������� ��
* �� �� ֵ�� 0 �ɹ���ʧ�ܷ��� -1
* ����˵���� �ַ��Ի��н����Żᱻ��ӡ���������ڲ����Ի��н�β���ַ�����
                ������Զ���ӻ��д�ӡ������
***********************************************************************/
int CPrint::MW_PrintString(char *pcString)
{
    //fputs ("Reading error",stderr); 

	int iByteSent;
    char tmp[100];
    //string sErr;
    unsigned char err = MW_RealTimeStatus(4);

    
    sprintf(tmp, "Printer Status 0x%X\n", err);
    if (0x12 != err)
    {
        fputs (tmp, stderr); 
        return -1;
    }
	iByteSent = WriteToPort((unsigned char*)pcString, strlen(pcString));
    if (pcString[iByteSent-1] != 0x0a)
    {
        MW_LF();
    }
	return -((size_t)iByteSent != strlen(pcString));
}


int CPrint::MW_SelectPrintMode(unsigned char mode)
{

    return 0;
}

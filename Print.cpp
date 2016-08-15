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
/////////////////////////////////////////////////////////////////////////
// Private Function
/////////////////////////////////////////////////////////////////////////
/**********************************************************************
* �������ƣ� GetPrintStatus
* ���������� �����ڴ�ӡʱ�жϴ�ӡ��״̬
* ��������� ��
* ��������� ��
* �� �� ֵ�� �ɹ����� 0 ��ʧ�ܷ��� -1
* ����˵���� ��
***********************************************************************/
int CPrint::GetPrintStatus()
{
    char tmp[100];
    unsigned char err = MW_RealTimeStatus(4);
    
    sprintf(tmp, "Printer error Status 0x%X\n", err);
    if (0x12 != err)
    {
        fputs (tmp, stderr); 
        return -1;
    }
    return 0;
}









/////////////////////////////////////////////////////////////////////////
// Public Function
/////////////////////////////////////////////////////////////////////////

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
    if (n < 1 || n > 4 )
    {
        fputs ("invalid argument!\n",stderr);
        return -1;
    }
	unsigned char cWriteBuf[3];
	char cReadBuf[1];
	
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
            Ŀǰֻ֧�� 24 λ���λͼ��λͼ���ܳ��� 384 ����
***********************************************************************/
int CPrint::MW_PrintBitmap(char *pcPicAddr)
{
    if (GetPrintStatus() != 0)
        return -1;

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
* ��������� n ��ʾ�ұ߾�Ϊ [ n * �����ƶ���λ�������ƶ���λ]Ӣ�� (0 <= n <= 255)
* ��������� ��
* �� �� ֵ�� ��
* ����˵����
***********************************************************************/
int CPrint::MW_SetCharRightSpace(unsigned char n)
{
    if (n < 0 || n > 255 )
    {
        fputs ("invalid argument!\n",stderr);
        return -1;
    }
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
    if (GetPrintStatus() != 0)
        return -1;
	int iByteSent;

	iByteSent = WriteToPort((unsigned char*)pcString, strlen(pcString));
    if (pcString[iByteSent-1] != 0x0a)
    {
        MW_LF();
    }
	return -((size_t)iByteSent != strlen(pcString));
}

/**********************************************************************
* �������ƣ� MW_SelectPrintMode
* ���������� ѡ���ӡģʽ
* ��������� ucMode ��ӡģʽ                   0 <= ucmode <= 255
                λ  1/0     ʮ��������  ʮ������    ����
                0   0       00          0           ��׼ ASCII ������(12x24)
                    1       01          1           ѹ�� ASCII ������(9x17)
                1/2 -       --          -           δ����
                3   0       00          0           ȡ���Ӵ�ģʽ 
                    1       08          8           ѡ��Ӵ�ģʽ
                4   0       00          0           ȡ������ģʽ
                    1       10          16          ѡ�񱶸�ģʽ
                5   0       00          0           ȡ������ģʽ
                    1       20          32          ѡ�񱶿�ģʽ
                6   -       --          --          --
                7   0       00          0           ȡ���»���ģʽ
                    1       80          128         ѡ���»���ģʽ

* ��������� ��
* �� �� ֵ�� 0 �ɹ���ʧ�ܷ��� -1
* ����˵���� 
***********************************************************************/
int CPrint::MW_SelectPrintMode(unsigned char ucMode)
{
    if (ucMode < 0 || ucMode > 255 )
    {
        fputs ("invalid argument!\n",stderr);
        return -1;
    }
    
    unsigned char cWriteBuf[10];
    cWriteBuf[0] = 0x1B;
    cWriteBuf[1] = 0x21;
    cWriteBuf[2] = ucMode;
    WriteToPort(cWriteBuf, 3);
	return 0;
}

/**********************************************************************
* �������ƣ� MW_SetAbsPrintPosition
* ���������� ���þ��Դ�ӡλ��
* ��������� iPost ��ǰλ�þ������׾��� (0 <= iPost <=65535)
* ��������� ��
* �� �� ֵ�� �ɹ����� 0 ��ʧ�ܷ��� -1
* ����˵���� 
***********************************************************************/
int CPrint::MW_SetAbsPrintPosition(unsigned int iPost)
{
    unsigned char nL,nH;
    nL = iPost % 255;
    nH = iPost / 255;

    unsigned char cWriteBuf[10];
    cWriteBuf[0] = 0x1B;
    cWriteBuf[1] = 0x24;
    cWriteBuf[2] = nL;
    cWriteBuf[3] = nH;
    WriteToPort(cWriteBuf, 4);
	return 0;
}

/**********************************************************************
* �������ƣ� MW_SetUndlineMode
* ���������� ѡ��/ȡ���»���ģʽ
* ��������� ucMode ѡ��/ȡ���»���ģʽ 
                    ֵ              ���� 
                    0            ȡ���»���ģʽ
                    1            ѡ���»���ģʽ(1 ���)
                    2            ѡ���»���ģʽ(2 ���)
* ��������� ��
* �� �� ֵ�� �ɹ����� 0 ��ʧ�ܷ��� -1
* ����˵���� 
***********************************************************************/
int CPrint::MW_SetUndlineMode(unsigned char ucMode)
{
    if (ucMode < 0 || ucMode > 2 )
    {
        fputs ("invalid argument!\n",stderr);
        return -1;
    }
    
    unsigned char cWriteBuf[10];
    cWriteBuf[0] = 0x1B;
    cWriteBuf[1] = 0x2D;
    cWriteBuf[2] = ucMode;
    WriteToPort(cWriteBuf, 3);
	return 0;
}

/**********************************************************************
* �������ƣ� MW_SetDefaultLineSpace
* ���������� ����Ĭ���м��
* ��������� ѡ��Լ 3.75mm �м��
* ��������� ��
* �� �� ֵ�� �ɹ����� 0 ��ʧ�ܷ��� -1
* ����˵���� 
***********************************************************************/
int CPrint::MW_SetDefaultLineSpace(void)
{
    unsigned char cWriteBuf[10];
    cWriteBuf[0] = 0x1B;
    cWriteBuf[1] = 0x32;
    WriteToPort(cWriteBuf, 2);
	return 0;
}

/**********************************************************************
* �������ƣ� MW_SetLineSpace
* ���������� �����м��
* ��������� �����м��Ϊ n x ���������ƶ���λ Ӣ��   (0 <= n <= 255)
* ��������� ��
* �� �� ֵ�� �ɹ����� 0 ��ʧ�ܷ��� -1
* ����˵���� 
***********************************************************************/
int CPrint::MW_SetLineSpace(unsigned char n)
{
    if (n < 0 || n > 255 )
    {
        fputs ("invalid argument!\n",stderr);
        return -1;
    }
    unsigned char cWriteBuf[10];
    cWriteBuf[0] = 0x1B;
    cWriteBuf[1] = 0x33;
    cWriteBuf[2] = n;
    WriteToPort(cWriteBuf, 3);
	return 0;
}

/**********************************************************************
* �������ƣ� MW_InitPrint
* ���������� ʵʼ����ӡ���������ӡ�����������ݣ���ӡģʽ����Ϊ�ϵ�ʱ��Ĭ��ֵģʽ
* ��������� ��
* ��������� ��
* �� �� ֵ�� �ɹ����� 0 ��ʧ�ܷ��� -1
* ����˵���� 
***********************************************************************/
int CPrint::MW_InitPrint()
{
    unsigned char cWriteBuf[10];
    cWriteBuf[0] = 0x1B;
    cWriteBuf[1] = 0x40;
    WriteToPort(cWriteBuf, 2);
	return 0;
}


/**********************************************************************
* �������ƣ� MW_SetBoldMode
* ���������� ѡ��/ȡ���Ӵ�ģʽ
* ��������� n�� 0 ȡ���Ӵ�ģʽ��
                 1 ѡ��Ӵ�ģʽ
* ��������� ��
* �� �� ֵ�� �ɹ����� 0 ��ʧ�ܷ��� -1
* ����˵���� 
***********************************************************************/
int CPrint::MW_SetBoldMode(unsigned char n)
{
    if (n < 0 || n > 1 )
    {
        fputs ("invalid argument!\n",stderr);
        return -1;
    }
    unsigned char cWriteBuf[10];
    cWriteBuf[0] = 0x1B;
    cWriteBuf[1] = 0x45;
    cWriteBuf[2] = n;
    WriteToPort(cWriteBuf, 3);
	return 0;
}

/**********************************************************************
* �������ƣ� MW_SelecAsciiFont
* ���������� ѡ������ 
* ��������� n�� 0 ѡ���׼ ASCII ������(12x24)
                 1 ѡ��ѹ�� ASCII ������(9x17)
* ��������� ��
* �� �� ֵ�� �ɹ����� 0 ��ʧ�ܷ��� -1
* ����˵���� 
***********************************************************************/
int CPrint::MW_SelecAsciiFont(unsigned char n)
{
    if (n < 0 || n > 1 )
    {
        fputs ("invalid argument!\n",stderr);
        return -1;
    }
    unsigned char cWriteBuf[10];
    cWriteBuf[0] = 0x1B;
    cWriteBuf[1] = 0x4D;
    cWriteBuf[2] = n;
    WriteToPort(cWriteBuf, 3);
	return 0;
}

/**********************************************************************
* �������ƣ� MW_SetRefPrintPosition
* ���������� ������Ժ����ӡλ��
* ��������� iPost ��ǰλ�þ������׾���
* ��������� ��
* �� �� ֵ�� �ɹ����� 0 ��ʧ�ܷ��� -1
* ����˵���� 
***********************************************************************/
int CPrint::MW_SetRefPrintPosition(int iPost)
{
    unsigned char nL,nH;
    nL = iPost % 255;
    nH = iPost / 255;
    
    unsigned char cWriteBuf[10];
    cWriteBuf[0] = 0x1B;
    cWriteBuf[1] = 0x5C;
    cWriteBuf[2] = nL;
    cWriteBuf[3] = nH;
    WriteToPort(cWriteBuf, 4);
	return 0;
}

/**********************************************************************
* �������ƣ� MW_SelectAlignMode
* ���������� ���ö��뷽ʽ
* ��������� n: 0    �����
                1    �м����
                2    �Ҷ��� 

* ��������� ��
* �� �� ֵ�� �ɹ����� 0 ��ʧ�ܷ��� -1
* ����˵���� 
***********************************************************************/
int CPrint::MW_SelectAlignMode(unsigned char n)
{
    unsigned char cWriteBuf[10];
    cWriteBuf[0] = 0x1B;
    cWriteBuf[1] = 0x61;
    cWriteBuf[2] = n;
    WriteToPort(cWriteBuf, 3);
	return 0;
}

/**********************************************************************
* �������ƣ� MW_DownloadBitmapToFlash
* ���������� ����λͼ�� Flash ��
* ��������� *pcBitmapAddr Ϊλͼ��·��
* ��������� ��
* �� �� ֵ�� �ɹ����� 0 ��ʧ�ܷ��� -1
* ����˵���� ��ӡ���Դ� 16K �Ĵ洢��
             ����ֻ֧�� 24 λ���λͼ���ҿ��߾�Ҫ�� 8 �ı�������
             λͼ������ Width <= 376, 
             λͼ������ 47 * Height < 16K
***********************************************************************/
int CPrint::MW_DownloadBitmapToFlash(char *pcBitmapAddr)
{

	FILE * pFile;  
    long lSize;  
    unsigned char * pucBuffer;  
    size_t result;  
      
    /* ��Ҫһ��char��©�ض��������ļ���ֻ�ܲ��ö����Ʒ�ʽ�� */   
    pFile = fopen (pcBitmapAddr, "rb" );  
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
	int iBMPBpp;        // bitmap λ��
	int iLineWidthAlign;// bitmap ÿ�ж������ֽ���
	int iLineWidthReal; // bitmap ÿ��ʵ���ֽ���
    int iFlashDataNum; // Flash ͼƬ���������ֽ���
	int iCurBlue;  // ��ǰ RGB �е���ɫֵ 
    int iCurFlashByte;  // ��ǰ Flash �ֽ�λ��
    int iColBytesNum;   // ÿ���ֽ���
	unsigned char ucaData[10] = {0};
	unsigned char *pucSrc; // ָ��ͼƬ����
    unsigned char * pucFlashData; // ָ�� Flash ͼƬ������

	// FS q m xL xH yL yH d1��dk   ���� Flash λͼ
	// k = (xL + xH * 256) * (yL + yH * 256) * 8 
	unsigned char escBmp[] = { 0x1C, 0x71, 0x01, 0x00, 0x00, 0x00, 0x00 };
    
	//xL, xH
    long tmpX = (ptBitmapH->biWidth + 7) / 8; 
	escBmp[3] = (unsigned char)(tmpX % 256);
	escBmp[4] = (unsigned char)(tmpX / 256);
	
    //yL, yH
    long tmpY = (ptBitmapH->biHeight + 7) / 8; 
    escBmp[5] = (unsigned char)(tmpY % 256);
	escBmp[6] = (unsigned char)(tmpY / 256);

    iFlashDataNum = (tmpX * tmpY * 8);
    pucFlashData = (unsigned char*) calloc (iFlashDataNum, sizeof(unsigned char));  

	iWidth = ptBitmapH->biWidth;
	iHeight = ptBitmapH->biHeight;
	iBMPBpp = ptBitmapH->biBitCount;
	
    // ĿǰͼƬֻ֧���У����� 8 �ı�����ͼƬ
    if ( ((iWidth % 8) != 0) || ((iHeight % 8) != 0))
    {
        fputs ("Bitmap iWidth or iHeight can not divide by 8 !",stderr); 
        return -1;
    }

	iLineWidthReal = iWidth * iBMPBpp / 8;
	iLineWidthAlign = (iLineWidthReal + 3) & ~0x3;   // �� 4 ȡ�� 
	
	

	pucSrc = pucBuffer + ptBitmap->bfOffBits; // ָ��ͼƬ����, ����Դ��ʼ�ĵط� 
    //pucSrc = pucSrc + (iHeight - 1) * iLineWidthAlign; 
    

    iColBytesNum = (iHeight + 7) / 8; 

    for (int x = 0; x < iWidth; x++) // �ڼ���
    {
        //for (int y = 0; y < iColBytesNum; y++) // ��ǰ�еڼ����ֽ�    δ�ߵ�
        for (int y = iColBytesNum; y > 0; y--) // ��ǰ�еڼ����ֽ�
        {
            // Flash �е�ǰ�ֽ� = x * (iHeight / 8 + 1) + y = ��ǰ�� * ÿ���ֽ��� + ��ǰ�����еڼ����ֽ�
            iCurFlashByte = x * iColBytesNum + y - 1;

            for (int z = 0; z < 8; z++) // ��ǰ�ֽ��ڵڼ�λ
            {
                // ��ǰ�ֽ�λ�õ���ɫ = ��Ӧ��ͼ���ֽ���ɫ
                
                // ��ǰ Flash ��λλ�� = z + y * 8 + x * (iHeight / 8 + 1) * 8 = ��ǰ�ֽ���λ�� + ǰ����λ�� + ǰ����λ��

                // ���� 4 �ֽڶ���������λ�� = ������ * �������п� + ������ * 3��24 λ�
                // ��ǰͼ���ж�Ӧ��ɫ�����ֽ�λ�� 
                //iCurBlue = (z + y * 8) * iLineWidthAlign + x * 3;             // δ�ߵ�
                iCurBlue = ((iHeight - 1) - ((7 - z) + (y -1) * 8)) * iLineWidthAlign + x * 3; 

                if (pucSrc[iCurBlue] != 0xFF ) // 0x00 0x00 0x00 ��ʾ��ɫ��0xFF 0xFF 0xFF ��ʾ��ɫ
                {
                    //pucFlashData[iCurFlashByte] |= (1 << (7 - z));            // δ�ߵ�
                    pucFlashData[iCurFlashByte] |= (1 << z);
                }
               

            }
        }
    }
    
    // printf("tmpX = %d tmpY = %d iFlashDataNum = %d\n",tmpX, tmpY, iFlashDataNum);

    WriteToPort(escBmp, 7);
    WriteToPort(pucFlashData, iFlashDataNum);
    Sleep(10000);

    free(pucFlashData);
	/////////////////////////////////////////////////////////////////////////
    /* ������ʾ���ر��ļ����ͷ��ڴ� */  
    fclose (pFile);  
    free (pucBuffer);  
    return 0; 
}
/**********************************************************************
* �������ƣ� MW_PrintFlashBitmap
* ���������� ��ӡ Flash �е�λͼ
* ��������� n Ϊ��ӡģʽ             (0 <= n <= 3)
                            n   ģʽ 
                            0   ����
                            1   ����
                            2   ����
                            3   �������� 
* ��������� ��
* �� �� ֵ�� �ɹ����� 0 ��ʧ�ܷ��� -1
* ����˵���� 
***********************************************************************/
int CPrint::MW_PrintFlashBitmap(unsigned char n)
{
    if (n < 0 || n > 3 )
    {
        fputs ("invalid argument!\n",stderr);
		return -1;
    }
    if (GetPrintStatus() != 0)
        return -1;
	unsigned char escBmp[] = { 0x1C, 0x71, 0x01, 0x00, 0x00, 0x00, 0x00 };
    escBmp[0] = 0x1C;
    escBmp[1] = 0x70;
    escBmp[2] = 0x01;
    escBmp[3] = 0x00;
    WriteToPort(escBmp, 4);
    return 0;

}

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
    if ((n < 1) || (n > 4))
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
	unsigned char escTmp[] = { 0x1B, 0x2A, 0x00, 0x00, 0x00 };


	/* ��ͬģʽ������ƣ�ÿ�й̶����ص�Ϊ 384
			0x00: 8 �㵥�ܶȣ�192 ���λͼ
			0x01: 8 ��˫�ܶȣ�384 ���λͼ 
			0x32: 24 �㵥�ܶȣ�192 ���λͼ 
			0x33: 24 ��˫�ܶȣ�384 ���λͼ 
	*/
	escTmp[2] = 0x21;
	
	//nL, nH
	escTmp[3] = (unsigned char)(ptBitmapH->biWidth % 256);
	escTmp[4] = (unsigned char)(ptBitmapH->biWidth / 256);
	
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
// 		WriteToPort(escTmp, 5);
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
		WriteToPort(escTmp, 5);
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
    if ((n < 0) || (n > 255))
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
    if ((ucMode < 0) || (ucMode > 255))
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
    nL = iPost % 256;
    nH = iPost / 256;

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
    if ((ucMode < 0) || (ucMode > 2) )
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
    if ((n < 0) || (n > 255))
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
    if ((n < 0) || (n > 1))
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
    if ((n < 0) || (n > 1))
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
int CPrint::MW_SetRefPrintPosition(unsigned int iPost)
{
    unsigned char nL,nH;
    nL = iPost % 256;
    nH = iPost / 256;
    
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
             ��ע����ͷ3�����һ��ص���ӡ��һ�У��������ס�
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
	unsigned char escTmp[] = { 0x1C, 0x71, 0x01, 0x00, 0x00, 0x00, 0x00 };
    
	//xL, xH
    long tmpX = (ptBitmapH->biWidth + 7) / 8; 
	escTmp[3] = (unsigned char)(tmpX % 256);
	escTmp[4] = (unsigned char)(tmpX / 256);
	
    //yL, yH
    long tmpY = (ptBitmapH->biHeight + 7) / 8; 
    escTmp[5] = (unsigned char)(tmpY % 256);
	escTmp[6] = (unsigned char)(tmpY / 256);

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
    
//     for(int i = 0; i < iFlashDataNum; i++)
//     {
//         printf("0x%02X ", pucFlashData[i]);
//         if ((i % 16) == 15)   // 0-7 8-15 
//         {
//             printf("\n");
//         }
//     }
//     printf("\n");


    printf("tmpX = %d tmpY = %d iFlashDataNum = %d\n",tmpX, tmpY, iFlashDataNum);

//     WriteToPort(escTmp, 7);
//     WriteToPort(pucFlashData, iFlashDataNum);
//     Sleep(10000);

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
    if ((n < 0) || (n > 3))
    {
        fputs ("invalid argument!\n",stderr);
		return -1;
    }
    if (GetPrintStatus() != 0)
        return -1;
	unsigned char escTmp[] = { 0x1C, 0x71, 0x01, 0x00, 0x00, 0x00, 0x00 };
    escTmp[0] = 0x1C;
    escTmp[1] = 0x70;
    escTmp[2] = 0x01;
    escTmp[3] = 0x00;
    WriteToPort(escTmp, 4);
    return 0;

}

/**********************************************************************
* �������ƣ� MW_selectFontSize
* ���������� ѡ�������С���� 0 �� 2 λѡ���ַ��߶ȣ�4 �� 7 λѡ���ַ����
* ��������� n �����С
                λ      ����
                0-3     �ַ��߶�ѡ��
                4-7     �ַ����ѡ�� 
             �߶ȡ���ȵķ�Χ���� 0-7����ʾ��������ʾ�ı�����ϵ 
    
* ��������� ��
* �� �� ֵ�� �ɹ����� 0 ��ʧ�ܷ��� -1
* ����˵���� 
***********************************************************************/
int CPrint::MW_SelectFontSize(unsigned char n)
{
    unsigned char nL,nH;
    nL = n % 16;
    nH = n / 16;
    if ((nL < 0) || (nL > 7) || (nH < 0) || (nH > 7) )
    {
        fputs ("invalid argument!\n",stderr);
        return -1;
    }

    unsigned char escTmp[10];
    escTmp[0] = 0x1D;
    escTmp[1] = 0x21;
    escTmp[2] = n;
    WriteToPort(escTmp, 3);
    return 0;    
}
/**********************************************************************
* �������ƣ�MW_SelectBlackAndWhiteReverse
* ����������ѡ��/ȡ���ڰ׷��Դ�ӡģʽ
* ���������n ���ڷ���ѡ��
              �� n �����λΪ 0 ʱ��ȡ�����Դ�ӡ
              �� n �����λΪ 1 ʱ��ѡ���Դ�ӡ
    
* ��������� ��
* �� �� ֵ�� �ɹ����� 0 ��ʧ�ܷ��� -1
* ����˵���� 
***********************************************************************/
int CPrint::MW_SelectBlackAndWhiteReverse(unsigned char n)
{
    unsigned char escTmp[10];
    escTmp[0] = 0x1D;
    escTmp[1] = 0x42;
    escTmp[2] = n;
    WriteToPort(escTmp, 3);
    return 0; 
}

/**********************************************************************
* �������ƣ�MW_SetHriLocation
* �������������������ַ���ӡλ��
* ���������n ����λ��ѡ��
                0       ����ӡ
                1       �����Ϸ�
                2       �����·�
                3       �����ϡ��·�����ӡ 
    
* ��������� ��
* �� �� ֵ�� �ɹ����� 0 ��ʧ�ܷ��� -1
* ����˵���� HRI �ַ��Ƕ���������ע�͵��ַ�
***********************************************************************/
int CPrint::MW_SetHriLocation(unsigned char n)
{
    if ((n < 0) || (n > 3))
    {
        fputs ("invalid argument!\n",stderr);
        return -1;
    }
    unsigned char escTmp[10];
    escTmp[0] = 0x1D;
    escTmp[1] = 0x48;
    escTmp[2] = n;
    WriteToPort(escTmp, 3);
    return 0; 
}

/**********************************************************************
* �������ƣ�MW_SetPrintLeftMargin
* �������������ô�ӡ��߾�
* ���������n Ϊ��߾���룬��λΪӢ��    
* ��������� ��
* �� �� ֵ�� �ɹ����� 0 ��ʧ�ܷ��� -1
* ����˵���� 
***********************************************************************/
int CPrint::MW_SetPrintLeftMargin(unsigned int n)
{
    unsigned char nL,nH;
    nL = n % 256;
    nH = n / 256;
    unsigned char escTmp[10];
    escTmp[0] = 0x1D;
    escTmp[1] = 0x48;
    escTmp[2] = nL;
    escTmp[3] = nH;
    WriteToPort(escTmp, 4);
    return 0; 


}


/**********************************************************************
* �������ƣ� MW_SetPrintWidth
* ���������� ���ô�ӡ���
* ��������� n Ϊ��ӡ��ȣ���λΪӢ�� 
* ��������� ��
* �� �� ֵ�� �ɹ����� 0 ��ʧ�ܷ��� -1
* ����˵���� 
***********************************************************************/
int CPrint::MW_SetPrintWidth(unsigned int n)
{
    unsigned char nL,nH;
    nL = n % 256;
    nH = n / 256;
    unsigned char escTmp[10];
    escTmp[0] = 0x1D;
    escTmp[1] = 0x57;
    escTmp[2] = nL;
    escTmp[3] = nH;
    WriteToPort(escTmp, 4);
    return 0; 
   
}

/**********************************************************************
* �������ƣ� MW_SetHriFont
* ���������� �������� HRI ����
* ��������� n ��������ѡ��
                0   ��׼ ASCII ���ַ�(12x24)
                1   ѹ�� ASCII ���ַ�(9x17)
* ��������� ��
* �� �� ֵ�� �ɹ����� 0 ��ʧ�ܷ��� -1
* ����˵���� HRI �ַ��Ƕ���������ע�͵��ַ�
***********************************************************************/
int CPrint::MW_SetHriFont(unsigned char n)
{
    if ((n < 0) || (n > 1))
    {
        fputs ("invalid argument!\n",stderr);
        return -1;
    }
    unsigned char escTmp[10];
    escTmp[0] = 0x1D;
    escTmp[1] = 0x66;
    escTmp[2] = n;
    WriteToPort(escTmp, 3);
    return 0; 
    
}

/**********************************************************************
* �������ƣ� MW_SetBarcodeHeight
* ���������� ��������߶�
* ��������� n Ϊ����߶ȣ���λ�㡾Ĭ��ֵ 162��
* ��������� ��
* �� �� ֵ�� �ɹ����� 0 ��ʧ�ܷ��� -1
* ����˵���� 
***********************************************************************/
int CPrint::MW_SetBarcodeHeight(unsigned char n)
{
    if ((n < 0) || (n > 255))
    {
        fputs ("invalid argument!\n",stderr);
        return -1;
    }
    unsigned char escTmp[10];
    escTmp[0] = 0x1D;
    escTmp[1] = 0x68;
    escTmp[2] = n;
    WriteToPort(escTmp, 3);
    return 0; 
    
}

/**********************************************************************
* �������ƣ� MW_PrintBarcode
* ���������� ��ӡ����
* ��������� ucMode Ҫ��ӡ�������ʽ��*pcDataָ����������
* ��������� ��
* �� �� ֵ�� �ɹ����� 0 ��ʧ�ܷ��� -1
* ����˵���� 
---------------------------------------------------------------------------------------
| m(ʮ����)   | ��������       | �ַ�����    | �ַ�                                   |
---------------------------------------------------------------------------------------
| ���ݸ�ʽһ��1D 6B m xx xx 00                                                        |
---------------------------------------------------------------------------------------
| 0(0x00)     | UPC-A          | 11          | 0-9                                    |
---------------------------------------------------------------------------------------
| 1(0x01)     | UPC-E          | 6           | 0-9                                    |
---------------------------------------------------------------------------------------
| 2(0x02)     | JAN13/EAN13    | 12          | 0-9                                    |
---------------------------------------------------------------------------------------
| 3(0x03)     | JAN8/EAN8      | 7           | 0-9                                    |
---------------------------------------------------------------------------------------
| 4(0x04)     | CODE39         | 1-255       | 0-9 A-Z SP,$,%,+,-,.,/, *(��ʼ�����ַ�)|
---------------------------------------------------------------------------------------
| 5(0x05)     | ITF            | 1-255(ż��) | 0-9                                    |
---------------------------------------------------------------------------------------
| 6(0x06)     | CODEBAR        | 1-255       | 0-9 A-D $,+,-,.,/,                     |
---------------------------------------------------------------------------------------
| ���ݸ�ʽ����1D 6B m n xx xx                                                         |
---------------------------------------------------------------------------------------
| 65(0x41)    | UPC-A          | 11          | 0-9                                    |
---------------------------------------------------------------------------------------
| 66(0x42)    | UPC-E          | 6           | 0-9                                    |
---------------------------------------------------------------------------------------
| 67(0x43)    | JAN13/EAN13    | 12          | 0-9                                    |
---------------------------------------------------------------------------------------
| 68(0x44)    | JAN8/EAN8      | 7           | 0-9                                    |
---------------------------------------------------------------------------------------
| 69(0x45)    | CODE39         | 1-255       | 0-9 A-Z SP,$,%,+,-,.,/, *(��ʼ�����ַ�)|
---------------------------------------------------------------------------------------
| 70(0x46)    | ITF            | 1-255(ż��) | 0-9                                    |
---------------------------------------------------------------------------------------
| 71(0x47)    | CODEBAR        | 1-255       | 0-9 A-D $,+,-,.,/,                     |
---------------------------------------------------------------------------------------
| 72(0x48)    | CODE93         | 1-255       | NUL-SP(7FH)                            |
---------------------------------------------------------------------------------------
| 73(0x49)    | CODE128        | 2-255       | NUL-SP(7FH)                            |
---------------------------------------------------------------------------------------
***********************************************************************/
int CPrint::MW_PrintBarcode(unsigned char ucMode, char *pcData)
{

    /*
     Code39: 0123456789
        1D 68 78        // ѡ������߶�
        1D 77 02        // ����������
        1D 48 02        // ѡ�� HRI �ַ��Ĵ�ӡλ�ã������·�
        0A 
        1D 6B 45 0A 30 31 32 33 34 35 36 37 38 39 0A 

     Code128:0123456789
        1D 68 78
        1D 77 02 
        1D 48 02 
        0a 
        1D 6B 49 0C 7B 41 30 31 32 33 34 35 36 37 38 39 0A

    //////////////////////////////////////////////////////////
    // �������
        1D 6B 00 30 31 32 33 34 35 36 37 38 39 39 00
    */
    unsigned char escTmp[270];
    int iStrLength = strlen(pcData);

    if (GetPrintStatus() != 0)
        return -1;

    if ((ucMode < 0) || ((ucMode > 6) && (ucMode < 65 )) || (ucMode > 73 ))
    {
        fputs ("invalid argument!\n",stderr);
        return -1;
    }
    if (iStrLength > 255)
    {
        fputs ("invalid string length > 255!\n",stderr);
        return -1;
    }
    //memcpy(ucData, pcData, sizeof(pcData));


    escTmp[0] = 0x1D;
    escTmp[1] = 0x6B;


    switch(ucMode)
    {
    case 0:
        strcpy((char *)escTmp+3, pcData);
        escTmp[2] = 0x00;
        WriteToPort(escTmp, 4+iStrLength);
        MW_LF();
        break;
    case 1:
        strcpy((char *)escTmp+3, pcData);
        escTmp[2] = 0x01;
        WriteToPort(escTmp, 4+iStrLength);
        MW_LF();
        break;
    case 2:
        strcpy((char *)escTmp+3, pcData);
        escTmp[2] = 0x02;
        WriteToPort(escTmp, 4+iStrLength);
        MW_LF();
        break;
    case 3:
        strcpy((char *)escTmp+3, pcData);
        escTmp[2] = 0x03;
        WriteToPort(escTmp, 4+iStrLength);
        MW_LF();
        break;
    case 4:
        strcpy((char *)escTmp+3, pcData);
        escTmp[2] = 0x04;
        WriteToPort(escTmp, 4+iStrLength);
        MW_LF();
        break;
    case 5:
        strcpy((char *)escTmp+3, pcData);
        escTmp[2] = 0x05;
        WriteToPort(escTmp, 4+iStrLength);
        MW_LF();
        break;
    case 6:
        strcpy((char *)escTmp+3, pcData);
        escTmp[2] = 0x06;
        WriteToPort(escTmp, 4+iStrLength);
        MW_LF();
        break;
    // �ڶ��ָ�ʽ�������ȷ���
    case 65:
        strcpy((char *)escTmp+4, pcData);
        escTmp[2] = 0x41;
        escTmp[3] = iStrLength;
        WriteToPort(escTmp, 4+iStrLength);
        MW_LF();
        break;
    case 66:
        strcpy((char *)escTmp+4, pcData);
        escTmp[2] = 0x42;
        escTmp[3] = iStrLength;
        WriteToPort(escTmp, 4+iStrLength);
        MW_LF();
        break;
    case 67:
        strcpy((char *)escTmp+4, pcData);
        escTmp[2] = 0x43;
        escTmp[3] = iStrLength;
        WriteToPort(escTmp, 4+iStrLength);
        MW_LF();
        break;
    case 68:
        strcpy((char *)escTmp+4, pcData);
        escTmp[2] = 0x44;
        escTmp[3] = iStrLength;
        WriteToPort(escTmp, 4+iStrLength);
        MW_LF();
        break;
    case 69:
        strcpy((char *)escTmp+4, pcData);
        escTmp[2] = 0x45;
        escTmp[3] = iStrLength;
        WriteToPort(escTmp, 4+iStrLength);
        MW_LF();
        break;
    case 70:
        strcpy((char *)escTmp+4, pcData);
        escTmp[2] = 0x46;
        escTmp[3] = iStrLength;
        WriteToPort(escTmp, 4+iStrLength);
        MW_LF();
        break;
    case 71:
        strcpy((char *)escTmp+4, pcData);
        escTmp[2] = 0x47;
        escTmp[3] = iStrLength;
        WriteToPort(escTmp, 4+iStrLength);
        MW_LF();
        break;
    case 72:
        strcpy((char *)escTmp+4, pcData);
        escTmp[2] = 0x48;
        escTmp[3] = iStrLength;
        WriteToPort(escTmp, 4+iStrLength);
        MW_LF();
        break;
    case 73:
        strcpy((char *)escTmp+4, pcData);
        escTmp[2] = 0x49;
        escTmp[3] = iStrLength;
        WriteToPort(escTmp, 4+iStrLength);
        MW_LF();
        break;
    default:
        fputs ("invalid argument!\n",stderr);
        return -1;
    }

    return 0; 
    
}

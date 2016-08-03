// Print.cpp: implementation of the CPrint class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Print.h"




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
* ��������� // �����������˵����
* �� �� ֵ�� 0 ����������ʱ���س�����, -1 ��ʾ��������ȷ
* ����˵���� // ����˵��
***********************************************************************/
int CPrint::MW_RealTimeStatus(int n)
{
	unsigned char cWriteBuf[10];
	if((n > 4)||(n < 1))
		return -1;

	cWriteBuf[0] = 0x10;
	cWriteBuf[1] = 0x04;
	cWriteBuf[2] = n;
	WriteToPort(cWriteBuf, 3);
	return 0;
}
// typedef struct tagBITMAPFILEHEADER 
// {
//     WORD bfType; 
//     DWORD bfSize; 
//     WORD bfReserved1; 
//     WORD bfReserved2; 
//     DWORD bfOffBits; 
// } BITMAPFILEHEADER; 

// typedef struct tagBITMAPINFOHEADER{ 
//     DWORD biSize; 
//     LONG biWidth; 
//     LONG biHeight; 
//     WORD biPlanes; 
//     WORD biBitCount DWORD biCompression; 
//     DWORD biSizeImage; 
//     LONG biXPelsPerMeter; 
//     LONG biYPelsPerMeter; 
//     DWORD biClrUsed; 
//     DWORD biClrImportant; 
// } BITMAPINFOHEADER; 
void CPrint::MW_PrintBitmap(const unsigned char *pcPicAddr)
{
 
	FILE * pFile;  
    long lSize;  
    unsigned char * pucBuffer;  
    size_t result;  
      
    /* ��Ҫһ��char��©�ض��������ļ���ֻ�ܲ��ö����Ʒ�ʽ�� */   
    pFile = fopen ("D:\\VC_Project\\Demo\\Bitmap.bmp", "rb" );  
    if (pFile==NULL)  
    {  
        fputs ("File error",stderr);  
        //exit (1);  
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
        //exit (2);  
    }  
	
    /* ���ļ�������buffer�� */  
    result = fread (pucBuffer,1,lSize,pFile);  
    if (result != (size_t)lSize)  
    {  
        fputs ("Reading error",stderr);  
        //exit (3);  
    }  
    /* ���������ļ��Ѿ���buffer�У����ɱ�׼�����ӡ���� */  
// 	for(int i = 0; i < 100; i++)
// 	{
// 		if (i % 10 == 0)
// 		{
// 			printf("\n");
// 		}
// 		printf("%X ", pucBuffer[i]); 
// 	}
// 	printf("\n");
//  	printf("lSize = %d  0x%X\n", lSize, lSize);
	
	/////////////////////////////////////////////////////////////////////////
	BITMAPFILEHEADER *ptBitmap = (BITMAPFILEHEADER *)pucBuffer;
	BITMAPINFOHEADER *ptBitmapH = (BITMAPINFOHEADER *)&pucBuffer[14];







	//�����ַ��м��Ϊ 0 ����
//     unsigned char data[] = { 0x1B, 0x33, 0x00 };
// 	WriteToPort(data, sizeof(data));

	//ESC * m nL nH d1��dk   ѡ��λͼģʽ
	// ESC * m nL nH
	unsigned char escBmp[] = { 0x1B, 0x2A, 0x00, 0x00, 0x00 };

	////////////////////////
	// for test
	unsigned char ucaBitData[10];
	/*
	// 8 �㵥�ܶ�, ���ɴ� 48 ������192 �У���ʾ��ͼ�㣬һ��Ӧ���� 384 
	escBmp[2] = 0x00;
	escBmp[3] = 0xC0;
	escBmp[4] = 0x00;

	WriteToPort(escBmp, 5);
	ucaBitData[0] = 0x81;
	ucaBitData[1] = 0x42;
	ucaBitData[2] = 0x24;
	ucaBitData[3] = 0x18;
	
	for(int i = 0; i < 48; i++)
	{
		WriteToPort(ucaBitData, 4);
	}
	MW_LF();
	*/

	/*
	// 8 ��˫�ܶȣ��ɴ�ӡ 38 ��������Ӧ��Ҳ�� 384
	escBmp[2] = 0x01;
	escBmp[3] = 0x80;
	escBmp[4] = 0x01;
	WriteToPort(escBmp, 5);
	ucaBitData[0] = 0x80;
	ucaBitData[1] = 0x40;
	ucaBitData[2] = 0x20;
	ucaBitData[3] = 0x10;
	ucaBitData[4] = 0x08;
	ucaBitData[5] = 0x04;
	ucaBitData[6] = 0x02;
	ucaBitData[7] = 0x01;
	ucaBitData[8] = 0x18;
	ucaBitData[9] = 0x24;
	for(int i = 0; i < 38; i++)
	{
		WriteToPort(ucaBitData, 10);
	}
	MW_LF();
	*/
	
	
	// 24 ��˫�ܶ�
	////////////////////////
	escBmp[2] = 0x21; // 0x21
	escBmp[3] = 0x02;
	escBmp[4] = 0x00;
	WriteToPort(escBmp, 5);
	ucaBitData[0] = 0x00;
	ucaBitData[1] = 0xFF;
	ucaBitData[2] = 0x00;

// 	ucaBitData[1] = 0x00;
// 	ucaBitData[4] = 0x00;
// 	ucaBitData[7] = 0x00;
// 
// 	ucaBitData[2] = 0x00;
// 	ucaBitData[5] = 0x00;
// 	ucaBitData[8] = 0x00;

	for(int i = 0; i < 2; i++)
	{
		WriteToPort(ucaBitData, 3);
	}
	MW_LF();
	



	/* ��ͬģʽ������ƣ�ÿ�й̶����ص�Ϊ 384
			0x00: 8 �㵥�ܶȣ�192 ���λͼ
			0x01: 8 ��˫�ܶȣ�384 ���λͼ 
			0x32: 24 �㵥�ܶȣ�192 ���λͼ 
			0x33: 24 ��˫�ܶȣ�384 ���λͼ 
	*/
// 	escBmp[2] = 0x33;
// 	
// 	//nL, nH
// 	escBmp[3] = (unsigned char)(ptBitmapH->biWidth % 256);
// 	escBmp[4] = (unsigned char)(ptBitmapH->biWidth / 256);
// 	
// 
// 
// 
// 	int iWidth;
// 	int iHeight;
// 	int iBMPBpp;
// 	int iLineWidthAlign;
// 	int iLineWidthReal;
// 	int iCurPiexl;
// 	int iCurBlue;
// 
// 	unsigned char ucaData[10] = {0};
// 	unsigned char *pucSrc; // ָ��ͼƬ����
// 
// 	iWidth = ptBitmapH->biWidth;
// 	iHeight = ptBitmapH->biHeight;
// 	iBMPBpp = ptBitmapH->biBitCount;
// 	
// 	iLineWidthReal = iWidth * iBMPBpp / 8;
// 	iLineWidthAlign = (iLineWidthReal + 3) & ~0x3;   // �� 4 ȡ�� 
// 	
// 	
// 
// 	pucSrc = pucBuffer + ptBitmap->bfOffBits;
// 	//pucSrc = pucSrc + (iHeight - 1) * iLineWidthAlign;  // ָ�����һ�� 
// 	for ( i = 0; i < 16; i++)
// 	{
// 		printf("%02d ", i); 
// 	}
// 	for(i = 0; i < 96; i++)
// 	{
// 		if (i % 16 == 0)
// 		{
// 			printf("\n");
// 		}
// 		printf("%02X ", pucSrc[i]); 
// 	}
// 	printf("\n");

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

// 	for (int y = 0; y < (iHeight / 24 + 1); y++)// ѭ���飬ÿ 8 ��һ�� + 1 �������һ��
// 	{	
// 		WriteToPort(escBmp, 5);
// 		for (int x = 0; x < iWidth; x++)// ѭ���У�ÿ��һ�ֽ�
// 		{
// 			ucaData[0] = 0x00;
// 			ucaData[1] = 0x00;
// 			ucaData[2] = 0x00;
// 			for (int z = 0; z < 24; z++) // ѭ��ÿ���ֽ�λ
// 			{
// 				//��ǰ���ص���(�� 0 ��ʼ) = �ڼ�λ * ÿ�������� + ���е�������� + �ڼ��� * ������ * ÿ�������� =(z * iWidth + x + y * 8 * iWidth);
// 				iCurPiexl = (z * iWidth + x + y * 24 * iWidth);
// 
// 				// �ж��ǻ��ڵ�ǰλͼ�� 
// 				if (iCurPiexl < iWidth * iHeight)
// 				{
// 					// iCurPiexl / iWidth = ������ 
// 					// iCurPiexl % iWidth = ������ = x
// 					// ���� 4 �ֽڶ���������λ�� = ������ * �������п� + ������ * 3��24 λ�
// 					iCurBlue = iCurPiexl / iWidth * iLineWidthAlign + ( (iWidth - x - 1) * 3); // (iWidth - x - 1) �����ݵߵ�
// 					if (pucSrc[iCurBlue] != 0xFF ) // 0x00 0x00 0x00 ��ʾ��ɫ��0xFF 0xFF 0xFF ��ʾ��ɫ
// 					{
// 						if ( z < 8)
// 						{
// 							ucaData[0] |= (1 << (7-z));
// 						}
// 						else
// 						if ( (z >= 8) && (z < 16) )
// 						{
// 							ucaData[1] |= (1 << (7-z));
// 						}
// 						else
// 						if((z >= 16) && (z < 24))
// 						{
// 							ucaData[2] |= (1 << (7-z));
// 						}
// 						
// 					}
// 				}	
// 			}
// 			WriteToPort(ucaData, 3);
// 			printf("0x%X ", ucaData[0]);
// 			printf("0x%X ", ucaData[1]);
// 			printf("0x%X ", ucaData[2]);
// 			printf("\n");
// 		}
// 		MW_LF();
// 	}


	/////////////////////////////////////////////////////////////////////////








    /* ������ʾ���ر��ļ����ͷ��ڴ� */  
    fclose (pFile);  
    free (pucBuffer);  
   // return 0; 
	

}

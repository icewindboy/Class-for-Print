// Demo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "SerialPort.h"
#include "Print.h"

CPrint * g_clsPrint = new CPrint();


char * pString = "-------------------------------\n"
				"�����ܶ                11.00\n"
				"���   �ֽ� �����     101.00\n"
				"���㣺   �ֽ� �����      90.00\n"
				"   �۳���Ʒ������4��\n"
				"      2005-09-13  16:50:19\n"
				"       ��ӭ����   ��л�ݹ�\n"
				"        ������Ʊ������Ч��\n"
				"       ���Ҹ��ٻ����ʵ�\n"
				"СƱ��270500027721 ����Ա��01012\n"
				"-------------------------------\n";
char *pString2 = "1234567891011121314151617181920\n";
char *pString3 = "123456789\n";
int main(int argc, char* argv[])
{

	g_clsPrint->setPortName(2);
	g_clsPrint->setBaud(115200);
	g_clsPrint->InitPort();
//////////////////////////////////////////////////////////
//�����Ǵ�ӡ���Ĺ��ܲ���
// 	//1. ��ӡλͼ
//	g_clsPrint->MW_PrintBitmap("D:\\VC_Project\\Demo\\��è.bmp");
	
// 	// 2. ʵʱ״̬���͡�1 ���������á�
// 	unsigned char err;
// 	for(int i = 1; i < 5; i++)
// 	{	
// 		err = 0;
// 		err = g_clsPrint->MW_RealTimeStatus(i);
// 		printf("i = %d err = 0x%X\n", i, err);
// 	}

//     // 3. �����ַ��Ҽ�࣬�� A B C ��� A    B   C 
//      g_clsPrint->MW_SetCharRightSpace(10);
//     
//     // 4. ��ӡ�ַ���
//     unsigned char err;
//     err = g_clsPrint->MW_PrintString(pString2);
//     printf("err = %d\n", err);

//     // 5. ��ӡģʽѡ��
//     unsigned char ucMode = 0;
// 
//     // (1 << 7) �»���  : ûʲôЧ��, �ܵ�
//     // (1 << 5) ���� 
//     // (1 << 4) ���� 
//     // (1 << 3) �Ӵ�   
//     ucMode |= (1 << 3);
//     g_clsPrint->MW_SelectPrintMode(ucMode);                                         
//     g_clsPrint->MW_PrintString(pString2);

//     // 6. ���þ��Դ�ӡλ�� 
//     g_clsPrint->MW_SetAbsPrintPosition(65500);
//     g_clsPrint->MW_PrintString(pString2);

//     // 7. ѡ��/ȡ���»���ģʽ
//     unsigned char ucMode = 2;
//     // 0,48            ȡ���»���ģʽ
//     // 1,49            ѡ���»���ģʽ(1 ���)
//     // 2,50            ѡ���»���ģʽ(2 ���)
//     g_clsPrint->MW_SetUndlineMode(ucMode);
//     ucMode = 0;
//     ucMode |= (1 << 7)|(1 << 3); // (1 << 7) �»��� 
//     g_clsPrint->MW_SelectPrintMode(ucMode); 
//     g_clsPrint->MW_PrintString(pString2);

//     // 8. �����м�� 
//     g_clsPrint->MW_PrintString(pString2);
//     g_clsPrint->MW_SetLineSpace(200);
//     g_clsPrint->MW_PrintString(pString2);
//     g_clsPrint->MW_SetDefaultLineSpace();
//     g_clsPrint->MW_PrintString(pString2);
    
//     // 9. ѡ����뷽ʽ
//     g_clsPrint->MW_SelectAlignMode(1);
//     g_clsPrint->MW_PrintString(pString3);
// 
//     g_clsPrint->MW_SelectAlignMode(2);
//     g_clsPrint->MW_PrintString(pString3);
// 
//     g_clsPrint->MW_SelectAlignMode(0);
//     g_clsPrint->MW_PrintString(pString3);

    // 10�� ����/��ӡ Flash �е�λͼ 
    g_clsPrint->MW_DownloadBitmapToFlash("D:\\VC_Project\\Demo\\Bitmap_�й�.bmp");

//////////////////////////////////////////////////////////
	g_clsPrint->ClosePort();



	return 0;
}

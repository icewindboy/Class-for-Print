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
char *pString2 = "123456789101112131415161718192021222324252627282930\n";

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

//     // 3. �����ַ��Ҽ��
//  	g_clsPrint->MW_SetCharRightSpace(10);
//     
    // 4. ��ӡ�ַ���
    unsigned char err;
    err = g_clsPrint->MW_PrintString(pString2);
    printf("err = %d\n", err);



//////////////////////////////////////////////////////////
	g_clsPrint->ClosePort();



	return 0;
}

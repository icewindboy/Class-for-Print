// Demo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "SerialPort.h"
#include "Print.h"

CPrint * g_clsPrint = new CPrint();


int main(int argc, char* argv[])
{

	g_clsPrint->setPortName(2);
	g_clsPrint->setBaud(115200);
	g_clsPrint->InitPort();
	//һ���Ǵ�ӡ���Ĺ���
 	//char cWriteBuf[10];
// 	cWriteBuf[0] = 0x0A;
// 	for(int i = 0; i < 2; i++)//ѭ������  ��ӡ�յ�5��
// 	{
// 		//g_clsPrint->WriteToPort("Hello!", 6);
// 		g_clsPrint->MW_LF(); //���ʹ�ӡ����
// 	}

	//err = g_clsPrint->MW_RealTimeStatus(4);


	g_clsPrint->MW_PrintBitmap((unsigned char*)"Hello");
	
	for( int i = 0; i < 400000; i++);

	g_clsPrint->ClosePort();



	return 0;
}

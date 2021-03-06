
#include "stdafx.h"
#include "Global.h"
#include "RdtSender.h"
#include "RdtReceiver.h"
#include "GBNRdtSender.h"
#include "GBNReceiver.h"
#include "SRReceiver.h"
#include"SRSender.h"
#include"TCPReceiver.h"
#include"TCPSender.h"
int main(int argc, char** argv[])
{
	RdtSender *ps = new TCPSender;
	RdtReceiver * pr = new TCPReceiver;
	pns->init();
	pns->setRtdSender(ps);
	pns->setRtdReceiver(pr);
	pns->setInputFile("C:\\Users\\KatowiZz\\Desktop\\CN\\GBN\\GBN\\Debug\\input.txt");
	pns->setOutputFile("C:\\Users\\KatowiZz\\Desktop\\CN\\GBN\\GBN\\Debug\\output.txt");
	pns->start();

	delete ps;
	delete pr;
	delete pUtils;									//指向唯一的工具类实例，只在main函数结束前delete
	delete pns;										//指向唯一的模拟网络环境类实例，只在main函数结束前delete
	//getchar();
	
	return 0;
}
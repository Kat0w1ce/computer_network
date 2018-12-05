
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
	delete pUtils;									//ָ��Ψһ�Ĺ�����ʵ����ֻ��main��������ǰdelete
	delete pns;										//ָ��Ψһ��ģ�����绷����ʵ����ֻ��main��������ǰdelete
	//getchar();
	
	return 0;
}
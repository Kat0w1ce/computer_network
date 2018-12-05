#include"stdafx.h"
#include"GBNRdtSender.h"
#include"Global.h"

bool GbnSender::send(Message& message)
{
	if (getWaitingState())
		return false;
	Packet* newPacket = new Packet;
	newPacket->acknum = -1;
	newPacket->seqnum = base + nextseqnum;
	newPacket->checksum = 0;
	memcpy(newPacket->payload, message.data, sizeof(message.data));
	newPacket->checksum = pUtils->calculateCheckSum(*newPacket);
	pUtils->printPacket("���ͷ����ͱ���", *newPacket);
	pns->startTimer(SENDER, Configuration::TIME_OUT, newPacket->seqnum);
	pns->sendToNetworkLayer(RECEIVER, *newPacket);
	nextseqnum++;
	senderbuffer.push_back(*newPacket);
	//cout << base<<"  "<<nextseqnum << endl;
	delete newPacket;
	return true;
	
}

void GbnSender::receive(Packet &ackPkt)
{
	//auto p = senderbuffer.begin();
	//cout << (pUtils->calculateCheckSum(ackPkt))<<"  "<<ackPkt.checksum << endl;
	//pUtils->printPacket("���յı���", ackPkt);
	  //todo
	//{
	//cout << senderbuffer.size() << endl;
	ofstream of;
	of.open("GBN_window.txt", ios::app);
	int checkSum = pUtils->calculateCheckSum(ackPkt);
		//cout << "checksum" << checkSum << endl;
		auto fpacket = senderbuffer.begin();
		if (!senderbuffer.empty()&&checkSum == ackPkt.checksum && (*fpacket).seqnum<=ackPkt.seqnum)
		{
			cout << "ack receiverd" << endl;
			int cnt = 0;
			//base = 1 + senderbuffer.begin()->seqnum;
			//if(!senderbuffer.empty())
			of << "���ͷ����ڻ���:" << senderbuffer.begin()->seqnum;
			while (!senderbuffer.empty()&&senderbuffer.begin()->seqnum<=ackPkt.seqnum)
			{
				pUtils->printPacket("���ͷ�����ȷ��", *senderbuffer.begin());
				pns->stopTimer(SENDER, senderbuffer.begin()->seqnum);
				cnt++;
				//getchar();
				senderbuffer.pop_front();
			}
			of << "��������:  " << cnt << endl;
		}
		else
		{
			pUtils->printPacket("���ͷ�ackУ�����", ackPkt);
			/*for (; fpacket != senderbuffer.end(); ++fpacket)
			{
				pUtils->printPacket("���ͷ�û����ȷ�յ�ȷ�ϣ��ش�", *fpacket);
				pns->stopTimer(SENDER, fpacket->seqnum);
				pns->startTimer(SENDER, Configuration::TIME_OUT, fpacket->seqnum);
				pns->sendToNetworkLayer(RECEIVER, *fpacket);
			}*/
		}
		of.close();
}

void GbnSender::timeoutHandler(int seqnum)
{

	for (auto pp = senderbuffer.begin(); pp != senderbuffer.end(); ++pp)
	{
		pUtils->printPacket("��ʱ�ش�", *pp);
		pns->stopTimer(SENDER, pp->seqnum);
		pns->startTimer(SENDER, Configuration::TIME_OUT, pp->seqnum);
		pns->sendToNetworkLayer(RECEIVER, *pp);
	}
}
bool GbnSender::getWaitingState()
{
	return senderbuffer.size() == max;
}
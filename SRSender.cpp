#include"SRSender.h"
#include<cstring>
#include"Global.h"

bool SRSender::getWaitingState()
{
	return sendbuffer.size() >= max;
}


bool SRSender::send(Message & message)
{
	if (getWaitingState())
		return false;
	Packet * newPkt = new Packet;
	newPkt->acknum = -1;
	newPkt->seqnum = nextseqnum;
	memcpy(newPkt->payload, message.data, sizeof(message.data));
	newPkt->checksum = 0;
	newPkt->checksum = pUtils->calculateCheckSum(*newPkt);
	pUtils->printPacket("���ͷ����ͱ���", *newPkt);
	//sendbuffer.insert(make_pair(nextseqnum, *newPkt));
	sendbuffer.push_back(*newPkt);
	pns->startTimer(SENDER, Configuration::TIME_OUT, nextseqnum);
	pns->sendToNetworkLayer(RECEIVER, *newPkt);
	nextseqnum++;
	delete newPkt;
	return true;
}




void SRSender::timeoutHandler(int seqnum)
{
	auto ip = sendbuffer.begin();
	for (; ip != sendbuffer.end(); ++ip)
		if (ip->seqnum == seqnum)
			break;
	if (ip == sendbuffer.end())
		return;
	pUtils->printPacket("��ʱ�ش�", *ip);
	pns->stopTimer(SENDER, seqnum);
	pns->startTimer(SENDER, Configuration::TIME_OUT, seqnum);
	pns->sendToNetworkLayer(RECEIVER, *ip);
}



void SRSender::receive(Packet &ackPkt)
{
	ofstream of;
	of.open("SR_Send.txt", ios::app);
	int checkSum = pUtils->calculateCheckSum(ackPkt);
	cout << checkSum << "  " << ackPkt.checksum << endl;
	if (checkSum == ackPkt.checksum&&ackPkt.seqnum>=send_base)
	{
		cout << "send recvive" << endl;
		auto ip = sendbuffer.begin();
		for (; ip != sendbuffer.end(); ++ip)
			if (ip->seqnum == ackPkt.seqnum)
				break;
		if (ip == sendbuffer.end())
			return;
		if(ip->acknum==-1)
			ip->acknum = ackPkt.seqnum;
	}
	of << "SR���ͷ����ڻ���:  " << send_base;
	int cnt = 0;
	while (!sendbuffer.empty()&&sendbuffer.begin()->acknum!=-1)
	{
		pUtils->printPacket("���ͷ��յ�ȷ��", *sendbuffer.begin());
		pns->stopTimer(SENDER, sendbuffer.begin()->seqnum);
		cnt++;
		send_base++;
		sendbuffer.pop_front();
	}
	of << "  ��������:  " << cnt<<endl;
	of.close();
}




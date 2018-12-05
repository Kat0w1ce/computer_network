#include"stdafx.h"
#include"TCPSender.h"
#include"Global.h"

bool TCPSender::send(Message& message)
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

void TCPSender::receive(Packet &ackPkt)
{
	ofstream of,of_2;
	of.open("fast_retransmit.txt", ios::app);
	of_2.open("TCPSend.txt", ios::app);
	//auto p = senderbuffer.begin();
	//cout << (pUtils->calculateCheckSum(ackPkt))<<"  "<<ackPkt.checksum << endl;
	//pUtils->printPacket("���յı���", ackPkt);
	  //todo
	//{
	//cout << senderbuffer.size() << endl;
	int checkSum = pUtils->calculateCheckSum(ackPkt);
	//cout << "checksum" << checkSum << endl;
	auto fpacket = senderbuffer.begin();
	if (!senderbuffer.empty() && checkSum == ackPkt.checksum && (*fpacket).seqnum <= ackPkt.seqnum)
	{
		cout << "ack receiverd" << endl;
		//base = 1 + senderbuffer.begin()->seqnum;
		//if(!senderbuffer.empty())
		if (ackPkt.seqnum == first_ACK)
			cnt++;
		of_2 << "������" << first_ACK;
		while (!senderbuffer.empty() && senderbuffer.begin()->seqnum <= ackPkt.seqnum)
		{
			pUtils->printPacket("���ͷ�����ȷ��", *senderbuffer.begin());
			cout << "���ͷ����ڻ���" << senderbuffer.begin()->seqnum << endl;
			pns->stopTimer(SENDER, senderbuffer.begin()->seqnum);
			first_ACK++;
			cnt++;
			senderbuffer.pop_front();

		}
		of_2 << "������" << first_ACK << endl;
		if (!senderbuffer.empty()&&cnt >= 3)
		{
			
			//pUtils->printPacket("�����ش�", *senderbuffer.begin());
			//getchar();
			of << "�����ش�:" << senderbuffer.begin()->seqnum << endl;
			pns->stopTimer(SENDER, senderbuffer.begin()->seqnum);
			pns->startTimer(SENDER, Configuration::TIME_OUT, senderbuffer.begin()->seqnum);
			pns->sendToNetworkLayer(RECEIVER, *senderbuffer.begin());
		}
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
	of_2.close();
}

void TCPSender::timeoutHandler(int seqnum)
{

	//for (auto pp = senderbuffer.begin(); pp != senderbuffer.end(); ++pp)
	//{
		
		auto pp = senderbuffer.begin();
		pUtils->printPacket("��ʱ�ش�", *pp);
		pns->stopTimer(SENDER, pp->seqnum);
		pns->startTimer(SENDER, Configuration::TIME_OUT, pp->seqnum);
		pns->sendToNetworkLayer(RECEIVER, *pp);
	//}
}
bool TCPSender::getWaitingState()
{
	return senderbuffer.size() == max;
}
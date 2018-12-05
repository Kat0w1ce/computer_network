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
	pUtils->printPacket("发送方发送报文", *newPacket);
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
	//pUtils->printPacket("接收的报文", ackPkt);
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
		of_2 << "窗口由" << first_ACK;
		while (!senderbuffer.empty() && senderbuffer.begin()->seqnum <= ackPkt.seqnum)
		{
			pUtils->printPacket("发送方接收确认", *senderbuffer.begin());
			cout << "发送方窗口滑动" << senderbuffer.begin()->seqnum << endl;
			pns->stopTimer(SENDER, senderbuffer.begin()->seqnum);
			first_ACK++;
			cnt++;
			senderbuffer.pop_front();

		}
		of_2 << "滑动到" << first_ACK << endl;
		if (!senderbuffer.empty()&&cnt >= 3)
		{
			
			//pUtils->printPacket("快速重传", *senderbuffer.begin());
			//getchar();
			of << "快速重传:" << senderbuffer.begin()->seqnum << endl;
			pns->stopTimer(SENDER, senderbuffer.begin()->seqnum);
			pns->startTimer(SENDER, Configuration::TIME_OUT, senderbuffer.begin()->seqnum);
			pns->sendToNetworkLayer(RECEIVER, *senderbuffer.begin());
		}
	}
	else
	{
		pUtils->printPacket("发送方ack校验错误", ackPkt);
		/*for (; fpacket != senderbuffer.end(); ++fpacket)
		{
			pUtils->printPacket("发送方没有正确收到确认，重传", *fpacket);
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
		pUtils->printPacket("超时重传", *pp);
		pns->stopTimer(SENDER, pp->seqnum);
		pns->startTimer(SENDER, Configuration::TIME_OUT, pp->seqnum);
		pns->sendToNetworkLayer(RECEIVER, *pp);
	//}
}
bool TCPSender::getWaitingState()
{
	return senderbuffer.size() == max;
}
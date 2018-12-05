#include"stdafx.h"
#include"SRReceiver.h"
#include<cstring>
#include"Global.h"

#include<algorithm>
bool cmp(Packet &a, Packet &b)
{
	return a.seqnum < b.seqnum;
}

bool SRReceiver::find(int seqnum)
{

	for (auto ip = recvbuffer.begin(); ip != recvbuffer.end(); ip++)
		if (ip->seqnum == seqnum)
			return true;
	return false;
}
void SRReceiver::receive(Packet &packet)
{
	ofstream of;
	of.open("SRReceive.txt", ios::app);
	int checkSum = pUtils->calculateCheckSum(packet);
	if (checkSum == packet.checksum&&packet.seqnum<recv_base+max)
	{
		pUtils->printPacket("接收方发送确认报文", packet);
		pns->sendToNetworkLayer(SENDER, packet);
		if ( packet.seqnum>=recv_base&&!find(packet.seqnum))
		{
			recvbuffer.push_back(packet);
			sort(recvbuffer.begin(), recvbuffer.end(), cmp);
		}
		
	}
	else
	{
		pUtils->printPacket("接收方数据包错误", packet);
		return;
	}
	int cnt = 0;
	of << "接收方窗口滑动:  " << recv_base;
	while (!recvbuffer.empty()&&recv_base==recvbuffer.begin()->seqnum)
	{
		Message msg;
		memcpy(msg.data, recvbuffer.begin()->payload, sizeof(packet.payload));
		cout << recv_base << "  ";
		pUtils->printPacket("发送方提交数据", *recvbuffer.begin());
		cnt++;
		pns->delivertoAppLayer(RECEIVER, msg);
		recv_base++;
		recvbuffer.pop_front();
	}
	of << "  滑动长度" << cnt << endl;
	of.close();
}

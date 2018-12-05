#include"stdafx.h"
#include"TCPReceiver.h"
#include"Global.h"
#include<cstring>
#include<cstdlib>

void TCPReceiver::receive(Packet &packet)
{
	int checksum = pUtils->calculateCheckSum(packet);
	if (checksum == packet.checksum&&expectedseqnum == packet.seqnum)
	{
		
			pUtils->printPacket("接收方正确收到报文", packet);
			Message msg;
			memcpy(msg.data, packet.payload, sizeof(packet.payload));
			pns->delivertoAppLayer(RECEIVER, msg);
			memcpy(lastAckPkt.payload, packet.payload, sizeof(packet.payload));
			lastAckPkt.checksum = packet.checksum;
			lastAckPkt.seqnum = packet.seqnum;
			pUtils->printPacket("接收方发送确认报文", lastAckPkt);
			pns->sendToNetworkLayer(SENDER, packet);
			expectedseqnum = lastAckPkt.seqnum + 1;
		//cout << expectedseqnum << endl;

	}
	else
	{
		if (checksum != packet.acknum)
			pUtils->printPacket("接收方没有正确收到发送方的报文,数据校验错误", packet);
		else
			pUtils->printPacket("接收方没有正确收到发送方的报文, 报文序号不对", packet);
		pUtils->printPacket("接收方重新发送上次的确认报", lastAckPkt);
		pns->sendToNetworkLayer(SENDER, lastAckPkt);
	}
}


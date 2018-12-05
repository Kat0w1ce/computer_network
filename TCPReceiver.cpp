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
		
			pUtils->printPacket("���շ���ȷ�յ�����", packet);
			Message msg;
			memcpy(msg.data, packet.payload, sizeof(packet.payload));
			pns->delivertoAppLayer(RECEIVER, msg);
			memcpy(lastAckPkt.payload, packet.payload, sizeof(packet.payload));
			lastAckPkt.checksum = packet.checksum;
			lastAckPkt.seqnum = packet.seqnum;
			pUtils->printPacket("���շ�����ȷ�ϱ���", lastAckPkt);
			pns->sendToNetworkLayer(SENDER, packet);
			expectedseqnum = lastAckPkt.seqnum + 1;
		//cout << expectedseqnum << endl;

	}
	else
	{
		if (checksum != packet.acknum)
			pUtils->printPacket("���շ�û����ȷ�յ����ͷ��ı���,����У�����", packet);
		else
			pUtils->printPacket("���շ�û����ȷ�յ����ͷ��ı���, ������Ų���", packet);
		pUtils->printPacket("���շ����·����ϴε�ȷ�ϱ�", lastAckPkt);
		pns->sendToNetworkLayer(SENDER, lastAckPkt);
	}
}


#include"RdtReceiver.h"
#include"Global.h"
#include <deque>
class TCPReceiver :public RdtReceiver
{
public:
	TCPReceiver() :expectedseqnum(0)
	{
		lastAckPkt.acknum = -1;
		lastAckPkt.checksum = 0;
		lastAckPkt.seqnum = -1;
		for (int i = 0; i != Configuration::PAYLOAD_SIZE; ++i)
			lastAckPkt.payload[i] = '.';
		lastAckPkt.checksum = pUtils->calculateCheckSum(lastAckPkt);
	};
	~TCPReceiver() {};
	void receive(Packet& packet);
private:
	int expectedseqnum;
	Packet lastAckPkt;
};

/*GbnReceiver::GbnReceiver() :

GbnReceiver::~GbnReceiver()
{
}
*/#pragma once

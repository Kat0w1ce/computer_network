#pragma once
#include"stdafx.h"
#include"RdtReceiver.h"
#include<queue>
class SRReceiver:public RdtReceiver
{
public:
	SRReceiver() : recv_base(0), max(5) {};
	~SRReceiver() {};
	void receive(Packet& packet) override;
	bool find(int seqnum);

private:
	int recv_base;
	deque<Packet> recvbuffer;
	const int max;
};




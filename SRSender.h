#pragma once
#include"stdafx.h"
#include"RdtSender.h"
#include<deque>
class SRSender :public RdtSender
{
public:
	SRSender() :max(5), nextseqnum(0), send_base(0) {};
	~SRSender() {};
	bool send(Message &message) override;
	void receive(Packet &ackPkt) override;
	void timeoutHandler(int seqNum) override;
	bool getWaitingState() override;
private:
	deque<Packet> sendbuffer;
	const int max;
	int nextseqnum;
	int send_base;	
};


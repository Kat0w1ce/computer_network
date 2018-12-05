#pragma once
#include "stdafx.h"
#include "RdtSender.h"
#include <queue>
class TCPSender :public RdtSender
{
public:
	TCPSender() :base(0), nextseqnum(0), max(5), senderbuffer(),first_ACK(0),cnt(0) {};
	~TCPSender() {};
	bool send(Message &message) override;
	void receive(Packet &ackPkt) override;
	void timeoutHandler(int seqNum) override;
	bool getWaitingState() override;


private:
	int base;
	int nextseqnum;
	const int max;
	deque<Packet> senderbuffer;
	int first_ACK;
	int cnt;
};


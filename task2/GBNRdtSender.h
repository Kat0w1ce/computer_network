#pragma once
#include"RdtSender.h"
#include <queue>
class GbnSender:public RdtSender
{
public:
	GbnSender() :base(0), nextseqnum(0), max(5), senderbuffer() {};
	~GbnSender() {};
	bool send(Message &message) override;
    void receive(Packet &ackPkt) override;
	void timeoutHandler(int seqNum) override;
	bool getWaitingState() override;
	

private:
	int base;
	int nextseqnum;
	const int max;
	deque<Packet> senderbuffer;

};


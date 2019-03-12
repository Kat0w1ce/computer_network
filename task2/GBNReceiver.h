#include"RdtReceiver.h"
#include"Global.h"
#include <deque>
class GbnReceiver :public RdtReceiver
{
public:
	GbnReceiver():expectedseqnum(0)
	{
		lastAckPkt.acknum = -1;
		lastAckPkt.checksum = 0;
		lastAckPkt.seqnum = -1;
		for (int i = 0; i != Configuration::PAYLOAD_SIZE; ++i)
			lastAckPkt.payload[i] = '.';
		lastAckPkt.checksum = pUtils->calculateCheckSum(lastAckPkt);
	};
	~GbnReceiver(){};
	void receive(Packet& packet);
private:
	int expectedseqnum;
	Packet lastAckPkt;
};

/*GbnReceiver::GbnReceiver() :

GbnReceiver::~GbnReceiver()
{
}
*/

#ifndef MOVE_TRANSCEIVER_SIMPLE_TRANSCEIVER_CONTROL
#define MOVE_TRANSCEIVER_SIMPLE_TRANSCEIVER_CONTROL

#include <string>
#include <thread>
#include <trctrl/LinkInterface.h>

#include <trctrl/ControlCallback.h>
#include <trctrl/ServerSocket.h>

class SimpleTransceiverControl :
	public Transceiver::ControlCallback
{
public:
	SimpleTransceiverControl(std::string path);
	~SimpleTransceiverControl();

	bool startModule(Transceiver::LinkInterface* link);
	void stopModule();

	virtual void handleStatus();
	virtual void handleCounts();
	virtual void handleParameters();

private:
	bool active;
	std::string path;

	ServerSocket server;
	Socket sock;

	std::thread serverLoop;
	void serverRoutine();
	bool sendText(const std::string& text);

	Transceiver::LinkInterface* link;
};

#endif // MOVE_TRANSCEIVER_SIMPLE_TRANSCEIVER_CONTROL

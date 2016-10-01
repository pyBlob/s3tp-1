#include <iostream>

#include <trctrl/Backend.h>
#include <trctrl/LinkInterface.h>
#include <trctrl/ControlInterface.h>

#include "../control/SimpleTransceiverControl.h"

class DummyTransceiver :
	public Transceiver::Backend,
	public Transceiver::LinkInterface,
	public Transceiver::ControlInterface
{
public:
	// Backend
	void start()
	{
		std::cout << "start ...\n";
	}
	void stop()
	{
		std::cout << "stop ...\n";
	}

	// LinkInterface
	int sendFrame(bool arq, int channel, const void* data, int length)
	{
		std::cout << "DT: sending arq=" << arq << " chan=" << channel << " #data=" << length << "\n";
	}
	bool getLinkStatus()
	{
		std::cout << "DT: linkStatus\n";
		return true;
	}
	bool getBufferFull(int channel)
	{
		std::cout << "DT: bufferFull\n";
		return false;
	}
	
	// ControlInterfce
	void updateStatus()
	{
		std::cout << "DT: updateStatus\n";
		this->controlCallback->handleStatus();
	}
	void updateCounts()
	{
		std::cout << "DT: updateCounts\n";
		this->controlCallback->handleCounts();
	}
	void setParameters(int parameters)
	{
		this->parameters = parameters;
		std::cout << "DT: params=" << parameters << "\n";
		this->controlCallback->handleParameters();
	}
	void updateParameters()
	{
		std::cout << "DT: updateParameters\n";
	}
};

int main()
{
	SimpleTransceiverControl stc("stc_unix");

	DummyTransceiver dt;
	dt.setControlCallback(stc);

	stc.startModule(&dt);
	dt.start();

	std::cin.get();

	dt.stop();
	stc.stopModule();
}

#include "SimpleTransceiverControl.h"
#include "../core/Logger.h"

#include <sstream>
#include <algorithm>

#include <unistd.h>

SimpleTransceiverControl::SimpleTransceiverControl(std::string path) :
	path(path),
	active(false)
{

}

SimpleTransceiverControl::~SimpleTransceiverControl()
{
	this->stopModule();
}

bool SimpleTransceiverControl::startModule(Transceiver::LinkInterface* link)
{
	this->active = true;
	this->link = link;

	this->serverLoop = std::thread(&SimpleTransceiverControl::serverRoutine, std::ref(*this));
}

void SimpleTransceiverControl::stopModule()
{
	this->active = false;
	this->server.close();
	this->sock.close();
	this->serverLoop.join();
}

void SimpleTransceiverControl::handleStatus()
{
	this->sendText("handle status");
}

void SimpleTransceiverControl::handleCounts()
{
	this->sendText("handle counts");
}

void SimpleTransceiverControl::handleParameters()
{
	this->sendText("handle parameters");
}

void SimpleTransceiverControl::serverRoutine()
{
	unlink(this->path.data());
	if (this->server.bind(this->path) < 0)
	{
		LOG_ERROR("control socket: socket creation failed\n");
		return;
	}

	while (this->active && this->server.isValid())
	{
		this->sock = this->server.accept();
		if (!this->sock.isValid())
		{
			LOG_INFO("control socket: accept failed");
			continue;
		}

		LOG_INFO("control socket: client arrived");

		std::vector<char> buffer(1000);
		while (this->sock.readmsg(buffer) == 0)
		{
			std::stringstream msg(std::string(buffer.begin(), buffer.end()));

			std::string cmd;
			msg >> cmd;

			std::stringstream out;

			if (cmd == "stop")
			{
				LOG_INFO("control socket: client finished");
				break;
			}
			else if (cmd == "set")
			{
				int flags = 0;

				std::string flag;
				while (msg >> flag)
				{
					if (flag == "off" || flag == "safe" || flag == "safemode")
					{
						flags |= TR_PARAMETER_SAFEMODE;
					}
					else if (flag == "pwr")
					{
						int pwr;
						msg >> pwr;

						flags |= (pwr << TR_PARAMETER_TXPOWER_OFFSET) & TR_PARAMETER_TXPOWER_MASK;
					}
				}

				LOG_DEBUG("control socket: setting parameters " + std::to_string(flags));
				this->control->setParameters(flags);
			}
			else if (cmd == "status")
			{
				out << "info status";
				for (int i=0 ; i<TR_STATUS_LENGTH ; i++)
				{
					out << " " << this->control->status[i];
				}
			}
			else if (cmd == "counts")
			{
				out << "info counts";
				for (int i=0 ; i<TR_COUNTS_LENGTH ; i++)
				{
					out << " " << this->control->counts[i];
				}
			}
			else if (cmd == "query")
			{
				std::string what;
				msg >> what;

				if (what == "status" || what == "all")
				{
					this->control->updateStatus();
				}
				if (what == "counts" || what == "all")
				{
					this->control->updateCounts();
				}
			}
			else if (cmd == "tele")
			{
				int size = std::min((int)buffer.size(), 1000);
				this->link->sendFrame(false, 7, buffer.data(), size);
			}

			if (!this->sendText(out.str()))
			{
				break;
			}
		}

		LOG_INFO("control socket: client gone");

		this->sock.close();
	}
}

bool SimpleTransceiverControl::sendText(const std::string& text)
{
	if (!text.empty())
	{
		std::vector<char> buffer(text.size());
		buffer.clear();
		buffer.insert(buffer.end(), text.begin(), text.end());
		if (this->sock.writemsg(buffer) < 0)
		{
			return false;
		}
	}

	return true;
}

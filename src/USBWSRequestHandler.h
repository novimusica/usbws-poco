/*
 * Copyright (C) 2015 Nobuo Iwata
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _USBIP_WEBSOCK_POCO_USBWS_REQUEST_HANDLER_H
#define _USBIP_WEBSOCK_POCO_USBWS_REQUEST_HANDLER_H

#include "USBWSWebSocket.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Logger.h"

namespace Usbip { namespace WebSock { namespace Poco {

using namespace ::Poco;
using namespace ::Poco::Net;

class USBWSRequestHandler:
	public ::Poco::Net::HTTPRequestHandler
{
public:
	USBWSRequestHandler(int pingPong, Logger& logger);
	~USBWSRequestHandler(void);
	void handleRequest(HTTPServerRequest& req, HTTPServerResponse& rsp);
private:
	void handleProtocol(HTTPServerRequest& req, HTTPServerResponse& rsp);
	void sendProtocolError(HTTPServerResponse& rsp, const std::string& msg);

	int fPingPong;
	Logger &fLogger;
};

}}}

#endif /* !_USBIP_WS_POCO_USBWS_REQUEST_HANDLER_H */

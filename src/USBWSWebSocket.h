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

#ifndef _USBIP_WEBSOCK_POCO_USBWS_WEBSOCKET_H
#define _USBIP_WEBSOCK_POCO_USBWS_WEBSOCKET_H

#include "Poco/Net/WebSocket.h"
#include "Poco/Util/Application.h"
#include "Poco/Timer.h"
#include "Poco/Mutex.h"

namespace Usbip { namespace WebSock { namespace Poco {

using namespace ::Poco;
using namespace ::Poco::Net;
using namespace ::Poco::Util;

class USBWSWebSocket :
	public ::Poco::Net::WebSocket,
	public ::Poco::Timer
{
public:
	USBWSWebSocket(HTTPServerRequest& req, HTTPServerResponse& rsp,
			int pingPong, Logger& logger);
	USBWSWebSocket(HTTPClientSession& cs,
			HTTPRequest& req, HTTPResponse& rsp,
			int pingPong, Logger& logger);
	static int sendProc(void *arg, void *buf, int len);
	static int recvProc(void *arg, void *buf, int len, int wait_all);
	static void shutdownProc(void *arg);
	int getSockfd(void) { return sockfd(); };
	void setApp(Application* app) { fApp = app; };
	Application* getApp(void) { return fApp; };

	static const std::string kProtocolKey;
	static const std::string kProtocolValue;
private:
	int sendProc(void *buf, int len);
	int recvProc(void *buf, int len, int wait_all);
	void shutdownProc(void);
	void startPingPong(void);
	void restartPingPong(void);
	void onTimer(Timer& timer);
	void sendPing(void);
	void sendPong(void);
	void sendPingPong(int op);

	Application* fApp;
	Logger& fLogger;
	bool fPingPongStarted;
	bool fFirstTimeout;
	Mutex fSendLock;
};

}}}

#endif /* !_USBIP_WEBSOCK_POCO_USBWS_WEBSOCKET_H */

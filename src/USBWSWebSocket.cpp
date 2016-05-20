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

#include "USBWSWebSocket.h"
#include "USBWSUtil.h"
#include "Poco/Exception.h"
#include "Poco/Net/NetException.h"

using namespace ::Poco::Net;
using namespace ::Usbip::WebSock::Poco;

const std::string USBWSWebSocket::kProtocolKey("Sec-WebSocket-Protocol");
const std::string USBWSWebSocket::kProtocolValue("USB/IP");

USBWSWebSocket::USBWSWebSocket(
	HTTPServerRequest& req,
	HTTPServerResponse& rsp,
	int pingPong,
	Logger& logger) :
	WebSocket(req, rsp),
	Timer(0, pingPong * 1000),
	fLogger(logger),
	fApp(0),
	fPingPongStarted(false),
	fFirstTimeout(true),
	fSendLock()
{
	setKeepAlive(true);
	if (pingPong > 0) {
		setReceiveTimeout(Timespan(pingPong + 60, 0));
		startPingPong();
		fPingPongStarted = true;
	}	
}

USBWSWebSocket::USBWSWebSocket(
	HTTPClientSession& cs,
	HTTPRequest& req,
	HTTPResponse& rsp,
	int pingPong,
	Logger& logger) :
	WebSocket(cs, req, rsp),
	Timer(0, 0),
	fLogger(logger),
	fApp(0),
	fPingPongStarted(false),
	fFirstTimeout(true),
	fSendLock()
{
	setKeepAlive(true);
	if (pingPong > 0)
		setReceiveTimeout(Timespan(pingPong + 60, 0));
}

int USBWSWebSocket::sendProc(void *arg, void *buf, int len)
{
	USBWSWebSocket *ws = (USBWSWebSocket *)arg;
	return ws->sendProc(buf, len);
}

int USBWSWebSocket::recvProc(void *arg, void *buf, int len, int all)
{
	USBWSWebSocket *ws = (USBWSWebSocket *)arg;
	return ws->recvProc(buf, len, all);
}

void USBWSWebSocket::shutdownProc(void *arg)
{
	USBWSWebSocket *ws = (USBWSWebSocket *)arg;
	ws->shutdownProc();
}

int USBWSWebSocket::sendProc(void *buf, int len)
{
	int ret;
	fSendLock.lock();
	try {
		ret = sendFrame(buf, len, WebSocket::FRAME_BINARY);
	} catch (::Poco::IOException& e) {
		fLogger.log(e);
		ret = -1;
		errno = EIO;
	}
	fSendLock.unlock();
	return ret;
}

int USBWSWebSocket::recvProc(void *buf, int len, int all)
{
	char *p = (char *)buf;
	try {
		int received = 0;
		int op, flags;
		do {
			int bytes = receiveFrame(p + received,
						     len - received, flags);
			op = flags & WebSocket::FRAME_OP_BITMASK;
			if (op == WebSocket::FRAME_OP_BINARY ||
				op == WebSocket::FRAME_OP_CONT) {
				if (op == WebSocket::FRAME_OP_CONT)
					fLogger.debug("Received cont");
				if (bytes == 0)
					return received;
				received += bytes;
			} else if (op == WebSocket::FRAME_OP_PING)
				sendPong();
			else if (op == WebSocket::FRAME_OP_PONG) {
				restartPingPong();
				fLogger.debug("Received pong");
			} else if (op == WebSocket::FRAME_OP_CLOSE)
				throw ::Poco::IOException(
					"Recieved close frame");
			else
				throw ::Poco::Net::WebSocketException(
					"Unsupported op code:" +
					USBWSUtil::i2s(op));
		} while ((all && received < len) ||
			op != WebSocket::FRAME_OP_BINARY);
		restartPingPong();
		return received;
	} catch (::Poco::Net::WebSocketException& e) {
		fLogger.log(e);
		errno = EINVAL;
	} catch (::Poco::TimeoutException& e) {
		fLogger.log(e);
		errno = ETIMEDOUT;
	} catch (::Poco::Net::NetException& e) {
		fLogger.log(e);
		errno = EIO;
	} catch (::Poco::IOException& e) {
		fLogger.log(e);
		errno = EIO;
	}
	return -1;
}

void USBWSWebSocket::shutdownProc(void)
{
	fLogger.debug("Shutting down websocket.");
	fPingPongStarted = false;
	shutdownReceive();
	close();
}

void USBWSWebSocket::startPingPong(void)
{
	TimerCallback<USBWSWebSocket> callback(*this, &USBWSWebSocket::onTimer);
	start(callback);
}

void USBWSWebSocket::restartPingPong(void)
{
	if (fPingPongStarted)
		restart();
}

void USBWSWebSocket::onTimer(Timer& timer)
{
	USBWSWebSocket& ws = (USBWSWebSocket&)timer; 
	if (ws.fFirstTimeout)
		ws.fFirstTimeout = false;
	else
		ws.sendPing();
}

void USBWSWebSocket::sendPing(void)
{
	if (fPingPongStarted) {
		fLogger.debug("Sending ping");
		sendPingPong(WebSocket::FRAME_OP_PING);
	}
}

void USBWSWebSocket::sendPong(void)
{
	fLogger.debug("Sending pong");
	sendPingPong(WebSocket::FRAME_OP_PONG);
}

void USBWSWebSocket::sendPingPong(int op)
{
	fSendLock.lock();
	try {
		sendFrame(NULL, 0, op | WebSocket::FRAME_FLAG_FIN);
	} catch (::Poco::IOException& e) {
		fLogger.log(e);
	}
	fSendLock.unlock();
}

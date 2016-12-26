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

#include "USBWSRequestHandler.h"
#include "USBWSUtil.h"
#include "Poco/Net/Socket.h"
#include "Poco/Net/NetException.h"
#include <linux/usbip_api.h>

using namespace ::Poco;
using namespace ::Poco::Net;
using namespace ::Usbip::WebSock::Poco;

USBWSRequestHandler::USBWSRequestHandler(int pingPong, Logger& logger) :
	HTTPRequestHandler(),
	fPingPong(pingPong),
	fLogger(logger)
{
}

USBWSRequestHandler::~USBWSRequestHandler(void)
{
}

void USBWSRequestHandler::handleRequest(
	HTTPServerRequest& req,
	HTTPServerResponse& rsp)
{
	int interrupted = 0;
	fLogger.information("WebSocket connection established.");
	try {
		handleProtocol(req, rsp);

		USBWSWebSocket ws(req, rsp, fPingPong, fLogger);

		struct usbip_sock sock;
		usbip_sock_init(&sock, ws.getSockfd(), &ws,
			USBWSWebSocket::sendProc, USBWSWebSocket::recvProc,
			USBWSWebSocket::shutdownProc);

		SocketAddress addr = ws.address();
		std::string port = USBWSUtil::i2s(addr.port());

		fLogger.information("Entering to usbipd_recv_pdu().");
		usbipd_recv_pdu(&sock,
			addr.host().toString().c_str(),
			port.c_str());
		fLogger.information("Exited from usbipd_recv_pdu().");
	} catch (WebSocketException& e) {
		fLogger.information("WebSocketException.");
		fLogger.log(e);
		switch(e.code()) {
		case WebSocket::WS_ERR_HANDSHAKE_UNSUPPORTED_VERSION:
			rsp.set("Sec-WebSocket-Version",
				WebSocket::WEBSOCKET_VERSION);
			// fallthrough
		case WebSocket::WS_ERR_NO_HANDSHAKE:
		case WebSocket::WS_ERR_HANDSHAKE_NO_VERSION:
		case WebSocket::WS_ERR_HANDSHAKE_NO_KEY:
			rsp.setContentLength(0);
			rsp.send();
			break;
		case WebSocket::WS_ERR_HANDSHAKE_ACCEPT:
		case WebSocket::WS_ERR_UNAUTHORIZED:
		case WebSocket::WS_ERR_PAYLOAD_TOO_BIG:
		case WebSocket::WS_ERR_INCOMPLETE_FRAME:
		default:
			break;
		}
	} catch (::Poco::Exception& e) {
		fLogger.information("OtherException.");
		fLogger.log(e);
	}
	fLogger.information("WebSocket connection terminated.");
}

void USBWSRequestHandler::handleProtocol(
	HTTPServerRequest& req,
	HTTPServerResponse& rsp)
{
	try {
		// TODO: multiple protocol headers
		const std::string& s = req.get(USBWSWebSocket::kProtocolKey);
		std::vector<std::string> protocols;
		MessageHeader::splitElements(s, protocols, true);
		for (int i = 0; i < protocols.size(); i++) {
			if (protocols[i] == USBWSWebSocket::kProtocolValue) {
				rsp.set(USBWSWebSocket::kProtocolKey,
					USBWSWebSocket::kProtocolValue);
				return;
			}
		}
		sendProtocolError(rsp, "Unsupported protocol");
		throw ::Poco::Net::WebSocketException("Unsupported protocol");
	} catch (NotFoundException& e) {
		sendProtocolError(rsp, "Missing protocol");
		throw ::Poco::Net::WebSocketException("Missing protocol");
	}
}

void USBWSRequestHandler::sendProtocolError(
	HTTPServerResponse& rsp,
	const std::string& msg)
{
	rsp.setStatus(HTTPResponse::HTTP_BAD_REQUEST);
	rsp.setReason(msg);
	rsp.send();
}

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

#include "USBWSCommand.h"
#include "USBWSWebSocket.h"
#include "USBWSUtil.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPSClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/NetException.h"
#include "Poco/Util/HelpFormatter.h"
#include "Poco/StringTokenizer.h"
#include "Poco/Exception.h"
#include <iostream>

using namespace ::Poco;
using namespace ::Poco::Net;
using namespace ::Poco::Util;
using namespace ::Usbip::WebSock::Poco;

const std::string USBWSCommand::kDefaultKey("cert/client.key");
const std::string USBWSCommand::kDefaultCert("cert/client.crt");
const int USBWSCommand::kMajorVersion(0);
const int USBWSCommand::kMinorVersion(0);
const int USBWSCommand::kRevision(1);

USBWSCommand::USBWSCommand(void) :
	fDebug(false),
	fURL(),
	fProxy(),
	fBusID(),
	fConnTimeout(5),
	fPingPong(60),
	fPort(),
	fLocal(false),
	fParsable(false),
	fKey(kDefaultKey),
	fCert(kDefaultCert),
	fRootCert(),
	fVerificationStr(),
	fVerification(Context::VERIFY_NONE),
	fSSL(false),
	fHost(),
	fTcpPort(0),
	fPath(),
	fProxyHost(),
	fProxyPort(),
	fProxyUser(),
	fProxyPwd(),
	fHelp(false),
	fVersion(false),
	fClientSession(0)
{
}

USBWSCommand::~USBWSCommand(void)
{
}

void USBWSCommand::initialize(Application& self)
{
	Application::initialize(self);
}

void USBWSCommand::uninitialize(void)
{
	Application::uninitialize();
}

void USBWSCommand::defineOptions(OptionSet& options)
{
	Application::defineOptions(options);
	options.addOption(Option("debug", "d", "Print debug information."));
#ifdef USBIP_WITH_LIBUSB
	options.addOption(Option("flags", "f",
		"Debug flags for stub emulator."
		).argument("flags"));
#endif
	options.addOption(Option("url", "u", "WebSocket URL for USB/IP."
		).argument("url"));
	options.addOption(Option("proxy", "x", "Proxy URL if used."
		).argument("proxy"));
	options.addOption(Option("busid", "b", "Bus ID of a device to export."
		).argument("busid"));
	options.addOption(Option("timeout", "o",
		"Connect timeout in seconds. Default is 5."
		).argument("timeout-sec"));
	options.addOption(Option("interval", "i",
		"No communication period to send ping-pong in seconds."
		" Default is 60. 0 denotes not to use ping-pong."
		).argument("interval-sec"));
	options.addOption(Option("port", "p", "Attached port to detach."
		).argument("port"));
	options.addOption(Option("local", "l", "List local devices."));
	options.addOption(Option("parsable", "P", "Parsable list output."));
	options.addOption(Option("key", "k",
		"Private key file. Default is " + kDefaultKey + "."
		).argument("key-file"));
	options.addOption(Option("cert", "c",
		"Certificate file. Default is " + kDefaultCert + "."
		).argument("cert-file"));
	options.addOption(Option("root-cert", "r",
		"Certificate file of root CA."
		).argument("root-cert-file"));
	options.addOption(Option("verification", "V",
		"Certificate verification mode - none(default) or relaxed."
		).argument("verification-mode"));
	options.addOption(Option("help", "h", "Print this help."));
	options.addOption(Option("version", "v", "Show version."));
}

void USBWSCommand::handleOption(const std::string& name,
				const std::string & value)
{
	Application::handleOption(name, value);

	if (name == "debug") {
		fDebug = true;
		usbip_set_use_debug(1);
#ifdef USBIP_WITH_LIBUSB
	} else if (name == "flags") {
		usbip_set_debug_flags(USBWSUtil::s2ul(value));
#endif
	} else if (name == "url") {
		fURL = value;
	} else if (name == "proxy") {
		fProxy = value;
	} else if (name == "busid") {
		fBusID = value;
	} else if (name == "timeout") {
		fConnTimeout = USBWSUtil::s2i(value);
	} else if (name == "interval") {
		fPingPong = USBWSUtil::s2i(value);
	} else if (name == "port") {
		fPort = value;
	} else if (name == "local") {
		fLocal = true;
	} else if (name == "parsable") {
		fParsable = true;
	} else if (name == "key") {
		fKey = value;
	} else if (name == "cert") {
		fCert = value;
	} else if (name == "root-cert") {
		fRootCert = value;
	} else if (name == "verification") {
		fVerificationStr = value;
	} else if (name == "help") {
		fHelp = true;
	} else if (name == "version") {
		fVersion = true;
	}
}

void USBWSCommand::help(void)
{
	HelpFormatter helpFormatter(options());
	helpFormatter.setCommand(commandName() +
#ifdef USBIP_WITH_LIBUSB
		" <connect | disconnect | list | bind | unbind> [options]");
#else
		" <connect | disconnect | list | port | bind |"
		" attach | detach | unbind> [options]");
#endif
	helpFormatter.format(std::cout);
}

void USBWSCommand::version(void)
{
	std::cout << kMajorVersion << "." << kMinorVersion << "."
		  << kRevision << std::endl;
}

void USBWSCommand::getProxyCredentials(const URI& url)
{
	std::string info = url.getUserInfo();
	if (info.size() > 0) {
		StringTokenizer t(info, ":");
		try {
			fProxyUser = t[0];
			fProxyPwd = t[1];
		} catch (RangeException& e) {
		}
	}
}

int USBWSCommand::main(const std::vector<std::string>& args)
{
	usbip_set_use_stderr(1);
	std::string cmd(args[0]);
	if (fHelp || cmd == "help") {
		help();
		return Application::EXIT_OK;
	} else if (fVersion || cmd == "version") {
		version();
		return Application::EXIT_OK;
	}
	if (fDebug)
		logger().setLevel(Message::PRIO_DEBUG);
	if (fURL.size() > 0) {
		try {
			URI url(fURL);
			std::string scheme = url.getScheme();
			if (scheme == "wss")
				fSSL = true;
			else if (scheme != "ws") {
				std::cerr << "Unsupported scheme in URL."
					<< std::endl;
				return Application::EXIT_USAGE;
			}
			fHost = url.getHost();
			fTcpPort = url.getPort();
			fPath = url.getPath();
		} catch (SyntaxException& e) {
			std::cerr << "URL syntax error." << std::endl;
			return Application::EXIT_USAGE;
		}
	}
	if (fTcpPort == 0) {
		if (fSSL)
			fTcpPort = 443;
		else
			fTcpPort = 80;

		logger().debug("TCP port has been set to %d", fTcpPort);
	}
	if ((cmd == "connect" || cmd == "disconnect" || cmd == "attach" ||
		(cmd == "list" && fURL.size() > 0)) &&
		fHost.size() <= 0) {
		std::cerr << "Missing host." << std::endl;
		return Application::EXIT_USAGE;
	}
	if ((cmd == "connect" || cmd == "disconnect" || cmd == "attach" ||
		cmd == "bind" || cmd == "unbind") &&
		fBusID.size() <= 0) {
		std::cerr << "Missing bus ID." << std::endl;
		return Application::EXIT_USAGE;
	}
	if (cmd == "detach" && fPort.size() <= 0) {
		std::cerr << "Missing port." << std::endl;
		return Application::EXIT_USAGE;
	}
	if (fVerificationStr.size() > 0) {
		if (fVerificationStr == "none")
			fVerification = Context::VERIFY_NONE;
		else if (fVerificationStr == "relaxed")
			fVerification = Context::VERIFY_RELAXED;
		else {
			std::cerr << "Unsupported verification mode."
				  << std::endl;
			return Application::EXIT_USAGE;
		}
	}
	if (fProxy.size() > 0) {
		try {
			URI url(fProxy);
			fProxyHost = url.getHost();
			fProxyPort = url.getPort();
			getProxyCredentials(url);
			logger().debug(
				"Using proxy host:%s port:%d user:%s pwd:%s",
				fProxyHost, fProxyPort, fProxyUser, fProxyPwd);
		} catch (SyntaxException& e) {
			std::cerr << "Proxy syntax error." << std::endl;
			return Application::EXIT_USAGE;
		}
	}

	std::string tcpPortStr = USBWSUtil::i2s(fTcpPort);

	usbip_conn_init(USBWSCommand::connect, USBWSCommand::close, this);

	if (cmd == "connect") {
		if (usbip_connect_device(
			fHost.c_str(), tcpPortStr.c_str(), fBusID.c_str()))
			return Application::EXIT_SOFTWARE;
	} else if (cmd == "disconnect") {
		if (usbip_disconnect_device(
			fHost.c_str(), tcpPortStr.c_str(), fBusID.c_str()))
			return Application::EXIT_SOFTWARE;
#ifndef USBIP_WITH_LIBUSB
	} else if (cmd == "attach") {
		if (usbip_attach_device(
			fHost.c_str(), tcpPortStr.c_str(), fBusID.c_str()))
			return Application::EXIT_SOFTWARE;
	} else if (cmd == "detach") {
		if (usbip_detach_port(fPort.c_str()))
			return Application::EXIT_SOFTWARE;
	} else if (cmd == "port") {
		if (usbip_list_imported_devices())
			return Application::EXIT_SOFTWARE;
	} else if (cmd == "list" && fURL.size() > 0) {
		if (usbip_list_importable_devices(
			fHost.c_str(), tcpPortStr.c_str()))
			return Application::EXIT_SOFTWARE;
#endif
	} else if (cmd == "list") {
		if (usbip_list_local_devices(fParsable))
			return Application::EXIT_SOFTWARE;
	} else if (cmd == "bind") {
		if (usbip_bind_device(fBusID.c_str()))
			return Application::EXIT_SOFTWARE;
	} else if (cmd == "unbind") {
		if (usbip_unbind_device(fBusID.c_str()))
			return Application::EXIT_SOFTWARE;
	} else {
		help();
		return Application::EXIT_USAGE;
	}
	return Application::EXIT_OK;
}

POCO_APP_MAIN(USBWSCommand)

int USBWSCommand::openSession(void)
{
	if (fSSL) {
		Context *ctx = new Context(Context::CLIENT_USE,
					fKey, fCert, fRootCert, fVerification);
		// TODO: manipulate other params.
		fClientSession = new HTTPSClientSession(fHost, fTcpPort, ctx);
	} else
		fClientSession = new HTTPClientSession(fHost, fTcpPort);

	if (fClientSession == NULL)
		return -1;

	if (fProxy.size() > 0) {
		fClientSession->setProxy(fProxyHost, fProxyPort);
		if (fProxyUser.size() > 0) {
			fClientSession->setProxyCredentials(
						fProxyUser, fProxyPwd);
		}
	}
	fClientSession->setKeepAlive(true);
	fClientSession->setTimeout(Timespan(fConnTimeout, 0));
	return 0;
}

void USBWSCommand::closeSession(void)
{
	if (fClientSession) {
		delete fClientSession;
		fClientSession = 0;
	}
}

struct usbip_sock *USBWSCommand::connect(const char *host, const char *port,
					 void *opt)
{
	USBWSCommand *app = (USBWSCommand *)opt;

	struct usbip_sock *sock =
		(struct usbip_sock *)malloc(sizeof(struct usbip_sock));
	if (sock == NULL) {
		app->logger().error("Fail to alloc sock.");
		return NULL;
	}

	try {
		if (app->openSession()) {
			app->logger().error("Fail to open session.");
			free(sock);
			return NULL;
		}

		USBWSWebSocket *ws;
		HTTPRequest req(HTTPRequest::HTTP_GET, app->fPath);
		req.set(USBWSWebSocket::kProtocolKey,
			USBWSWebSocket::kProtocolValue);
		HTTPResponse rsp;
		ws = new USBWSWebSocket(*app->fClientSession, req, rsp,
					app->fPingPong, app->logger());
		ws->setApp(app);
		usbip_sock_init(sock, ws->getSockfd(), ws,
			USBWSWebSocket::sendProc, USBWSWebSocket::recvProc,
			USBWSWebSocket::shutdownProc);
		return sock;
	} catch (WebSocketException& e) {
		app->logger().log(e);
	} catch (::Poco::Exception& e) {
		app->logger().log(e);
	}
	free(sock);
	return NULL;
}

void USBWSCommand::close(struct usbip_sock  *sock)
{
	if (sock) {
		USBWSWebSocket *ws = (USBWSWebSocket *)sock->arg;
		if (ws) {
			USBWSCommand *app = (USBWSCommand *)ws->getApp();
			try {
				app->closeSession();
			} catch (::Poco::Exception& e) {
				app->logger().log(e);
			} 
			delete ws;
		}
		free(sock);
	}
}

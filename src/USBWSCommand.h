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

#ifndef _USBIP_WEBSOCK_POCO_USBWS_COMMAND_H
#define _USBIP_WEBSOCK_POCO_USBWS_COMMAND_H

#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/Context.h"
#include "Poco/Util/Application.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/URI.h"
#include <linux/usbip_nppi.h>

namespace Usbip { namespace WebSock { namespace Poco {

using namespace ::Poco;
using namespace ::Poco::Net;
using namespace ::Poco::Util;

class USBWSCommand :
	public ::Poco::Util::Application
{
public:
	USBWSCommand(void);
	~USBWSCommand(void);
protected:
	// For Application
	void initialize(Application& self);
	void uninitialize(void);
	void defineOptions(OptionSet& options);
	void handleOption(const std::string& name, const std::string& value);
	int main(const std::vector<std::string>& args);
public:
	static struct usbip_sock *connect(const char *host, const char *port,
					  void *opt);
	static void close(struct usbip_sock *sock);
private:
	void help(void);
	void version(void);
	void getProxyCredentials(const URI&url);
	int openSession(void);
	void closeSession(void);

	bool fDebug;
	std::string fURL;
	std::string fProxy;
	std::string fBusID;
	std::string fPort;
	int fConnTimeout;
	int fPingPong;
	bool fLocal;
	bool fParsable;
	std::string fKey;
	std::string fCert;
	std::string fRootCert;
	std::string fVerificationStr;
	Context::VerificationMode fVerification;
	bool fSSL;
	std::string fHost;
	int fTcpPort;
	std::string fPath;
	std::string fProxyHost;
	int fProxyPort;
	std::string fProxyUser;
	std::string fProxyPwd;
	bool fHelp;
	bool fVersion;
	HTTPClientSession* fClientSession;

	static const std::string kDefaultKey;
	static const std::string kDefaultCert;
	static const int kMajorVersion;
	static const int kMinorVersion;
	static const int kRevision;
};

}}}

#endif /* !_USBIP_WEBSOCK_POCO_USBWS_COMMAND_H */

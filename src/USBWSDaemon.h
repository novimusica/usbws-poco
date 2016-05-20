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

#ifndef _USBIP_WEBSOCK_POCO_USBWS_DAEMON_H
#define _USBIP_WEBSOCK_POCO_USBWS_DAEMON_H

#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/Context.h"
#include "Poco/Util/ServerApplication.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"

namespace Usbip { namespace WebSock { namespace Poco {

using namespace ::Poco;
using namespace ::Poco::Net;
using namespace ::Poco::Util;

class USBWSDaemon:
	public ::Poco::Util::ServerApplication
{
public:
	USBWSDaemon(void);
	~USBWSDaemon(void);
protected:
	// For ServerApplication
	void initialize(Application& self);
	void uninitialize(void);
	void defineOptions(OptionSet& options);
	void handleOption(const std::string& name, const std::string& value); 
	int main(const std::vector<std::string>& args);
private:
	void help(void);
	void version(void);
	int openSocket(void);
	void closeSocket(void);

	bool fDebug;
	int fTcpPort;
	std::string fPath;
	int fPingPong;
	bool fSSL;
	std::string fKey;
	std::string fCert;
	std::string fRootCert;
	std::string fVerificationStr;
	Context::VerificationMode fVerification;
	bool fHelp;
	bool fVersion;
	ServerSocket* fSocket;

	static const int kDefaultTcpPort;
	static const std::string kDefaultPath;
	static const std::string kDefaultKey;
	static const std::string kDefaultCert;
	static const int kMajorVersion;
	static const int kMinorVersion;
	static const int kRevision;
};

}}}

#endif /* !_USBIP_WEBSOCK_POCO_USBWS_DAEMON_H */


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

#include "USBWSRequestHandlerFactory.h"
#include "USBWSRequestHandler.h"
#include "Poco/URI.h"

using namespace ::Poco::Net;
using namespace ::Usbip::WebSock::Poco;

USBWSRequestHandlerFactory::USBWSRequestHandlerFactory(
	std::string& path, int pingPong, Logger& logger) :
		fPath(path),
		fPingPong(pingPong),
		fLogger(logger)
{
}

USBWSRequestHandlerFactory::~USBWSRequestHandlerFactory(void)
{
}

HTTPRequestHandler*
USBWSRequestHandlerFactory::createRequestHandler(const HTTPServerRequest& req)
{
	URI uri(req.getURI());
	const std::string& path = uri.getPath();
	fLogger.information("Handling path:" + path);
	if (path == fPath) {
		return new USBWSRequestHandler(fPingPong, fLogger);
	}
	return 0;
}

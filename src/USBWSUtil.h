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

#ifndef _USBIP_WEBSOCK_POCO_USBWS_UTIL_H
#define _USBIP_WEBSOCK_POCO_USBWS_UTIL_H

#include <sstream>

namespace Usbip { namespace WebSock { namespace Poco {

using namespace ::Poco::Net;

class USBWSUtil
{
public:
	static int s2i(const std::string& s) {
		int i;
		std::stringstream ss(s);
		ss >> i;
		return i;
	};
	static unsigned long s2ul(const std::string& s) {
		unsigned long ul;
		std::stringstream ss(s);
		ss >> ul;
		return ul;
	};
	static int c2i(const char *c) {
		std::string s(c);
		return USBWSUtil::s2i(s);
	};
	static std::string i2s(int i) {
		std::stringstream ss;
		ss << i;
		return ss.str();
	};
};

}}}

#endif /* !_USBIP_WEBSOCK_POCO_USBWS_UTIL_H */

/*
* PARSEC - node_t class header
*
*  Author : Hans-Juergen Lange <hjl@simulated-universe.de>
*
*  This program is free software; you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation; either version 2 of the License, or
*  (at your option) any later version.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with this program; if not, write to the Free Software
*  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifndef _NET_NODE_H_
#define _NET_NODE_H_

#include <cstdint>
#include <cstring>

#include <arpa/inet.h>

// maximum number of bytes in a remote address --------------------------------
// (regardless of protocol; accommodates IPv6)
#define MAX_NODE_ADDRESS_BYTES	16

// encapsulate node address in portable manner --------------------------------
//
struct node_t {
	//
	//  The constructor creates a virtual node by setting the
	//  address array to all zeros.
	node_t() {
		memset(address, 0, sizeof(address));
	}
	inline bool operator==(const node_t& aOther) const;
	inline bool operator!=(const node_t& aOther) const;
	inline bool operator<(const node_t& aOther) const;
	inline bool operator>(const node_t& aOther) const;

	inline void setAddress(const struct sockaddr_in & aAddress);
	void setIP(const std::string& aIp );
	inline void setPort(uint16_t aPort) ;
	inline std::string getIP() const;
	inline uint16_t getPort() const;

	std::string print() const;
	inline bool isLocal() const;
	inline bool isVirtual() const ;

	uint8_t address[ MAX_NODE_ADDRESS_BYTES ];
};

bool node_t::operator==(const node_t &aOther) const {
	return (memcmp(address, aOther.address, sizeof(address)) == 0);
}

bool node_t::operator!=(const node_t &aOther) const {
	return !operator==(aOther);
}

bool node_t::operator<(const node_t &aOther) const {
	return (memcmp(address, aOther.address, sizeof(address)) < 0);
}

bool node_t::operator>(const node_t &aOther) const {
	return (memcmp(address, aOther.address, sizeof(address)) > 0);
}

void node_t::setAddress(const struct sockaddr_in &aAddress) {
	memcpy( address, &aAddress.sin_addr, sizeof(aAddress.sin_addr) );
	address[ 4 ] = aAddress.sin_port >> 8;  // TODO: probably wrong order of port number.
	address[ 5 ] = aAddress.sin_port & 0xff;
}

std::string node_t::getIP() const
{
	char ipString[INET_ADDRSTRLEN];

	inet_ntop( AF_INET, &address, ipString, INET_ADDRSTRLEN );

	return std::string(ipString);
}
//
//  convert aPort (network byte order) to storage (host byte order)
void node_t::setPort(uint16_t aPort) {
	address[ 4 ] = aPort >> 8;
	address[ 5 ] = aPort & 0xff;
}

//  convert from host to network byteorder.
uint16_t node_t::getPort() const {
	return ( ( (uint16_t)address[ 4 ] << 8 ) | address[ 5 ] );
}
//
//  The default construted node_t is used for local
bool node_t::isVirtual() const {
	node_t empty;

	return (*this == empty);
}

#endif // _NET_NODE_H_

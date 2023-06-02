// MIT License
//
// Copyright (c) 2023 Salomon Lee
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//

#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/timeb.h>
#include <util/ntp/NtpClient.hpp>
#include <util/logging/Log.hpp>
#ifdef __linux__
#include <cstring>
#endif

#define NTP_TIMESTAMP_DELTA 2208988800ull
#define NTP_RFC_ONE_SEC 4294967295

NtpClient::NtpClient( const std::string &hostname, int port) : hostname(hostname) , port(port){
    this->convert_to_timestamp(this->get_timestamp());
}

void NtpClient::error(std::string msg) {
}

NtpClient NtpClient::instance(std::string hostname, int port) {
    return NtpClient(hostname, port);
}

time_t NtpClient::get_timestamp() {
    packet = { 3, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    memset( &packet, 0, sizeof( ntp_pkt ) );

    // Set the first byte's bits to 00,011,011 for li = 0, vn = 3, and mode = 3. The rest will be left set to zero.

    *( ( char * ) &packet + 0 ) = 0x1b; // Represents 27 in base 10 or 00011011 in base 2.

    // Create a UDP socket, convert the host-name to an IP address, set the port number,
    // connect to the server, send the packet, and then read in the return packet.

    struct sockaddr_in serv_addr; // Server address data structure.
    struct hostent *server;      // Server data structure.

    socket_fd = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP ); // Create a UDP socket.
//    socket_fd = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP ); // Create a TCP socket.
    if ( socket_fd < 0 )
        LOG_ERROR << "ERROR opening socket" ;

    server = gethostbyname( hostname.c_str() ); // Convert URL to IP.

    if ( server == NULL )
        LOG_ERROR << "ERROR no such host" ;

    // Zero out the server address structure.
    bzero( ( char* ) &serv_addr, sizeof( serv_addr ) );

    serv_addr.sin_family = AF_INET;

    // Copy the server's IP address to the server address structure.
    bcopy( ( char* )server->h_addr, ( char* ) &serv_addr.sin_addr.s_addr, server->h_length );

    // Convert the port number integer to network big-endian style and save it to the server address structure.
    serv_addr.sin_port = htons( port );

    // Call up the server using its IP address and port number.

    if (connect(socket_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        LOG_ERROR << "ERROR connecting";

    // Send it the NTP packet it wants. If n == -1, it failed.
    socket_result = write(socket_fd, (char *)&packet, sizeof(ntp_pkt));

    if (socket_result < 0)
        LOG_ERROR << "ERROR writing to socket";

    // Wait and receive the packet back from the server. If n == -1, it failed.
    socket_result= read(socket_fd, (char *)&packet, sizeof( ntp_pkt ) );

    if (socket_result < 0)
        LOG_ERROR << "ERROR reading from socket";

    // These two fields contain the time-stamp seconds as the packet left the NTP server.
    // The number of seconds correspond to the seconds passed since 1900.
    // ntohl() converts the bit/byte order from the network's to host's "endianness".

    packet.txTm_s = ntohl( packet.txTm_s ); // Time-stamp seconds.
    packet.txTm_f = ntohl( packet.txTm_f ); // Time-stamp fraction of a second.

    // Extract the 32 bits that represent the time-stamp seconds (since NTP epoch) from when the packet left the server.
    // Subtract 70 years worth of seconds from the seconds since 1900.
    // This leaves the seconds since the UNIX epoch of 1970.
    // (1900)------------------(1970)**************************************(Time Packet Left the Server)

    time_t txTm = ( time_t ) ( packet.txTm_s - NTP_TIMESTAMP_DELTA );

    /*
    LOG << "Eight bits. Stratum level of the local clock.        " << packet.stratum;
    LOG << "Eight bits. Maximum interval between successive msg. " << packet.poll;
    LOG << "Eight bits. Precision of he local clock.             " << packet.precision;
    LOG << "32 bits. Total round trip delay time.                " << packet.rootDelay;
    LOG << "32 bits. Max error aloud from primary clock source.  " << packet.rootDispersion;
    LOG << "32 bits. Reference clock identifier.                 " << packet.refId;
    LOG << "32 bits. Reference time-stamp seconds.               " << packet.refTm_s;
    LOG << "32 bits. Reference time-stamp fraction of a second.  " << packet.refTm_f;
    LOG << "32 bits. Originate time-stamp seconds.               " << packet.origTm_s;
    LOG << "32 bits. Originate time-stamp fraction of a second.  " << packet.origTm_f;
    LOG << "32 bits. Received time-stamp seconds.                " << packet.rxTm_s;
    LOG << "32 bits. Received time-stamp fraction of a second.   " << packet.rxTm_f;
    LOG << "32 bits (most important field). TxTs seconds.        " << packet.txTm_s;
    LOG << "32 bits. Transmit time-stamp fraction of a second.   " << packet.txTm_f;
    // Print the time we got from the server, accounting for local timezone and conversion from UTC time.
    LOG << ctime( ( const time_t* ) &txTm );
     */
    return txTm;
}

void NtpClient::convert_to_timestamp(time_t t) {
    this->epoc_sec = t;
    epoc_msec = (double) t * 1000.0;
    epoc_msec += ((double) packet.txTm_f / NTP_RFC_ONE_SEC) * 1000.0;

    epoc_usec = (double) t * 1000000.0;
    epoc_usec += ((double) packet.txTm_f / NTP_RFC_ONE_SEC) * 1000000.0;
}

double NtpClient::getEpocSec() const {
    return epoc_sec;
}

unsigned long NtpClient::getEpocMSec() const {
    return epoc_msec;
}

double NtpClient::getEpocUSec() const {
    return epoc_usec;
}

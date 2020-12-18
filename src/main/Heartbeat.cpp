/*============================================================================*/
/* Heartbeat.cpp - heartbeat message                                          */
/*============================================================================*/

#include "StorageEngine.h"
#include "Heart.h"
#include "Heartbeat.hpp"
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>

/*
*  sAddr  The address to convert to a numeric IP.
*  iPort  The port number.
*/
HeartbeatSnd::HeartbeatSnd(char *szAddr, int iPort)
    : m_iPort(iPort) {

  std::string szPort = std::to_string(iPort);
  m_sAddr = szAddr;

  struct addrinfo hints;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_DGRAM;
  hints.ai_protocol = IPPROTO_UDP;

  int iRet = getaddrinfo(szAddr, szPort.c_str(), &hints, &m_pAddrInfo);
  if (iRet != 0 || m_pAddrInfo == NULL) {
    throw Heartbeat_RTE(("Invalid address or port: \"" + m_sAddr + ":" + szPort + "\"").c_str());
  }

  // set up destination address
  //
  struct sockaddr_in addr;
  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;

  if (m_sAddr.compare("BROADCAST") == 0) {
    // BROADCAST
    addr.sin_addr.s_addr = inet_addr("192.168.0.255");
  } else {
    // Unicast
    addr.sin_addr.s_addr = inet_addr(m_sAddr.c_str());
  }
  addr.sin_port = htons(m_iPort);

  // create socket
  m_iSocket = socket(AF_INET, SOCK_DGRAM, 0);
  if (m_iSocket < 0) {
    throw Heartbeat_RTE("Could not get socket");
  }

  // Set socket to allow broadcast
  // This also allows us to send to multiple processes in the same machine with a same port
  int broadcastPermission = 1;
  if (setsockopt(m_iSocket, SOL_SOCKET, SO_BROADCAST, (void *) &broadcastPermission,
                 sizeof(broadcastPermission)) < 0)
    perror("setsockopt(SO_BROADCAST) failed");

}

HeartbeatSnd::~HeartbeatSnd() {
  freeaddrinfo(m_pAddrInfo);
  close(m_iSocket);
}


/*
 */
int HeartbeatSnd::iSend(const char *pPacket, size_t iSize) {
  return sendto(
      m_iSocket,
      pPacket,
      iSize,
      0,
      m_pAddrInfo->ai_addr, m_pAddrInfo->ai_addrlen
  );

}



//===============================================================================================
/*
 *
 * sAddr  The address we receive on.
 * iPort  The port we receive from.
 */
HeartbeatRcv::HeartbeatRcv(const std::string &sAddr, int iPort)
    : m_iPort(iPort), m_sAddr(sAddr) {

  std::string strPort = std::to_string(iPort);

  // create socket
  m_iSocket = socket(AF_INET, SOCK_DGRAM, 0);
  if (m_iSocket < 0) {
    throw Heartbeat_RTE("Could not get socket");
  }

  // Set up a SOCKADDR_IN structure that will tell bind that we
  // want to receive datagrams from all interfaces using input port.

  memset(&m_AddrIn, 0, sizeof(m_AddrIn));
  // The IPv4 family
  m_AddrIn.sin_family = AF_INET;
  // Port no.
  m_AddrIn.sin_port = htons(m_iPort);
  // From all interface (0.0.0.0)
  m_AddrIn.sin_addr.s_addr = htonl(INADDR_ANY);
  m_AddrIn.sin_addr.s_addr = INADDR_ANY;


  int reuse = 1;
  if (setsockopt(m_iSocket, SOL_SOCKET, SO_REUSEADDR, (const char *) &reuse, sizeof(reuse)) < 0)
    perror("setsockopt(SO_REUSEADDR) failed");

  reuse = 1;
  if (setsockopt(m_iSocket, SOL_SOCKET, SO_REUSEPORT, (const char *) &reuse, sizeof(reuse)) < 0)
    perror("setsockopt(SO_REUSEPORT) failed");

  // Set socket to allow broadcast mode
  int iBroadcastPermission = 1;
  if (setsockopt(m_iSocket, SOL_SOCKET, SO_BROADCAST, (void *) &iBroadcastPermission,
                 sizeof(iBroadcastPermission)) < 0)
    perror("setsockopt(SO_BROADCAST) failed");


  int iRet = bind(m_iSocket, (struct sockaddr *) &m_AddrIn, sizeof(m_AddrIn));
  if (iRet != 0) {
    close(m_iSocket);
    throw Heartbeat_RTE(("could not bind UDP socket with: \"" + sAddr + ":" + strPort + "\"").c_str());
  }


}


HeartbeatRcv::~HeartbeatRcv() {

  close(m_iSocket);
}


/*
 * Returns  number of bytes read or -1 if an error occurs.
 */
int HeartbeatRcv::iRecv(char *pPacket, size_t iSize) {
  struct sockaddr_in SenderAddr;
  socklen_t len = sizeof(SenderAddr);

  struct timeval WaitTime;
  fd_set readfds;
  int iRet;

  WaitTime.tv_sec = 2;
  WaitTime.tv_usec = 0;

  FD_ZERO(&readfds);
  FD_SET(m_iSocket, &readfds);

  iRet = select(m_iSocket + 1, &readfds, NULL, NULL, &WaitTime);

  if (iRet > 0 && FD_ISSET(m_iSocket, &readfds)) {
    iRet = recvfrom(m_iSocket, (char *) pPacket, iSize,
                    MSG_WAITALL, (struct sockaddr *) &SenderAddr,
                    (socklen_t *) &len);

    // For debug logging only
    if (iRet > 0) {
      char *strSenderAddress = inet_ntoa(SenderAddr.sin_addr);
      unsigned short sPort = ntohs(SenderAddr.sin_port);

      //printf("received from %s %d\n", strSenderAddress, sPort);
    }

    // We can send back a message to the sender
    // or track wich node send to this one.
    // sendto(sockfd, (const char *)"Got it", strlen("Got it"),
    //      MSG_CONFIRM, (const struct sockaddr *) &SenderAddr,
    //           len);
  }

  return iRet;

}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

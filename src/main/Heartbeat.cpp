/*============================================================================*/
/* Heartbeat.cpp - heartbeat message                                          */
/*============================================================================*/



#include "base/BASE.h"

#include "Heart.h"
#include "Heartbeat.hpp"
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>

/**
 * @brief HeartbeatSnd Constructor
 * @details to create object and initialize variables
 * <p>
 * @param[in] pszAddr  the address to convert to a numeric IP.
 * @param[in] iPort   the port number.
 *
 * @return none
 *
 * @throws Heartbeat_RTE
 */
HeartbeatSnd::HeartbeatSnd(const std::string &sAddr, int iPort)
    : m_iPort(iPort), m_sAddr(sAddr) {

  std::string szPort = std::to_string(iPort);

  struct addrinfo hints;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_DGRAM;
  hints.ai_protocol = IPPROTO_UDP;

  if (m_sAddr.compare("BROADCAST") == 0) {
    m_sAddr = "255.255.255.255";
  }
  int iRet = getaddrinfo(m_sAddr.c_str(), szPort.c_str(), &hints, &m_pAddrInfo);
  if (iRet != 0 || m_pAddrInfo == NULL) {
    throw Heartbeat_RTE(("Invalid address or port: \"" + m_sAddr + ":" + szPort + "\"").c_str());
  }


  // set up destination address
  //
  struct sockaddr_in addr;
  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;

  addr.sin_addr.s_addr = inet_addr(m_sAddr.c_str());

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

/**
 * @brief HeartbeatRcv Constructor
 * @details to create object and initialize variables
 * <p>
 * @param[in] szAddr  The address we receive on.
 * @param[in] iPort   The port we receive from.
 *
 *
 * @return none
 *
 *
 * @throws Heartbeat_RTE
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

      (void)strSenderAddress;
      (void)sPort;
      //printf("received from %s %d\n", strSenderAddress, sPort);
    }

    // We can send back a message to the sender
    // or track which node send to this one.
    // sendto(sockfd, (const char *)"Got it", strlen("Got it"),
    //      MSG_CONFIRM, (const struct sockaddr *) &SenderAddr,
    //           len);
  }

  return iRet;

}


TestConnection::TestConnection(Stz *pAddr, i8 iPort)
{
  m_pszAddr = pAddr->azChar();
  m_iPort = iPort;
  m_iSockClient = -1;
}
TestConnection::~TestConnection(void)
{
  if (m_iSockClient != -1)
  {
    close(m_iSockClient);
  }
}


bool TestConnection::bTryConnect(void)
{
  struct sockaddr_in xNodeAddr;

  if ((m_iSockClient = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    vTrace("\n Socket creation error \n"_Stz);
    return false;
  }

  xNodeAddr.sin_family = AF_INET;
  xNodeAddr.sin_port = htons(m_iPort);

  // Convert IPv4 and IPv6 addresses from text to binary form
  if (inet_pton(AF_INET, m_pszAddr, &xNodeAddr.sin_addr)<=0)
  {
    vTrace("\nInvalid address/ Address not supported \n"_Stz);
    return false;
  }

  if (connect(m_iSockClient, (struct sockaddr *)&xNodeAddr, sizeof(xNodeAddr)) < 0)
  {
    vTrace("\nConnection Failed \n"_Stz);
    return false;
  }
  close(m_iSockClient);
  return true;
}


/**
 * @brief Checksum Constructor
 * @details has two methods to get the checksum/hash values
 * Fletcher16 and Bernstein
 * <p>
 * @param[in] pData  pointer to char buffer
 * @param[in] iLen   length of data
 *
 *
 * @return none
 *
 */
CheckSum::CheckSum (const byte *pData, size_t iLen)
{
  m_pData = pData;
  m_iDataLen = iLen;
}


/**
 * @brief Checksum by Fletcher
 * @details uses 8-bit blocks, it is not affected by byte endianness.
 * <p>
 *
 * @return 16-bit value
 *
 */
u2 CheckSum::uGetFletcher16(void)
{
  u4 c0 = 0, c1 = 0;
  m_uValue = 0;
  /*  Found by solving for c1 overflow: */
  /* n > 0 and n * (n+1) / 2 * (2^8-1) < (2^32-1). */
  while (m_iDataLen > 0) {
    size_t blocklen = m_iDataLen;
    if (blocklen > 5002) {
      blocklen = 5002;
    }
    m_iDataLen -= blocklen;

    do {
      c0 = c0 + *m_pData++;
      c1 = c1 + c0;
    } while (--blocklen);

    c0 = c0 % 255;
    c1 = c1 % 255;
  }
  m_uValue = (c1 << 8 | c0);
  return m_uValue;
}


/**
 * @brief Hash by Bernstein
 * @details An algorithm produced by Professor Daniel J. Bernstein
 * <p>
 *
 * @return i8 value
 *
 */
i8 CheckSum::iGetBernstein(void)
{
  i8 hash = 5381;
  unsigned int i    = 0;

  for (i = 0; i < m_iDataLen; ++m_pData, ++i)
  {
    hash = ((hash << 5) + hash) + (*m_pData);
  }

  return hash & (1024-1);

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

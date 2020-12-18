/*============================================================================*/
/* Heartbeat.hpp -- heartbeat message                                         */
/*============================================================================*/
#pragma once
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdexcept>
#include "MachineTypes.h"
using namespace Resources;


class Heartbeat {
  
public:
  
  i8   m_iHeader;            //   8    8
  i8   m_iSeqNo;             //   8   16
  i8   m_iSincePosixEpoch;   //   8   24
  char m_acBuildGitHash[32]; //  32   56
  i8   m_iValue0;            //   8   64
  i8   m_iValue1;            //   8   72
  i8   m_iValue2;            //   8   80
  char m_acString0[32];      //  32  112
  char m_acString1[32];      //  32  144
  char m_acString2[32];      //  32  176
  i8   m_iChecksum;          //   8  184
  i8   m_iFooter;            //   8  192
  char m_acSpare[304];       // 304  496
                             // ---
                             // 496 bytes
};

class HeartbeatSnd
{
public:
  HeartbeatSnd(char* sAddr, int iPort);
  ~HeartbeatSnd();

  int                 iSocket() const {return m_iSocket;}
  int                 iPort() const {return m_iPort;}
  std::string         sAddr() const {return m_sAddr;}

  int                 iSend(const char *msg, size_t size);

private:
  int                 m_iSocket;
  int                 m_iPort;
  std::string         m_sAddr;
  struct addrinfo *   m_pAddrInfo;
};


class HeartbeatRcv
{
public:
  HeartbeatRcv(const std::string& addr, int port);
  ~HeartbeatRcv();

  int                 iSocket() const {return m_iSocket;}
  int                 iPort() const {return m_iPort;}
  std::string         sAddr() const {return m_sAddr;}


  int                 iRecv(char *msg, size_t max_size);


private:
  int                 m_iSocket;
  int                 m_iPort;
  std::string         m_sAddr;
  //struct addrinfo     m_AddrInfo;
  sockaddr_in         m_AddrIn;
};

class Heartbeat_RTE : public std::runtime_error
{
public:
  Heartbeat_RTE(const char *w) : std::runtime_error(w) {}
};


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/


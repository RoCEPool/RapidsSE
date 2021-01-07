/*============================================================================*/
/* Heartbeat.hpp -- heartbeat message                                         */
/*============================================================================*/
#pragma once
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdexcept>
#include <memory>
#include <mutex>
#include "MachineTypes.h"
#include <base/BASE.h>

#include <stdatomic.h>

/*----------------------------------------------------------------------------*/
/* Heartbeat class contains data for heartbeat                                */
/*----------------------------------------------------------------------------*/
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

/*------------------------------------------------------------------------------------*/
/* RingBuffer class contains to receive for heartbeat                                 */
/* If our system is a single producer - single consumer, we can get away without lock */
/* For now, to be safe...                                                             */
/*------------------------------------------------------------------------------------*/
template<class T>
class RingBuffer {
private:
  //std::mutex m_mutexHB;
  //size_t m_iWriteIdx;
  //size_t m_iReadIdx;

  std::unique_ptr<T[]> m_tBuffer;


  // Using C++11 atomic here
  std::atomic<int64_t> m_atmWrite;
  std::atomic<int64_t> m_atmRead;

  const size_t m_iMaxSize;
  bool m_bFull;

public:
  explicit RingBuffer(size_t iMaxSize) :
      m_tBuffer(std::unique_ptr<T[]>(new T[iMaxSize])),
      m_iMaxSize(iMaxSize) {
    //m_iWriteIdx = 0;
    //m_iReadIdx = 0;

    m_atmWrite.store(0,std::memory_order_release);
    m_atmRead.store(0, std::memory_order_release);
    m_bFull = false;
  }

  /*
  void vWrite(const T& tItem) {
    std::lock_guard<std::mutex> lock(m_mutexHB);

    m_tBuffer[m_iWriteIdx] = tItem;

    if (m_bFull) {
      m_iReadIdx = (m_iReadIdx + 1) % m_iMaxSize;
    }

    m_iWriteIdx = (m_iWriteIdx + 1) % m_iMaxSize;

    //If read is ahead the write by 1, we are full
    m_bFull = m_iWriteIdx == m_iReadIdx;
  } // Lock is automatically released here

  T tRead() {
    std::lock_guard<std::mutex> lock(m_mutexHB);

    if (bEmpty()) {
      return T();// return empty value
    }

    //Read data and advance the read index (we now have a free space)
    auto val = m_tBuffer[m_iReadIdx];
    m_bFull = false;
    m_iReadIdx = (m_iReadIdx + 1) % m_iMaxSize;

    return val;
  } // Lock is automatically released here

  bool bRead(T& tItem) {
    std::lock_guard<std::mutex> lock(m_mutexHB);

    if (bEmpty()) {
      return false;// return false for empty
    }

    //Read data and advance the read index (we now have a free space)
    tItem = m_tBuffer[m_iReadIdx];
    m_bFull = false;
    m_iReadIdx = (m_iReadIdx + 1) % m_iMaxSize;

    return true;
  } // Lock is automatically released here
 */

  // Atomic operation here
  bool bTryWrite(const T& tItem) {

    auto const currentWrite = m_atmWrite.load(std::memory_order_relaxed);

    auto nextWrite = (currentWrite + 1) % m_iMaxSize;

    if (nextWrite != m_atmRead.load(std::memory_order_acquire)) {
      m_tBuffer[currentWrite] = tItem;
      m_atmWrite.store(nextWrite, std::memory_order_release);
      return true;
    }

    // queue is full
    return false;
  }

  // Atomic operation here
  bool bTryRead(T& tItem)
  {
    auto const currentRead= m_atmRead.load(std::memory_order_relaxed);

    if (currentRead == m_atmWrite.load(std::memory_order_acquire))
    {
      return false; // return false for empty
    }

    //Read data and advance the read index (we now have a free space)
    tItem = m_tBuffer[currentRead];
    m_bFull = false;

    m_atmRead.store((currentRead + 1) % m_iMaxSize, std::memory_order_release);

    return true;
  }


/*
  void vReset() {
    std::lock_guard<std::mutex> lock(m_mutexHB);
    m_iWriteIdx = m_iReadIdx;
    m_bFull = false;
  }

  bool bEmpty() const {
    //if head and tail are equal, we are empty
    return (!m_bFull && (m_iWriteIdx == m_iReadIdx));
  }

  bool bFull() const {
    //If tail is ahead the head by 1, we are full
    return m_bFull;
  }

  size_t iSize() const {
    size_t size = m_iMaxSize;

    if (!m_bFull) {
      if (m_iWriteIdx >= m_iReadIdx) {
        size = m_iWriteIdx - m_iReadIdx;
      } else {
        size = m_iMaxSize + m_iWriteIdx - m_iReadIdx;
      }
    }
  */

  size_t iMaxSize() const {
    return m_iMaxSize;
  }

  size_t iSize() const {
    size_t size = m_iMaxSize;

    auto iWriteIdx = m_atmWrite.load(std::memory_order_relaxed);
    auto iReadIdx = m_atmRead.load(std::memory_order_relaxed);
    if (!m_bFull) {
      if (iWriteIdx >= iReadIdx) {
        size = iWriteIdx - iReadIdx;
      } else {
        size = m_iMaxSize + iWriteIdx - iReadIdx;
      }
    }

    return size;
  }

  // For debug only
  size_t iWriteIdx() const { return m_atmWrite.load(std::memory_order_relaxed);; }//m_iWriteIdx; }

  size_t iReadIdx() const { return m_atmRead.load(std::memory_order_relaxed);; } //m_iReadIdx; }

};


class HeartbeatSnd {
public:
  HeartbeatSnd(char *sAddr, int iPort);

  ~HeartbeatSnd();

  int iSocket() const { return m_iSocket; }

  int iPort() const { return m_iPort; }

  std::string sAddr() const { return m_sAddr; }

  int iSend(const char *msg, size_t size);

private:
  int m_iSocket;
  int m_iPort;
  std::string m_sAddr;
  struct addrinfo *m_pAddrInfo;
};


class HeartbeatRcv {
public:
  HeartbeatRcv(const std::string &addr, int port);

  ~HeartbeatRcv();

  int iSocket() const { return m_iSocket; }

  int iPort() const { return m_iPort; }

  std::string sAddr() const { return m_sAddr; }


  int iRecv(char *msg, size_t max_size);


private:
  int m_iSocket;
  int m_iPort;
  std::string m_sAddr;
  //struct addrinfo     m_AddrInfo;
  sockaddr_in m_AddrIn;
};

class Heartbeat_RTE : public std::runtime_error {
public:
  Heartbeat_RTE(const char *w) : std::runtime_error(w) {}
};


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/


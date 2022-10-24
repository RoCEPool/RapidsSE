/*============================================================================*/
/* BufferTransport.hpp - A Thrift transport that reads & writes from a buffer */
/*============================================================================*/

#include "BASE.h"
#include "BufferTransport.hpp"

using namespace wlp;

/*----------------------------------------------------------------------------*/
/* Constructor                                                                */
/*----------------------------------------------------------------------------*/
BufferTransport::BufferTransport(char* pcBuf, u8 iLen)
: apache::thrift::transport::TTransport(),
  m_pcBuf(pcBuf), m_iLen(iLen), m_iNext(0)
{}

/*----------------------------------------------------------------------------*/
/* Destructor                                                                 */
/*----------------------------------------------------------------------------*/
BufferTransport::~BufferTransport()
{}


/*----------------------------------------------------------------------------*/
/* Allocate a byte for a wireline message header and return a ptr to it       */
/*----------------------------------------------------------------------------*/
WlpHeader* BufferTransport::pAllocHeader(MessageTypes::type eType)
{
  WlpHeader* pHeader = (WlpHeader*) (m_pcBuf + m_iNext);
  m_iNext += sizeof(WlpHeader);

  pHeader->m_iMsgType = eType;
  return pHeader;
}

/*----------------------------------------------------------------------------*/
/* Returns the number of bytes written to the buffer                          */
/*----------------------------------------------------------------------------*/
i8 BufferTransport::iBytesWritten()
{
  return m_iNext;
}

/*----------------------------------------------------------------------------*/
/* Returns the number of bytes read from the buffer                           */
/*----------------------------------------------------------------------------*/
i8 BufferTransport::iBytesRead()
{
  return iBytesWritten();
}

/*----------------------------------------------------------------------------*/
/* Returns the number of bytes still to be read or written in the buffer      */
/*----------------------------------------------------------------------------*/
i8 BufferTransport::iBytesLeft()
{
  return (i8) (m_iLen - m_iNext);
}

/*----------------------------------------------------------------------------*/
/* Return whether the transport is "open". This is always open.               */
/*----------------------------------------------------------------------------*/
bool BufferTransport::isOpen() const
{
  return true;
}


/*----------------------------------------------------------------------------*/
/* Returns true if there is more data to read or write                        */
/*----------------------------------------------------------------------------*/
bool BufferTransport::peek()
{
  return m_iNext < m_iLen;
}


void BufferTransport::open()
{
  // No-op
}


void BufferTransport::close()
{
  // No-op
}

/*----------------------------------------------------------------------------*/
/* Attempt to read up to the specified number of bytes into the buffer.       */
/* Returns how many bytes were actually read.                                 */
/*----------------------------------------------------------------------------*/
uint32_t BufferTransport::read_virt(uint8_t* pBuf, uint32_t iLen)
{
  i8 iRemaining = (i8)(m_iLen - m_iNext);

  if (iLen == 0 || iRemaining == 0) {
    return 0;
  }

  if (iLen > iRemaining) {
    iLen = iRemaining;
  }

  memcpy(pBuf, m_pcBuf + m_iNext, iLen);
  m_iNext += iLen;
  return iLen;
}


/*----------------------------------------------------------------------------*/
/* Write the specified number of bytes into the buffer.                       */
/*----------------------------------------------------------------------------*/
void BufferTransport::write_virt(const uint8_t* piBuf, uint32_t iLen)
{
  i8 iRemaining = (i8)(m_iLen - m_iNext);
  if (iLen > iRemaining) {
    throw apache::thrift::transport::TTransportException( "Buffer is full.");
  }

  memcpy(m_pcBuf + m_iNext, piBuf, iLen);
  m_iNext += iLen;
}


/*----------------------------------------------------------------------------*/
/* Attempts to return a pointer to \c len bytes, possibly copied into \c buf. */
/* Does not consume the bytes read (i.e.: a later read will return the same   */
/* data).  This method is meant to support protocols that need to read        */
/* variable-length fields.  They can attempt to borrow the maximum amount of  */
/* data that they will need, then consume (see next method) what they         */
/* actually use.                                                              */
/*  pBuf - A buffer where the data can be stored if needed.                   */
/*  piLen - *len should initially contain the number of bytes to borrow.      */
/*          If borrow succeeds, *len will contain the number of bytes         */
/*          available in the returned pointer.  This will be at least         */
/*          what was requested, but may be more if borrow returns             */
/*          a pointer to an internal buffer, rather than pBuf.                */
/* Returns a pointer to the borrowed data. This might be equal to pBuf, or    */
/* it might be a pointer into the transport's internal buffers.               */
/*----------------------------------------------------------------------------*/
const uint8_t* BufferTransport::borrow_virt(uint8_t* pBuf, uint32_t* piLen)
{
  if (!piLen) {
    return nullptr;
  }

  i8 iRemaining = (i8)(m_iLen - m_iNext);

  if (*piLen == 0 || iRemaining == 0) {
    return nullptr;
  }

  if (*piLen < iRemaining) {
    *piLen = iRemaining;
  }

  if (pBuf) {
    memcpy(pBuf, m_pcBuf + m_iNext, *piLen);
    return pBuf;
  }

  return (uint8_t*)(m_pcBuf + m_iNext);
}

/*----------------------------------------------------------------------------*/
/* Remove len bytes from the transport.                                       */
/*----------------------------------------------------------------------------*/
void BufferTransport::consume_virt(uint32_t iLen)
{
  m_iNext += iLen;

  if (m_iNext > m_iLen) {
    m_iNext = m_iLen;
  }
}

/*============================================================================*/
/* ScanBuffer.cpp -- Scanning Buffer                                          */
/*============================================================================*/

#include "DataStructures.h"

extern BufferPool* gpBufferPool;

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* ScanBuffer::operator new(size_t)
{
  Buffer* pBuffer = gpBufferPool->pAllocBuffer();
  
  return pPoolCheck<void*>(pBuffer);
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void ScanBuffer::operator delete(void* pBufferAddress)
{
  Buffer* pBuffer = static_cast<Buffer*>(pBufferAddress);
  
  gpBufferPool->vFree(pBuffer);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ScanBuffer::ScanBuffer(void)
: Buffer(CN_SCANBUFFER)
{
  _poolAddress_(this);
  _magic_();
  
  vReset();
  
  ASSERT(m_pEndData);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
ScanBuffer::~ScanBuffer(void)
{
  _poolAddress_(this);
  _magic_();
}

/*----------------------------------------------------------------------------*/
/* get into area and number of available bytes                                */
/*----------------------------------------------------------------------------*/
char* ScanBuffer::pInto(i8& iAvailableBytes)
{
  //
  // a) no previous dangling portion
  //
  // v- pFirstByte()                                          pLastByte() -v
  // +---------------------------------------------------------------------+
  // | ..available..                                                       |
  // +---------------------------------------------------------------------+
  // ^- m_pEndData
  //
  
  //
  // b) previous dangling portion
  //
  // v- pFirstByte()                                          pLastByte() -v
  // +---------------------------------------------------------------------+
  // | dangling...| ..available..                                          |
  // +---------------------------------------------------------------------+
  //              ^- m_pEndData (of copied up dangling portion)
  //
  
  iAvailableBytes = iBytesAvailable();
  
  ASSERT(m_pEndData >= pFirstByte());
  ASSERT(m_pEndData <= pLastByte());
  return m_pEndData;
}

/*----------------------------------------------------------------------------*/
/* increment number of data bytes in the workarea                             */
/*----------------------------------------------------------------------------*/
void ScanBuffer::vMore(i8& iMoreBytes)
{
  //
  // a) data fills workarea
  //
  // v- pFirstByte()                                          pLastByte() -v
  // +---------------------------------------------------------------------+
  // | data...                                                             |
  // +---------------------------------------------------------------------+
  //                                                           m_pEndData -^
  //
  // b) data doesnt fill workarea
  //
  // v- pFirstByte()                                          pLastByte() -v
  // +---------------------------------------------------------------------+
  // | data...                                                | ..no data..|
  // +---------------------------------------------------------------------+
  //                                              m_pEndData -^
  
  m_pEndData = pcAdd(m_pEndData,iMoreBytes - 1);
  
  m_pNextMsg = pFirstByte();
  ASSERT(m_pEndData >= pFirstByte());
  ASSERT(m_pEndData <= pLastByte());
}

/*----------------------------------------------------------------------------*/
/* scan for next message                                                      */
/*----------------------------------------------------------------------------*/
char* ScanBuffer::pScan(i8& iMessageBytes)
{
  //
  // v- pFirstByte()                                          pLastByte() -v
  // +---------------------------------------------------------------------+
  // | msg1\nmsg2\nmsg3\n...                                               |
  // +---------------------------------------------------------------------+
  //         ^- m_pNextMsg                                     m_pEndData -^
  //
  
  // initial case of no data
  if ( ! m_pNextMsg ) {
    iMessageBytes = 0;
    ASSERT(m_pEndData >= pFirstByte());
    ASSERT(m_pEndData <= pLastByte());
    return nullptr;
  }
  
  // look for end of next record
  char* pMsg = m_pNextMsg;
  while (m_pNextMsg <= m_pEndData) {
  
    if (*m_pNextMsg == '\n') {
      iMessageBytes = iSub(m_pNextMsg,pMsg);
      ASSERT(iMessageBytes <= C_WORKAREA);
      m_pNextMsg++;
      ASSERT(m_pEndData >= pFirstByte());
      ASSERT(m_pEndData <= pLastByte());
      return pMsg;
    }
    
    m_pNextMsg++;
  }
  
  // no records left, copy dangling bytes to start of workarea
  vCopyUp(pMsg);
  
  iMessageBytes = 0;
  ASSERT(m_pEndData >= pFirstByte());
  ASSERT(m_pEndData <= pLastByte());
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* calc available bytes                                                       */
/*----------------------------------------------------------------------------*/
i8 ScanBuffer::iBytesAvailable(void)
{
  //
  // a) no previous dangling portion
  //
  // v- pFirstByte()                                          pLastByte() -v
  // +---------------------------------------------------------------------+
  // | ..available..                                                       |
  // +---------------------------------------------------------------------+
  // ^- m_pEndData
  //
  
  //
  // b) previous dangling portion
  //
  // v- pFirstByte()                                          pLastByte() -v
  // +---------------------------------------------------------------------+
  // | dangling...| ..available..                                          |
  // +---------------------------------------------------------------------+
  //              ^- m_pEndData (of prev copied up dangling portion)
  //
  ASSERT(m_pEndData >= pFirstByte());
  ASSERT(m_pEndData <= pLastByte());
  
  i8 iBytes = iSub(pLastByte(),m_pEndData) + 1;
  if (iBytes > C_WORKAREA) {
    i8 i = 0;
  }
  ASSERT(iBytes <= C_WORKAREA);
  
  return iBytes;
}

/*----------------------------------------------------------------------------*/
/* copy dangling portion to beginning of workarea                             */
/*----------------------------------------------------------------------------*/
void ScanBuffer::vCopyUp(char* pDangling)
{
  // used all of the workarea?
  if (pDangling >= pLastByte()) {
    vReset();
    return;
  }
  
  //
  // v- pFirstByte()                                          pLastByte() -v
  // +---------------------------------------------------------------------+
  // | msg1\nmsg2\nmsg3\n...                                 | dangling... |
  // +---------------------------------------------------------------------+
  //                                              pDangling -^
  //
  //
  // v- pFirstByte()                                          pLastByte() -v
  // +---------------------------------------------------------------------+
  // | dangling... | ..available..                                         |
  // +---------------------------------------------------------------------+
  //               ^- m_pEndData (of copied up dangling portion)
  //
  
  ASSERT(pDangling >= pFirstByte());
  ASSERT(pDangling <= pLastByte());
  
  // copy dangling data bytes to begining of workarea
  i8 iDanglingBytes = iSub(pLastByte(),pDangling);
  ASSERT(iDanglingBytes <= C_WORKAREA);
  Memory::vCopy(pFirstByte(),pDangling,iDanglingBytes);
  m_pEndData = pcAdd(pFirstByte(),iDanglingBytes - 1);
  if (m_pEndData < pFirstByte())
    i8 i = 0;
  
  ASSERT(m_pEndData >= pFirstByte());
  ASSERT(m_pEndData <= pLastByte());
  
  // invalidate starting position (requires more)
  m_pNextMsg = nullptr;
  
  // wipe the remaining data area (debug)
  vWipe(m_pEndData+1);
}

/*----------------------------------------------------------------------------*/
/* first byte of workarea                                                     */
/*----------------------------------------------------------------------------*/
char* ScanBuffer::pFirstByte(void)
{
  char* pFirst = &m_wWorkarea[0];
  
  return pFirst;
}

/*----------------------------------------------------------------------------*/
/* last byte of workarea                                                      */
/*----------------------------------------------------------------------------*/
char* ScanBuffer::pLastByte(void)
{
  char* pLast = &m_wWorkarea[C_WORKAREA - 1];
  
  return pLast;
}

/*----------------------------------------------------------------------------*/
/* reset to initial position (no data)                                        */
/*----------------------------------------------------------------------------*/
void ScanBuffer::vReset(void)
{
  //
  // v- pFirstByte()                                          pLastByte() -v
  // +---------------------------------------------------------------------+
  // | available...                                                        |
  // +---------------------------------------------------------------------+
  // ^- m_pEnd
  //
  
  m_pEndData = pFirstByte();
  ASSERT(m_pEndData >= pFirstByte());
  ASSERT(m_pEndData <= pLastByte());
  
  m_pNextMsg = nullptr;
  vWipe();
  
  ASSERT(C_WORKAREA == iBytesAvailable());
}

/*----------------------------------------------------------------------------*/
/* wipe work area                                                             */
/*----------------------------------------------------------------------------*/
void ScanBuffer::vWipe(void)
{
  Memory::vSet(m_wWorkarea,0xEF,C_WORKAREA);
}

/*----------------------------------------------------------------------------*/
/* wipe work area                                                             */
/*----------------------------------------------------------------------------*/
void ScanBuffer::vWipe(char* pFrom)
{
  ASSERT(pFrom >= m_wWorkarea);
  
  i8 iBytes = iSub(pLastByte(),pFrom);
  
  Memory::vSet(pFrom,0xEF,iBytes);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
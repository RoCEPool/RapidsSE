/*============================================================================*/
/* ScanBuffer.cpp -- Scanning Buffer                                          */
/*============================================================================*/

#include "Structures.h"

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
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCANBUFFER,_FOOTER_);

  vReset();
  
  ASSERT(m_pEndData);
  
  Memory::vCheckInitialised(this,sizeof(ScanBuffer));
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_SCANBUFFER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
ScanBuffer::~ScanBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCANBUFFER,_FOOTER_);
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
  
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCANBUFFER,_FOOTER_);
  
  iAvailableBytes = iBytesAvailable();
  
  ASSERT(m_pEndData >= pFirstByte() - 1);
  ASSERT(m_pEndData <= pLastByte());
  
  return m_pEndData + 1;
}

/*----------------------------------------------------------------------------*/
/* increment number of data bytes in the workarea                             */
/*----------------------------------------------------------------------------*/
void ScanBuffer::vMore(i8& iBytesRead)
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
  
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCANBUFFER,_FOOTER_);
  
  m_pEndData = pcAdd(m_pEndData,iBytesRead);
  
  m_pNextMsg = pFirstByte();
  ASSERT(m_pEndData >= pFirstByte() - 1);
  ASSERT(m_pEndData <= pLastByte());
  
  
//  printf("More: ");
//  for (char* c = pFirstByte(); c <= pFirstByte() + 1000; c++) {
//    if (*c == '\n')
//      break;
//    printf("%c",*c);
//  }
//  printf("\n");
  
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
  
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCANBUFFER,_FOOTER_);
  
  // initial case of no data
  if ( ! m_pNextMsg ) {
    iMessageBytes = 0;
    ASSERT(m_pEndData >= pFirstByte() - 1);
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
      ASSERT(m_pEndData >= pFirstByte() - 1);
      ASSERT(m_pEndData <= pLastByte());
      return pMsg;
    }
    
    m_pNextMsg++;
  }
  
  // no records left, copy dangling bytes to start of workarea
  vCopyUp(pMsg);
  
  iMessageBytes = 0;
  ASSERT(m_pEndData >= pFirstByte() - 1);
  ASSERT(m_pEndData <= pLastByte());
  m_pNextMsg = nullptr;
  
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
  
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCANBUFFER,_FOOTER_);
  
  ASSERT(m_pEndData >= pFirstByte() - 1);
  ASSERT(m_pEndData <= pLastByte());
  
  i8 iBytes = iSub(pLastByte(),m_pEndData);
  ASSERT(iBytes <= C_WORKAREA);
  
  return iBytes;
}

/*----------------------------------------------------------------------------*/
/* copy dangling portion to beginning of workarea                             */
/*----------------------------------------------------------------------------*/
void ScanBuffer::vCopyUp(char* pDangling)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCANBUFFER,_FOOTER_);
  
  // used all of the workarea?
  if (pDangling > pLastByte()) {
    vReset();
    ASSERT(m_pEndData >= pFirstByte() - 1);
    ASSERT(m_pEndData <= pLastByte());
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
  
  ASSERT(pDangling >= pFirstByte() - 1);
  ASSERT(pDangling <= pLastByte());
  
  // copy dangling data bytes to begining of workarea
  i8 iDanglingBytes = iSub(pLastByte(),pDangling) + 1;
  ASSERT(iDanglingBytes <= C_WORKAREA);
  
  Memory::vCopy(pFirstByte(),pDangling,iDanglingBytes);
  m_pEndData = pcAdd(pFirstByte(),iDanglingBytes - 1);
  
//  if (iDanglingBytes < 1000) {
//    printf("CopyUp %lld: ",iDanglingBytes);
//    for (char* c = pFirstByte(); c <= m_pEndData; c++) {
//      ASSERT(*c != 0xEF);
//      printf("%c",*c);
//    }
//    printf("\n");
//  }
  
  ASSERT(m_pEndData >= pFirstByte() - 1);
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCANBUFFER,_FOOTER_);
  
  char* pFirst = &m_wWorkarea[0];
  
  return pFirst;
}

/*----------------------------------------------------------------------------*/
/* last byte of workarea                                                      */
/*----------------------------------------------------------------------------*/
char* ScanBuffer::pLastByte(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCANBUFFER,_FOOTER_);
  
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
  
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCANBUFFER,_FOOTER_);
  
  m_pEndData = pFirstByte() - 1;
  ASSERT(m_pEndData >= pFirstByte() - 1);
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCANBUFFER,_FOOTER_);
  
  Memory::vSet(m_wWorkarea,0xEF,C_WORKAREA);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_SCANBUFFER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* wipe work area                                                             */
/*----------------------------------------------------------------------------*/
void ScanBuffer::vWipe(char* pFrom)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCANBUFFER,_FOOTER_);
  
  ASSERT(pFrom >= m_wWorkarea);
  
  i8 iBytes = iSub(pLastByte(),pFrom) + 1;
  
  Memory::vSet(pFrom,0xEF,iBytes);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_SCANBUFFER,_FOOTER_);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
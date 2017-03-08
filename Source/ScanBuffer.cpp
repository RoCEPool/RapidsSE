/*============================================================================*/
/* ScanBuffer.cpp -- Scanning Buffer                                          */
/*============================================================================*/

#include "Structures.h"

extern BufferPool* gpBufferPool;

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* ScanBuffer::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  Buffer* pBuffer = (Buffer*)Heap::pAllocate(CN_SCANBUFFER);
  
  return pPoolCheck<void*>(pBuffer);
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void ScanBuffer::operator delete(void* aBufferAddress)
{
  Buffer* pBuffer = static_cast<Buffer*>(aBufferAddress);
  
  Heap::vFree(pBuffer);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ScanBuffer::ScanBuffer(void)
: Buffer(CN_SCANBUFFER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCANBUFFER,_FOOTER_);

  vReset();
  
  ASSERT(m_aEndData);
  
  _checkInitialised_(CN_SCANBUFFER);
  
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
Str* ScanBuffer::aInto(i8& iAvailableBytes)
{
  //
  // a) no previous dangling portion
  //
  // v- pFirstByte()                                          pLastByte() -v
  // +---------------------------------------------------------------------+
  // | ..available..                                                       |
  // +---------------------------------------------------------------------+
  // ^- m_aEndData
  //
  
  //
  // b) previous dangling portion
  //
  // v- pFirstByte()                                          pLastByte() -v
  // +---------------------------------------------------------------------+
  // | dangling...| ..available..                                          |
  // +---------------------------------------------------------------------+
  //              ^- m_aEndData (of copied up dangling portion)
  //
  
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCANBUFFER,_FOOTER_);
  
  iAvailableBytes = iBytesAvailable();
  
  ASSERT(m_aEndData >= aFirstByte() - 1);
  ASSERT(m_aEndData <= aLastByte());
  
  return m_aEndData + 1;
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
  //                                                           m_aEndData -^
  //
  // b) data doesnt fill workarea
  //
  // v- pFirstByte()                                          pLastByte() -v
  // +---------------------------------------------------------------------+
  // | data...                                                | ..no data..|
  // +---------------------------------------------------------------------+
  //                                              m_aEndData -^
  
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCANBUFFER,_FOOTER_);
  
  m_aEndData = pAdd<Str*>(m_aEndData,iBytesRead);
  
  m_aNextMsg = aFirstByte();
  ASSERT(m_aEndData >= aFirstByte() - 1);
  ASSERT(m_aEndData <= aLastByte());
  
  
//  vInfo("More: "_Str);
//  for (char* c = pFirstByte(); c <= pFirstByte() + 1000; c++) {
//    if (*c == '\n')
//      break;
//    vInfo("%c"_Str,*c);
//  }
//  vInfo("\n"_Str);
  
}

/*----------------------------------------------------------------------------*/
/* scan for next message                                                      */
/*----------------------------------------------------------------------------*/
Str* ScanBuffer::aScan(i8& nMessageBytes)
{
  //
  // v- pFirstByte()                                          pLastByte() -v
  // +---------------------------------------------------------------------+
  // | msg1\nmsg2\nmsg3\n...                                               |
  // +---------------------------------------------------------------------+
  //         ^- m_aNextMsg                                     m_aEndData -^
  //
  
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCANBUFFER,_FOOTER_);
  
  // initial case of no data
  if ( ! m_aNextMsg ) {
    nMessageBytes = 0;
    ASSERT(m_aEndData >= aFirstByte() - 1);
    ASSERT(m_aEndData <= aLastByte());
    return nullptr;
  }
  
  // look for end of next record
  Str* pMsg = m_aNextMsg;
  while (m_aNextMsg <= m_aEndData) {
  
    if (*(char*)m_aNextMsg == '\n') {
      nMessageBytes = nSub(m_aNextMsg,pMsg);
      ASSERT(nMessageBytes <= m_xWsp.nLength());
      m_aNextMsg++;
      ASSERT(m_aEndData >= aFirstByte() - 1);
      ASSERT(m_aEndData <= aLastByte());
      return pMsg;
    }
    
    m_aNextMsg++;
  }
  
  // no records left, copy dangling bytes to start of workarea
  vCopyUp(pMsg);
  
  nMessageBytes = 0;
  ASSERT(m_aEndData >= aFirstByte() - 1);
  ASSERT(m_aEndData <= aLastByte());
  m_aNextMsg = nullptr;
  
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
  // ^- m_aEndData
  //
  
  //
  // b) previous dangling portion
  //
  // v- pFirstByte()                                          pLastByte() -v
  // +---------------------------------------------------------------------+
  // | dangling...| ..available..                                          |
  // +---------------------------------------------------------------------+
  //              ^- m_aEndData (of prev copied up dangling portion)
  //
  
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCANBUFFER,_FOOTER_);
  
  ASSERT(m_aEndData >= aFirstByte() - 1);
  ASSERT(m_aEndData <= aLastByte());
  
  i8 nBytes = nSub(aLastByte(),m_aEndData);
  ASSERT(nBytes <= m_xWsp.nLength());
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* copy dangling portion to beginning of workarea                             */
/*----------------------------------------------------------------------------*/
void ScanBuffer::vCopyUp(Str* aDangling)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCANBUFFER,_FOOTER_);
  
  // used all of the workarea?
  if (aDangling > aLastByte()) {
    vReset();
    ASSERT(m_aEndData >= aFirstByte() - 1);
    ASSERT(m_aEndData <= aLastByte());
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
  //               ^- m_aEndData (of copied up dangling portion)
  //
  
  ASSERT(aDangling >= aFirstByte() - 1);
  ASSERT(aDangling <= aLastByte());
  
  // copy dangling data bytes to begining of workarea
  i8 nDanglingBytes = nSub(aLastByte(),aDangling) + 1;
  ASSERT(nDanglingBytes <= m_xWsp.nLength());
  
  MemLib::vCopy(aFirstByte(),aDangling,nDanglingBytes);
  m_aEndData = pAdd<Str*>(aFirstByte(),nDanglingBytes - 1);
  
//  if (iDanglingBytes < 1000) {
//    vInfo("CopyUp %lld: "_Str,nDanglingBytes);
//    for (char* c = pFirstByte(); c <= m_aEndData; c++) {
//      ASSERT(*c != 0xEF);
//      vInfo("%c"_Str,*c);
//    }
//    vInfo("\n"_Str);
//  }
  
  ASSERT(m_aEndData >= aFirstByte() - 1);
  ASSERT(m_aEndData <= aLastByte());
  
  // invalidate starting position (requires more)
  m_aNextMsg = nullptr;
  
  // wipe the remaining data area (debug)
  vWipe(m_aEndData+1);
}

/*----------------------------------------------------------------------------*/
/* first byte of workarea                                                     */
/*----------------------------------------------------------------------------*/
Str* ScanBuffer::aFirstByte(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCANBUFFER,_FOOTER_);
  
  Str* pFirst = &m_xWsp.aArea()[0];
  
  return pFirst;
}

/*----------------------------------------------------------------------------*/
/* last byte of workarea                                                      */
/*----------------------------------------------------------------------------*/
Str* ScanBuffer::aLastByte(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCANBUFFER,_FOOTER_);
  
  Str* pLast = &m_xWsp.aArea()[m_xWsp.nLength() - 1];
  
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
  
  m_aEndData = aFirstByte() - 1;
  ASSERT(m_aEndData >= aFirstByte() - 1);
  ASSERT(m_aEndData <= aLastByte());
  
  m_aNextMsg = nullptr;
  vWipe();
  
  ASSERT(m_xWsp.nLength() == iBytesAvailable());
}

/*----------------------------------------------------------------------------*/
/* wipe work area                                                             */
/*----------------------------------------------------------------------------*/
void ScanBuffer::vWipe(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCANBUFFER,_FOOTER_);
  
  m_xWsp.vFill(0xEF);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_SCANBUFFER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* wipe work area                                                             */
/*----------------------------------------------------------------------------*/
void ScanBuffer::vWipe(Str* aFrom)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCANBUFFER,_FOOTER_);
  
  ASSERT(aFrom >= m_xWsp.aArea());
  
  i8 nBytes = nSub(aLastByte(),aFrom) + 1;
  
  MemLib::vSet(aFrom,0xEF,nBytes);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_SCANBUFFER,_FOOTER_);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

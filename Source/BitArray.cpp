/*============================================================================*/
/* BitArray.cpp - Bit array                                                   */
/*============================================================================*/

#include "DBobjects.h"

extern BufferPool*  gpBufferPool;

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* BitArray::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  void* pObject = Heap::pAllocate(CN_BITARRAY);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void BitArray::operator delete(void* pDataAddress)
{
  Data* pData = reinterpret_cast<Data*>(pDataAddress);
  
  Heap::vFree(pData);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
BitArray::BitArray(void)
: Object(CN_BITARRAY)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BITARRAY,_FOOTER_);
  
  m_zLock.vInitLock(LK_BIT_ARRAY);
  m_nBitsOn = 0;
  MemLib::vZero(m_bBytes,sizeof(m_bBytes),CN_BITARRAY);
  
  _checkInitialised_(CN_BITARRAY);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_BITARRAY,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
BitArray::~BitArray(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BITARRAY,_FOOTER_);
  
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get number of bits that are on                                             */
/*----------------------------------------------------------------------------*/
i8 BitArray::nBitsOn(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BITARRAY,_FOOTER_);
  
  return m_nBitsOn;
}

/*----------------------------------------------------------------------------*/
/* get bit state                                                              */
/*----------------------------------------------------------------------------*/
bool BitArray::bBitState(i8 iBitNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BITARRAY,_FOOTER_);
  
  i8 iByte;
  i8 iBit;
  char cMask;
  
  vDecode(iBitNo,iByte,iBit,cMask);

  char cByte = m_bBytes[iByte];
  
  char cTest = cByte & cMask;
  
  if (cTest)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* set bit on                                                                 */
/*----------------------------------------------------------------------------*/
void BitArray::vFlipBit(i8 iBitNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BITARRAY,_FOOTER_);
  
  i8 iByte;
  i8 iBit;
  char cMask;
  
  vDecode(iBitNo,iByte,iBit,cMask);
  
  if (bBitState(iBitNo))
    vSetOff(iBitNo);
  else
    vSetOn(iBitNo);
}

/*----------------------------------------------------------------------------*/
/* is bit off                                                                 */
/*----------------------------------------------------------------------------*/
bool BitArray::bIsOff(i8 iBitNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BITARRAY,_FOOTER_);
  
  i8 iByte;
  i8 iBit;
  char cMask;
  
  vDecode(iBitNo,iByte,iBit,cMask);
  
  if (! bBitState(iBitNo))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is bit on                                                                  */
/*----------------------------------------------------------------------------*/
bool BitArray::bIsOn(i8 iBitNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BITARRAY,_FOOTER_);
  
  i8 iByte;
  i8 iBit;
  char cMask;
  
  vDecode(iBitNo,iByte,iBit,cMask);
  
  if (bBitState(iBitNo))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* set bit off                                                                */
/*----------------------------------------------------------------------------*/
void BitArray::vSetOff(i8 iBitNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BITARRAY,_FOOTER_);
  
  i8 iByte;
  i8 iBit;
  char cMask;
  
  vDecode(iBitNo,iByte,iBit,cMask);
  
  m_zLock.bGrabWrite();
  
  if (bBitState(iBitNo)) {
    char cChar = m_bBytes[iByte];
    cChar &= ~cMask;
    m_bBytes[iByte] = cChar;
    m_nBitsOn--;
  }
  
  m_zLock.vDropWrite();
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_BITARRAY,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* set bit on                                                                 */
/*----------------------------------------------------------------------------*/
void BitArray::vSetOn(i8 iBitNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BITARRAY,_FOOTER_);
  
  i8 iByte;
  i8 iBit;
  char cMask;
  
  vDecode(iBitNo,iByte,iBit,cMask);
  
  //vTrace("SetOn %lld : %lld.%lld\n"_Str,iBitNo,iByte,iBit);
  
  m_zLock.bGrabWrite();
  
  if (! bBitState(iBitNo)) {
    char cChar = m_bBytes[iByte];
    cChar |= cMask;
    m_bBytes[iByte] = cChar;
    m_nBitsOn++;
  }
  
  m_zLock.vDropWrite();
  _postcondition_(_ADDRESS_,_MAGIC_,CN_BITARRAY,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* decode bit number into bit manipulation info                               */
/*----------------------------------------------------------------------------*/
void BitArray::vDecode(i8 iBitNo, i8& iByte, i8& iBit, char& cMask)
{
  iByte = iBitNo / 8;
  ASSERT(iByte >= 0 && iByte < C_BIT_ARRAY);
  
  iBit  = iBitNo % 8;
  cMask = 1 << iBit;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* BitArray::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BITARRAY);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

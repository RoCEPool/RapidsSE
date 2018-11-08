/*============================================================================*/
/* BitArray.cpp - Bit array                                                   */
/*============================================================================*/

#include "DBobjects.h"

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* BitArray::operator new(size_t)
{
  void* pObject = HeapMem::pAllocate(CN_BITARRAY);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void BitArray::operator delete(void* pAddress)
{
  BitArray* pBitArray = reinterpret_cast<BitArray*>(pAddress);
  
  HeapMem::vFree(pBitArray);
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
BitArray::~BitArray(void) noexcept(false)
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
bool BitArray::bBitState(i8 i_BitNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BITARRAY,_FOOTER_);
  
  i8 i_Byte;
  i8 i_Bit;
  char cMask;
  
  vDecode(i_BitNo,i_Byte,i_Bit,cMask);

  char cByte = m_bBytes[i_Byte];
  
  char cTest = cByte & cMask;
  
  if (cTest)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* set bit on                                                                 */
/*----------------------------------------------------------------------------*/
void BitArray::vFlipBit(i8 i_BitNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BITARRAY,_FOOTER_);
  
  i8 i_Byte;
  i8 i_Bit;
  char cMask;
  
  vDecode(i_BitNo,i_Byte,i_Bit,cMask);
  
  if (bBitState(i_BitNo))
    vSetOff(i_BitNo);
  else
    vSetOn(i_BitNo);
}

/*----------------------------------------------------------------------------*/
/* is bit off                                                                 */
/*----------------------------------------------------------------------------*/
bool BitArray::bIsOff(i8 i_BitNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BITARRAY,_FOOTER_);
  
  i8 i_Byte;
  i8 i_Bit;
  char cMask;
  
  vDecode(i_BitNo,i_Byte,i_Bit,cMask);
  
  if (! bBitState(i_BitNo))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is bit on                                                                  */
/*----------------------------------------------------------------------------*/
bool BitArray::bIsOn(i8 i_BitNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BITARRAY,_FOOTER_);
  
  i8 i_Byte;
  i8 i_Bit;
  char cMask;
  
  vDecode(i_BitNo,i_Byte,i_Bit,cMask);
  
  if (bBitState(i_BitNo))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* set bit off                                                                */
/*----------------------------------------------------------------------------*/
void BitArray::vSetOff(i8 i_BitNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BITARRAY,_FOOTER_);
  
  i8 i_Byte;
  i8 i_Bit;
  char cMask;
  
  vDecode(i_BitNo,i_Byte,i_Bit,cMask);
  
  m_zLock.bGrabWrite();
  
  if (bBitState(i_BitNo)) {
    char cChar = m_bBytes[i_Byte];
    cChar &= (~ cMask);
    m_bBytes[i_Byte] = cChar;
    m_nBitsOn--;
  }
  
  m_zLock.vDropWrite();
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_BITARRAY,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* set bit on                                                                 */
/*----------------------------------------------------------------------------*/
void BitArray::vSetOn(i8 i_BitNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BITARRAY,_FOOTER_);
  
  i8 i_Byte;
  i8 i_Bit;
  char cMask;
  
  vDecode(i_BitNo,i_Byte,i_Bit,cMask);
  
  //vTrace("SetOn %lld : %lld.%lld\n"_Stz,i_BitNo,i_Byte,i_Bit);
  
  m_zLock.bGrabWrite();
  
  if (! bBitState(i_BitNo)) {
    char cChar = m_bBytes[i_Byte];
    cChar |= cMask;
    m_bBytes[i_Byte] = cChar;
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
void BitArray::vDecode(i8 i_BitNo, i8& i_Byte, i8& i_Bit, char& cMask)
{
  i_Byte = i_BitNo / 8;
  ASSERT(i_Byte >= 0 && i_Byte < C_BIT_ARRAY);
  
  i_Bit  = i_BitNo % 8;
  cMask = 1 << i_Bit;
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

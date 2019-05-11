/*============================================================================*/
/* HashInsert.cpp -- HashInsert                                               */
/*============================================================================*/

#include "Structures.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
HashInsert* Structures::_pHashInsert(void* aAddr)
{
  HashInsert* pHashInsert = reinterpret_cast<HashInsert*>(aAddr);
  
  return pHashInsert;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
HashInsert::HashInsert(void)
{
  MemLib::vZero(m_yaKey,sizeof(m_yaKey),CN_HASHBUFFER);
  m_nBytes = C_UNKNOWN;
  m_iValue = C_UNKNOWN;
  
  m_uSignature = C_UNKNOWN;
  
  m_bArcTrc = false;
  m_bDebug  = false;
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
HashInsert::~HashInsert(void) noexcept(false)
{
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* compute arc bits and entry hash from bit tape                              */
/*----------------------------------------------------------------------------*/
void HashInsert::vArcEntry(void)
{
  // get the next lower 3 bits from tape
  m_uArcBits = m_uBitTape & 0b111;
  
  // shift off lower 3 bits
  m_uBitTape >>= 3;
  
  // bit tape has 3 bits less
  m_nTapeBits -= 3;
  
  // compute entry hash
  m_uEntryHash = HshLib::iHash64(m_yaKey,m_nBytes);
}

/*----------------------------------------------------------------------------*/
/* is this hash entry equal to given key                                      */
/*----------------------------------------------------------------------------*/
void HashInsert::vDump(void)
{
  s32 sKey;

  StrLib::vCopyLen(sKey.aChar(),(char*)m_yaKey,m_nBytes);

  vTrace("%s/%lld %lld\n"_Stz,
         sKey.aChar(),
         m_nBytes,
         m_iValue);
}

/*----------------------------------------------------------------------------*/
/* set entry                                                                  */
/*----------------------------------------------------------------------------*/
void HashInsert::vSet(byte* yaKey, i8 nBytes, i8 iValue)
{
  MemLib::vCopy(m_yaKey,yaKey,nBytes,CN_HASHBUFFER);
  m_nBytes = nBytes;
  m_iValue = iValue;
  
  m_uSignature = HshLib::iHash64(m_yaKey,m_nBytes);
  
  m_bArcTrc = false;
  m_bDebug  = false;
  
  m_uBitTape   = m_uSignature;
  m_nTapeBits  = 64;
  m_uArcBits   = 0;
  m_uEntryHash = m_uSignature;
  
  //vTrace(" iValue %lld uSignature %llx\n"_Stz,m_iValue,m_uSignature);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

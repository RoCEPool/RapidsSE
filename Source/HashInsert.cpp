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
HashInsert::HashInsert(i8 nEntriesPerBuffer)
{
  MemLib::vZero(m_yaKey,sizeof(m_yaKey),CN_HASHBUFFER);
  m_nBytes = C_UNKNOWN;
  m_iValue = C_UNKNOWN;
  
  m_uSignature = 0;
  
  m_uEntryTape     = 0;
  m_nEntryTapeBits = 0;
  m_uEntryHash     = 0;

  m_uArcTape     = 0;
  m_nArcTapeBits = 0;
  m_uArcBits     = 0;

  m_bArcTrc = false;
  m_bDebug  = false;
  
  for (i8 i_History = 0; i_History < 32; i_History++) {
    xHistory[i_History].m_uArcTape   = 0;
    xHistory[i_History].m_uArcBits   = 0;
    xHistory[i_History].m_i_Arc      = C_UNKNOWN;
    
    xHistory[i_History].m_uEntryTape = 0;
    xHistory[i_History].m_uEntryHash = 0;
    xHistory[i_History].m_i_Entry    = C_UNKNOWN;
  }
  
  m_nHistory = 0;
  
  m_nEntriesPerBuffer = nEntriesPerBuffer;
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
  m_uArcBits = m_uArcTape & 0b111;
  
  // shift off lower 3 bits
  m_uArcTape >>= 3;
  
  // bit tape has 3 bits less
  m_nArcTapeBits -= 3;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // get next entry hash
  
  m_uEntryTape >>= 1;
  m_nEntryTapeBits -= 1;

  m_uEntryHash = m_uEntryTape; //HshLib::iHash64(m_yaKey,m_nBytes);
  
  xHistory[m_nHistory].m_uArcTape   = m_uArcTape;
  xHistory[m_nHistory].m_uArcBits   = m_uArcBits;
  xHistory[m_nHistory].m_i_Arc      = m_uArcBits;
  
  xHistory[m_nHistory].m_uEntryTape = m_uEntryTape;
  xHistory[m_nHistory].m_uEntryHash = m_uEntryHash;
  xHistory[m_nHistory].m_i_Entry    = m_uEntryHash;

  m_nHistory++;
}
/*----------------------------------------------------------------------------*/
//void HashInsert::vArcEntry(void)
//{
//  // get the next lower 3 bits from tape
//  m_uArcBits = m_uEntryTape & 0b111;
//
//  // shift off lower 3 bits
//  m_uEntryTape >>= 3;
//
//  // bit tape has 3 bits less
//  m_nEntryTapeBits -= 3;
//
//  // compute entry hash
//  m_uEntryHash = m_uEntryTape; //HshLib::iHash64(m_yaKey,m_nBytes);
//}

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
/* dump history                                                               */
/*----------------------------------------------------------------------------*/
void HashInsert::vDumpHistory(void)
{
  for (i8 i_History = 0; i_History < m_nHistory; i_History++) {
    
    vTrace("  Arc %08lx %llu, %lld; Entry %016llx %llu, %lld\n"_Stz,
           
           xHistory[i_History].m_uArcTape,
           xHistory[i_History].m_uArcBits,
           xHistory[i_History].m_i_Arc,
           
           xHistory[i_History].m_uEntryTape,
           xHistory[i_History].m_uEntryHash,
           xHistory[i_History].m_i_Entry
           );
  }
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
  
  m_uEntryTape     = m_uSignature;
  m_nEntryTapeBits = 64;
  m_uEntryHash     = m_uEntryTape;
  
  m_uArcTape       = HshLib::iHash32(m_yaKey,m_nBytes);;
  m_nArcTapeBits   = 32;
  m_uArcBits       = 0;

  m_bArcTrc = false;
  m_bDebug  = false;
  
  xHistory[m_nHistory].m_uArcTape   = m_uArcTape;
  xHistory[m_nHistory].m_uArcBits   = m_uArcBits;
  xHistory[m_nHistory].m_i_Arc      = m_uArcBits;
  
  xHistory[m_nHistory].m_uEntryTape = m_uEntryTape;
  xHistory[m_nHistory].m_uEntryHash = m_uEntryHash;
  xHistory[m_nHistory].m_i_Entry    = m_uEntryHash;

  m_nHistory++;
  
  //vTrace(" iValue %lld uSignature %llx\n"_Stz,m_iValue,m_uSignature);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

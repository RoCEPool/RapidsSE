/*============================================================================*/
/* HashEntry.cpp -- HashEntry                                                 */
/*============================================================================*/

#include "Structures.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
HashEntry* Structures::_pHashEntry(void* aAddr)
{
  HashEntry* pHashEntry = reinterpret_cast<HashEntry*>(aAddr);
  
  return pHashEntry;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
HashEntry::HashEntry(void)
{
  m_iHash  = C_UNKNOWN;
  MemLib::vZero(m_yaKey,sizeof(m_yaKey),CN_HASHBUFFER);
  m_nBytes = C_UNKNOWN;
  m_iValue = C_UNKNOWN;
  
  m_aDupeChain = nullptr;
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
HashEntry::~HashEntry(void) noexcept(false)
{
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* hash a key for arcing based on buffer depth                                */
/*----------------------------------------------------------------------------*/
i8 HashEntry::iHashArc(byte* yaKey, i8 nBytes, i8 iDepth)
{
  i8 iHashArc;
  
  if (0 == (iDepth % 2))
    iHashArc = iHashRotate(yaKey,nBytes,iDepth);
  else
    iHashArc = iHashKey(yaKey,nBytes);

  return iHashArc;
}

/*----------------------------------------------------------------------------*/
/* hash a key for entry based on buffer depth                                 */
/*----------------------------------------------------------------------------*/
i8 HashEntry::iHashEntry(byte* yaKey, i8 nBytes, i8 iDepth)
{
  i8 iHashEntry = iHashReverse(yaKey,nBytes,iDepth);
  
  return iHashEntry;
}

/*----------------------------------------------------------------------------*/
/* hash a key                                                                 */
/*----------------------------------------------------------------------------*/
i8 HashEntry::iHashKey(byte* yaKey, i8 nBytes)
{
  i8 iHashKey = HshLib::iHash(yaKey,nBytes);
  
  return iHashKey;
}

/*----------------------------------------------------------------------------*/
/* hash a key by first reversing it                                           */
/*----------------------------------------------------------------------------*/
i8 HashEntry::iHashReverse(byte* yaKey, i8 nBytes, i8 nByChars)
{
  if (! nByChars) {
    i8 iHashRev = iHashKey(yaKey,nBytes);
    return iHashRev;
  }
  
  byte yaKeyRev[32];
  
  i8 i_Src = nBytes - 1;
  for (i8 i_Dst = 0; i_Dst < nBytes; i_Dst++, i_Src--)
    yaKeyRev[i_Dst] = yaKey[i_Src];
  
  i8 iHashRev = HshLib::iHash(yaKeyRev,nBytes);
  
  return iHashRev;
}

/*----------------------------------------------------------------------------*/
/* hash a key by first reversing it                                           */
/*----------------------------------------------------------------------------*/
i8 HashEntry::iHashRotate(byte* yaKey, i8 nBytes, i8 nByChars)
{
  if (! nByChars) {
    i8 iHashRot = HshLib::iHash(yaKey,nBytes);
    return iHashRot;
  }
  
  byte yaKeyRot[32];
  MemLib::vCopy(yaKeyRot,yaKey,nBytes,CN_NONE);
  MemLib::vRotate(yaKeyRot,nBytes,nByChars);
  
  i8 iHashRot = HshLib::iHash(yaKeyRot,nBytes);
  return iHashRot;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* is this hash entry equal to given key                                      */
/*----------------------------------------------------------------------------*/
void HashEntry::vDump(void)
{
  s32 sKey;

  StrLib::vCopyLen(sKey.aChar(),(char*)m_yaKey,m_nBytes);

  vTrace("%s/%lld %lld\n"_Stz,
         sKey.aChar(),
         m_nBytes,
         m_iValue);
}

/*----------------------------------------------------------------------------*/
/* is this hash entry equal to given key                                      */
/*----------------------------------------------------------------------------*/
bool HashEntry::bEqualTo(byte* yaKey, i8 nBytes)
{
  if (nBytes == m_nBytes && MemLib::bCompareBytes(m_yaKey,yaKey,nBytes))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* set entry based on another entry (same key but alternate value)            */
/*----------------------------------------------------------------------------*/
void HashEntry::vSet(HashEntry* aSrcEntry, i8 iAltValue)
{
  vSet(aSrcEntry->m_yaKey,
       aSrcEntry->m_nBytes,
       iAltValue,
       aSrcEntry->m_iHash);
}

/*----------------------------------------------------------------------------*/
/* set entry                                                                  */
/*----------------------------------------------------------------------------*/
void HashEntry::vSet(byte* yaKey, i8 nBytes, i8 iValue, i8 iHash)
{
  MemLib::vCopy(m_yaKey,yaKey,nBytes,CN_HASHBUFFER);
  m_nBytes = nBytes;
  m_iValue = iValue;
  
  m_iHash = iHash;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

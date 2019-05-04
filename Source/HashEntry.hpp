/*============================================================================*/
/* HashEntry.hpp -- HashEntry                                                 */
/*============================================================================*/
#pragma once

namespace Structures
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class HashEntry
  {
  public:
    i8   m_iHash;
    byte m_yaKey[32];
    i8   m_nBytes;
    i8   m_iValue;
  
    HashEntry* m_aDupeChain;

  public:
    HashEntry(void);
    ~HashEntry(void) noexcept(false);
  
    static i8 iHash(byte* yaKey, i8 nBytes, i8 iDepth);
  
    bool bEqualTo(byte* yaKey, i8 nBytes);
    void vSet(i8 iHash, byte* yaKey, i8 nBytes, i8 iValue);
  };
  
  HashEntry* _pHashEntry(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

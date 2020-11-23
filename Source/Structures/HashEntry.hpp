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
    byte m_yaKey[32];
    i8   m_nBytes;
    i8   m_iValue;

    u8   m_uSignature;

    HashEntry* m_aDupeChain;

  public:
    HashEntry(void);
    ~HashEntry(void) noexcept(false);
  
    static i8 iHashArc(byte* yaKey, i8 nBytes, i8 iDepth);
    static i8 iHashEntry(byte* yaKey, i8 nBytes, i8 iDepth);
    static i8 iHashKey(byte* yaKey, i8 nBytes);
    static i8 iHashReverse(byte* yaKey, i8 nBytes, i8 nChars);
    static i8 iHashRotate(byte* yaKey, i8 nBytes, i8 nChars);

    void vDump(void);
    bool bEqualTo(byte* yaKey, i8 nBytes);
    void vSet(HashInsert* pHi);
    void vSet(HashEntry* aSrcEntry, i8 iAltValue);
    void vSet(byte* yaKey, i8 nBytes, i8 iValue, u8 uSignature);
  };
  
  HashEntry* _pHashEntry(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

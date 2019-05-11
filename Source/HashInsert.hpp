/*============================================================================*/
/* HashInsert.hpp -- HashInsert                                               */
/*============================================================================*/
#pragma once

namespace Structures
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class HashInsert
  {
  public:
    byte m_yaKey[32];
    i8   m_nBytes;
    i8   m_iValue;

    u8   m_uSignature;
  
    bool m_bArcTrc;
    bool m_bDebug;
  
    u8   m_uBitTape;
    u8   m_nTapeBits;
    u8   m_uArcBits;
    u8   m_uEntryHash;

  public:
    HashInsert(void);
    ~HashInsert(void) noexcept(false);
  
    void vArcEntry(void);
    void vDump(void);
    void vSet(byte* yaKey, i8 nBytes, i8 iValue);
  };
  
  HashInsert* _pHashInsert(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

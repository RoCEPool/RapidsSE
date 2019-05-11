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
  
    u8   m_uEntryTape;
    u8   m_nEntryTapeBits;
    u8   m_uEntryHash;
  
    u8   m_uArcTape;
    u8   m_nArcTapeBits;
    u8   m_uArcBits;

    bool m_bArcTrc;
    bool m_bDebug;
  
    struct {
      i8 m_i_Arc;
      u8 m_uEntryHash;
      i8 m_i_Entry;
    } xHistory[32];
  
    i8 m_nHistory;
  
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

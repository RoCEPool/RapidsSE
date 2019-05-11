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

  public:
    HashInsert(void);
    ~HashInsert(void) noexcept(false);
  
    void vDump(void);
    void vSet(byte* yaKey, i8 nBytes, i8 iValue, u8 uSignature);
  };
  
  HashInsert* _pHashInsert(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

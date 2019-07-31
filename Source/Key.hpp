/*============================================================================*/
/* Key.hpp -- key part of an index entry                                      */
/*============================================================================*/
#pragma once

namespace Transform
{
  enum eKC // KeyCompare
  {
  /*  0 */ KC_NONE=0,
  /*  1 */ KC_BEFORE,
  /*  2 */ KC_SAME,
  /*  3 */ KC_AFTER,
  /*  4 */ KC_ENUMS
  };
  
  static Stz* aKC[]
  {
  /*  0 */ "NONE"_Stz,
  /*  1 */ "BEFORE"_Stz,
  /*  2 */ "SAME"_Stz,
  /*  3 */ "AFTER"_Stz,
  /*  4 */ "ENUMS"_Stz,
  };

  class Key
  {
  private:
    i8 m_nKeyBytes; // size of key (in bytes)
  
    // ..byte sequence (of the key)

  public:
    void* operator new(size_t);
    void* operator new(size_t, Key*);
  
    Key(void);
    ~Key(void) noexcept(false);
  
    static eKC eCompare(Key* aLeft, Key* aRight);
  
    i8   iGetInt(void);
    void vIncKeyBytes(i8 nDeltaBytes);
    i8   nKeyBytes(void);
    void vKeyBytes(i8 nKeyBytes);
    void vShow(IdxColVec* pIdxColVec);

    /*------------------------------------------------------------------------*/
    /* get the byte sequence of the key                                       */
    /*------------------------------------------------------------------------*/
    inline byte* ayByteSeq(void)
    {
      return aAdd<byte*>(this,sizeof(m_nKeyBytes));
    }
  };
  
  Key* _aKey(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* Key.hpp -- key part of an index entry                                      */
/*============================================================================*/
#pragma once

#include "eKC.h"

namespace STORE
{
  class Key
  {
  private:
    i8 m_nElmSeqSize; // size of element sequence (in bytes)
    // .. element sequence (of the key)

  public:
    void* operator new(size_t);
    void* operator new(size_t, Key*);
  
    Key(void);
   ~Key(void) noexcept(false);

    static eKC  eCompare(IndexDef* pIndexDef, Key* aLeft, Key* aRight);
    static eKC  eCompareAsc(Key* aLeft, Key* aRight);
    static eKC  eCompareAscDsc(IndexDef* pIndexDef, Key* aLeft, Key* aRight);
    static bool bEqualTo(IndexDef* pIndexDef, Key* aLeft, Key* aRight);
    static bool bEqualToAsc(Key* aLeft, Key* aRight);
    static bool bEqualToAscDsc(IndexDef* pIndexDef, Key* aLeft, Key* aRight);
    static i8   iGetStrSize(byte* ayKeyBytes);
    static bool bGreaterThan(IndexDef* pIndexDef, Key* aLeft, Key* aRight);
    static bool bGreaterThanAsc(Key* aLeft, Key* aRight);
    static bool bGreaterThanAscDsc(IndexDef* pIndexDef, Key* aLeft, Key* aRight);
    static bool bGreaterEqual(IndexDef* pIndexDef, Key* aLeft, Key* aRight);
    static bool bGreaterEqualAsc(Key* aLeft, Key* aRight);
    static bool bGreaterEqualAscDsc(IndexDef* pIndexDef, Key* aLeft, Key* aRight);
    static bool bLessEqual(IndexDef* pIndexDef, Key* aLeft, Key* aRight);
    static bool bLessEqualAsc(Key* aLeft, Key* aRight);
    static bool bLessEqualAscDsc(IndexDef* pIndexDef, Key* aLeft, Key* aRight);
    static bool bLessThan(IndexDef* pIndexDef, Key* aLeft, Key* aRight);
    static bool bLessThanAsc(Key* aLeft, Key* aRight);
    static bool bLessThanAscDsc(IndexDef* pIndexDef, Key* aLeft, Key* aRight);
    static bool bNotEqual(IndexDef* pIndexDef, Key* aLeft, Key* aRight);
    static bool bNotEqualAsc(Key* aLeft, Key* aRight);
    static bool bNotEqualAscDsc(IndexDef* pIndexDef, Key* aLeft, Key* aRight);
    static eKC  eMixCompare(IndexDef* pIndexDef, Key* aKey_l, Key* aKey_r);

    i8    iGetIntElm(void);
    i8    iGetIntElm(IndexDef* pIndexDef, i8 i_Element);
    i8    iGetStrElm(IndexDef* pIndexDef, i8 i_Element, char* sKeyValue, i8 iKeyBufSize);
    Stz*  szGetStrElm(void);
    void  vIncElmSeqSize(i8 nDeltaBytes);
    i8    nElmSeqSize(void);
    void  vElmSeqSize(i8 nElmSeqSize);
    void  vReset(void);
    void  vShow(IndexDef* pIndexDef);
    void  vShowIntKey(void);

    /*------------------------------------------------------------------------*/
    /* get the element sequence of this key                                   */
    /*------------------------------------------------------------------------*/
    inline Dtm* aElmSeq_dtm(void)
    {
      return aAdd<Dtm*>(this,sizeof(m_nElmSeqSize));
    }
  };
  
  Key* _aKey(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

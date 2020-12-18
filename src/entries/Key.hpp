/*============================================================================*/
/* Key.hpp -- key part of an index entry                                      */
/*============================================================================*/
#pragma once

#include "eKC.h"

namespace Entries
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
  
    static eKC  eCompare(Key* aLeft, Key* aRight);
    static bool bEqualTo(Key* aLeft, Key* aRight);
    static bool bGreaterThan(Key* aLeft, Key* aRight);
    static bool bGreaterEqual(Key* aLeft, Key* aRight);
    static bool bLessEqual(Key* aLeft, Key* aRight);
    static bool bLessThan(Key* aLeft, Key* aRight);
    static bool bNotEqual(Key* aLeft, Key* aRight);

    i8   iGetIntElm(void);
    i8   iGetIntElm(IndexDef* pIndexDef, i8 i_Element);
    Stz* szGetStrElm(void);
    void vIncElmSeqSize(i8 nDeltaBytes);
    i8   nElmSeqSize(void);
    void vElmSeqSize(i8 nElmSeqSize);
    void vReset(void);
    void vShow(IndexDef* pIndexDef);
    void vShowIntKey(void);

    /*------------------------------------------------------------------------*/
    /* get the element sequence of this key                                   */
    /*------------------------------------------------------------------------*/
    inline Dtm* ayElmSeq_dtm(void)
    {
      return aAdd<Dtm*>(this,sizeof(m_nElmSeqSize));
    }
  };
  
  Key* _aKey(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

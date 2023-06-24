/*============================================================================*/
/* ExtendedTextBuffer.hpp -- Extended Text Buffer                              */
/*============================================================================*/
#pragma once

#include "BASE.h"

namespace STORE
{
  class ExtendedTextBuffer : public Object
  {
  public:

    s2K m_sTextBlock;

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    ExtendedTextBuffer(void);
   ~ExtendedTextBuffer(void) noexcept(false);

    bool bConcat(Stz* szSubStr);
    bool bConcat(Stz* szSubStr, i8 nLen);

    i8 nFreeBytes(void);

    Stz* szTextBlock(void);

  protected:
    Footer* aFooter(void);
  };

  ExtendedTextBuffer* _pExtendedTextBuffer(void* aAddr);

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

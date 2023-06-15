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
    char m_acTextBlock[C_1M];

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    ExtendedTextBuffer(void);
   ~ExtendedTextBuffer(void) noexcept(false);

    bool bAddSubStr(Stz* szSubStr);

    Stz* szTextBlock(void);

  protected:
    Footer* aFooter(void);
  };

  ExtendedTextBuffer* _pExtendedTextBuffer(void* aAddr);

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

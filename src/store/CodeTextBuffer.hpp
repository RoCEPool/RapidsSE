/*============================================================================*/
/* CodeTextBuffer.hpp -- Code Text Buffer                                     */
/*============================================================================*/
#pragma once

#include "BASE.h"

namespace STORE
{
  class CodeTextBuffer : public Object
  {
  public:
    char m_acTextBlock[C_256KB];

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    CodeTextBuffer(void);
   ~CodeTextBuffer(void) noexcept(false);

    bool bAddSubStr(Stz* szSubStr);

    Stz* szTextBlock(void);

  protected:
    Footer* aFooter(void);
  };

  CodeTextBuffer* _pCodeTextBuffer(void* aAddr);

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

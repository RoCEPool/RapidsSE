/*============================================================================*/
/* CodeText.hpp -- Code Text                                                  */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "CodeTextBuffer.hpp"

namespace STORE
{
  class CodeText : public Object
  {
  public:
    i8 m_nTextBlocks;
    CodeTextBuffer* m_p_CodeTextBuffer;

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    CodeText(void);
    ~CodeText(void) noexcept(false);

    void vAddSubStr(Stz* szSubStr);

    i8 nTextBlocks(void);
    Stz* szTextBlock(i8 i_TextBlock);

  protected:
    Footer* aFooter(void);
  };

  CodeText* _pCodeText(void* aAddr);

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

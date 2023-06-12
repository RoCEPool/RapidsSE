/*============================================================================*/
/* ExtendedText.hpp -- Extended (multi-buffer) Text                           */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "ExtendedTextBuffer.hpp"

namespace STORE
{
  class ExtendedText : public Object
  {
  public:
    i8 m_nTextBlocks;
    ExtendedTextBuffer* m_p_VlTextBuffer;

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    ExtendedText(void);
    ~ExtendedText(void) noexcept(false);

    void vAddSubStr(Stz* szSubStr);

    i8 nTextBlocks(void);
    Stz* szTextBlock(i8 i_TextBlock);

  protected:
    Footer* aFooter(void);
  };

  ExtendedText* _pExtendedText(void* aAddr);

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

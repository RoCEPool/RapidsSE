/*============================================================================*/
/* VlText.hpp -- Very Large Text                                              */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "CodeTextBuffer.hpp"

namespace STORE
{
  class VlText : public Object
  {
  public:
    i8 m_nTextBlocks;
    CodeTextBuffer* m_p_VlTextBuffer;

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    VlText(void);
    ~VlText(void) noexcept(false);

    void vAddSubStr(Stz* szSubStr);

    i8 nTextBlocks(void);
    Stz* szTextBlock(i8 i_TextBlock);

  protected:
    Footer* aFooter(void);
  };

  VlText* _pVlText(void* aAddr);

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

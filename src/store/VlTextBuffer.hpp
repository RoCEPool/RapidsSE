/*============================================================================*/
/* VlTextBuffer.hpp -- Very-Large Text Buffer                                 */
/*============================================================================*/
#pragma once

#include "BASE.h"

namespace STORE
{
  class VlTextBuffer : public Object
  {
  public:
    char m_acTextBlock[C_256KB];

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    VlTextBuffer(void);
   ~VlTextBuffer(void) noexcept(false);

    bool bAddSubStr(Stz* szSubStr);

    Stz* szTextBlock(void);

  protected:
    Footer* aFooter(void);
  };

  VlTextBuffer* _pVlTextBuffer(void* aAddr);

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

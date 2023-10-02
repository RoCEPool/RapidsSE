/*============================================================================*/
/* ShortForm.hpp -- Deck Short Form                                           */
/*============================================================================*/
#pragma once

#include "BASE.h"

namespace STORE
{
  class ShortForm : public Object
  {
    static const i8 C_SHORTFORM = 64;

  public:

    i8 m_nCols;

    s32 m_sStage_curr;

    struct {
      i8   m_i_CrwCol;     // column of compact row (base 0)
    } m_axCol[C_SHORTFORM];

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    ShortForm(void);

   ~ShortForm(void) noexcept(false);

    void vAdd(i8 i_CrwCol);

    void vDump(i8 i_Tab=0);
    
    void vValidate(void);

  protected:
    Footer* aFooter(void);
  };

  ShortForm* _pShortForm(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
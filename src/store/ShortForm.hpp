/*============================================================================*/
/* ShortForm.hpp -- Deck Short Form                                           */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Board.hpp"

namespace STORE
{
  class ShortForm : public Object
  {
    static const i8 C_SHORTFORM = 128;

  public:

    i8  m_iPrint;
    i8  m_i_Stage_curr;
    s32 m_sStage_curr;

    i8 m_nOps;
    struct {
      i8   m_i_Stage;
      s32  m_sStage;
      s32  m_sOp;
      s32  m_sMeta;
      s256 m_sInjected;
      s256 m_sProjection;
      s256 m_sKey;
    } m_axOp[C_SHORTFORM];

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    ShortForm(CardIter* pCI);
   ~ShortForm(void) noexcept(false);

    void vAdd(CardIter* pCI);

    void vDump(i8 i_Tab=0, bool bTrim=true);
    
    void vValidate(void);

  protected:
    Footer* aFooter(void);
  };

  ShortForm* _pShortForm(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
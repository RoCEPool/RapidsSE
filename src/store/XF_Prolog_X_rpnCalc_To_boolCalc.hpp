/*============================================================================*/
/* XF_Prolog_X_rpnCalc_To_boolCalc.hpp -- RpnCalc to BoolCalc                          */
/*============================================================================*/
#pragma once

#include "XF_PROLOG.hpp"

namespace STORE
{
  class XF_Prolog_X_rpnCalc_To_boolCalc : public XF_PROLOG
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Prolog_X_rpnCalc_To_boolCalc(CardIter* pCI);
   ~XF_Prolog_X_rpnCalc_To_boolCalc(void) noexcept(false);

    static void vConvert(CardIter* pCI);

    void vX_rpnCalc_To_boolCalc(void);

  private:
    Footer* aFooter(void);
  };

  XF_Prolog_X_rpnCalc_To_boolCalc* _pXF_Prolog_X_rpnCalc_To_boolCalc(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

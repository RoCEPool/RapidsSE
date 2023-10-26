/*============================================================================*/
/* XF_Prolog_X_rpnCalc_To_boolPred.hpp -- RpnCalc to BoolPred                 */
/*============================================================================*/
#pragma once

#include "XF_PROLOG.hpp"

namespace STORE
{
  class XF_Prolog_X_rpnCalc_To_boolPred : public XF_PROLOG
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Prolog_X_rpnCalc_To_boolPred(CardIter* pCI);
   ~XF_Prolog_X_rpnCalc_To_boolPred(void) noexcept(false);

    static void vConvert(CardIter* pCI);

    void vX_rpnCalc_To_boolPred(void);

  private:
    Footer* aFooter(void);
  };

  XF_Prolog_X_rpnCalc_To_boolPred* _pXF_Prolog_X_rpnCalc_To_boolPred(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

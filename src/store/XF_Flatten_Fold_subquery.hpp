/*============================================================================*/
/* XF_Flatten_Fold_subquery.hpp -- Flatten                                             */
/*============================================================================*/
#pragma once

#include "XF_FLATTEN.hpp"

namespace STORE
{
  class XF_Flatten_Fold_subquery : public XF_FLATTEN
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Flatten_Fold_subquery(CardIter* pCI);
   ~XF_Flatten_Fold_subquery(void) noexcept(false);

    static void vDecisionTree(CardIter* pCI, Symbol* pSym_sqyNo);

    void v10_F(void);
    void v24_W_A_C_ro(void);
    void v32_W_A_NC_I(void);
    void v34_W_A_NC_ro(void);
    void v40_W_L_C_E(void);
    void v52_W_L_NC_I_join(void);
    void v53_W_L_NC_NI_exists(void);
    void v53_W_L_NC_NI_join(void);
    void v74_H_A_NC_ro(void);

  private:
    Footer* aFooter(void);
  };

  XF_Flatten_Fold_subquery* _pXF_Flatten_Fold_subquery(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* XF_Flatten.hpp -- Flatten SubQueries                                         */
/*============================================================================*/
#pragma once

#include "XF_TRANSFORM.hpp"


namespace STORE
{
  class XF_Flatten : public XF_TRANSFORM
  {
  private:

    CardIter* m_pCI;

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    XF_Flatten(CardIter* pCI);
   ~XF_Flatten(void) noexcept(false);

    static void vFlatten(CardIter* pCI);

    void vFlatten(void);

  private:

    void vFlatten_recurse(i8 iDepth);

    void vFlattenByDecisionTree(void);

    void vFlatten_10_F(void);
    void vFlatten_24_W_A_C_ro(void);
    void vFlatten_32_W_A_NC_I(void);
    void vFlatten_34_W_A_NC_ro(void);
    void vFlatten_40_W_L_C_E(void);
    void vFlatten_52_W_L_NC_I_exists(void);
    void vFlatten_52_W_L_NC_I_join(void);
    void vFlatten_53_W_L_NC_NI_exists(void);
    void vFlatten_53_W_L_NC_NI_join(void);
    void vFlatten_74_H_A_NC_ro(void);

    void vStmColToTblCol(void);

  protected:
    Footer* aFooter(void);
  };

  XF_Flatten* _pXF_Flatten(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

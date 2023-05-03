/*============================================================================*/
/* MakeFlat.hpp -- Flatten SubQueries                                         */
/*============================================================================*/
#pragma once

#include "Bench.hpp"


namespace STORE
{
  class MakeFlat : public Object
  {
  private:

    CardIter* m_pCI;

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    MakeFlat(CardIter* pCI);
   ~MakeFlat(void) noexcept(false);

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

  MakeFlat* _pMakeFlat(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

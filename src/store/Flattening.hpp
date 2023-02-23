/*============================================================================*/
/* Flattening.hpp -- Join Ordering                                          */
/*============================================================================*/
#pragma once

#include "Bench.hpp"


namespace STORE
{
  class Flattening : public Object
  {
  private:

    CardIter* m_pCI;

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Flattening(CardIter* pCI);
   ~Flattening(void) noexcept(false);

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

  protected:
    Footer* aFooter(void);
  };

  Flattening* _pFlattening(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

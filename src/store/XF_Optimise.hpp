/*============================================================================*/
/* XF_Optimise_max_To_top.hpp -- Optimise max to top                          */
/*============================================================================*/
#pragma once

#include "XF_TRANSFORM.hpp"
#include "eOT.h"

namespace STORE
{
  class XF_Optimise : public XF_TRANSFORM
  {
  private:

    eOT m_eOptimisation;

  public:

    XF_Optimise(eCN eClass, eOT eOptimisation, CardIter* pCI);
   ~XF_Optimise(void) noexcept(false);

    static void vOptimise(CardIter* pCI);

    void vOptimised(bool b);

   };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

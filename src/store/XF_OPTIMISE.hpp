/*============================================================================*/
/* XF_Optimise_max_To_top.hpp -- Optimise max to top                          */
/*============================================================================*/
#pragma once

#include "XF_TRANSFORM.hpp"
#include "eOT.h"

namespace STORE
{
  class XF_OPTIMISE : public XF_TRANSFORM
  {
  private:

    eOT m_eOptimisation;

  public:

    XF_OPTIMISE(eCN eClass, eOT eOptimisation, CardIter* pCI);
   ~XF_OPTIMISE(void) noexcept(false);

    static void vOptimise(CardIter* pCI);

    void vOptimised(bool b);

   };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

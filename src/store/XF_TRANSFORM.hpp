/*============================================================================*/
/* XF_TRANSFORM.hpp -- Abstract Transform                                     */
/*============================================================================*/
#pragma once

#include "CardIter.hpp"

namespace STORE
{
  class XF_TRANSFORM : public Object
  {
  private:
    CardIter* m_pCI;

  public:
    XF_TRANSFORM(eCN eClass, CardIter* pCI=nullptr);
   ~XF_TRANSFORM(void) noexcept(false);

  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

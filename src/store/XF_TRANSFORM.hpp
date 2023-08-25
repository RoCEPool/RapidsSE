/*============================================================================*/
/* XF_TRANSFORM.hpp -- Abstract Transform                                     */
/*============================================================================*/
#pragma once

#include "CardIter.hpp"

namespace STORE
{
  class XF_TRANSFORM : public Object
  {
  protected:
    CardIter* m_pCI;

    // must be last
    Footer m_xFooter;

  public:
    XF_TRANSFORM(eCN eClass, CardIter* pCI=nullptr);
   ~XF_TRANSFORM(void) noexcept(false);

  protected:
    void vValidateBench(void);

  private:
    Footer* aFooter(void);

  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

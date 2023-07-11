/*============================================================================*/
/* XF_Smooth.hpp -- XF_Smooth                                                       */
/*============================================================================*/
#pragma once

#include "BASE.h"

namespace STORE
{
  class CardIter;
  class CarryForward;

  class XF_Smooth : public Object
  {
  private:

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    XF_Smooth(void);
   ~XF_Smooth(void) noexcept(false);

    void vSmooth(CardIter* pCI, CarryForward* pCarryForward, bool bEarlyExit=false);

  private:
    Footer* aFooter(void);
  };

  XF_Smooth* _pXF_Smooth(void* aAddr);

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

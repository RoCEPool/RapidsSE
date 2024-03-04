/*============================================================================*/
/* XF_Flatten_Smooth.hpp -- XF_Flatten_Smooth                                                       */
/*============================================================================*/
#pragma once

#include "BASE.h"

namespace STORE
{
  class CardIter;
  class CarryForward;

  class XF_Flatten_Smooth : public Object
  {
  private:

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    XF_Flatten_Smooth(void);
   ~XF_Flatten_Smooth(void) noexcept(false);


  private:
    Footer* aFooter(void);
  };

  XF_Flatten_Smooth* _pXF_Flatten_Smooth(void* aAddr);

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

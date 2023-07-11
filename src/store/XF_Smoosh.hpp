/*============================================================================*/
/* XF_Smoosh.hpp -- XF_Smoosh                                                       */
/*============================================================================*/
#pragma once

#include "BASE.h"
#include "eSM.h"

namespace STORE
{
  class CardIter;
  class CarryForward;

  class XF_Smoosh : public Object
  {
  private:

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    XF_Smoosh(void);
   ~XF_Smoosh(void) noexcept(false);

    void vSmoosh(CardIter* pCI, eSM eSmoosh);

    void vInto_KEYnotEXISTS(CardIter* pCI);

  private:
    Footer* aFooter(void);
  };

  XF_Smoosh* _pXF_Smoosh(void* aAddr);

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

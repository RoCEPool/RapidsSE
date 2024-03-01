/*============================================================================*/
/* XF_Flatten_Smoosh.hpp -- XF_Flatten_Smoosh                                                       */
/*============================================================================*/
#pragma once

#include "BASE.h"
#include "eSM.h"

namespace STORE
{
  class CardIter;
  class CarryForward;

  class XF_Flatten_Smoosh : public Object
  {
  private:

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    XF_Flatten_Smoosh(void);
   ~XF_Flatten_Smoosh(void) noexcept(false);


    void vInto_KEYnotEXISTS(CardIter* pCI);

  private:
    Footer* aFooter(void);
  };

  XF_Flatten_Smoosh* _pXF_Flatten_Smoosh(void* aAddr);

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

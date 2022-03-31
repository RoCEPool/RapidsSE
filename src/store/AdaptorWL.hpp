/*============================================================================*/
/* AdaptorWL.hpp -- Wire Line Adaptor                                         */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "ApiHelper.hpp"

namespace STORE
{
  class AdaptorWL : public Object
                  , public ApiHelper
  {
  private:
   
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    AdaptorWL(void);
   ~AdaptorWL(void) noexcept(false);
  
    static void vRepair(Object* pObject);

    void vAction0(void);
    void vAction1(void);
    void vAction2(void);

  private:
  
    Footer* aFooter(void);

  };

  AdaptorWL* _pAdaptorWL(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

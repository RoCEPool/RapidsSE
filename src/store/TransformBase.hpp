/*============================================================================*/
/* TransformBase.hpp --                                                       */
/*============================================================================*/
#pragma once

#include "Bench.hpp"
#include "BoardIter.hpp"


namespace STORE
{
  class TransformBase : public Object
  {
  public:

    // must be last
    Footer m_xFooter;


  public:
    void* operator new(size_t);
    void  operator delete(void*);

    TransformBase(eCN eClass);
   ~TransformBase(void) noexcept(false);


    Footer* aFooter(void);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

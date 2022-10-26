/*============================================================================*/
/* Xlator.hpp --                                                              */
/*============================================================================*/
#pragma once

#include "TransformerTool.hpp"

namespace STORE
{
  class Xlator : public TransformerTool
  {
  private:

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Xlator(eCN eClassNo, Bench* pBench);
   ~Xlator(void) noexcept(false);




    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -




private:

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

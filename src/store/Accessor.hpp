/*============================================================================*/
/* Accessor.hpp --                                                            */
/*============================================================================*/
#pragma once

#include "TransformerTool.hpp"

namespace STORE
{
  class Accessor : public TransformerTool
  {
  private:

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Accessor(eCN eClassNo, Bench* pBench);
   ~Accessor(void) noexcept(false);

  private:

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

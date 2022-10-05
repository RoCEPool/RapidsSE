/*============================================================================*/
/* Columnar.hpp --                                                             */
/*============================================================================*/
#pragma once

#include "TransformerTool.hpp"

namespace STORE
{
  class Columnar : public TransformerTool
  {
  private:

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Columnar(Bench* pBench);
   ~Columnar(void) noexcept(false);

    void vAdd_columnRef(void);

  private:

    Footer* aFooter(void);
  };

  Columnar* _pColumnar(void* aAddr);
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

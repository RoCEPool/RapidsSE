/*============================================================================*/
/* Deleter.hpp --                                                             */
/*============================================================================*/
#pragma once

#include "TransformerTool.hpp"

namespace STORE
{
  class Deleter : public TransformerTool
  {
  private:

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Deleter(Bench* pBench);
  
   ~Deleter(void) noexcept(false);

    void vDelete(void);
  private:

    Footer* aFooter(void);
  };

  Deleter* _pDeleter(void* aAddr);
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

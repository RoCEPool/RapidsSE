 /*============================================================================*/
/* Bench.hpp -- Bench                                                         */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Dissect.hpp"
#include "Board.hpp"

namespace STORE
{
  static const i8 C_QUERY_SUBQUERIES = 8;

  class Bench : public Object
  {
  private:
    Board* m_pBoard;
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Bench(void);
   ~Bench(void) noexcept(false);
  
    void vDissect(PipelineSpec* pPipelineSpec);
    void vRework(void);
    void vSynthesize(void);
    void vPrintBench(void);

  private:
    Footer* aFooter(void);
  };

  Bench* _pBench(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

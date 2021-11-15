 /*============================================================================*/
/* Bench.hpp -- Bench                                                         */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Board.hpp"
#include "Tools.hpp"

namespace STORE
{
  static const i8 C_QUERY_SUBQUERIES = 8;

  class Bench : public Object
              , public ListExo2_t<Board>
  {
  private:
  
    Tools* m_pTools;
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Bench(void);
   ~Bench(void) noexcept(false);
  
    void vDissect(PipelineSpec* pPipelineSpec);
  
    void vWalkCards(std::function<void(Card* pCard)> vfClosure);
    void vWalkSubCards(std::function<void(Card* pCard)> vfClosure);
  
    void vExpand(void);
  
    void vOrdinals(void);
  
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

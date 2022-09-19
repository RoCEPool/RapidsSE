/*============================================================================*/
/* Transformer.hpp --                                                         */
/*============================================================================*/
#pragma once

#include "OptimiserTool.hpp"

#include "Booler.hpp"
#include "Joiner.hpp"
#include "Predicator.hpp"
#include "Simplifier.hpp"
#include "Streamer.hpp"

namespace STORE
{
  class Transformer : public OptimiserTool
  {
  private:

    Optimiser*   m_pOptimiser;

    Bench*       m_pBench;
    Board*       m_pBoard;
    Card_SubSet* m_pCard_SubSet;

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Transformer(Optimiser* pOptimiser);
  
   ~Transformer(void) noexcept(false);

    void vTransform(void);

  private:

    Footer* aFooter(void);
  };

  Transformer* _pTransformer(void* aAddr);
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

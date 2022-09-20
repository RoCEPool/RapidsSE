/*============================================================================*/
/* Transformer.hpp --                                                         */
/*============================================================================*/
#pragma once

#include "OptimiserTool.hpp"

#include "Curator.hpp"
#include "Filtrate.hpp"
#include "Joiner.hpp"
#include "Predicator.hpp"
#include "Streamer.hpp"

namespace STORE
{
  class Transformer : public OptimiserTool
  {
  private:

    Optimiser*   m_pOptimiser;

    Bench*       m_pBench;
    Board*       m_pBoard;

    Curator*     m_pCurator;
    Filtrate*    m_pFiltrate;
    Joiner*      m_pJoiner;
    Predicator*  m_pPredicator;
    Streamer*    m_pStreamer;

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

/*============================================================================*/
/* Transformer.hpp --                                                         */
/*============================================================================*/
#pragma once

#include "OptimiserTool.hpp"

#include "Combiner.hpp"
#include "Curator.hpp"
#include "Filtrate.hpp"
#include "Joiner.hpp"
#include "Predicator.hpp"
#include "Sender.hpp"
#include "Streamer.hpp"
#include "Stager.hpp"

namespace STORE
{
  class Transformer : public OptimiserTool
  {
  private:

    Optimiser*   m_pOptimiser;

    Bench*       m_pBench;
    Board*       m_pBoard;

    Combiner*    m_pCombiner;
    Curator*     m_pCurator;
    Filtrate*    m_pFiltrate;
    Joiner*      m_pJoiner;
    Predicator*  m_pPredicator;
    Sender*      m_pSender;
    Stager*      m_pStager;
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

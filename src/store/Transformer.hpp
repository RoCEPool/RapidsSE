/*============================================================================*/
/* Transformer.hpp --                                                         */
/*============================================================================*/
#pragma once

#include "OptimiserTool.hpp"

#include "Transforms.hpp"

#include "Combiner.hpp"
#include "Curator.hpp"
#include "Estimator.hpp"
#include "Filtrate.hpp"
#include "Indexer.hpp"
#include "Joiner.hpp"
#include "Partitioner.hpp"
#include "Permutor.hpp"
#include "Scanning.hpp"
#include "Sender.hpp"
#include "Sorter.hpp"
#include "Streamer.hpp"
#include "Stager.hpp"
#include "Tabular.hpp"
#include "Columnar.hpp"

namespace STORE
{
  class Transformer : public OptimiserTool
  {
  private:

    CardCtl*     m_pCardCtl;

    Transforms*  m_pTransforms;

    Columnar*    m_pColumnar;
    Combiner*    m_pCombiner;
    Curator*     m_pCurator;
    Estimator*   m_pEstimator;
    Filtrate*    m_pFiltrate;
    Indexer*     m_pIndexer;
    Joiner*      m_pJoiner;
    Partitioner* m_pPartitioner;
    Permutor*    m_pPermutor;
    Scanning*    m_pScanning;
    Sender*      m_pSender;
    Sorter*      m_pSorter;
    Stager*      m_pStager;
    Streamer*    m_pStreamer;
    Tabular*     m_pTabular;


    b8           m_bInteractive;

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Transformer(Bench* pBench);
   ~Transformer(void) noexcept(false);

    void vTransform(void);

  private:

    void vTransform2(void);
    void vTransform3(void);

    Footer* aFooter(void);
  };

  Transformer* _pTransformer(void* aAddr);
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

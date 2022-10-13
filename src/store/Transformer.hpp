/*============================================================================*/
/* Transformer.hpp --                                                         */
/*============================================================================*/
#pragma once

#include "OptimiserTool.hpp"

#include "Transforms.hpp"

#include "Curator.hpp"

namespace STORE
{
  class Transformer : public OptimiserTool
  {
  private:

    CardCtl*     m_pCardCtl;

    Transforms*  m_pTransforms;

    Curator*     m_pCurator;


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

    void vTransform3(void);

    void vPhase_1_ReducePipeline(void);
    void vPhase_2_TableAnalysis(void);
    void vPhase_3_WherePredicates(void);
    void vPhase_4_InitialJoin(void);
    void wPhase_5_JoinOrdering(void);
    void wPhase_6_IndexScan(void);
    void wPhase_7_StreamOrdering(void);
    void wPhase_8_FilterRefinement(void);

    void vPhase_Z_RestatePipeline(void);

    Footer* aFooter(void);
  };

  Transformer* _pTransformer(void* aAddr);
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

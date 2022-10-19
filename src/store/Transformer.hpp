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

    void vPhase_1_ReducePipeline  (bool bPrintPhase, bool bPrintXform, bool bTrace);
    void vPhase_2_TableAnalysis   (bool bPrintPhase, bool bPrintXform, bool bTrace);
    void vPhase_3_WherePredicates (bool bPrintPhase, bool bPrintXform, bool bTrace);
    void vPhase_U_ExistsSubqueries(bool bPrintPhase, bool bPrintXform, bool bTrace);
    void vPhase_4_InitialJoin     (bool bPrintPhase, bool bPrintXform, bool bTrace);
    void vPhase_5_JoinOrdering    (bool bPrintPhase, bool bPrintXform, bool bTrace);
    void vPhase_6_IndexScan       (bool bPrintPhase, bool bPrintXform, bool bTrace);
    void vPhase_7_StreamOrdering  (bool bPrintPhase, bool bPrintXform, bool bTrace);
    void vPhase_8_FilterRefinement(bool bPrintPhase, bool bPrintXform, bool bTrace);

    void vPhase_Z_RestatePipeline (bool bPrintPhase, bool bPrint, bool bTrace);

    Footer* aFooter(void);
  };

  Transformer* _pTransformer(void* aAddr);
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

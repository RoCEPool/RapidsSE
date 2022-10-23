/*============================================================================*/
/* Transformer.hpp --                                                         */
/*============================================================================*/
#pragma once

#include "OptimiserTool.hpp"

#include "Transforms.hpp"

namespace STORE
{
  class Transformer : public OptimiserTool
  {
  private:

    CardCtl*     m_pCardCtl;
    TagSet*      m_pJobTags;
    b8           m_bInteractive;

    Transforms*  m_pTransforms;


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

    void vPhase_10_ReducePipeline  (bool bPrintPhase, bool bPrintXform, bool bTrace);
    void vPhase_20_AccessMetadata  (bool bPrintPhase, bool bPrintXform, bool bTrace);
    void vPhase_30_StreamAnalysis  (bool bPrintPhase, bool bPrintXform, bool bTrace);
    void vPhase_40_PredicatePullup (bool bPrintPhase, bool bPrintXform, bool bTrace);
    void vPhase_50_FlattenExists   (bool bPrintPhase, bool bPrintXform, bool bTrace);
    void vPhase_60_InitialJoin     (bool bPrintPhase, bool bPrintXform, bool bTrace);
    void vPhase_70_JoinOrdering    (bool bPrintPhase, bool bPrintXform, bool bTrace);
    void vPhase_80_IndexScan       (bool bPrintPhase, bool bPrintXform, bool bTrace);
    void vPhase_90_StreamOrdering  (bool bPrintPhase, bool bPrintXform, bool bTrace);
    void vPhase_A0_FilterRefinement(bool bPrintPhase, bool bPrintXform, bool bTrace);

    void vPhase_Z0_RestatePipeline (bool bPrintPhase, bool bPrint, bool bTrace);

    void vEndPhase(void);
    void vPhase(eBK eBoard, i8 i_SubBoard, bool bPrintPhase, const char* azPhase);
    bool bPrintBench(void);
    void vPrintBench(eBK eBoard, i8 i_SubBoard=C_UNKNOWN);

    Footer* aFooter(void);
  };

  Transformer* _pTransformer(void* aAddr);
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

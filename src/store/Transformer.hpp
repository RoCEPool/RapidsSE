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
    Transforms* pTransforms(void);

  private:

    void vTransform3(void);

    void vHand_10_ReducePipeline  (bool bPrintHand, bool bPrintXform, bool bTrace);
    void vHand_20_AccessMetadata  (bool bPrintHand, bool bPrintXform, bool bTrace);
    void vHand_30_StreamAnalysis  (bool bPrintHand, bool bPrintXform, bool bTrace);
    void vHand_40_PredicatePullup (bool bPrintHand, bool bPrintXform, bool bTrace);
    void vHand_50_FlattenExists   (bool bPrintHand, bool bPrintXform, bool bTrace);
    void vHand_60_InitialJoin     (bool bPrintHand, bool bPrintXform, bool bTrace);
    void vHand_70_JoinOrdering    (bool bPrintHand, bool bPrintXform, bool bTrace);
    void vHand_80_IndexScan       (bool bPrintHand, bool bPrintXform, bool bTrace);
    void vHand_90_StreamOrdering  (bool bPrintHand, bool bPrintXform, bool bTrace);
    void vHand_A0_FilterRefinement(bool bPrintHand, bool bPrintXform, bool bTrace);

    void vHand_Z0_RestatePipeline (bool bPrintHand, bool bPrint, bool bTrace);

    void vEndHand(void);
    void vHand(eBK eBoard, i8 i_SubBoard, bool bPrintHand, const char* azHand);
    bool bPrintBench(void);
    void vPrintBench(eBK eBoard, i8 i_SubBoard=C_UNKNOWN);

    Footer* aFooter(void);
  };

  Transformer* _pTransformer(void* aAddr);
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

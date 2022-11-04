/*============================================================================*/
/* Transformer.hpp --                                                         */
/*============================================================================*/
#pragma once

#include "OptimiserTool.hpp"

#include "Xtransforms.hpp"

namespace STORE
{
  class Transformer : public OptimiserTool
  {
  private:

    CardCtl*     m_pCardCtl;
    TagSet*      m_pJobTags;
    b8           m_bInteractive;

    Xtransforms*  m_pTransforms;


    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Transformer(Bench* pBench);
   ~Transformer(void) noexcept(false);

    void vTransform(void);
    Xtransforms* pTransforms(void);

  private:

    void vTransform3(void);

    void vHand_10_CondenseInfo    (i8 iBelow, ...);
    void vHand_20_TableAnalysis   (i8 iBelow, ...);
    void vHand_30_StreamAnalysis  (i8 iBelow, ...);
    void vHand_40_PredicatePullup (i8 iBelow, ...);
    void vHand_50_FlattenCall     (i8 iBelow, ...);
    void vHand_55_FlattenExists   (i8 iBelow, ...);
    void vHand_60_IndexJoin       (i8 iBelow, ...);
    void vHand_70_JoinOrdering    (i8 iBelow, ...);
    void vHand_75_EE_operators    (i8 iBelow, ...);
    void vHand_80_IndexScan       (i8 iBelow, ...);
    void vHand_90_StreamOrdering  (i8 iBelow, ...);
    void vHand_A0_FilterRefinement(i8 iBelow, ...);
    void vHand_B0_RestoreStmOdn   (i8 iBelow, ...);
    void vHand_Z0_RestatePipeline (i8 iBelow, ...);

    void vEndHand(eBK eBoard, i8 i_Board=C_UNKNOWN);
    void vHand(eBK eBoard, i8 i_Board, const char* azHand);

    Footer* aFooter(void);
  };

  Transformer* _pTransformer(void* aAddr);
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

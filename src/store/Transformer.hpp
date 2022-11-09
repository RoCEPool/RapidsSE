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

    void vHand_10_Condense_Info    (i8 iBelow, ...);
    void vHand_20_Table_Analysis   (i8 iBelow, ...);
    void vHand_30_Stream_Analysis  (i8 iBelow, ...);
    void vHand_35_Flatten_From     (i8 iBelow, ...);
    void vHand_36_Flatten_Where    (i8 iBelow, ...);
    void vHand_37_Flatten_Having   (i8 iBelow, ...);
    void vHand_40_Predicate_Pullup (i8 iBelow, ...);
    void vHand_50_FlattenCall      (i8 iBelow, ...);
    void vHand_52_FlattenIn        (i8 iBelow, ...);
    void vHand_55_FlattenExists    (i8 iBelow, ...);
    void vHand_60_Index_Join       (i8 iBelow, ...);
    void vHand_70_Join_Ordering    (i8 iBelow, ...);
    void vHand_75_EE_operators     (i8 iBelow, ...);
    void vHand_80_Index_Scan       (i8 iBelow, ...);
    void vHand_90_Stream_Ordering  (i8 iBelow, ...);
    void vHand_A0_Filter_Refinement(i8 iBelow, ...);
    void vHand_Z0_Restate_Pipeline (i8 iBelow, ...);

    void vEndHand(eBK eBoard, i8 i_Board=C_UNKNOWN);
    void vHand(eBK eBoard, i8 i_Board, const char* azHand);

    Footer* aFooter(void);
  };

  Transformer* _pTransformer(void* aAddr);
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

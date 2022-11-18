/*============================================================================*/
/* Xseries.hpp --                                                              */
/*============================================================================*/
#pragma once

#include "OptimiserTool.hpp"

#include "Xtransforms.hpp"

namespace STORE
{
  class Xseries : public OptimiserTool
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
  
    Xseries(Bench* pBench);
   ~Xseries(void) noexcept(false);
   
    void vTransformSeries(void);

  private:

    void vSeries_10_Condense_Info    (bool bEnabled); //
    void vSeries_20_Table_Analysis   (bool bEnabled); //
    void vSeries_22_Index_Analysis   (bool bEnabled);
    void vSeries_24_Stream_Analysis  (bool bEnabled); //
    void vSeries_26_Query_Analysis   (bool bEnabled); //
//  void vSeries_30_Literal_Arithmetic  (bool bEnabled); //
    void vSeries_32_To_BoolCalc      (bool bEnabled); //
    void vSeries_34_Predicate_Pullup (bool bEnabled); //
    void vSeries_36_Bound_Analysis   (bool bEnabled); //
    void vSeries_40_Flatten_Query    (bool bEnabled); //
    void vSeries_50_Join_Ordering    (bool bEnabled); // 2 days
    void vSeries_60_Index_Join       (bool bEnabled); //
    void vSeries_75_EE_operators     (bool bEnabled); // 1 day
//  void vSeries_79_Add_Partitioning (bool bEnabled); //
    void vSeries_80_Index_Scan       (bool bEnabled); // 1 day
    void vSeries_85_Cross_Join       (bool bEnabled); // 1 day
    void vSeries_90_Stream_Ordering  (bool bEnabled); // 1 day
    void vSeries_92_Ordered_Aggreg   (bool bEnabled); // 0.5 day
//  void vSeries_94_Simplify_Group   (bool bEnabled); // 0.5 day
//  void vSeries_96_Eliminate_Order  (bool bEnabled); // 0.5 day
//  void vSeries_98_Reduce_Aggreg    (bool bEnabled); // 1 day
    void vSeries_A0_Filter_Refinement(bool bEnabled); //
//  void vSeries_B0_Trick_1          (bool bEnabled); // 1 day
//  void vSeries_B2_Trick_2          (bool bEnabled); // 1 day
//  void vSeries_B3_Trick_3          (bool bEnabled); // 1 day
//  void vSeries_B4_Earlier_Aggreg   (bool bEnabled); // 2 days
//  void vSeries_B5_Limit_Pullup     (bool bEnabled); // 2 days
    void vSeries_Z0_Restate_Pipeline (bool bEnabled); //
                                                      // ---------
                                                      // 2.5 weeks

    void vSeries_35X_Flatten_From     (bool bEnabled); //
    void vSeries_36X_Flatten_Where    (bool bEnabled); //
    void vSeries_37X_Flatten_Having   (bool bEnabled); //

    void vEndSeries(eBK eBoard, i8 i_Board=C_UNKNOWN);
    void vSeries(eBK eBoard, i8 i_Board, const char* azHand);

    Footer* aFooter(void);
  };

  Xseries* _pXseries(void* aAddr);
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

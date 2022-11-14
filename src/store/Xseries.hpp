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
    
    void vSeries_20_Table_Analysis   (bool bEnabled);
    void vSeries_10_Condense_Info    (bool bEnabled);
    void vSeries_25_Stream_Analysis  (bool bEnabled);
    void vSeries_29_Query_Analysis   (bool bEnabled);
    void vSeries_35_Flatten_From     (bool bEnabled);
    void vSeries_36_Flatten_Where    (bool bEnabled);
    void vSeries_37_Flatten_Having   (bool bEnabled);
    void vSeries_40_Predicate_Pullup (bool bEnabled);
    void vSeries_60_Index_Join       (bool bEnabled);
    void vSeries_70_Join_Ordering    (bool bEnabled);
    void vSeries_75_EE_operators     (bool bEnabled);
    void vSeries_80_Index_Scan       (bool bEnabled);
    void vSeries_85_Cross_Join       (bool bEnabled);
    void vSeries_90_Stream_Ordering  (bool bEnabled);
    void vSeries_A0_Filter_Refinement(bool bEnabled);
    void vSeries_Z0_Restate_Pipeline (bool bEnabled);

    void vEndSeries(eBK eBoard, i8 i_Board=C_UNKNOWN);
    void vSeries(eBK eBoard, i8 i_Board, const char* azHand);

    Footer* aFooter(void);
  };

  Xseries* _pXseries(void* aAddr);
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

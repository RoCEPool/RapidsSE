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

    // we start with multiple sub_queries...
    void vSeries_10_Make_Runnable(void);

    // main query only from here...
    void vSeries_50_Join_Ordering     (bool bEnabled); // 2 days
    void vSeries_50_Join_Ordering_v2  (void);
    void vSeries_80_Index_Scan        (bool bEnabled); // 1 day
    void vSeries_90_Stream_Ordering   (bool bEnabled); // 1 day
    void vSeries_92_Project_Pullup    (bool bEnabled); // 1 day
    void vSeries_A0_Filter_Refinement (bool bEnabled); //
                                                      // ---------
                                                      // 2.5 weeks

    void vEndSeries(eBK eBoard, i8 i_Board=C_UNKNOWN);
    void vSeries(eBK eBoard, i8 i_Board, const char* azHand);

    Footer* aFooter(void);
  };

  Xseries* _pXseries(void* aAddr);
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

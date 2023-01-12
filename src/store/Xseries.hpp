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
   
    bool bTransformSeries(eEC* peErrorCode);
    bool bTransformSeries_v1(eEC* peErrorCode);

  private:

    void vSeries_05_Prepare_Pipelines_v1(void);

    // we start with multiple sub_queries...
    void vSeries_10_Make_Runnable(void);
    void vSeries_10_Make_Runnable_v1(void);

    // main query only from here...
    void vSeries_80_Index_Scan(bool bEnabled);
    void vSeries_90_Stream_Ordering(bool bEnabled);
    void vSeries_A0_Make_Weavable(void);
    void vSeries_A0_Make_Weavable_v1(void);

    void vEndSeries(eBK eBoard, i8 i_Board=C_UNKNOWN);
    void vSeries(eBK eBoard, i8 i_Board, const char* azHand);

    Footer* aFooter(void);
  };

  Xseries* _pXseries(void* aAddr);
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* Xseries.hpp --                                                              */
/*============================================================================*/
#pragma once

#include "OptimiserTool.hpp"

namespace STORE
{
  class Xseries : public OptimiserTool
  {
  private:

    CardCtl*     m_pCardCtl;
    TagSet*      m_pJobTags;
    b8           m_bInteractive;


    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Xseries(Bench* pBench);
   ~Xseries(void) noexcept(false);

  private:

    // we start with multiple sub_queries...
    void vSeries_10_Make_Runnable(void);

    // main query only from here...
    void vSeries_80_Index_Scan(bool bEnabled);
    void vSeries_90_Stream_Ordering(bool bEnabled);
    void vSeries_A0_Make_Weavable(void);

    Footer* aFooter(void);
  };

  Xseries* _pXseries(void* aAddr);
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

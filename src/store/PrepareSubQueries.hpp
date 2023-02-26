/*============================================================================*/
/* PrepareSubQueries.hpp -- Prepare SubQueries                                */
/*============================================================================*/
#pragma once

#include "Bench.hpp"


namespace STORE
{
  class PrepareSubQueries : public Object
  {
  private:

    CardIter* m_pCI;

    char m_cTableSyn;

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    PrepareSubQueries(CardIter* pCI);
   ~PrepareSubQueries(void) noexcept(false);

    void vSeries_PrepareSubQueries(void);

    void vGroup_InitialAdjustments(void);
    void vGroup_CondenseInfo(void);
    void vGroup_MetaAnalysis(void);
    void vGroup_PreliminaryConversions(void);

    void vTransform_OuterQueryAnalysis(void);

    void vTransform_MoveOn(void);
    void vTransform_DeleteUnneeded(void);
    void vTransform_ScanBecomesTable(void);
    void vTransform_AddLevel(void);
    void vTransform_NotOps(void);

    void vTransform_TableAnalysis(char* acTableSyn);

    void vTransform_IndexAnalysis(void);
    void vTransform_ProjectionAnalysis(void);

    void vTransform_RpnCalcToBoolCalc(void);


  protected:
    Footer* aFooter(void);
  };

  PrepareSubQueries* _pPrepareSubQueries(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

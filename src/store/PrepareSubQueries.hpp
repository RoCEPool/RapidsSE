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

    void vPrepareSubQueries(void);

  private:
    void vAddLevel(void);
    void vDeleteUnneeded(void);
    void vIndexAnalysis(void);
    void vMoveOn(void);
    void vNotOps(void);
    void vProjectionAnalysis(void);
    void vRpnCalcToBoolCalc(void);
    void vScanBecomesTable(void);
    void vShortenQueryNames(void);
    void vTableAnalysis(char* acTableSyn);

  protected:
    Footer* aFooter(void);
  };

  PrepareSubQueries* _pPrepareSubQueries(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

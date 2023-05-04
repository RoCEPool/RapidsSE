/*============================================================================*/
/* MakeSimpler.hpp -- Prepare SubQueries                                      */
/*============================================================================*/
#pragma once

#include "Bench.hpp"


namespace STORE
{
  class MakeSimpler : public Object
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
    
    MakeSimpler(CardIter* pCI);
   ~MakeSimpler(void) noexcept(false);

    void vSimplify(void);

  private:
    void vAddLevel(void);
    void vAsReview(void);
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

  MakeSimpler* _pMakeSimpler(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

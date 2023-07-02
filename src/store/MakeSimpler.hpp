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
    void vSimplify_passThru(void);

  private:
    void vIndexAnalysis(void);
    void vIndexAnalysis_passThru(void);
    void vRpnCalcToBoolCalc(void);
    void vTableAnalysis(char* acTableSyn);
    void vTableAnalysis_passThru(char* acTableSyn);

  protected:
    Footer* aFooter(void);
  };

  MakeSimpler* _pMakeSimpler(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

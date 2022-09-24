/*============================================================================*/
/* Joiner.hpp --                                                           */
/*============================================================================*/
#pragma once

#include "TransformerTool.hpp"

namespace STORE
{
  class Joiner : public TransformerTool
  {
  private:

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Joiner(Bench* pBench);
   ~Joiner(void) noexcept(false);

    void vAppend_index(void);
    void vAppendPrelim_indexJoin(void);
    void vAppend_indexPred(void);
    void vConvert_absJoin(void);
    void vDelete_joinPred(void);
    void vDelete_scan_R(void);
    void vInsert_joinPred(void);
    void vMark_idxCol(void);

    void vResolve_joinPred(void);
    void vSubsume_scan(void);

  private:
    Card_IndexPred*  pTo_IndexPred (Card_AbsJoin* pCard_AbsJoin,
                                    Card_PredCmp* pCard_PredCmp);

    Footer* aFooter(void);
  };

  Joiner* _pJoiner(void* aAddr);
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

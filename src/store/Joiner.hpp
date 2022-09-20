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
    void vAppend_indexPred(void);
    void vConvert_absJoin(void);
    void vDelete_joinPred(void);
    void vDelete_scan_R(void);
    void vInsert_joinPred(void);

  private:
    void vDiscoverRecursive(Card* pMainCard_filter);
    Card_IndexPred*  pTo_IndexPred (Card_AbsJoin* pCard_AbsJoin,
                                    Card_PredCmp* pCard_PredCmp);

    Footer* aFooter(void);
  };

  Joiner* _pJoiner(void* aAddr);
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

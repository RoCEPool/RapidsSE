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

    void vInsert_preliminary_indexJoin(void);
    void vDeleteAll_absJoin(void);
    void vForm_joinPred(void);
    void vConvertAll_idxColNm_To_streamColOrdinal(void);
    void vRefactor_absJoin_Into_indexJoin(void);
    void vSubsume_scan_Under_absJoin(void);

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

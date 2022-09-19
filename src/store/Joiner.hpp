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

    void vJoin(void);

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

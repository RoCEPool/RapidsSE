/*============================================================================*/
/* Xderive.hpp --                                                       */
/*============================================================================*/
#pragma once

#include "Xmodify.hpp"

namespace STORE
{
  class Table;

  class Xderive : public Xmodify
  {
  public:

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Xderive(eCN eClass);
   ~Xderive(void) noexcept(false);

    void D_vDerive_JoinOrderSeq(Card_JoinOrderSeq* p_joinOrderSeq, Card_PtndJoin* p_PtndJoin);
    void D_vDerive_JoinPredList(Card_JoinPredList* p_joinPredList, Card_BoolCalc* p_boolCalc);

    void D_vDerive_ObyElist(Card_ObyElist* p_obyElist, Index* pIndex);

    void D_vDerive_PtndJoin(Card_PtndJoin* p_ptndJoin, Card_TableList* p_tableList);

    char D_cPRED_TableSyn_First(Card_PRED* p_PRED);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

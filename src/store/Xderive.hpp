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












    void X_vJoinOrderSeq_ExtractJoinOrder(Card_JoinOrderSeq* p_joinOrderSeq, Card_PtndJoin* p_PtndJoin);

    void X_vJoinPredList_Extract(Card_JoinPredList* p_joinPredList, Card_BoolCalc* p_boolCalc);

    void X_vObyElist_Populate(Card_ObyElist* p_obyElist, Index* pIndex);

    void X_vPtndJoin_JoinEst(Card_PtndJoin* p_ptndJoin, Card_TableList* p_tableList);






    Footer* aFooter(void);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

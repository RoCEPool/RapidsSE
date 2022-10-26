/*============================================================================*/
/* Xreshape.hpp --                                                       */
/*============================================================================*/
#pragma once

#include "Xmodify.hpp"


namespace STORE
{
  class Table;

  class Xreshape : public Xmodify
  {
  public:

    // must be last
    Footer m_xFooter;


  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Xreshape(eCN eClass);
   ~Xreshape(void) noexcept(false);











    void X_vIdxColList_Ordinate(Card_IdxColList* p_idxColList, Card_Stream* pCard_Stream);

    void X_vJoinOrderSeq_ExtractJoinOrder(Card_JoinOrderSeq* p_joinOrderSeq, Card_PtndJoin* p_PtndJoin);

    void X_vJoinPredList_Extract(Card_JoinPredList* p_joinPredList, Card_BoolCalc* p_boolCalc);

    void X_vObyElist_Populate(Card_ObyElist* p_obyElist, Index* pIndex);

    void X_vPtndJoin_JoinEst(Card_PtndJoin* p_ptndJoin, Card_TableList* p_tableList);


    void X_vSubCard_ReplaceSub(Card* pSub, Card* pInParent, Card* pWith);




    Footer* aFooter(void);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

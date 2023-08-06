/*============================================================================*/
/* Card_StreamPer.hpp -- Stream Permutation Sequence                          */
/*============================================================================*/
#pragma once

#include "Card_symbol.hpp"

#include "PermGen.hpp"

namespace STORE
{
   class Card_StreamPer : public Card_symbol<Card_StreamPer>
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_StreamPer(void);
   ~Card_StreamPer(void) noexcept(false);

     void vEstStreamCost(Card_TableList* p_tableList,
                         Card_FiltPredList* p_filtPredList);

     eXK eJoinCast(Card_StreamPtn*  p_streamPtn,
                   Card_TableList*  p_tableList,
                   Card_StreamJoin* p_streamJoin);

     i8 iCreatePerSeq(Card_TableList*    p_tableList,
                      Card_JoinPredList* p_joinPredList,
                      PermGen*           pPermGen);

     i8 iOnePermutedJoin_v2(Card_TableList*    p_tableList,
                         Card_JoinPredList* p_joinPredList,
                         PermGen*           pPermGen);

     bool bHasKnownJoin(char cTblSyn);

     bool bIsHigherCost(Card_StreamPer* p_joinPermSeq_arg);

     bool bIsLowerCost(Card_StreamPer* p_joinPermSeq_arg);

     bool bUnicastPermitted(char cTblSyn_prior,
                            char cTblSyn_prev,
                            char cTblSyn_locus);
   };

  Card_StreamPer* _pCard_StreamPer(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

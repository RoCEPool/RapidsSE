/*============================================================================*/
/* Card_JoinPermSeq.hpp -- Join Permutation Sequence                          */
/*============================================================================*/
#pragma once

#include "Card.hpp"

#include "PermGen.hpp"

namespace STORE
{
   class Card_JoinPermSeq : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_JoinPermSeq(void);
   ~Card_JoinPermSeq(void) noexcept(false);

     void vCalculateLoad(Card_TableList* p_tableList);

     eXK eCastKind(Card_TableList* p_tableList,
                   Card_JoinNext*  p_joinNext);

     i8 iOnePermutedJoin(Card_TableList*    p_tableList,
                           Card_JoinPredList* p_joinPredList,
                           PermGen*           pPermGen);

     i8 iOnePermutedJoin_v2(Card_TableList*    p_tableList,
                         Card_JoinPredList* p_joinPredList,
                         PermGen*           pPermGen);

     bool bHasKnownJoin(char cTblSyn);

     bool bIsLessLoad(Card_JoinPermSeq* p_joinPermSeq_arg);

     bool bUnicastSeqFollows(char cTblSyn);
   };

  Card_JoinPermSeq* _pCard_JoinPermSeq(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

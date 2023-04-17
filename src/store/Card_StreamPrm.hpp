/*============================================================================*/
/* Card_StreamPrm.hpp -- Stream Permutation Sequence                          */
/*============================================================================*/
#pragma once

#include "Card.hpp"

#include "PermGen.hpp"

namespace STORE
{
   class Card_StreamPrm : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_StreamPrm(void);
   ~Card_StreamPrm(void) noexcept(false);

     void vEstStreamCost(Card_TableList* p_tableList,
                         Card_FiltPredList* p_filtPredList);

     eXK eStreamCast(Symbol*         p_Y_streamPtn_running,
                     Card_TableList* p_tableList,
                     Card_StreamJoin* p_joinLocus);

     i8 iOnePermutedJoin(Card_TableList*    p_tableList,
                           Card_JoinPredList* p_joinPredList,
                           PermGen*           pPermGen);

     i8 iOnePermutedJoin_v2(Card_TableList*    p_tableList,
                         Card_JoinPredList* p_joinPredList,
                         PermGen*           pPermGen);

     bool bHasKnownJoin(char cTblSyn);

     bool bIsLessCost(Card_StreamPrm* p_joinPermSeq_arg);

     bool bUnicastPermitted(char cTblSyn_prior,
                            char cTblSyn_prev,
                            char cTblSyn_locus);
   };

  Card_StreamPrm* _pCard_StreamPrm(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

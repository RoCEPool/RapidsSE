/*============================================================================*/
/* Card_JoinPermOne.hpp -- Join Permutation List                             */
/*============================================================================*/
#pragma once

#include "Card.hpp"

#include "PermGen.hpp"

namespace STORE
{
   class Card_JoinPermOne : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_JoinPermOne(void);
   ~Card_JoinPermOne(void) noexcept(false);

   void vOnePermutedJoin(Card_TableList*   p_tableList,
                         Card_JoinPredList* p_joinPredList,
                         Card_JoinPermAll*  p_joinPermAll,
                         PermGen*           pPermGen);

     bool bSupportedJoin(char cTblSyn_prior);

   };

  Card_JoinPermOne* _pCard_JoinPermOne(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* Card_JoinPermList.hpp -- Join Permutation List                             */
/*============================================================================*/
#pragma once

#include "Card.hpp"

#include "PermGen.hpp"

namespace STORE
{
   class Card_JoinPermList : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_JoinPermList(void);
   ~Card_JoinPermList(void) noexcept(false);

   void vOnePermutedJoin(Card_TableList*   p_tableList,
                         Card_JoinPredList* p_joinPredList,
                         Card_JoinPermAll*  p_joinPermAll,
                         PermGen*           pPermGen);

   };

  Card_JoinPermList* _pCard_JoinPermList(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

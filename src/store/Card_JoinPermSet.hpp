/*============================================================================*/
/* Card_JoinPermSet.hpp -- Join Permutation Set (of lists)                    */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
   class Card_JoinPermSet : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_JoinPermSet(void);
   ~Card_JoinPermSet(void) noexcept(false);

    static Card_JoinPermSet* pPermute(Card_TableList* p_tableList,
                                      Card_JoinPredList* p_joinPredList);

   };

  Card_JoinPermSet* _pCard_JoinPermSet(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

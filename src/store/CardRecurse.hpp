/*============================================================================*/
/* CardRecurse.hpp -- Recursive Card Iterator                                 */
/*============================================================================*/
#pragma once

#include "CardIter.hpp"

namespace STORE
{
  class CardRecurse : public CardIter
//                    , public ListExo2_t<CardIter>
  {
  protected:
    //bool m_bDontDescend;

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    CardRecurse(Card* pCard);
   ~CardRecurse(void) noexcept(false);

    void vDontDescend(void);
    Card* pRecurseEach(void);
    Card* pRecurseEach(eCK eCard);

  private:

//    bool bPopIter(void);
//    void vPushIter(void);


    Footer* aFooter(void);
  };

  CardRecurse* _pCardRecurse(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

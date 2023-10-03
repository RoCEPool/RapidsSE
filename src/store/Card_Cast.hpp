/*============================================================================*/
/* Card_Cast.hpp -- Stage Cast (aka Next)                                     */
/*============================================================================*/
#pragma once

#include "Card_symbol.hpp"

namespace STORE
{
  class Card_Cast : public Card_symbol<Card_Cast>
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Cast(void);
   ~Card_Cast(void) noexcept(false);

    static eXK eStageCast(Card_StreamPtn* p_streamPtn,
                          Symbol* p_Y_tblCol_prior,
                          Symbol* p_Y_tablePtn_join,
                          Symbol* p_Y_idxCol_join);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_Cast* _pCard_Cast(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

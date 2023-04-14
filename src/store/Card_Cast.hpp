/*============================================================================*/
/* Card_Cast.hpp -- Stage Cast (aka Next)                                     */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_Cast : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Cast(void);
   ~Card_Cast(void) noexcept(false);

    static eXK eStageCast(Card_TablePtn* p_tablePtn_prev,
                          Card_TablePtn* p_tablePtn_curr,
                          Card_Index*    p_index_curr,
                          Card_Bound*    p_bound_curr,
                          Card_Term*&    rp_term_tblCol_to_be_hashed);

  };

  Card_Cast* _pCard_Cast(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* Card_Key.hpp -- Key                                                        */
/*============================================================================*/
#pragma once

#include "Card_ELIST.hpp"
#include "IndexDef.hpp"

namespace STORE
{
  class Card_Key : public Card_ELIST
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Key(void);
   ~Card_Key(void) noexcept(false);

    void vAdd_key_expr_BasedOn_expr(Card_Expr* p_expr, bool bAsc);

    bool bIsEqual(Card_Key* p_key_arg);

    void vTypes_For_stmColOdn(CardIter *pCI);

    void vFactorAll_col_IntoDistinct_expr(CardIter* pCI);

    Stz* szP_Type(void);

    IndexDef* pToIndexDef(void);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);

  };

  Card_Key* _pCard_Key(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

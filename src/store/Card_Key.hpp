/*============================================================================*/
/* Card_Key.hpp -- Key                                                        */
/*============================================================================*/
#pragma once

#include "Card_EXPRLIST.hpp"
#include "IndexDef.hpp"

namespace STORE
{
  class Card_Key : public Card_EXPRLIST
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Key(void);
    Card_Key(eCK eCard, i8 iMaxDepth);
   ~Card_Key(void) noexcept(false);

    void vAdd_key_expr_BasedOn_expr(Card_Expr* p_expr, bool bAsc);
    void vApplyCast(CardIter *pCI, Card_Emit* p_Dic_expr);
    Card_Expr* pDecode_scalar_expr_Via_symbol(Symbol* pSymbol);
    bool bIsEqual(Card_Key* p_key_arg);
    Card_Expr* pFind_expr_key_Matches(Symbol *pSymbol_arg);
    Card_Direction* pGet_direction(i8 i_Direction);

    void vMapColumns(CardIter *pCI);
    void vPrint(Ribbon* pRibbon, i8 iDepth);

    IndexDef* pTo_IndexDef(void);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_Key* _pCard_Key(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

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

    static void vAssemble_injCrlKey(CardIter* pCI);

    void vAdd_key_expr_BasedOn_expr(CardIter *pCI, Card_Expr* p_expr, bool bAsc, bool bAppend);

    Card_Expr* pDecode_scalar_expr_Via_symbol(CardIter *pCI, Symbol* pSymbol);
    Stz* szDirection(i8 i_Element);
    Card_Expr* pFind_expr_key_Matches(CardIter *pCI, Symbol *pSymbol_arg);
    Card_Expr* pFind_expr_key_Matches(CardIter *pCI, eSF eFacet_arg, Symbol* pSymbol_arg);

    void vMapColumns(CardIter *pCI);

    IndexDef* pTo_IndexDef(CardIter* pCI);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_Key* _pCard_Key(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

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
    Cast xCast(void);
    bool bIsEqual(Card_Key* p_key_arg);
    void vEnsureTypeId_expCol(CardIter *pCI);
    void vMapColumns(CardIter *pCI);
    TypeId xTypeId(void);
    void vShortForm(String* pString);

    IndexDef* pToIndexDef(void);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);

  };

  Card_Key* _pCard_Key(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

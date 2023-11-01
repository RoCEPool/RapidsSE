/*============================================================================*/
/* Card_Payload.hpp -- Key                                                        */
/*============================================================================*/
#pragma once

#include "Card_EXPRLIST.hpp"
#include "IndexDef.hpp"

namespace STORE
{
  class Card_Payload : public Card_EXPRLIST
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Payload(void);
    Card_Payload(eCK eCard, i8 iMaxDepth);
   ~Card_Payload(void) noexcept(false);

    bool bIsEqual(Card_Payload* p_key_arg);

    void vTypes_For_stmCol(CardIter *pCI);

    void vFactorAll_col_IntoDistinct_expr(CardIter* pCI);

    void vMapColumns(CardIter *pCI);
    eCT  eColType(void);

    void vShortForm(String* pString);

    IndexDef* pToIndexDef(void);

    void vX(void);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);

  };

  Card_Payload* _pCard_Payload(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

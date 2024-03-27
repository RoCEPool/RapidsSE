/*============================================================================*/
/* Card_RpnExpr.hpp -- RPN Sequence of Terms                                 */
/*============================================================================*/
#pragma once

#include "Card_symbol.hpp"

namespace STORE
{
  class Card_RpnExpr : public Card_symbol<Card_RpnExpr>
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_RpnExpr(void);
   ~Card_RpnExpr(void) noexcept(false);

   Card_Term* pHead_term(void);
   Facet      xPop_facet(void);
   Card_Term* pPop_term(void);
   Card_Term* pPop_term_If(eSF eFacet);
   Card_Term* pPop_term_If(eSF eFacet, Stz* szFacet);
  };

  Card_RpnExpr* _pCard_RpnExpr(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

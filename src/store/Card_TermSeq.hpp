/*============================================================================*/
/* Card_TermSeq.hpp -- RPN Sequence of Terms                                 */
/*============================================================================*/
#pragma once

#include "Card_symbol.hpp"

namespace STORE
{
  class Card_TermSeq : public Card_symbol<Card_TermSeq>
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_TermSeq(void);
   ~Card_TermSeq(void) noexcept(false);

   Card_Term* pHead_term(void);
   Facet      xPop_facet(void);
   Card_Term* pPop_term(void);
   Card_Term* pPop_term_If(eSF eFacet);
   Card_Term* pPop_term_If(eSF eFacet, Stz* szFacet);
  };

  Card_TermSeq* _pCard_TermSeq(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

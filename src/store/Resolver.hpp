/*============================================================================*/
/* Resolver.hpp --                                                           */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Dissector.hpp"

namespace STORE
{
  class Resolver : public Object
  {
  private:

    Card_SubSet* m_pCard_SubSet;

    Card_Stream* m_pStream_a;
    Card_Stream* m_pStream_b;

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Resolver(Card_SubSet* pCard_SubSet);
  
   ~Resolver(void) noexcept(false);

    void vResolve(Card* pMainCard, Stz* szTerm, Stz*& rszStreamNm, Stz*& rszStreamColNm);

  private:
    void vLocateStreams(Card* pMainCard);
    void vLocateStreams_two(Card_AbsJoin* pCard_AbsJoin);

    void vSet_Stream_a(Card_Scan* pCard_Scan);
    void vSet_Stream_b(Card_Scan* pCard_Scan);

    Footer* aFooter(void);
  };

  Resolver* _pResolver(void* aAddr);
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

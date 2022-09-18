/*============================================================================*/
/* Resolver.hpp --                                                           */
/*============================================================================*/
#pragma once

#include "OptimiserTool.hpp"

namespace STORE
{
  class Resolver : public OptimiserTool
  {
  private:

    Card_Stream* m_pStream_a;
    Card_Stream* m_pStream_b;

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Resolver(Bench* pBench);
  
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

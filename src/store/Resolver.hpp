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

    Card_Stream* m_pStream_l;
    Card_Stream* m_pStream_r;

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Resolver(Bench* pBench);
  
   ~Resolver(void) noexcept(false);

    void vResolve(Card* pMainCard, Stz* szTerm, String* pStreamNm, String* pStreamColNm);

  private:
    void vLocateStreams(Card* pMainCard);

    void vSet_Stream_a(Card_Scan* pCard_Scan);
    void vSet_Stream_b(Card_Scan* pCard_Scan);

    Footer* aFooter(void);
  };

  Resolver* _pResolver(void* aAddr);
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

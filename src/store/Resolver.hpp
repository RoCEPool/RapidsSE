/*============================================================================*/
/* Resolver.hpp --                                                           */
/*============================================================================*/
#pragma once

#include "TransformerTool.hpp"

namespace STORE
{
  class Resolver : public TransformerTool
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

    void vResolve_streamColOrdinal_To_tableColOrdinal(String* psTableColOrdinal, Stz* szStreamColOrdinal);

    void vResolve(Card* pMainCard, Stz* szTerm,
                  String* pStreamNm, String* pStreamColNm, i8& i_StreamCol);



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

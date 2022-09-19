/*============================================================================*/
/* Streamer.hpp --                                                           */
/*============================================================================*/
#pragma once

#include "TransformerTool.hpp"

namespace STORE
{
  class Streamer : public TransformerTool
  {
  private:

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Streamer(Bench* pBench);
  
   ~Streamer(void) noexcept(false);

    void vStream(void);

  private:

    void vResolveRecursive(Card* pCard);

    Footer* aFooter(void);
  };

  Streamer* _pStreamer(void* aAddr);
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

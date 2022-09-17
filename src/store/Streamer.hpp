/*============================================================================*/
/* Streamer.hpp --                                                           */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Dissector.hpp"

namespace STORE
{
  class Streamer : public Object
  {
  private:

    Card_SubSet* m_pCard_SubSet;

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Streamer(Card_SubSet* pCard_SubSet);
  
   ~Streamer(void) noexcept(false);


  private:

    Footer* aFooter(void);
  };

  Streamer* _pStreamer(void* aAddr);
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

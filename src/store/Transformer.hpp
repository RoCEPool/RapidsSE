/*============================================================================*/
/* Transformer.hpp --                                                         */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Dissector.hpp"
#include "Simplifier.hpp"
#include "Streamer.hpp"
#include "Joiner.hpp"

namespace STORE
{
  class Transformer : public Object
  {
  private:

    Optimiser*   m_pOptimiser;

    Board*       m_pBoard;
    Card_SubSet* m_pCard_SubSet;

    Simplifier*  m_pSimplifier;
    Streamer*    m_pStreamer;
    Joiner*      m_pJoiner;

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Transformer(Optimiser* pOptimiser);
  
   ~Transformer(void) noexcept(false);

    void vTransform(void);

  private:

    Footer* aFooter(void);
  };

  Transformer* _pTransformer(void* aAddr);
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

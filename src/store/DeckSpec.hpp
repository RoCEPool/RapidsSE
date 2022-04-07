/*============================================================================*/
/* StageSpecSet.hpp -- List of Stage Specifications                           */
/*============================================================================*/
#pragma once

#include "StageSpec.hpp"

namespace STORE
{
  class DeckSpec : public ListNL_t<StageSpec>
  {
  private:
   
    VertexSpec*  m_pVertexSpec; // vertexSpec this deckSpec belongs to

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    DeckSpec(VertexSpec* pVertexSpec);
   ~DeckSpec(void) noexcept(false);
  
  private:
  
    Footer* aFooter(void);

  };

  DeckSpec* _pDeckSpec(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

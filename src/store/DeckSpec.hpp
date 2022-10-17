/*============================================================================*/
/* StageSpecSet.hpp -- List of Stage Specifications                           */
/*============================================================================*/
#pragma once

#include "StageSpec.hpp"
#include "ParamsSpec.hpp"

namespace STORE
{
  class DeckSpec : public ListNL_t<StageSpec>
  {
  private:
   
    VertexSpec*  m_pVertexSpec; // vertexSpec this deckSpec belongs to

    ParamsSpec*  m_pParamsSpec; // parameter set for stage set

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    DeckSpec(VertexSpec* pVertexSpec);
   ~DeckSpec(void) noexcept(false);
  
    ParamsSpec* pParamsSpec(void);
    void vParamsSpec(ParamsSpec* pParamsSpec);

  private:
    Footer* aFooter(void);
  };

  DeckSpec* _pDeckSpec(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

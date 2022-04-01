/*============================================================================*/
/* StageSet.hpp -- List of Stages                                             */
/*============================================================================*/
#pragma once

#include "Stage.hpp"

namespace STORE { class VertexSpec; }
namespace STORE
{
  class Deck : public ListNL_t<Stage>
  {
  private:
    Vertex*     m_pVertex;
    VertexSpec* m_pVertexSpec; // specification of this vertex

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Deck(Vertex* pVertex, VertexSpec* pPipeSpec);
   ~Deck(void) noexcept(false);
  
    Pipeline* pPipeline(void);
    Vertex* pVertex(void);

  private:
  
    Footer* aFooter(void);

  };

  Deck* _pDeck(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

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
   
    Vertex* m_pPipeWork;
    VertexSpec* m_pPipeSpec; // specification of this pipework

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Deck(Vertex* pPipeWork, VertexSpec* pPipeSpec);
   ~Deck(void) noexcept(false);
  
    Pipeline* pPipeline(void);
    Vertex* pPipeWork(void);

  private:
  
    Footer* aFooter(void);

  };

  Deck* _pDeck(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

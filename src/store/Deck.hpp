/*============================================================================*/
/* Deck.hpp -- Deck (of Vertex of Pipeline)                                   */
/*============================================================================*/
#pragma once

#include "Stage.hpp"

namespace STORE { class DeckSpec; class VertexSpec; }

namespace STORE
{
  class Deck : public VectorNL_t<CN_DECK,Stage,C_MAX_STAGES>
  {
  private:
    Vertex*     m_pVertex;
    DeckSpec*   m_pDeckSpec;

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Deck(Vertex*   pVertex,
         DeckSpec* pDeckSpec);
   ~Deck(void) noexcept(false);
  
    static void vRepair(Object *pObject);

    Op*  pOp0(void);
    i8   nOpsOnNode(i8 iNode);

    Pipeline* pPipeline(void);
  
    void vSetInvoker(Task* pInvoker);
    Stage* pStage0(void);
  
    Vertex* pVertex(void);

  private:
    void vCreateStages(void);

    Footer* aFooter(void);

  };

  Deck* _pDeck(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

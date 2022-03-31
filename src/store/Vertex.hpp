/*============================================================================*/
/* Vertex.hpp -- Vertex of Pipeline                                           */
/*============================================================================*/
#pragma once

namespace STORE { class Stage;    class Deck;
                  class VertexSpec; class SubPipeWorks; }

namespace STORE
{
  class Vertex : public ListNL_t<Vertex>
               , public ListItem_t<Vertex>
  {
  private:
    Pipeline*   m_pPipeline;     // which pipeline bubble this vertex is within
    Vertex*     m_pParentVertex; // parent of this vertex
  
    VertexSpec* m_pVertexSpec;   // specification of this vertex
  
    Deck*    m_pStaging;
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Vertex(Pipeline* pPipeline,
           Vertex*   pParentVertex,
           VertexSpec* pPipeSpec);
   ~Vertex(void) noexcept(false);
  
    bool bCancel(Task* pTriggeringTask);

    VertexSpec* pPipeSpec(void);
    Op*  pOp0(void);
    i8   nOpsOnNode(i8 iNode);
    Pipeline* pPipeline(void);
    void vSetInvoker(Task* pInvoker);
    void vSetupJob(void);
    void vSpawnOpTasks(void);
    Stage* pStage0(void);
    i8   nStages(void);
    Deck* pStaging(void);
    Stream* pStream0(void);

    void vCreatePipework();

  private:
    Footer* aFooter(void);
  };
  
  Vertex* _pVertex(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

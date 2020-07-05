/*============================================================================*/
/* ProjectRsb.hpp -- Project RsbBuffers into new RsbBuffers                   */
/*============================================================================*/
#pragma once

namespace Expressions
{
  class ProjectRsb : public Object
  {
  private:
    Pot*       m_pPot;          // working pot of destination buffers
    RsbBuffer* m_pSrcRsbBuffer; // source rsb buffer
    RowSlab*   m_pDstRowSlab;   // current destination slab (in pot)
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    ProjectRsb(void);
   ~ProjectRsb(void) noexcept(false);
  
    void vProject(RsbBuffer* pSrcRsbBuffer, Expr* pExpr, Pot* pPot);
    
  private:
    void vCopyIntoDst(Rsb* aSrcRsbc);
    void vNewDstRsb(void);

    Footer* aFooter(void);
  };

  ProjectRsb* _pProjectRsb(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

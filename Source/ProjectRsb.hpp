/*============================================================================*/
/* ProjectRsb.hpp -- Project RsbBuffers into new RsbBuffers                   */
/*============================================================================*/
#pragma once

namespace Expressions
{
  class ProjectRsb : public Object
  {
  private:
      
    Pot*       m_pPot;  

    RsbBuffer* m_pFromRsbBuffer; // the
  
    RsbBuffer* m_pIntoRsbBuffer; // the
    RowSlab*   m_pIntoRowSlab;
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    ProjectRsb(void);
   ~ProjectRsb(void) noexcept(false);
  
    void vProject(RsbBuffer* pRsbBuffer, Expr* pExpr, Pot* pPot);
  
    RsbBuffer* pCompleted(i8 i_Ptn);
    RsbBuffer* pDangling(i8 i_Ptn);
  
  private:
    void vCopyRsb(i8 i_Ptn, Rsb* aRsbSrc);
    void vNewDstRsb(i8 i_Ptn);

    Footer* aFooter(void);
  };

  ProjectRsb* _pProjectRsb(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

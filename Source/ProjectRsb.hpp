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
    RowSlab*   m_pDstRowSlab;   // current destination slab (in pot)
    RsbBuffer* m_pSrcRsbBuffer; // source rsb buffer
    StreamDef* m_pStreamDefOut; // output stream def
  
    bool       m_bPixInited;
    StreamPix  m_oStreamPix;    // condensed stream def
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    ProjectRsb(void);
   ~ProjectRsb(void) noexcept(false);
  
    void vProject(Pot* pPot, RsbBuffer* pSrcRsbBuffer,
                  Expr* pExpr, StreamDef* pStreamDefOut);
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

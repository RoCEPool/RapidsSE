/*============================================================================*/
/* ProjectRsb.cpp -- Project RsbBuffers into new RsbBuffers                   */
/*============================================================================*/

#include "Expressions.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(Expressions,ProjectRsb,CN_PROJECTRSB)
__aNewFunction(ProjectRsb,CN_PROJECTRSB)
__vDeleteFunction(ProjectRsb,CN_PROJECTRSB)
__aFooterFunction(ProjectRsb)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ProjectRsb::ProjectRsb(void)
: Object(CN_PROJECTRSB)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PROJECTRSB,_FOOTER_);
  
  m_pPot          = nullptr;
  m_pSrcRsbBuffer = nullptr;
  m_pDstRowSlab   = nullptr;
  
  _checkInitialised_(CN_PROJECTRSB);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_PROJECTRSB,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
ProjectRsb::~ProjectRsb(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PROJECTRSB,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* convert                                                                    */
/*----------------------------------------------------------------------------*/
void ProjectRsb::vProject(RsbBuffer* pSrcRsbBuffer, Expr* pExpr, Pot* pPot)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PROJECTRSB,_FOOTER_);
  
  m_pSrcRsbBuffer = pSrcRsbBuffer;
  
  m_pPot = pPot;
  
  i8 i_Ptn = 0;
  
  RowSlab* pSrcRowSlab = pSrcRsbBuffer->pRowSlab();
  
  i8 nRows = pSrcRowSlab->nRows();
  for (i8 i_Row = 0; i_Row < nRows; i_Row++) {
    
    Rsb* aSrcRsb = pSrcRowSlab->aRsbAdr(i_Row);
    
    i8 bCopy = pExpr->iEvaluate(aSrcRsb, pSrcRowSlab);
        
    if (bCopy)
      vCopyIntoDst(aSrcRsb);
  }
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* copy source rsb into destination partition row slab                        */
/*----------------------------------------------------------------------------*/
void ProjectRsb::vCopyIntoDst(Rsb* aSrcRsb)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PROJECTRSB,_FOOTER_);
    
  // new a destination row in target partition row slab
  vNewDstRsb();
  
  // copy that
  bool bSrcHasPtnHash = true;
  bool bCopyPtnHash   = false;
  m_pDstRowSlab->vCopyRsb(aSrcRsb,bSrcHasPtnHash,bCopyPtnHash);
}

/*----------------------------------------------------------------------------*/
/* new a destination rsb row                                                  */
/*----------------------------------------------------------------------------*/
void ProjectRsb::vNewDstRsb(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PROJECTRSB,_FOOTER_);
    
  // get rsb buffer for given partition (new a first buffer if ncessary)
  RsbBuffer* pDstRsbBuffer = _pRsbBuffer(m_pPot->pBuffer());
  if (! pDstRsbBuffer) {
    pDstRsbBuffer = new RsbBuffer;
    pDstRsbBuffer->pRowSlab()->vCopyPix(m_pSrcRsbBuffer->pRowSlab()->aStreamPix());
    //pDstRsbBuffer->pRowSlab()->aStreamPix()->vRemovePtnHash();
    m_pPot->vAddBuffer(pDstRsbBuffer);
  }
  
  // new a row in rsb buffer (new another rsb buffer if necessary)
  bool bNewed = pDstRsbBuffer->pRowSlab()->bNewRow();
  if (! bNewed) {
    pDstRsbBuffer = new RsbBuffer;
    pDstRsbBuffer->pRowSlab()->vCopyPix(m_pSrcRsbBuffer->pRowSlab()->aStreamPix());
    //pDstRsbBuffer->pRowSlab()->aStreamPix()->vRemovePtnHash();
    m_pPot->vAddBuffer(pDstRsbBuffer);
    bNewed = pDstRsbBuffer->pRowSlab()->bNewRow();
    ASSERT(bNewed);
  }
  
  pDstRsbBuffer->vPartitionNo(0);
  
  m_pDstRowSlab = pDstRsbBuffer->pRowSlab();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* FilterRsb.cpp -- Filter RsbBuffers into new RsbBuffers                     */
/*============================================================================*/

#include "Expressions.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(Expressions,FilterRsb,CN_FILTERRSB)
__aNewFunction(FilterRsb,CN_FILTERRSB)
__vDeleteFunction(FilterRsb,CN_FILTERRSB)
__aFooterFunction(FilterRsb)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
FilterRsb::FilterRsb(void)
: Object(CN_FILTERRSB)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FILTERRSB,_FOOTER_);
  
  m_pPot          = nullptr;
  m_pSrcRsbBuffer = nullptr;
  m_pDstRowSlab   = nullptr;
  
  _checkInitialised_(CN_FILTERRSB);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_FILTERRSB,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
FilterRsb::~FilterRsb(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FILTERRSB,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* convert                                                                    */
/*----------------------------------------------------------------------------*/
void FilterRsb::vFilter(Pot* pPot, RsbBuffer* pSrcRsbBuffer, Expr* pExpr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FILTERRSB,_FOOTER_);
  
  m_pSrcRsbBuffer = pSrcRsbBuffer;
  
  m_pPot = pPot;
  
  i8 i_Ptn = 0;
  
  RowSlab* pSrcRowSlab = pSrcRsbBuffer->pRowSlab();
  
  i8 nRows = pSrcRowSlab->nRows();
  for (i8 i_Row = 0; i_Row < nRows; i_Row++) {
    
    Rsb* aSrcRsb = pSrcRowSlab->aRsbAdr(i_Row);
    
    i8 bCopy = pExpr->iFilter(aSrcRsb, pSrcRowSlab);
        
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
void FilterRsb::vCopyIntoDst(Rsb* aSrcRsb)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FILTERRSB,_FOOTER_);
    
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
void FilterRsb::vNewDstRsb(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FILTERRSB,_FOOTER_);
    
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

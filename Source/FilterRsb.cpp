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

  // m_oHashSet

  m_pFromRsbBuffer = nullptr;

  m_pIntoRowSlab   = nullptr;
  
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
void FilterRsb::vFilter(RsbBuffer* pRsbBuffer, Expr* pExpr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FILTERRSB,_FOOTER_);
  
  m_pFromRsbBuffer = pRsbBuffer;
  
  i8 i_Ptn = 0;
  
  RowSlab* pRowSlab = pRsbBuffer->pRowSlab();
  
  i8 nRows = pRowSlab->nRows();
  for (i8 i_Row = 0; i_Row < nRows; i_Row++) {
    
    Rsb* aRsbSrc = pRowSlab->aRsbAdr(i_Row);
        
    vCopyRsb(i_Ptn,aRsbSrc);
  }
}

/*----------------------------------------------------------------------------*/
/* get any completed/full buffer                                              */
/*----------------------------------------------------------------------------*/
RsbBuffer* FilterRsb::pCompleted(i8 i_Ptn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FILTERRSB,_FOOTER_);
  
  RsbBuffer* pRsbBuffer = _pRsbBuffer(m_oHashSet.pCompletedBuffer(i_Ptn));
  
  return pRsbBuffer;
}

/*----------------------------------------------------------------------------*/
/* get any dangling/last buffer                                               */
/*----------------------------------------------------------------------------*/
RsbBuffer* FilterRsb::pDangling(i8 i_Ptn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FILTERRSB,_FOOTER_);
  
  RsbBuffer* pRsbBuffer = _pRsbBuffer(m_oHashSet.pDanglingBuffer(i_Ptn));
  
  return pRsbBuffer;}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* copy source rsb into target partition row slab                             */
/*----------------------------------------------------------------------------*/
void FilterRsb::vCopyRsb(i8 i_Ptn, Rsb* aRsbSrc)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FILTERRSB,_FOOTER_);
    
  // new a destination row in target partition row slab
  vNewDstRsb(i_Ptn);
  
  // copy that
  bool bSrcHasPtnHash = true;
  bool bCopyPtnHash   = false;
  m_pIntoRowSlab->vCopyRsb(aRsbSrc,bSrcHasPtnHash,bCopyPtnHash);
}

/*----------------------------------------------------------------------------*/
/* new a destination rsb row in rsb slab in hash set                          */
/*----------------------------------------------------------------------------*/
void FilterRsb::vNewDstRsb(i8 i_Ptn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FILTERRSB,_FOOTER_);
    
  // get rsb buffer for given partition (new a first buffer if ncessary)
  RsbBuffer* pDstRsbBuffer = _pRsbBuffer(m_oHashSet.pBuffer(i_Ptn));
  if (! pDstRsbBuffer) {
    pDstRsbBuffer = new RsbBuffer;
    pDstRsbBuffer->pRowSlab()->vCopyPix(m_pFromRsbBuffer->pRowSlab()->aStreamPix());
    //pDstRsbBuffer->pRowSlab()->aStreamPix()->vRemovePtnHash();
    m_oHashSet.vAddBuffer(i_Ptn,pDstRsbBuffer);
  }
  
  // new a row in rsb buffer (new another rsb buffer if necessary)
  bool bNewed = pDstRsbBuffer->pRowSlab()->bNewRow();
  if (! bNewed) {
    pDstRsbBuffer = new RsbBuffer;
    pDstRsbBuffer->pRowSlab()->vCopyPix(m_pFromRsbBuffer->pRowSlab()->aStreamPix());
    //pDstRsbBuffer->pRowSlab()->aStreamPix()->vRemovePtnHash();
    m_oHashSet.vAddBuffer(i_Ptn,pDstRsbBuffer);
    bNewed = pDstRsbBuffer->pRowSlab()->bNewRow();
    ASSERT(bNewed);
  }
  
  pDstRsbBuffer->vPartitionNo(i_Ptn);
  
  m_pIntoRowSlab = pDstRsbBuffer->pRowSlab();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

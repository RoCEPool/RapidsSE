/*============================================================================*/
/* ToPtnRsb.cpp -- Convert rsb buffers to partitioned row slab buffers        */
/*============================================================================*/

#include "Load.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(Load,ToPtnRsb,CN_TOPTNRSB)
__aNewFunction(ToPtnRsb,CN_TOPTNRSB)
__vDeleteFunction(ToPtnRsb,CN_TOPTNRSB)
__aFooterFunction(ToPtnRsb)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ToPtnRsb::ToPtnRsb(void)
: Object(CN_TOPTNRSB)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOPTNRSB,_FOOTER_);

  // m_oHashSet

  m_pFromRsbBuffer = nullptr;

  m_pIntoRowSlab   = nullptr;
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_TOPTNRSB,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
ToPtnRsb::~ToPtnRsb(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOPTNRSB,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* convert                                                                    */
/*----------------------------------------------------------------------------*/
void ToPtnRsb::vConvert(RowBuffer* pRsbBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOPTNRSB,_FOOTER_);
  
  m_pFromRsbBuffer = pRsbBuffer;
  
  i8 i_Ptn = 0;
  
  RowSlab* pRowSlab = pRsbBuffer->pRowSlab();
  
  i8 nRows = pRowSlab->nRows();
  for (i8 i_Row = 0; i_Row < nRows; i_Row++) {
    
    Row* aRowSrc = pRowSlab->pRowAddress(i_Row);
    
    i_Ptn = pRowSlab->iPtnOfRow(aRowSrc);
    
    vCopyRow(i_Ptn,aRowSrc);
  }
}

/*----------------------------------------------------------------------------*/
/* get any completed/full buffer                                              */
/*----------------------------------------------------------------------------*/
RowBuffer* ToPtnRsb::pCompleted(i8 i_Ptn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOPTNRSB,_FOOTER_);
  
  RowBuffer* pRsbBuffer = _pRowBuffer(m_oHashSet.pCompletedBuffer(i_Ptn));
  
  return pRsbBuffer;
}

/*----------------------------------------------------------------------------*/
/* get any dangling/last buffer                                               */
/*----------------------------------------------------------------------------*/
RowBuffer* ToPtnRsb::pDangling(i8 i_Ptn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOPTNRSB,_FOOTER_);
  
  RowBuffer* pRsbBuffer = _pRowBuffer(m_oHashSet.pDanglingBuffer(i_Ptn));
  
  return pRsbBuffer;}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* copy source row into target partition row slab                             */
/*----------------------------------------------------------------------------*/
void ToPtnRsb::vCopyRow(i8 i_Ptn, Row* aRowSrc)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOPTNRSB,_FOOTER_);
    
  // new a destination row in target partition row slab
  vNewDstRow(i_Ptn);
  
  // copy that
  bool bSrcHasPtnHash = true;
  bool bCopyPtnHash   = false;
  m_pIntoRowSlab->vCopyRow(aRowSrc,bSrcHasPtnHash,bCopyPtnHash);
}

/*----------------------------------------------------------------------------*/
/* new a destination row in rsb slab in hash set                              */
/*----------------------------------------------------------------------------*/
void ToPtnRsb::vNewDstRow(i8 i_Ptn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOPTNRSB,_FOOTER_);
    
  // get rsb buffer for given partition (new a first buffer if ncessary)
  RowBuffer* pDstRsbBuffer = _pRowBuffer(m_oHashSet.pBuffer(i_Ptn));
  if (! pDstRsbBuffer) {
    pDstRsbBuffer = new RowBuffer;
    pDstRsbBuffer->pRowSlab()->vCopyPix(m_pFromRsbBuffer->pRowSlab()->aStreamPix());
    pDstRsbBuffer->pRowSlab()->aStreamPix()->vRemovePtnHash();
    m_oHashSet.vAddBuffer(i_Ptn,pDstRsbBuffer);
  }
  
  // new a row in rsb buffer (new another rsb buffer if necessary)
  Row* aNewRow = pDstRsbBuffer->pRowSlab()->aAllotNewRow();
  if (! aNewRow) {
    pDstRsbBuffer = new RowBuffer;
    pDstRsbBuffer->pRowSlab()->vCopyPix(m_pFromRsbBuffer->pRowSlab()->aStreamPix());
    pDstRsbBuffer->pRowSlab()->aStreamPix()->vRemovePtnHash();
    m_oHashSet.vAddBuffer(i_Ptn,pDstRsbBuffer);
    aNewRow = pDstRsbBuffer->pRowSlab()->aAllotNewRow();
    ASSERT(aNewRow);
  }
  
  pDstRsbBuffer->vPtnNo(i_Ptn);
  
  m_pIntoRowSlab = pDstRsbBuffer->pRowSlab();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

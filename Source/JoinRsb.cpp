/*============================================================================*/
/* JoinRsb.cpp -- Project RsbBuffers into new RsbBuffers                      */
/*============================================================================*/

#include "Expressions.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(Expressions,JoinRsb,CN_JOINRSB)
__aNewFunction(JoinRsb,CN_JOINRSB)
__vDeleteFunction(JoinRsb,CN_JOINRSB)
__aFooterFunction(JoinRsb)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
JoinRsb::JoinRsb(void)
: Object(CN_JOINRSB)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOINRSB,_FOOTER_);
  
  m_pPot          = nullptr;
  m_pSrcRsbBuffer = nullptr;
  m_pDstRowSlab   = nullptr;
  m_pStreamDefOut = nullptr;
  
  m_bPixInited    = false;
  
  _checkInitialised_(CN_JOINRSB);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOINRSB,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
JoinRsb::~JoinRsb(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOINRSB,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* convert                                                                    */
/*----------------------------------------------------------------------------*/
void JoinRsb::vJoin(Pot* pPot, RsbBuffer* pSrcRsbBuffer,
                    Expr* pExpr, StreamDef* pStreamDefOut)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOINRSB,_FOOTER_);
  
  m_pStreamDefOut = pStreamDefOut;

  m_pSrcRsbBuffer = pSrcRsbBuffer;
  
  m_pPot = pPot;

  if (! m_bPixInited) {
    m_bPixInited = true;
    m_pStreamDefOut->vToStreamPix(&m_oStreamPix,true);
  }
  
  RowSlab* pSrcRowSlab = pSrcRsbBuffer->pRowSlab();
  
  i8 nRows = pSrcRowSlab->nRows();
  for (i8 i_Row = 0; i_Row < nRows; i_Row++) {
    
    Rsb* aSrcRsb = pSrcRowSlab->aRsbAdr(i_Row);
    
    // new a destination row in destination partition row slab
    vNewDstRsb();

    i8 bCopy = pExpr->iProject(pPot,m_pDstRowSlab,aSrcRsb,pSrcRowSlab);
        
    //vCopyIntoDst(aSrcRsb);
  }
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* copy source rsb into destination partition row slab                        */
/*----------------------------------------------------------------------------*/
void JoinRsb::vCopyIntoDst(Rsb* aSrcRsb)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOINRSB,_FOOTER_);
    
  // copy src into rst
  bool bSrcHasPtnHash = true;
  bool bCopyPtnHash   = false;
  m_pDstRowSlab->vCopyRsb(aSrcRsb,bSrcHasPtnHash,bCopyPtnHash);
}

/*----------------------------------------------------------------------------*/
/* new a destination rsb row                                                  */
/*----------------------------------------------------------------------------*/
void JoinRsb::vNewDstRsb(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOINRSB,_FOOTER_);
    
  // get rsb buffer for given partition (new a first buffer if ncessary)
  RsbBuffer* pDstRsbBuffer = _pRsbBuffer(m_pPot->pBuffer());
  if (! pDstRsbBuffer) {
    pDstRsbBuffer = new RsbBuffer;
    pDstRsbBuffer->pRowSlab()->vCopyPix(&m_oStreamPix);
    //pDstRsbBuffer->pRowSlab()->vCopyPix(m_pSrcRsbBuffer->pRowSlab()->aStreamPix());
    //pDstRsbBuffer->pRowSlab()->aStreamPix()->vRemovePtnHash();
    m_pPot->vAddBuffer(pDstRsbBuffer);
  }
  
  // new a row in rsb buffer (new another rsb buffer if necessary)
  bool bNewed = pDstRsbBuffer->pRowSlab()->bNewRow();
  if (! bNewed) {
    pDstRsbBuffer = new RsbBuffer;
    pDstRsbBuffer->pRowSlab()->vCopyPix(&m_oStreamPix);
    //pDstRsbBuffer->pRowSlab()->vCopyPix(m_pSrcRsbBuffer->pRowSlab()->aStreamPix());
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

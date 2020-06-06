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
void ToPtnRsb::vConvert(RsbBuffer* pRsbBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOPTNRSB,_FOOTER_);
  
  // for the moment we partition to 0
  i8 i_Ptn = 0;
  
  RowSlab* pRowSlab = pRsbBuffer->pRowSlab();
  
  i8 nRows = pRowSlab->nRows();
  for (i8 i_Row = 0; i_Row < nRows; i_Row++) {
    
    Rsb* aRsb = pRowSlab->aRsb(i_Row);
    
    vCopyIntoHashSet(i_Ptn,aRsb);
  }
}

/*----------------------------------------------------------------------------*/
/* get any completed/full buffer                                              */
/*----------------------------------------------------------------------------*/
RsbBuffer* ToPtnRsb::pCompleted(i8 i_Ptn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOPTNRSB,_FOOTER_);
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get any dangling/last buffer                                               */
/*----------------------------------------------------------------------------*/
RsbBuffer* ToPtnRsb::pDangling(i8 i_Ptn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOPTNRSB,_FOOTER_);
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* copy rsb into hash set                                                     */
/*----------------------------------------------------------------------------*/
void ToPtnRsb::vCopyIntoHashSet(i8 i_Ptn, Rsb* aRsbSrc)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);
    
}

/*----------------------------------------------------------------------------*/
/* new a rsb row in hash set                                                  */
/*----------------------------------------------------------------------------*/
void ToPtnRsb::vNewHashSetRow(i8 i_Ptn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);
    
  RsbBuffer* pRsbBuffer = _pRsbBuffer(m_oHashSet.pBuffer(i_Ptn));
  if (! pRsbBuffer) {
    pRsbBuffer = new RsbBuffer;
    m_oHashSet.vAddBuffer(i_Ptn,pRsbBuffer);
  }
  
  bool bNewed = pRsbBuffer->pRowSlab()->bNewRow();
  if (! bNewed) {
    pRsbBuffer = new RsbBuffer;
    m_oHashSet.vAddBuffer(i_Ptn,pRsbBuffer);
    bNewed = pRsbBuffer->pRowSlab()->bNewRow();
    
    ASSERT(bNewed);
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

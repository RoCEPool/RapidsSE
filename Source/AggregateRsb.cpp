/*============================================================================*/
/* AggregateRsb.cpp -- Aggregate RsbBuffers into new RsbBuffers               */
/*============================================================================*/

#include "Expressions.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(Expressions,AggregateRsb,CN_AGGREGATERSB)
__aNewFunction(AggregateRsb,CN_AGGREGATERSB)
__vDeleteFunction(AggregateRsb,CN_AGGREGATERSB)
__aFooterFunction(AggregateRsb)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
AggregateRsb::AggregateRsb(void)
: Object(CN_AGGREGATERSB)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_AGGREGATERSB,_FOOTER_);
  
  m_pPot          = nullptr;
  m_pSrcRsbBuffer = nullptr;
  m_pDstRowSlab   = nullptr;
  m_pStreamDefOut = nullptr;
  
  m_bPixInited    = false;
  
  _checkInitialised_(CN_AGGREGATERSB);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_AGGREGATERSB,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
AggregateRsb::~AggregateRsb(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_AGGREGATERSB,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* convert                                                                    */
/*----------------------------------------------------------------------------*/
void AggregateRsb::vAggregate(Pot* pPot, RsbBuffer* pSrcRsbBuffer,
                              IndexTree* pIndexTree, Expr* pExpr, StreamDef* pStreamDefOut)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_AGGREGATERSB,_FOOTER_);
  
  m_pStreamDefOut = pStreamDefOut;

  m_pSrcRsbBuffer = pSrcRsbBuffer;
  
  m_pPot = pPot;

  if (! m_bPixInited) {
    m_bPixInited = true;
    m_pStreamDefOut->vToStreamPix(&m_oStreamPix,true);
  }
  
  EntWrk xEntWrk;
  SP_Ent spEnt;
  SP_Key spKey;
  Ent* aPayloadEntry = nullptr;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the indexDef
  IndexDef* pIndexDef = pIndexTree->pIndexDef();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the src rowSlab from the SrcRsbBuffer
  RowSlab* pSrcRowSlab = pSrcRsbBuffer->pRowSlab();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get no of rows in the srcRsbSlab
  i8 nRows = pSrcRowSlab->nRows();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -- - - - - - -
  // for each row
  for (i8 i_Row = 0; i_Row < nRows; i_Row++) {
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  - - - - - - -
    // get the srcRsb
    Rsb* aSrcRsb = pSrcRowSlab->aRsbAdr(i_Row);
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  - - - - - - -
    // extract the key from the SrcRsb
    xEntWrk.vElmSeqSize(0);
    pIndexTree->vExtractKey(&xEntWrk,aSrcRsb);
    vTrace("\n\nEntWrk : %s\n"_Stz,spEnt.sz(xEntWrk.aEnt(),pIndexDef));
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  - - - - - - -
    // position in the indexTree
    EntBuffer* pEntH0Buf = pIndexTree->pPositionH0Buffer(xEntWrk.aKey());
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  - - - - - - -
    if (! pEntH0Buf) {
      vTrace("Key not found!\n"_Stz);
      
      // add the key with a load
      vTrace("Inserting a payload entry for key.\n"_Stz);
      aPayloadEntry = pIndexTree->aInsertPayloadEntry(xEntWrk, 8);
    }
    else {
      
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  - - - - - -
      // position at prior entry
      Ent* aEnt_prior = pEntH0Buf->aPositionPriorEntry(xEntWrk.aKey());
      
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  - - - - - -
      // prior entry match the key ?
      if (KC_SAME == Key::eCompare(aEnt_prior->aKey(),xEntWrk.aKey())) {
        aPayloadEntry = aEnt_prior;
      }
      else {
        // move to next entry greater than equal to
        aPayloadEntry = EntBuffer::pMoveToGreaterEqualEntry(aEnt_prior, xEntWrk.aKey(), &pEntH0Buf);
        if ( aPayloadEntry && KC_SAME == Key::eCompare(aPayloadEntry->aKey(),xEntWrk.aKey())) {
          vTrace("Key Found: %s\n"_Stz,spKey.sz(aPayloadEntry->aKey(),pIndexDef));
        }
        else {
          vTrace("Key not found!\n"_Stz);
          
          // add the key with a load
          vTrace("Inserting a payload entry for key.\n"_Stz);
          aPayloadEntry = pIndexTree->aInsertPayloadEntry(xEntWrk, 8);
        }
      }
      
      ASSERT(aPayloadEntry);
      ASSERT(aPayloadEntry->bPayload());
      
      Pay* aPay = aPayloadEntry->aPay();
      ASSERT(aPay->nPayloadBytes() == 8);
      
    }
    //i8 iAgg = pExpr->iAggregate(aPayloadEntry->ayPayload(),aSrcRsb,pSrcRowSlab);
    //vTrace("Aggregate value so far: %lld\n"_Stz,iAgg);
  }
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* copy source rsb into destination partition row slab                        */
/*----------------------------------------------------------------------------*/
void AggregateRsb::vCopyIntoDst(Rsb* aSrcRsb)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_AGGREGATERSB,_FOOTER_);
    
  // copy src into rst
  bool bSrcHasPtnHash = true;
  bool bCopyPtnHash   = false;
  m_pDstRowSlab->vCopyRsb(aSrcRsb,bSrcHasPtnHash,bCopyPtnHash);
}

/*----------------------------------------------------------------------------*/
/* new a destination rsb row                                                  */
/*----------------------------------------------------------------------------*/
void AggregateRsb::vNewDstRsb(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_AGGREGATERSB,_FOOTER_);
    
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

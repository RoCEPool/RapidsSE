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
AggregateRsb::AggregateRsb(Pot* pPot, StreamDef* pStreamDefOut)
: Object(CN_AGGREGATERSB)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_AGGREGATERSB,_FOOTER_);
 
  m_pPot          = pPot;
  m_pSrcRsbBuffer = nullptr;
  m_pDstRowSlab   = nullptr;
  m_pStreamDefOut = pStreamDefOut;
  
  m_bPixInited = true;
  m_pStreamDefOut->vToStreamPix(&m_oStreamPix,true);
  
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
void AggregateRsb::vAggregate(RsbBuffer* pSrcRsbBuffer, Expr* pExpr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_AGGREGATERSB,_FOOTER_);
  
  m_pSrcRsbBuffer = pSrcRsbBuffer;
  
  RowSlab* pSrcRowSlab = pSrcRsbBuffer->pRowSlab();
  
  i8 nRows = pSrcRowSlab->nRows();
  for (i8 i_Row = 0; i_Row < nRows; i_Row++) {
    
    Rsb* aSrcRsb = pSrcRowSlab->aRsbAdr(i_Row);
    
    i8 bCopy = pExpr->iAggregate(m_pDstRowSlab,aSrcRsb,pSrcRowSlab);
    
    //vCopyIntoDst(aSrcRsb);
  }
}

/*----------------------------------------------------------------------------*/
/* convert                                                                    */
/*----------------------------------------------------------------------------*/
void AggregateRsb::vAggregateCpy(IndexTree* pIndexTree, Expr* pExpr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_AGGREGATERSB,_FOOTER_);
  
  EntWrk xEntWrk;
  SP_Ent spEnt;
  SP_Key spKey;
  Ent* aEnt;

  i8 i_Cell_ntv;
  Rsb* aRsb;
  EntBuffer* pEntBuffer;
  i8 iAgg;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -- - - - - - -
  // for each entry buffer
  for (EntBuffer* pEntBuffer = pIndexTree->pFirstH0Buffer();
        pEntBuffer; pEntBuffer = pEntBuffer->pNextBuffer()) {

   // get the first entry
   Ent* aEnt = pEntBuffer->aLeadEntry();
   
   while (aEnt) {

     // new a destination row in destination partition row slab
     vNewDstRsb();
     
     aRsb = (Rsb*)aEnt->ayPayload();
     i_Cell_ntv = aRsb->i_ColToCell_ntv(0);
     iAgg = aRsb->iVal_ntv(i_Cell_ntv);
     m_pDstRowSlab->vPositionOnCell(0);
     m_pDstRowSlab->vDeposit(iAgg);
     
     // move to the next entry
     aEnt = pEntBuffer->aNextEntry(aEnt, 0);
   }
  }
}
  
/*----------------------------------------------------------------------------*/
/* convert                                                                    */
/*----------------------------------------------------------------------------*/
void AggregateRsb::vAggregateGby(RsbBuffer* pSrcRsbBuffer, IndexTree* pIndexTree, Expr* pExpr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_AGGREGATERSB,_FOOTER_);
  /*
  m_pSrcRsbBuffer = pSrcRsbBuffer;
  
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
  
  i8 i_Cell_ntv;
  Rsb* aRsb;
  EntBuffer* pEntH0Buf;
  
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
    Ent* aEnt_prior = pIndexTree->aPositionPriorEntry(xEntWrk.aKey(), &pEntH0Buf);
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  - - - - - - -
    if (! aEnt_prior) {
      vTrace("Key not found!\n"_Stz);
      
      // create a payload for the entry
      xEntWrk.vPayload(true);
      xEntWrk.vPayloadBytes(8);
      
      // insert 0 in the payload at cell 0
      aRsb = (Rsb*)xEntWrk.ayPayload();
      i_Cell_ntv = aRsb->i_ColToCell_ntv(0);
      aRsb->vVal_ntv(i_Cell_ntv, (i8)0);
      
      // insert the entry in the indexTree
      vTrace("Inserting a payload entry for key.\n"_Stz);
      aPayloadEntry = pIndexTree->aInsertEntry(xEntWrk.aEnt());
    }
    else {
      
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  - - - - - -
      // prior entry match the key ?
      if (KC_SAME == Key::eCompare(aEnt_prior->aKey(),xEntWrk.aKey())) {
        aPayloadEntry = aEnt_prior;
      }
      else {
        // move to next entry 
        aPayloadEntry = EntBuffer::pMoveToNextEntry(aEnt_prior, &pEntH0Buf);
        if ( aPayloadEntry && KC_SAME == Key::eCompare(aPayloadEntry->aKey(),xEntWrk.aKey())) {
          vTrace("Key Found: %s\n"_Stz,spKey.sz(aPayloadEntry->aKey(),pIndexDef));
        }
        else {
          vTrace("Key not found!\n"_Stz);
          
          // create a payload for the entry
          xEntWrk.vPayload(true);
          xEntWrk.vPayloadBytes(8);
          
          // insert 0 in the payload at cell 0
          aRsb = (Rsb*)xEntWrk.ayPayload();
          i_Cell_ntv = aRsb->i_ColToCell_ntv(0);
          aRsb->vVal_ntv(i_Cell_ntv, (i8)0);
          
          // insert the entry in the indexTree
          vTrace("Inserting a payload entry for key.\n"_Stz);
          aPayloadEntry = pIndexTree->aInsertEntry(xEntWrk.aEnt());
        }
      }
      
      ASSERT(aPayloadEntry);
      ASSERT(aPayloadEntry->bPayload());
      
      Pay* aPay = aPayloadEntry->aPay();
      ASSERT(aPay->nPayloadBytes() == 8);
      
    }
    
    //i8 iAgg = pExpr->iAggregateGby(pIndexTree,aPayloadEntry,nullptr,aSrcRsb,pSrcRowSlab);
    //vTrace("Aggregate value so far: %lld\n"_Stz,iAgg);
    pIndexTree->vDump(false);
  }
  */
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the indexDef
  IndexDef* pIndexDef = pIndexTree->pIndexDef();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the src rowSlab from the SrcRsbBuffer
  m_pSrcRsbBuffer = pSrcRsbBuffer;
  RowSlab* pSrcRowSlab = pSrcRsbBuffer->pRowSlab();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get no of rows in the srcRsbSlab
  i8 nRows = pSrcRowSlab->nRows();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -- - - - - - -
  // for each row in the rowSlab
  for (i8 i_Row = 0; i_Row < nRows; i_Row++) {
    
    Rsb* aSrcRsb = pSrcRowSlab->aRsbAdr(i_Row);
    
    bool bResult = pExpr->iAggregateGby(pIndexTree,nullptr,aSrcRsb,pSrcRowSlab);
    pIndexTree->vDump(false);
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

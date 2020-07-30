/*============================================================================*/
/* Steps_IndexTree.cpp - indexTree[] step group                                */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_IndexTree(void)
{
  vAddStep("IndexTree[]"_Stz,           &Step::vIndexTreeGet);
  vAddStep("IndexTree[].aggregate"_Stz, &Step::vIndexTreeAggregate);
  vAddStep("IndexTree[].asAlt"_Stz,     &Step::vIndexTreeAsAlt);
  vAddStep("IndexTree[].asSrc"_Stz,     &Step::vIndexTreeAsSrc);
  vAddStep("IndexTree[].asTmp"_Stz,     &Step::vIndexTreeAsTmp);
  vAddStep("IndexTree[].delete"_Stz,    &Step::vIndexTreeDelete);
  vAddStep("IndexTree[].new"_Stz,       &Step::vIndexTreeNew);
  vAddStep("IndexTree[].set"_Stz,       &Step::vIndexTreeSet);
  
  vAddStep("IndexTree.insEntBuffer"_Stz,    &Step::vIndexTreeInsEntBuffer);
}

/*----------------------------------------------------------------------------*/
/* IndexTree[].aggregate -- aggregate                                         */
/*----------------------------------------------------------------------------*/
void Step::vIndexTreeAggregate(void)
{
  ENTRY();
  
  EntWrk xEntWrk;
  SP_Ent spEnt;
  SP_Key spKey;
  Ent* aPayloadEntry = nullptr;
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the indexTree
  IndexTree* pIndexTree = * m_pLogicUnit->m_xpIndexTree;
  IndexDef* pIndexDef = pIndexTree->pIndexDef();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the src rsb buffer
  RsbBuffer* pSrcRsbBuffer = _pRsbBuffer(* m_pLogicUnit->m_xpOperandA);
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
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* IndexTree[].asAlt -- set this index tree as m_pAltIdxTree                  */
/*----------------------------------------------------------------------------*/
void Step::vIndexTreeAsAlt(void)
{
  ENTRY();
  
  m_pLogicUnit->m_pDevAltIxt->m_pIdxTree = * m_pLogicUnit->m_xpIndexTree;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* IndexTree[].asSrc -- set this index tree as m_pSrcIdxTree                  */
/*----------------------------------------------------------------------------*/
void Step::vIndexTreeAsSrc(void)
{
  ENTRY();
  
  m_pLogicUnit->m_pDevSrcIxt->m_pIdxTree = * m_pLogicUnit->m_xpIndexTree;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* IndexTree[].asTmp -- set this index tree as m_pIndexTree                   */
/*----------------------------------------------------------------------------*/
void Step::vIndexTreeAsTmp(void)
{
  ENTRY();
  
  m_pLogicUnit->m_pIndexTree = * m_pLogicUnit->m_xpIndexTree;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* IndexTree[].delete -- delete index tree                                    */
/*----------------------------------------------------------------------------*/
void Step::vIndexTreeDelete(void)
{
  ENTRY();
  
  IndexTree* pIndexTree = * m_pLogicUnit->m_xpIndexTree;
  
  m_pLogicUnit->m_xpIndexTree = nullptr;
  
  delete pIndexTree;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* IndexTree[].get -- get index tree                                           */
/*----------------------------------------------------------------------------*/
void Step::vIndexTreeGet(void)
{
  ENTRY();
  
  m_pDataStack->vPushPtr(*m_pLogicUnit->m_xpIndexTree);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* IndexTree[].new -- new a index tree                                        */
/*----------------------------------------------------------------------------*/
void Step::vIndexTreeNew(void)
{
  ENTRY();
  
  IndexDef* pIndexDef = _pIndexDef(m_pDataStack->pPopPtr(__func__));
  m_pLogicUnit->m_xpIndexTree = new IndexTree(nullptr, pIndexDef, C_UNKNOWN);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* IndexTree[].set -- set index tree                                          */
/*----------------------------------------------------------------------------*/
void Step::vIndexTreeSet(void)
{
  ENTRY();
  
  m_pLogicUnit->m_xpIndexTree = _pIndexTree(m_pDataStack->pPopPtr(__func__));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* IndexTree.insEntBuffer -- insert all entries from given ent buffer          */
/*----------------------------------------------------------------------------*/
void Step::vIndexTreeInsEntBuffer(void)
{
  ENTRY();
  
  IndexTree* pIndexTree = _pIndexTree(m_pDataStack->pPopPtr(__func__));
  
  EntBuffer* pEntBuffer = _pEntBuffer(m_pDataStack->pPopPtr(__func__));
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  i8 nEntries = 0;
  
  for (Ent* aEnt = pEntBuffer->aFirstItem();
       aEnt;
       aEnt = pEntBuffer->aNextEntry(aEnt,0)) {
    
    // ..add code here to insert entry into index tree..
    
    bool bInserted = pIndexTree->bInsertEntry(aEnt);
    
    nEntries++;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  m_pDataStack->vPushInt(nEntries);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

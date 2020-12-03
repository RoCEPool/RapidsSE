/*============================================================================*/
/* IndexStream.cpp -- Index Stream                                            */
/*============================================================================*/

#include "Partitioning.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(Partitioning,IndexStream,CN_INDEXSTREAM)
__aNewFunction(IndexStream,CN_INDEXSTREAM)
__vDeleteFunction(IndexStream,CN_INDEXSTREAM)
__aFooterFunction(IndexStream)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
IndexStream::IndexStream(TablePtn* pTablePtn, Index* pIndex)
: Object(CN_INDEXSTREAM)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXSTREAM,_FOOTER_);
  
  m_pTablePtn    = pTablePtn;
  m_pIndex       = pIndex;

  m_pEntBuffer   = nullptr;
  m_aLowerEntry  = nullptr;
  m_aUpperEntry  = nullptr;
  
  _checkInitialised_(CN_INDEXSTREAM);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_INDEXSTREAM,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
IndexStream::~IndexStream(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXSTREAM,_FOOTER_);
  
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* close                                                                      */
/*----------------------------------------------------------------------------*/
void IndexStream::vClose(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXSTREAM,_FOOTER_);
 
  m_pEntBuffer = nullptr;
}

/*----------------------------------------------------------------------------*/
/* delete next buffer                                                         */
/*----------------------------------------------------------------------------*/
bool IndexStream::bDelete(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXSTREAM,_FOOTER_);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // assert we have a lower entry ?
  ASSERT( m_aLowerEntry );
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Table* pTable = m_pIndex->pTable();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get IndexTree
  i8 iIndexNo = m_pIndex->iIndexNo();
  IndexTree* pIndexTree = m_pTablePtn->pIndexTree(iIndexNo);
  
  HV_Row hRow(pTable->pStreamDef(),C_ALL);
  HV_Rec hvRec(pTable->pStreamDef(),C_ALL);
  
  bool bBefore = true;
  
  vTrace("------------------------------------------\n"_Stz);
  vTrace("Deleting rows:\n"_Stz);
  
  Ent* aEntryToDelete;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // iterate thru the buffers starting from aLowerEntry
  while (m_aLowerEntry) {
    
    // is lowerEntry before upperEntry ?
    if (m_aUpperEntry)
      bBefore = (m_aLowerEntry == m_aUpperEntry) ? false : true;
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // lowerEntry is before upperEntry
    if (bBefore) {
      
      // get the crw entry from the lowerEntry
      Rrf xRef = m_aLowerEntry->rxRef();
      
      Crw* aCrw = CrwBuffer::pRefdCrw(xRef);
      
      RowInflator::vInflate_AllColumns_into_RowSingle(hRow,aCrw);
      hvRec->vAppend(hRow);
      hvRec->vPrintRow();
      
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // delete the refernced crw
      bool bDeleted = CrwBuffer::bDeleteRefdCrw(xRef);
      ASSERT(bDeleted);
      
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // save the entry
      aEntryToDelete = m_aLowerEntry;
      
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // move to next entry in the buffer
      m_aLowerEntry = m_pEntBuffer->aNextEntry(m_aLowerEntry, 0);
      
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // now purge the entry
      pIndexTree->vDelete(m_pEntBuffer, aEntryToDelete);
      
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // no more entries ?
      if ( ! m_aLowerEntry ) {
        
        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // move to next buffer
        m_pEntBuffer = m_pEntBuffer->pNextBuffer();
        
        // next buffer ?
        if ( m_pEntBuffer )
          m_aLowerEntry = m_pEntBuffer->aLeadEntry();
      }
    }
    else {
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // we are done
      m_aLowerEntry = nullptr;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // check if session exiting
    //if (pSession->m_bSessionExiting)
    //  aItem = nullptr;
  }
  
  return m_aLowerEntry ? true : false;
}

/*----------------------------------------------------------------------------*/
/* open                                                                       */
/*----------------------------------------------------------------------------*/
bool IndexStream::bOpen(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXSTREAM,_FOOTER_);
  
  vReset();
    
  return true;
}

/*----------------------------------------------------------------------------*/
/* read next buffer                                                           */
/*----------------------------------------------------------------------------*/
CrwBuffer* IndexStream::pRead_idxstm(RowOnce* pRowOnce)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXSTREAM,_FOOTER_);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  CrwBuffer* pCrwBuffer = nullptr;
  RowCompactor* pRowCompactor = nullptr;
  Table* pTable = m_pIndex->pTable();
  HV_Row hRow(pTable->pStreamDef(),C_ALL);
  
  bool bBefore = true;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // did we get a lower entry ?
  if ( ! m_aLowerEntry) {
    goto L_Exit;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create a result buffer for the results
  pCrwBuffer = new CrwBuffer;
  pCrwBuffer->vTable(m_pTablePtn->pTable());
  pCrwBuffer->vPtnNo(m_pTablePtn->tPtnNo()) ;
  
  // create a row compactor for the crw buffer
  pRowCompactor = new RowCompactor;
  pCrwBuffer->vAppendOpen(pRowCompactor, false);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // iterate thru the buffers starting from aLowerEntry
  while (m_aLowerEntry) {
    
    // is lowerEntry before upperEntry ?
    if (m_aUpperEntry)
      bBefore = (m_aLowerEntry == m_aUpperEntry) ? false : true;
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // lowerEntry is before upperEntry
    if (bBefore) {
      
      // get the crw entry from the lowerEntry
      Rrf xRef = m_aLowerEntry->rxRef();
      
      // has this row already been seen ?
      if ( ! pRowOnce || ! pRowOnce->bRowSeen(xRef) ) {
        
        Crw* aCrw = CrwBuffer::pRefdCrw(xRef);
        
        RowInflator::vInflate_AllColumns_into_RowSingle(hRow,aCrw);
        
        // insert row from entry in crwBuffer
        if ( ! pCrwBuffer->bAppendSrw(hRow)) {
          
          // Buffer is full ?
          goto L_Exit;
        }
      }
      
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // move to next entry in the buffer
      m_aLowerEntry = m_pEntBuffer->aNextEntry(m_aLowerEntry, 0);
      
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // no more entries ?
      if ( ! m_aLowerEntry ) {
        
        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // move to next buffer
        m_pEntBuffer = m_pEntBuffer->pNextBuffer();
        
        // next buffer ?
        if ( m_pEntBuffer )
          m_aLowerEntry = m_pEntBuffer->aLeadEntry();
      }
    }
    else {
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // we are done
      m_aLowerEntry = nullptr;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // check if session exiting
    //if (pSession->m_bSessionExiting)
    //  aItem = nullptr;
  }
  
L_Exit:
  
  // cleanup and return the buffer
  if (pCrwBuffer) {
    pCrwBuffer->vAppendClose();
    delete pRowCompactor;
  }
  return pCrwBuffer;
}

/*----------------------------------------------------------------------------*/
/* reset                                                                      */
/*----------------------------------------------------------------------------*/
void IndexStream::vReset(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXSTREAM,_FOOTER_);
 
  m_pEntBuffer = nullptr;
}

/*----------------------------------------------------------------------------*/
/* position                                                                   */
/*----------------------------------------------------------------------------*/
void IndexStream::vSeek(eRO eLowerRelnOp, Stz* szLowerKey, eRO eUpperRelnOp, Stz* szUpperKey)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXSTREAM,_FOOTER_);
  
  EntBuffer* aUpperEntBuffer;
  Ent* aUpperEntry = nullptr;
  
  EntWrk xEntWrkLower;
  EntWrk xEntWrkUpper;
  
  EntBuffer* aLowerEntBuffer = nullptr;
  Ent* aLowerEntry = nullptr;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get IndexTree
  i8 iIndexNo = m_pIndex->iIndexNo();
  IndexTree* pIndexTree = m_pTablePtn->pIndexTree(iIndexNo);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get no of columns in the index
  IndexDef* pIndexDef = m_pIndex->pIndexDef();
  i8 nIdxCols = pIndexDef->nColumns();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get column0 of the index
  Column* pColumn0 = pIndexDef->pGetColumn(0);
  
  bool bExactQuery = false;
  bool bHaveLowerKey = false;
  bool bHaveUpperKey = false;
  i8 iLowerInt;
  i8 iUpperInt;
  
  Format* pFmt = AM::pFmt();
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Lower Key
  
  if ( ! StrLib::bSame(szLowerKey->azChar(), "") ) {
    bHaveLowerKey = true;
    
    // form the lower key
    KeyBuilder::vAddElement(pIndexDef->eKeyKind(),
                            &xEntWrkLower,pColumn0,szLowerKey,pFmt);
    
    for (i8 iKey = 1; iKey < nIdxCols; iKey++) {
      xEntWrkLower.vAppendMinusInfinity();
    }
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Upper Key
  
  if ( ! StrLib::bSame(szUpperKey->azChar(), "") ) {
    bHaveUpperKey = true;
    
    // form the upper key
    KeyBuilder::vAddElement(pIndexDef->eKeyKind(),
                            &xEntWrkUpper,pColumn0,szUpperKey,pFmt);
    
    for (i8 iKey = 1; iKey < nIdxCols; iKey++) {
      xEntWrkUpper.vAppendPlusInfinity();
    }
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // if we have both upper and lower keys
  if ( bHaveLowerKey && bHaveUpperKey) {
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // check if upperKey > lowerKey
    if (pIndexDef->eKeyKind() == KE_INT) {
      iLowerInt = xEntWrkLower.aEnt()->iIntElm0();
      iUpperInt = xEntWrkUpper.aEnt()->iIntElm0();
      
      if (iUpperInt < iLowerInt)
        goto L_Exit;
    }
    else {
      eKC eCompare = Key::eCompare(xEntWrkUpper.aKey(),
                                   xEntWrkLower.aKey());
      
      if (eCompare == KC_BEFORE)
        goto L_Exit;
    }
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Position for Lower Key
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (bHaveLowerKey ) {
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // get the lower relational OP
    //eLowerRelnOp = pPlanner->eGetFirstLowerPredicateRO(iPhrase);
    ASSERT(RO_NONE != eLowerRelnOp);
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // exact key ?
//    if (eLowerRelnOp == RO_EQUAL_TO) {
//      bExactQuery = true;
//      eLowerRelnOp = RO_GREATER_EQUAL;
//    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // first position at a level 0 buffer with lowerKey
    aLowerEntBuffer = pIndexTree->pPositionH0Buffer(xEntWrkLower.aKey());
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // did we get an entry buffer ?
    if (aLowerEntBuffer) {
      
      // position in the index buffer at an entry prior to the lowerKey
      aLowerEntry = aLowerEntBuffer->aFindEntryBefore(xEntWrkLower.aKey());
      
      if (eLowerRelnOp == RO_GREATER_THAN) {
        aLowerEntry = EntBuffer::pMoveToGreaterThanEntry(aLowerEntry,
                                                         xEntWrkLower.aKey(),
                                                         &aLowerEntBuffer);
      }
      else {
        RASSERT (eLowerRelnOp == RO_GREATER_EQUAL)
        aLowerEntry = EntBuffer::pMoveToGreaterEqualEntry(aLowerEntry, xEntWrkLower.aKey(), &aLowerEntBuffer);
      }
    }
  }
  else {
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // no lower key
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // position at the first level 0 buffer
    aLowerEntBuffer = pIndexTree->pFirstH0Buffer();
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // did we get an entryBuffer ?
    if ( ! aLowerEntBuffer) {
      goto L_Exit;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // get the first entry in the buffer
    aLowerEntry = aLowerEntBuffer->aLeadEntry();
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Position for Upper Key
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // there is no upper key ?
  if ( ! bHaveUpperKey )
    goto L_Exit;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get upper relational OP
//  if (bExactQuery) {
//    eUpperRelnOp = RO_LESS_EQUAL;
//  }
//  else {
//    ASSERT(RO_NONE != eUpperRelnOp);
//    //eUpperRelnOp = pPlanner->eGetFirstUpperPredicateRO(iPhrase);
//  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // now position at a level 0 buffer for the upper key
  aUpperEntBuffer = pIndexTree->pPositionH0Buffer(xEntWrkUpper.aKey());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // do we have an upper Entry buffer ?
  if (aUpperEntBuffer) {
    
    aUpperEntry = aUpperEntBuffer->aFindEntryBefore(xEntWrkUpper.aKey());
    aUpperEntry = EntBuffer::pMoveToNextEntry(aUpperEntry, &aUpperEntBuffer);
    
    if (aUpperEntry && eUpperRelnOp == RO_LESS_EQUAL) {
      aUpperEntry = EntBuffer::pMoveToGreaterThanEntry(aUpperEntry,
                                                       xEntWrkUpper.aKey(),
                                                       &aUpperEntBuffer);
    }
  }
  
L_Exit:
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set the lower and upper entries
  m_aLowerEntry = aLowerEntry;
  m_aUpperEntry = aUpperEntry;
  m_pEntBuffer  = aLowerEntBuffer;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

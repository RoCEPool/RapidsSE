/*============================================================================*/
/* JoinOp.cpp -- Join                                                         */
/*============================================================================*/

#include "PushDown.h"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add row refs                                                                 */
/*----------------------------------------------------------------------------*/
RefBuffer* JoinOp::pAddRowRefs(RefBuffer* pRefBuffer, Ent* aLeftEntry, Ent* aRightEntry)
{
  
  // allot a refSet
  RefSet* pRefSet = pRefBuffer->aAllotRefSet(2);
  
  if ( ! pRefSet )  {
    // Reference Buffer is full
    
    RASSERT(0);
    
    // put this refBuffer in session bufferQueue
    //pQueue->vAppend(pRefBuffer);
    
    // create another refBuffer for the refEnt's
    pRefBuffer = new RefBuffer;
    
    // insert row from entry in temporary buffer
    pRefSet = pRefBuffer->aAllotRefSet(2);
    RASSERT(pRefSet);
  }
  
  // get the row buffers
  RowBuffer* pLeftRowBuffer = aLeftEntry ? RowBuffer::pRefdRowBuffer(aLeftEntry->xId()) : nullptr;
  RowBuffer* pRightRowBuffer = aRightEntry ? RowBuffer::pRefdRowBuffer(aRightEntry->xId()) : nullptr;
  
  // set the rowRefs
  HeadLine* aLeftRowData = aLeftEntry ? pLeftRowBuffer->pRow(aLeftEntry->xId().iItem()) : nullptr;
  HeadLine* aRightRowData = aRightEntry ? pRightRowBuffer->pRow(aRightEntry->xId().iItem()) : nullptr;
  
  // set the rowData in the refSet
  pRefSet->vRowRef(0, (i8)aLeftRowData);
  pRefSet->vRowRef(1, (i8)aRightRowData);
  
  return pRefBuffer;
}

/*----------------------------------------------------------------------------*/
/* index join                                                                 */
/*----------------------------------------------------------------------------*/
void JoinOp::vIndexJoin(IndexTree* pLeftIndexTree, IndexTree* pRightIndexTree)
{
  vTrace("Joining: L(%s) ⋈ R(%s)\n"_Stz,
         pLeftIndexTree->pIndex()->pTable()->aTableName(),
         pRightIndexTree->pIndex()->pTable()->aTableName());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get initial buffers
  
  EntBuffer* pLeftBuffer  = pLeftIndexTree->pRoot();
  EntBuffer* pRightBuffer = pRightIndexTree->pRoot();

  // check for null buffers
  if ( ! pLeftBuffer ) {
    RASSERT( ! pRightBuffer );
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create a reference buffer
  
  RefBuffer* pRefBuffer = new RefBuffer;
  
  RefSet* pRefSet;
  
  // dump the first buffers
  pLeftBuffer->vDump();
  pRightBuffer->vDump();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // join loop

  Ent* aLeftEntry  = pLeftBuffer->aLeadEntry();
  Ent* aRightEntry = pRightBuffer->aLeadEntry();
  
  eNE eNextEnt = NE_NEXT_NONE;
  
  Ent* aFirstRightMatch = nullptr;
  Ent* aFirstLeftMatch  = nullptr;
  
  EntBuffer* pFirstRightBuffer = nullptr;
  
  while (NE_NEXT_EXIT != eNextEnt) {
    
    // get next left entry, right, both, or none
    switch (eNextEnt) {
        
      case NE_NEXT_LEFT: // get next left entry
        aLeftEntry  = EntBuffer::aGetNextEntry(&pLeftBuffer,aLeftEntry);
        
        // check if it matches a first left
        if (aLeftEntry && aFirstLeftMatch) {
          eKC eCompare = EntWrk::eCompare(aLeftEntry,aFirstLeftMatch);
          
          // if matches a first left start with teh first right
          if (eCompare == KC_SAME) {
            aRightEntry = aFirstRightMatch;
            pRightBuffer = pFirstRightBuffer;
          }
          else {
            aFirstLeftMatch = nullptr;
            aFirstRightMatch = nullptr;
          }
        }
        break;
        
      case NE_NEXT_RIGHT: // get next right entry
        aRightEntry = EntBuffer::aGetNextEntry(&pRightBuffer,aRightEntry);
        break;

      case NE_NEXT_LEFT_RIGHT: // get next left and right entry
        aLeftEntry  = EntBuffer::aGetNextEntry(&pLeftBuffer,aLeftEntry);
        aRightEntry = EntBuffer::aGetNextEntry(&pRightBuffer,aRightEntry);
        break;
          
      case NE_NEXT_NONE: // get neither, fall thru
        break;
        
      default:
        NEVER_REACH;
    }
          
    // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
    
    // join action based on whether there is a left and/or right entry
    
    // (a) both left and right entry
    if (aLeftEntry && aRightEntry) {
      
      // compare the two keys
      eKC eCompare = EntWrk::eCompare(aLeftEntry,aRightEntry);

      // how do they compare?
      switch (eCompare) {
        case KC_SAME:   // leftKey == rightKey
          vTrace("Joining: L(%p) ⋈ R(%p)\n"_Stz,aLeftEntry,aRightEntry);
          pRefBuffer = pAddRowRefs(pRefBuffer, aLeftEntry, aRightEntry);
          eNextEnt = NE_NEXT_RIGHT;
          
          // save first right
          if ( ! aFirstRightMatch ) {
            aFirstRightMatch = aRightEntry;
            pFirstRightBuffer = pRightBuffer;
          }
          
          // save first left
          if ( ! aFirstLeftMatch )
            aFirstLeftMatch = aLeftEntry;
          
          break;
          
        case KC_BEFORE: // leftKey < rightKey
          if ( ! aFirstRightMatch ) {
            vTrace("Joining: L(%p) ⋈ R(NULL)\n"_Stz,aLeftEntry);
            pRefBuffer = pAddRowRefs(pRefBuffer, aLeftEntry, nullptr);
          }
          eNextEnt = NE_NEXT_LEFT;
          break;
          
        case KC_AFTER:  // leftKey > rightKey
          vTrace("Joining: L(NULL) ⋈ R(%p)\n"_Stz,aRightEntry);
          pRefBuffer = pAddRowRefs(pRefBuffer, aLeftEntry, aRightEntry);
          eNextEnt = NE_NEXT_RIGHT;
          break;
          
        default:
          NEVER_REACH;
      }
    
      continue;
    }

    // (b) left entry only
    if (aLeftEntry) {
      
      // no previous first right ?
      if ( ! aFirstRightMatch ) {
        vTrace("Joining: L(%p) ⋈ R(NULL)\n"_Stz,aLeftEntry);
        pRefBuffer = pAddRowRefs(pRefBuffer, aLeftEntry, nullptr);
      }
      
      // get next left
      eNextEnt = NE_NEXT_LEFT;
      continue;
    }

    // (c) right entry only
    if (aRightEntry) {
      
      vTrace("Joining: L(NULL) ⋈ R(%p)\n"_Stz,aRightEntry);
      pRefBuffer = pAddRowRefs(pRefBuffer, nullptr, aRightEntry);
  
      // get next right
      eNextEnt = NE_NEXT_RIGHT;
      continue;
    }

    // (d) neither
    eNextEnt = NE_NEXT_EXIT;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // join completed
  
  vTrace("Joined\n"_Stz);
  
  pRefBuffer->vShow();
  
  vTrace("Rows (by reference):\n"_Stz);
  
  ColumnVec* pLeftColVec  = pLeftIndexTree->pIndex()->pTable()->pColumnVec();
  ColumnVec* pRightColVec = pRightIndexTree->pIndex()->pTable()->pColumnVec();
  
  i8 nJoinRows = pRefBuffer->nItems();
  pRefSet = pRefBuffer->aFirstRefSet();
  
  HV_Rec hvRecLeft(pLeftColVec);
  HV_Rec hvRecRight(pRightColVec);
  
  // create a left and a right row
  HV_Row hLeftRow(pLeftColVec);
  HV_Row hRightRow(pRightColVec);
  
  while (nJoinRows-- > 0) {
    
    // get the left and right rows from the refSet
    HeadLine* aLeftRow = (HeadLine*)pRefSet->iRowRef(0);
    HeadLine* aRightRow = (HeadLine*)pRefSet->iRowRef(1);
    
    // inflate left row
    if (aLeftRow) {
      RowCoder::vInflate(hLeftRow,aLeftRow);
      hvRecLeft->vAppend(hLeftRow);
    }
  
    // inflate right row
    if (aRightRow) {
      RowCoder::vInflate(hRightRow,aRightRow);
      hvRecRight->vAppend(hRightRow);
    }
  
    vTrace("row: (%s) ⋈ (%s)\n"_Stz,
           aLeftRow  ? hvRecLeft->szRecStr()  : "NULL"_Stz,
           aRightRow ? hvRecRight->szRecStr() : "NULL"_Stz);
    
    // get next row ref
    pRefSet = pRefBuffer->aNextRefSet(pRefSet);
  }
  
  vTrace("\n"_Stz);
  vTrace("=================================================="
         "==================================================\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
JoinOp* PushDown::_pJoinOp(void* aAddr)
{
  JoinOp* pJoinOp = reinterpret_cast<JoinOp*>(aAddr);
  
  return pJoinOp;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

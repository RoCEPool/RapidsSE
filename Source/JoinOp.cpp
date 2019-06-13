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
RefBuffer* JoinOp::pAddRowRefs(Queue* pQueue, RefBuffer* pRefBuffer, Ent* aLeftEntry, Ent* aRightEntry)
{
  
  // allot a refSet
  RefSet* pRefSet = pRefBuffer->aAllotRefSet(2);
  
  if ( ! pRefSet )  {
    // Reference Buffer is full
    
    RASSERT(0);
    
    // put this refBuffer in session bufferQueue
    pQueue->vAppend(pRefBuffer);
    
    // create another refBuffer for the refEnt's
    pRefBuffer = new RefBuffer;
    
    // insert row from entry in temporary buffer
    pRefSet = pRefBuffer->aAllotRefSet(2);
    RASSERT(pRefSet);
  }
  
  // get the row buffers
  RowBuffer* pLeftRowBuffer = aLeftEntry ? RowBuffer::pRefdRowBuffer(aLeftEntry->xRef()) : nullptr;
  RowBuffer* pRightRowBuffer = aRightEntry ? RowBuffer::pRefdRowBuffer(aRightEntry->xRef()) : nullptr;
  
  // set the rowRefs
  HeadLine* aLeftRowData = aLeftEntry ? pLeftRowBuffer->pRow(aLeftEntry->xRef().iItem()) : nullptr;
  HeadLine* aRightRowData = aRightEntry ? pRightRowBuffer->pRow(aRightEntry->xRef().iItem()) : nullptr;
  
  // set the rowData in the refSet
  pRefSet->vRowRef(0, (i8)aLeftRowData);
  pRefSet->vRowRef(1, (i8)aRightRowData);
  
  return pRefBuffer;
}

/*----------------------------------------------------------------------------*/
/* index join                                                                 */
/*----------------------------------------------------------------------------*/
void JoinOp::vIndexJoin(Queue* pQueue, IndexTree* pLeftIndexTree, IndexTree* pRightIndexTree)
{
//  vTrace("Joining: L(%s) ⋈ R(%s)\n"_Stz,
//         pLeftIndexTree->pIndex()->pTable()->aTableName(),
//         pRightIndexTree->pIndex()->pTable()->aTableName());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get initial buffers
  
  EntBuffer* pLeftBuffer  = pLeftIndexTree->pRoot();
  EntBuffer* pRightBuffer = pRightIndexTree->pRoot();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create a reference buffer
  
  RefBuffer* pRefBuffer = new RefBuffer;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // join loop

  Ent* aLeftEntry  = pLeftBuffer ? pLeftBuffer->aLeadEntry() : nullptr;
  Ent* aRightEntry = pRightBuffer ? pRightBuffer->aLeadEntry() : nullptr;

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
          //vTrace("Joining: L(%p) ⋈ R(%p)\n"_Stz,aLeftEntry,aRightEntry);
          pRefBuffer = pAddRowRefs(pQueue, pRefBuffer, aLeftEntry, aRightEntry);
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
            //vTrace("Joining: L(%p) ⋈ R(NULL)\n"_Stz,aLeftEntry);
            pRefBuffer = pAddRowRefs(pQueue, pRefBuffer, aLeftEntry, nullptr);
          }
          eNextEnt = NE_NEXT_LEFT;
          break;
          
        case KC_AFTER:  // leftKey > rightKey
          //vTrace("Joining: L(NULL) ⋈ R(%p)\n"_Stz,aRightEntry);
          pRefBuffer = pAddRowRefs(pQueue, pRefBuffer, aLeftEntry, aRightEntry);
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
        //vTrace("Joining: L(%p) ⋈ R(NULL)\n"_Stz,aLeftEntry);
        pRefBuffer = pAddRowRefs(pQueue, pRefBuffer, aLeftEntry, nullptr);
      }
      
      // get next left
      eNextEnt = NE_NEXT_LEFT;
      continue;
    }

    // (c) right entry only
    if (aRightEntry) {
      
      //vTrace("Joining: L(NULL) ⋈ R(%p)\n"_Stz,aRightEntry);
      pRefBuffer = pAddRowRefs(pQueue, pRefBuffer, nullptr, aRightEntry);
  
      // get next right
      eNextEnt = NE_NEXT_RIGHT;
      continue;
    }

    // (d) neither
    eNextEnt = NE_NEXT_EXIT;
  }

  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // join completed
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // mark last buffer as the last buffer
  pRefBuffer->vIsLast(true);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // put the buffer in the session bufferQueue
  pQueue->vAppend(pRefBuffer);
  
  vTrace("Joined\n"_Stz);
  
  
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

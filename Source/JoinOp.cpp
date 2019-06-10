/*============================================================================*/
/* JoinOp.cpp -- Join                                                         */
/*============================================================================*/

#include "PushDown.h"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
void JoinOp::vIndexJoin(IndexTree* pLeftIndexTree, IndexTree* pRightIndexTree)
{
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the first left Buffer
  EntBuffer* pLeftBuffer = pLeftIndexTree->pRoot();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the right left Buffer
  EntBuffer* pRightBuffer = pRightIndexTree->pRoot();
  
  // get the lead entry of the left buffer
  Ent* aLeftEntry = pLeftBuffer->aLeadEntry();
  
  // get the lead entry of the right buffer
  Ent* aRightEntry = pRightBuffer->aLeadEntry();
  
  bool bMoveLeft;
  bool bMoveRight;
  
  // dump the left and right buffers
  pLeftBuffer->vDump();
  pRightBuffer->vDump();
  
  while (aLeftEntry || aRightEntry) {
    
    bMoveLeft = false;
    bMoveRight = false;
    
    //    if (aLeftEntry)
    //      pLeftBuffer->vShowEntry(aLeftEntry);
    //    if (aRightEntry)
    //      pRightBuffer->vShowEntry(aRightEntry);
    
    // we have a left entry
    if (aLeftEntry) {
      
      // right entry ?
      if (aRightEntry) {
        
        // compare the keys
        eKC eCompare = Key::eCompare(aLeftEntry, aRightEntry);
        
        switch (eCompare) {
          case KC_SAME:
            // leftKey == rightKey
            vTrace("Joining rows %p %p\n"_Stz, aLeftEntry, aRightEntry);
            bMoveLeft = true;
            bMoveRight = true;
            break;
          case KC_BEFORE:
            // leftKey < rightKey
            vTrace("Joining rows %p NULL\n"_Stz, aLeftEntry);
            bMoveLeft = true;
            break;
          case KC_AFTER:
            // leftKey > rightKey
            vTrace("Joining rows NULL %p\n"_Stz, aRightEntry);
            bMoveRight = true;
            break;
          default:
            NEVER_REACH;
        }
      }
      else {
        // no right entry
        vTrace("Joining rows %p NULL\n"_Stz, aLeftEntry);
        bMoveLeft = true;
      }
    }
    else {
      // no left entry
      
      // right entry ?
      if (aRightEntry) {
        vTrace("Joining rows NULL %p\n"_Stz, aRightEntry);
        bMoveRight = true;
      }
      
    }
    
    if (bMoveLeft) {
      // move to the next left
      if (aLeftEntry) {
        aLeftEntry = EntBuffer::aGetNextEntry(&pLeftBuffer, aLeftEntry);
      }
    }
    
    if (bMoveRight) {
      // move to the next right
      if (aRightEntry) {
        aRightEntry = EntBuffer::aGetNextEntry(&pRightBuffer, aRightEntry);
      }
    }
  }
  

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

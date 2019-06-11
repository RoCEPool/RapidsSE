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
        eKC eCompare = EntWrk::eCompare(aLeftEntry, aRightEntry);
        
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

#define NEXT_LEFT       0  // probably make these constants or an enum
#define NEXT_RIGHT      1
#define NEXT_LEFT_RIGHT 2
#define NEXT_NONE       3
#define NEXT_EXIT       4


void JoinOp::vIndexJoin_v2(IndexTree* pLeftIndexTree, IndexTree* pRightIndexTree)
{
  vTrace("Joining: L(%s) ⋈ R(%s)\n"_Stz,
         pLeftIndexTree->pIndex()->pTable()->aTableName(),
         pRightIndexTree->pIndex()->pTable()->aTableName());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get initial buffers
  
  EntBuffer* pLeftBuffer  = pLeftIndexTree->pRoot();
  EntBuffer* pRightBuffer = pRightIndexTree->pRoot();

  // dump the first buffers
  pLeftBuffer->vDump();
  pRightBuffer->vDump();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // join loop

  Ent* aLeftEntry  = pLeftBuffer->aLeadEntry();
  Ent* aRightEntry = pRightBuffer->aLeadEntry();
  
  i8 iNextEnt = NEXT_NONE;
  
  while (NEXT_EXIT != iNextEnt) {
  
    // get next left entry, right, both, or none
    switch (iNextEnt) {
        
      case NEXT_LEFT: // get next left entry
        aLeftEntry  = EntBuffer::aGetNextEntry(&pLeftBuffer,aLeftEntry);
        break;
        
      case NEXT_RIGHT: // get next right entry
        aRightEntry = EntBuffer::aGetNextEntry(&pRightBuffer,aRightEntry);
        break;

      case NEXT_LEFT_RIGHT: // get next left and right entry
        aLeftEntry  = EntBuffer::aGetNextEntry(&pLeftBuffer,aLeftEntry);
        aRightEntry = EntBuffer::aGetNextEntry(&pRightBuffer,aRightEntry);
        break;
          
      case NEXT_NONE: // get neither, fall thru
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
          break;
        case KC_BEFORE: // leftKey < rightKey
          vTrace("Joining: L(%p) ⋈ R(NULL)\n"_Stz,aLeftEntry);
          break;
        case KC_AFTER:  // leftKey > rightKey
          vTrace("Joining: L(NULL) ⋈ R(%p)\n"_Stz,aRightEntry);
          break;
        default:
          NEVER_REACH;
      }
      
      // get next left and right
      iNextEnt = NEXT_LEFT_RIGHT;
      continue;
    }

    // (b) left entry only
    if (aLeftEntry) {
      vTrace("Joining: L(%p) ⋈ R(NULL)\n"_Stz,aLeftEntry);

      // get next left
      iNextEnt = NEXT_LEFT;
      continue;
    }

    // (c) right entry only
    if (aRightEntry) {
      vTrace("Joining: L(NULL) ⋈ R(%p)\n"_Stz,aRightEntry);

      // get next right
      iNextEnt = NEXT_RIGHT;
      continue;
    }

    // (d) neither
    iNextEnt = NEXT_EXIT;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // join completed
  
  vTrace("Joined\n"_Stz);
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

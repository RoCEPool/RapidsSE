/*============================================================================*/
/* SortOp.cpp -- Sort                                                         */
/*============================================================================*/

#include "PushDown.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
SortOp* PushDown::_pSortOp(void* aAddr)
{
  SortOp* pSortOp = reinterpret_cast<SortOp*>(aAddr);
  
  return pSortOp;
}

/*----------------------------------------------------------------------------*/
/* new                                                                        */
/*----------------------------------------------------------------------------*/
void* SortOp::operator new(size_t)
{
  SortOp* pSortOp =
  static_cast<SortOp*>(HeapMem::pAllocate(CN_SORTOP));
  
  return pSortOp;
}

/*----------------------------------------------------------------------------*/
/* delete                                                                     */
/*----------------------------------------------------------------------------*/
void SortOp::operator delete(void* aAddress)
{
  SortOp* pSortOp = static_cast<SortOp*>(aAddress);
  
  HeapMem::vFree(pSortOp);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SortOp::SortOp(Stz* szOpLabel, Jobs::Job* pJob)
: Op(CN_SORTOP,nullptr,szOpLabel,pJob)
{
  _precondition_(_MAGIC_,CN_SORTOP);
  
  m_fpOpFun = SortOp::eCallLoop;
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_SORTOP,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SortOp::~SortOp(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_SORTOP);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* call the sort loop function                                               */
/*----------------------------------------------------------------------------*/
eOR SortOp::eCallLoop(void* aThis)
{
  eOR eCallAgain = ((SortOp*)aThis)->eSortLoop();
  
  return eCallAgain;
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Partition                                                                  */
/*----------------------------------------------------------------------------*/
BufferCursor* SortOp::pPartitionBuffer(IntBuffer* pIntBuffer, BufferCursor* pLowCursor,
                               BufferCursor* pHighCursor)
{
  
  //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // point a cusror at the pivot point
  BufferCursor* pPivotCursor = new BufferCursor(pLowCursor);
  
//  vTrace("iPivot %lld Low %lld High %lld\n"_Stz,
//         *pPivotCursor->m_aElement,
//         *pLowCursor->m_aElement,
//         *pHighCursor->m_aElement);
  
  //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // define the start and end positions
  BufferCursor* pLeft = new BufferCursor(pLowCursor);
  BufferCursor* pRight = new BufferCursor(pHighCursor);
  
  //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the pivot element value
  i8 iPivotValue = *pPivotCursor->m_aElement;
  
  //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Move start and end toward each other, until they detect an inversion:
  // a pair of elements, one greater than or equal to the pivotValue,
  // one lesser or equal, that are in the wrong order relative to each other.
  
  while (true) {
    
    //vTrace("\nLeft %lld Right %lld\n"_Stz, *pLeft->m_aElement, *pRight->m_aElement);
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -- - - - - -
    // move start cursor forward till we go beyond the pivot value
    while (pLeft->bIsLessThan(iPivotValue)) {
      pLeft->vMoveRight();
    }
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // move end cursor backward till pivot value is beyond it
    while (pRight->bIsGreaterThan(iPivotValue)) {
      pRight->vMoveLeft();
    }
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // crossed
    if (pLeft->bIsAfterOrSame(pRight)) {
      // create a Partition cursor intially with a nullptr element
      BufferCursor* pPartitionCursor = new BufferCursor(pRight);
      //vTrace("Returning %lld\n"_Stz, *pRight->m_aElement);
      
      // cleanup
      delete pLeft;
      delete pRight;
      delete pPivotCursor;
      
      return pPartitionCursor;
    }
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -- - - - - - -
    // swap the inverted eIright and left) lements
    //vTrace("Swapping %lld %lld\n"_Stz, *pLeft->m_aElement, *pRight->m_aElement);
    pLeft->vSwap(pRight);
    //pIntBuffer->vDump();
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // move start cursor forward
    pLeft->vMoveRight();
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // move end cursor backward
    pRight->vMoveLeft();
  }
}

/*----------------------------------------------------------------------------*/
/* QuickSort                                                                  */
/*----------------------------------------------------------------------------*/
void SortOp::vQuickSortBuffer(IntBuffer* pIntBuffer, BufferCursor* pLowCursor, BufferCursor* pHighCursor)
{
  
  if (pLowCursor->bIsBefore(pHighCursor)) {
    BufferCursor* pPartitionCursor = pPartitionBuffer(pIntBuffer, pLowCursor, pHighCursor);
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // quick sort from low to partition
    vQuickSortBuffer(pIntBuffer, pLowCursor, pPartitionCursor);
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // quick sort from partition+1 to high
    pPartitionCursor->vMoveRight();
    BufferCursor* pPartitionNext = new BufferCursor(pPartitionCursor);
    vQuickSortBuffer(pIntBuffer, pPartitionNext, pHighCursor);
    
    delete pPartitionCursor;
    delete pPartitionNext;
  }
  
}

/*----------------------------------------------------------------------------*/
/* the sort operator loop function                                            */
/*----------------------------------------------------------------------------*/
eOR SortOp::eSortLoop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SORTOP,_FOOTER_);
  
  for (;;) {
    
    Operand* pOperand = pPopOperand();
    
    if (! pOperand) {
      ASSERT(bIsLocked());
      return OR_BIDE;
    }
    
    if (CN_ENDMARKER == pOperand->eClass()) {
      EndMarker* pEndMarker = _pEndMarker(pOperand);
      delete pEndMarker;
      break;
    }
    
    // (ray added this)
    if (CN_STREAMDEF == pOperand->eClass()) {
      continue;
    }
    
    IntBuffer* pIntBuffer = _pIntBuffer(pOperand);
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // dump the buffer
    //vTrace("\n\nSorting buffer:\n"_Stz);
    //pIntBuffer->vDump();
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // create a Low cursor pointing to the first element in the array
    BufferCursor* pLowCursor = new BufferCursor(pIntBuffer, 0, pIntBuffer->aGetElement(0));
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // create a High cursor pointing to the last element in the array
    BufferCursor* pHighCursor = new BufferCursor(pIntBuffer, 0, pIntBuffer->aGetElement(pIntBuffer->nCount() - 1));
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // call quick sort (recursive call)
    vQuickSortBuffer(pIntBuffer, pLowCursor, pHighCursor);
    
    //vTrace("\nSorted buffer:\n"_Stz);
    //pIntBuffer->vDump();
    
    // before feeding to mergeOp, delink the buffer first
    pIntBuffer->vNextBuffer(nullptr);
    pIntBuffer->vPrevBuffer(nullptr);
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // verify that the data is sorted
    RASSERT(pIntBuffer->bVerifyOrder());
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // delete the bufferCursors
    delete pLowCursor;
    delete pHighCursor;
    
    vPipeOut(pOperand);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  vPipeOut_endMarker();
  
  return OR_EXIT;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__aFooterFunction(SortOp);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

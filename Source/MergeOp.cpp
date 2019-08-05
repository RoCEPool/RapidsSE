/*============================================================================*/
/* MergeOp.cpp -- Merge                                                       */
/*============================================================================*/

#include "PushDown.h"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
MergeOp* PushDown::_pMergeOp(void* aAddr)
{
  MergeOp* pMergeOp = reinterpret_cast<MergeOp*>(aAddr);
  
  return pMergeOp;
}

/*----------------------------------------------------------------------------*/
/* new                                                                        */
/*----------------------------------------------------------------------------*/
void* MergeOp::operator new(size_t)
{
  MergeOp* pMergeOp =
  static_cast<MergeOp*>(HeapMem::pAllocate(CN_SORTOP));
  
  return pMergeOp;
}

/*----------------------------------------------------------------------------*/
/* delete                                                                     */
/*----------------------------------------------------------------------------*/
void MergeOp::operator delete(void* aAddress)
{
  MergeOp* pMergeOp = static_cast<MergeOp*>(aAddress);
  
  HeapMem::vFree(pMergeOp);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
MergeOp::MergeOp(Stz* szOpLabel, Digest* pDigest)
: Op(CN_MERGEOP,szOpLabel,pDigest,nullptr)
{
  _precondition_(_MAGIC_,CN_MERGEOP);
  
  for (i8 i_Index = 0; i_Index < C_OPERANDS; i_Index++)
    m_pOperand[i_Index] = nullptr;
  
  m_iOperands = 0;
  
  m_fpCall = MergeOp::eCallLoop;
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
MergeOp::~MergeOp(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_MERGEOP);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* call the merge loop function                                               */
/*----------------------------------------------------------------------------*/
eOR MergeOp::eCallLoop(void* aThis)
{
  eOR eCallAgain = ((MergeOp*)aThis)->eMergeLoop();
  
  return eCallAgain;
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* merge dangling operand sets into a single operand set                      */
/*----------------------------------------------------------------------------*/
void MergeOp::vMergeDangling(void)
{
  i8 i_Index;
  i8 j_Index = 0;
  Operand* pOperandPair[2];
  
  // merge remaining operand set pairs till we are left with one single set
  while (m_iOperands != 1) {
    
    // find a buffer set
    for (i_Index = 0; i_Index < C_OPERANDS; i_Index++) {
      
      if (m_pOperand[i_Index]) {
        pOperandPair[j_Index++] = m_pOperand[i_Index];
        m_pOperand[i_Index] = nullptr;
        m_iOperands--;
        
        // do we have a pair ?
        if (j_Index == 2)
          break;
      }
    }
    
    // merge the operand set pair
    vMergeOperands(pOperandPair[0], pOperandPair[1]);
  }
}

/*----------------------------------------------------------------------------*/
/* the merge operator loop function                                           */
/*----------------------------------------------------------------------------*/
eOR MergeOp::eMergeLoop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_MERGEOP,_FOOTER_);
  
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
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // check if we have another buffer binary magnitude 0
    if (m_pOperand[0]) {
      
      //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // get the operand of same magnitude
      Operand* pOperand2 = m_pOperand[0];
      
      // remove the operand from the m_pOperands array
      m_pOperand[0] = nullptr;
      m_iOperands--;
      
      // merge the two operands
      vMergeOperands(pOperand, pOperand2);
    }
    else {
      // save the buffer set till we have a similar magnitude buffer set to merge with
      m_pOperand[0] = pOperand;
      m_iOperands++;
    }
    
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // merge all remaining operand sets into a single operand set
  vMergeDangling();
  RASSERT(m_iOperands == 1);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // find the one remaining operand set
  i8 i_Index;
  for (i_Index = 0; i_Index < C_OPERANDS; i_Index++)
    if (m_pOperand[i_Index])
      break;

  IntBuffer* pIntBuffer = _pIntBuffer(m_pOperand[i_Index]);
  
  i8 iCount = 0;
  while (pIntBuffer) {
    vPipeOut(pIntBuffer);
    iCount++;
    pIntBuffer = pIntBuffer->pNextBuffer();
  }
  
  vTrace("MERGE Piped Out %lld buffer\n"_Stz, iCount);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // pipe out an end marker
  vPipeOut_endMarker();
  
  return OR_EXIT;
}

/*----------------------------------------------------------------------------*/
/* mergeSelf                                                                  */
/*----------------------------------------------------------------------------*/
void MergeOp::vMergeOperands(Operand* pOperand1, Operand* pOperand2)
{
  //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // dump the buffer
  //vTrace("\n\nSorting buffer:\n"_Stz);
  //pIntBuffer->vDump();
  
  // link the buffers
  IntBuffer* pIntBuffer1 = _pIntBuffer(pOperand1);
  RASSERT(pIntBuffer1->bVerifyNextPrev());
  
  IntBuffer* pIntBuffer2 = _pIntBuffer(pOperand2);
  RASSERT(pIntBuffer2->bVerifyNextPrev());
  
  // count no. of linked buffers received
  IntBuffer* pLastBuffer = pIntBuffer1;
  IntBuffer* pIntBuffer = pIntBuffer1;
  i8 iCount = 0;
  while (pIntBuffer) {
    iCount++;
    pLastBuffer = pIntBuffer;
    pIntBuffer = pIntBuffer->pNextBuffer();
  }
  
  pLastBuffer->vNextBuffer(pIntBuffer2);
  pIntBuffer2->vPrevBuffer(pLastBuffer);
  RASSERT(pIntBuffer1->bVerifyNextPrev());
  
  // count no. of linked buffers received
  pIntBuffer = pIntBuffer1;
  pLastBuffer = pIntBuffer1;
  iCount = 0;
  while (pIntBuffer) {
    iCount++;
    pLastBuffer = pIntBuffer;
    pIntBuffer = pIntBuffer->pNextBuffer();
  }
  
  //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create a Low cursor pointing to the first element in the array
  BufferCursor* pLowCursor = new BufferCursor(pIntBuffer1, 0, pIntBuffer1->aGetElement(0));
  
  //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create a High cursor pointing to the last element in the array
  
  BufferCursor* pHighCursor = new BufferCursor(pLastBuffer, iCount-1, pLastBuffer->aGetElement(pLastBuffer->nCount() - 1));
  
  //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // call quick sort (recursive call)
  vQuickSortBuffer(pIntBuffer1, pLowCursor, pHighCursor);
  
  //vTrace("\nSorted buffer:\n"_Stz);
  //pIntBuffer->vDump();
  
  //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // delete the bufferCursors
  delete pLowCursor;
  delete pHighCursor;
  
  //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // verify that the data is sorted
  RASSERT(pIntBuffer1->bVerifyOrder());
  
  i8 iCardinality = (i8)log2(iCount);
  if ( ! m_pOperand[iCardinality]) {
    m_pOperand[iCardinality] = pIntBuffer1;
    m_iOperands++;
  }
  else {
    vTrace("Merging self %p(%lld)\n"_Stz, pIntBuffer1, iCardinality);
    Operand* pOperand = m_pOperand[iCardinality];
    m_pOperand[iCardinality] = nullptr;
    m_iOperands--;
    vMergeOperands(pIntBuffer1, pOperand);
  }
  
}
/*----------------------------------------------------------------------------*/
/* Partition                                                                  */
/*----------------------------------------------------------------------------*/
BufferCursor* MergeOp::pPartitionBuffer(IntBuffer* pIntBuffer, BufferCursor* pLowCursor,
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
      
      // return partition cursor
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
void MergeOp::vQuickSortBuffer(IntBuffer* pIntBuffer, BufferCursor* pLowCursor, BufferCursor* pHighCursor)
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
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__aFooterFunction(MergeOp);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

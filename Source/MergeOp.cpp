/*============================================================================*/
/* MergeOp.cpp -- Merge                                                       */
/*============================================================================*/

#include "DataOperators.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
MergeOp* DataOperators::_pMergeOp(void* aAddr)
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
: Op(CN_MERGEOP,szOpLabel,pDigest,nullptr,false)
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_MERGEOP,_FOOTER_);
  
  i8 i_Index;
  i8 j_Index;
  Operand* pOperandPair[2];
  
  // merge remaining operand set pairs till we are left with one single set
  while (m_iOperands != 1) {
    
    j_Index = 0;
    
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
    
    Operand* pOperand = pPopQueue();
    
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
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // pipe out merge-sorted buffers
  i8 iCount = 0;
  while (pIntBuffer) {
    vPipeOut(pIntBuffer);
    iCount++;
    pIntBuffer = pIntBuffer->pNextBuffer();
  }
  
  vTrace("MERGE PIPED OUT %lld buffer\n"_Stz, iCount);
  
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_MERGEOP,_FOOTER_);

  vTrace("Merging operands %p %p\n"_Stz, pOperand1, pOperand2);
/*
  IntBuffer* pIntBuffer1 = _pIntBuffer(pOperand1);
  RASSERT(pIntBuffer1->bVerifyNextPrev());
  
  IntBuffer* pIntBuffer2 = _pIntBuffer(pOperand2);
  RASSERT(pIntBuffer2->bVerifyNextPrev());
  
  //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // link the buffers
  pIntBuffer1->vLinkBuffer(pIntBuffer2);
  RASSERT(pIntBuffer1->bVerifyNextPrev());
  
  //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get count and last buffer
  i8 iCount = 0;
  IntBuffer* pLastBuffer = pIntBuffer1->pLastBuffer(iCount);
  
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
  
  //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // calculate the magnitude of this sorted buffer
  i8 iMagnitude = (i8)log2(iCount);
  
  //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // do we have another buffer set of the same magnitude ?
  if ( ! m_pOperand[iMagnitude]) {
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // save this buffer set
    m_pOperand[iMagnitude] = pIntBuffer1;
    m_iOperands++;
  }
  else {
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // merge this buffer set with another of same magnitude
    Operand* pOperand = m_pOperand[iMagnitude];
    m_pOperand[iMagnitude] = nullptr;
    m_iOperands--;
    vMergeOperands(pIntBuffer1, pOperand);
  }
 */
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__aFooterFunction(MergeOp);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

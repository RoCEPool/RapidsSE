/*============================================================================*/
/* MergeOp.cpp -- Merge                                                       */
/*============================================================================*/

#include "DataOperators.h"

OpDesc __Op_Merge (
  CN_MERGEOP,         // class number
  OK_SIMPLE,          // op kind
  "Merge"_Stz,        // first word of prompt command
  "Mrg"_Stz,          // prefix
  true,               // partitioned
  MergeOp::pNew,      // function which parses the command and new's an op
  MergeOp::eCall,     // function which calls the op loop
  nullptr,            // function which calls the drain loop
  
  "<help>"_Stz
  );

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
MergeOp::MergeOp(Stz* szOpLabel, Digest* pDigest, OpDefn* aOpDefn, bool bAppR)
: Op(CN_MERGEOP,szOpLabel,pDigest,aOpDefn,false)
{
  _precondition_(_MAGIC_,CN_MERGEOP);
  
  for (i8 i_Index = 0; i_Index < C_OPERANDS; i_Index++)
    m_pOperand[i_Index] = nullptr;
  
  m_iOperands = 0;
  
  m_fpCall = MergeOp::eCall;
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
/* parse & new                                                                */
/*----------------------------------------------------------------------------*/
Op* MergeOp::pNew(Stz* szLabel,
                 Digest* pDigest, OpDefn* aOpDefn, bool bAppR,
                 TokenVec* pTknVec, i8& i_Tkn_, i8 i_Slot)
{
  i_Tkn_++;
  
  MergeOp* pMergeOp = new MergeOp(szLabel,pDigest,aOpDefn,bAppR);
  
  pMergeOp->m_nSlot = i_Slot;
  
  return pMergeOp;
}

/*----------------------------------------------------------------------------*/
/* call the merge loop function                                               */
/*----------------------------------------------------------------------------*/
eOR MergeOp::eCall(void* aThis)
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

  SrtBuffer* pSrtBuffer1 = _pSrtBuffer(pOperand1);
  RASSERT(pSrtBuffer1->bVerifyNextPrev());
  
  SrtBuffer* pSrtBuffer2 = _pSrtBuffer(pOperand2);
  RASSERT(pSrtBuffer2->bVerifyNextPrev());
  
  //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // link the buffers
  pSrtBuffer1->vLinkBuffer(pSrtBuffer2);
  RASSERT(pSrtBuffer1->bVerifyNextPrev());
  
  //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get count and last buffer
  i8 iCount = 0;
  SrtBuffer* pLastBuffer = pSrtBuffer1->pLastBuffer(iCount);
  
  //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create a Low cursor pointing to the first element in the array
  BufferCursor* pLowCursor = new BufferCursor(pSrtBuffer1, 0, 0);
  
  //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create a High cursor pointing to the last element in the array
  
  BufferCursor* pHighCursor = new BufferCursor(pLastBuffer, iCount-1, pLastBuffer->nItems());
  
  //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // call quick sort (recursive call)
  BufferSort::vQuickSortBuffer(pSrtBuffer1, pLowCursor, pHighCursor);
  
  //vTrace("\nSorted buffer:\n"_Stz);
  //pIntBuffer->vDump();
  
  //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // delete the bufferCursors
  delete pLowCursor;
  delete pHighCursor;
  
  //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // verify that the data is sorted
  RASSERT(pSrtBuffer1->bVerifyOrder());
  
  //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // calculate the magnitude of this sorted buffer
  i8 iMagnitude = (i8)log2(iCount);
  
  //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // do we have another buffer set of the same magnitude ?
  if ( ! m_pOperand[iMagnitude]) {
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // save this buffer set
    m_pOperand[iMagnitude] = pSrtBuffer1;
    m_iOperands++;
  }
  else {
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // merge this buffer set with another of same magnitude
    Operand* pOperand = m_pOperand[iMagnitude];
    m_pOperand[iMagnitude] = nullptr;
    m_iOperands--;
    vMergeOperands(pSrtBuffer1, pOperand);
  }

}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__aFooterFunction(MergeOp);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

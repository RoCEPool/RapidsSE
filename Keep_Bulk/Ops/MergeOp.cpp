/*============================================================================*/
/* MergeOp.cpp -- Merge                                                       */
/*============================================================================*/

#include "DataOperators.h"

OpDesc __Op_Merge (
  CN_MERGEOP,         // class number
  OK_SINGLE,          // op kind
  "Merge"_Stz,        // first word of prompt command
  "Mrg"_Stz,          // prefix
                   
  true,               // partitioned
  false,              // compiler op

  false,              // accepts CrwBuffer?
  false,              // accepts EntBuffer?
  false,              // accepts IrwBuffer?
  true,               // accepts SrtBuffer?
                   
  false,              // accepts CrwBuffer?
  false,              // accepts EntBuffer?
  false,              // accepts IrwBuffer?
  true,               // accepts SrtBuffer?
                   
  CN_NONE,            // what kind of output
                   
  MergeOp::pNew,      // function which parses the command and new's an op
  MergeOp::eCall,     // function which calls the op loop
  nullptr,            // function which calls the drain loop
  
  "merge"_Stz
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
MergeOp::MergeOp(OpSpec* pOpSpec, Pipeline* pPipeline)
: Op(CN_MERGEOP,pOpSpec,pPipeline)
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
Op* MergeOp::pNew(OpSpec* pOpSpec, Pipeline* pPipeline)
{  
  MergeOp* pMergeOp = new MergeOp(pOpSpec,pPipeline);
  
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
  
  i8 iOperandMag[2];
  
  // merge remaining operand set pairs till we are left with one single set
  while (m_iOperands > 1) {
    
    j_Index = 0;
    
    // find a buffer set
    for (i_Index = 0; i_Index < C_OPERANDS; i_Index++) {
      
      if (m_pOperand[i_Index]) {
        iOperandMag[j_Index] = i_Index;
        pOperandPair[j_Index++] = m_pOperand[i_Index];
        m_pOperand[i_Index] = nullptr;
        m_iOperands--;
        
        // do we have a pair ?
        if (j_Index == 2)
          break;
      }
    }
    
    if (AM::bVerbose())
      vTrace("P:%lld MERGING DANGLING OF MAGNITUDES %lld %lld\n"_Stz,
             m_nPartition, iOperandMag[0], iOperandMag[1]);
    
    // merge the operand set pair
    AM::vLogOpr(SL_INF,this,OA_OP_INF,
                "P:%lld Merging Dangling of Mags %lld %lld"_Stz, //!format-LogOpr
                m_nPartition, iOperandMag[0], iOperandMag[1]);
    
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
    
    if (CN_STREAMDEF == pOperand->eClass()) {
      m_pStreamDef = new StreamDef(_pStreamDef(pOperand));
      vPipeOut(pOperand);
      continue;
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
      AM::vLogOpr(SL_INF,this,OA_OP_INF,
                  "P:%lld Merging Operands of Mag %lld"_Stz, //!format-LogOpr
                  m_nPartition, 0);
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
  RASSERT(m_iOperands <=1);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // find the one remaining operand set
  i8 i_Index;
  for (i_Index = 0; i_Index < C_OPERANDS; i_Index++)
    if (m_pOperand[i_Index])
      break;

  SrtBuffer* pMergedBuffer = _pSrtBuffer(m_pOperand[i_Index]);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // pipe out merge-sorted buffers
  //  if (pMergedBuffer) {
  //    SrtBuffer* pSrtBuffer = pMergedBuffer;
  //    while (pSrtBuffer) {
  //      vPipeOut(pSrtBuffer);
  //      pSrtBuffer = pSrtBuffer->pNextBuffer();
  //    }
  //
  //    if (AM::bVerbose())
  //      vTrace("MERGE PIPED OUT %lld buffer\n"_Stz, pMergedBuffer->nItemCount());
  //  }
  vPipeOut(pMergedBuffer);
  
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

  if (AM::bVerbose())
    vTrace("Merging operands %p %p\n"_Stz, pOperand1, pOperand2);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the left buffer
  SrtBuffer* pLeftBuffer = _pSrtBuffer(pOperand1);
  RASSERT(pLeftBuffer->bVerifyNextPrev());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the right buffer
  SrtBuffer* pRightBuffer = _pSrtBuffer(pOperand2);
  RASSERT(pRightBuffer->bVerifyNextPrev());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // left and right buffers should belong to the same partition
  RASSERT(pLeftBuffer->iPartitionNo() == pRightBuffer->iPartitionNo());
  
  //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get count of left and right buffers
  i8 iLeftCount = pLeftBuffer->nItemCount();
  i8 iRightCount = pRightBuffer->nItemCount();
  
  //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create a left cursor pointing to the first element in the left buffer
  BufferCursor* pLeftCursor = new BufferCursor(pLeftBuffer, 0, 0);
  
  //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create a right cursor pointing to the first element in the right buffer
  BufferCursor* pRightCursor = new BufferCursor(pRightBuffer, 0, 0);
  
  //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create a new srtBuffer to copy elements after merging
  SrtBuffer* pFirstMergeBuffer  = new SrtBuffer(pLeftBuffer->nRowRefs(), pLeftBuffer->iPartitionNo());
  i8 iMergeBufferCount = 1;
  
  //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // save left and right remaining row refs
  i8 iLeftRemaining = iLeftCount;
  i8 iRightRemaining = iRightCount;
  
  SrtSet* pSrtSet = nullptr;
  
  SrtBuffer* pMergeBuffer = pFirstMergeBuffer;
  
  //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // compare and copy till we complete either left or right
  while (iLeftRemaining != 0 && iRightRemaining != 0) {
    
    RASSERT(pMergeBuffer);
    
    if (pLeftCursor->bIsLessThan(pRightCursor)) {
      
      // copy left value to new buffer
      pMergeBuffer = pMergeBuffer->aCopySrtSet(pLeftCursor->aSrtSet());

      // move left cursor to the right
      pLeftCursor->bMoveRight();
      iLeftRemaining--;
    }
    else if (pLeftCursor->bIsGreaterThan(pRightCursor))
    {
      
      // copy right value to new buffer
      pMergeBuffer = pMergeBuffer->aCopySrtSet(pRightCursor->aSrtSet());
      RASSERT(pMergeBuffer);
      // move right cursor to the right
      pRightCursor->bMoveRight();
      iRightRemaining--;
    }
    else {
      // copy both
      pMergeBuffer = pMergeBuffer->aCopySrtSet(pLeftCursor->aSrtSet());
      pMergeBuffer = pMergeBuffer->aCopySrtSet(pRightCursor->aSrtSet());
   
      // move both
      pLeftCursor->bMoveRight();
      pRightCursor->bMoveRight();
      iLeftRemaining--;
      iRightRemaining--;
    }
    
    //vTrace("\nMerged buffers:\n"_Stz);
    //pMergeBuffer->vShow();
  }
  
  // now copy the remaining
  RASSERT(iLeftRemaining == 0 || iRightRemaining == 0);
  
  if (iLeftRemaining) {
    RASSERT(iRightRemaining == 0);
    
    while (iLeftRemaining--) {
      
      // copy left value to new buffer
      pMergeBuffer = pMergeBuffer->aCopySrtSet(pLeftCursor->aSrtSet());
      
      // move left cursor to the right
      pLeftCursor->bMoveRight();
    }
  }
  else {
    RASSERT(iLeftRemaining == 0);
    
    if (iRightRemaining) {
      while (iRightRemaining--) {
        
        // copy left value to new buffer
        pMergeBuffer = pMergeBuffer->aCopySrtSet(pRightCursor->aSrtSet());
        
        // move left cursor to the right
        pRightCursor->bMoveRight();
      }
    }
  }
  
  //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // delete the bufferCursors
  delete pLeftCursor;
  delete pRightCursor;
  
  //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // verify that the data is sorted
  //RASSERT(pFirstMergeBuffer->bVerifyOrder());
  
  //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // calculate the magnitude of this sorted buffer
  i8 iMergedCount = pFirstMergeBuffer->nItemCount();
  RASSERT(iLeftCount + iRightCount == iMergedCount);
  
  iMergeBufferCount = pFirstMergeBuffer->nBufferCount();
  i8 iMagnitude = (i8)log2(iMergeBufferCount);
  
  AM::vLogOpr(SL_INF,this,OA_OP_INF,
              "P:%lld Merged Operands generated Mag %lld"_Stz, //!format-LogOpr
              m_nPartition, iMagnitude);
  
  // delete all left buffers
  SrtBuffer* pBuffer;
  while (pLeftBuffer) {
    pBuffer = pLeftBuffer;
    pLeftBuffer = pLeftBuffer->pNextBuffer();
    delete pBuffer;
  }
  
  // delete all right buffers
  while (pRightBuffer) {
    pBuffer = pRightBuffer;
    pRightBuffer = pRightBuffer->pNextBuffer();
    delete pBuffer;
  }
  
  //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // do we have another buffer set of the same magnitude ?
  if ( ! m_pOperand[iMagnitude]) {
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // save this buffer set
    m_pOperand[iMagnitude] = pFirstMergeBuffer;
    m_iOperands++;
  }
  else {
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // merge this buffer set with another of same magnitude
    Operand* pOperand = m_pOperand[iMagnitude];
    m_pOperand[iMagnitude] = nullptr;
    m_iOperands--;

    
    AM::vLogOpr(SL_INF,this,OA_OP_INF,
                "P:%lld Merging Operands of Mag %lld"_Stz, //!format-LogOpr
                m_nPartition, iMagnitude);
    vMergeOperands(pFirstMergeBuffer, pOperand);
  }

  
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__aFooterFunction(MergeOp);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

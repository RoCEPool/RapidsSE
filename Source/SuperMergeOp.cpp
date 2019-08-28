/*============================================================================*/
/* SuperMergeOp.cpp -- Super Merge                                            */
/*============================================================================*/

#include "DataOperators.h"

OpDesc __Op_SuperMerge (
  CN_SUPERMERGEOP,         // class number
  OK_SIMPLE,          // op kind
  "SuperMerge"_Stz,   // first word of prompt command
  "Smg"_Stz,          // prefix
  false,              // partitioned
  SuperMergeOp::pNew, // function which parses the command and new's an op
  SuperMergeOp::eCall,// function which calls the op loop
  nullptr,            // function which calls the drain loop
  
  "<help>"_Stz
  );

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
SuperMergeOp* DataOperators::_pSuperMergeOp(void* aAddr)
{
  SuperMergeOp* pSuperMergeOp = reinterpret_cast<SuperMergeOp*>(aAddr);
  
  return pSuperMergeOp;
}

/*----------------------------------------------------------------------------*/
/* new                                                                        */
/*----------------------------------------------------------------------------*/
void* SuperMergeOp::operator new(size_t)
{
  SuperMergeOp* pSuperMergeOp =
  static_cast<SuperMergeOp*>(HeapMem::pAllocate(CN_SORTOP));
  
  return pSuperMergeOp;
}

/*----------------------------------------------------------------------------*/
/* delete                                                                     */
/*----------------------------------------------------------------------------*/
void SuperMergeOp::operator delete(void* aAddress)
{
  SuperMergeOp* pSuperMergeOp = static_cast<SuperMergeOp*>(aAddress);
  
  HeapMem::vFree(pSuperMergeOp);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SuperMergeOp::SuperMergeOp(Stz* szOpLabel, Digest* pDigest, OpDefn* aOpDefn, bool bAppR)
: Op(CN_SUPERMERGEOP,szOpLabel,pDigest,aOpDefn,false)
{
  _precondition_(_MAGIC_,CN_SUPERMERGEOP);
  
  for (i8 i_Index = 0; i_Index < C_MAX_SLOTS; i_Index++)
    m_pOperand[i_Index] = nullptr;
  
  m_iOperands = 0;
  
  m_fpCall = SuperMergeOp::eCall;
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SuperMergeOp::~SuperMergeOp(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_SUPERMERGEOP);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* parse & new                                                                */
/*----------------------------------------------------------------------------*/
Op* SuperMergeOp::pNew(Stz* szLabel,
                       Digest* pDigest, OpDefn* aOpDefn, bool bAppR,
                       TokenVec* pTknVec, i8& i_Tkn_, i8 i_Slot)
{
  i_Tkn_++;
  
  SuperMergeOp* pSuperMergeOp = new SuperMergeOp(szLabel,pDigest,aOpDefn,bAppR);
  
  pSuperMergeOp->m_nSlot = i_Slot;
  
  return pSuperMergeOp;
}

/*----------------------------------------------------------------------------*/
/* call the merge loop function                                               */
/*----------------------------------------------------------------------------*/
eOR SuperMergeOp::eCall(void* aThis)
{
  eOR eCallAgain = ((SuperMergeOp*)aThis)->eMergeLoop();
  
  return eCallAgain;
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* merge dangling operand sets into a single operand set                      */
/*----------------------------------------------------------------------------*/
void SuperMergeOp::vMergeDangling(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SUPERMERGEOP,_FOOTER_);
  
  i8 i_Index;
  i8 j_Index;
  Operand* pOperandPair[2];
  
  i8 iOperandMag[2];
  
  // merge remaining operand set pairs till we are left with one single set
  while (m_iOperands > 1) {
    
    j_Index = 0;
    
    // find a buffer set
    for (i_Index = 0; i_Index < AM::nSlotsPerNode(); i_Index++) {
      
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
             m_nSlot, iOperandMag[0], iOperandMag[1]);
    
    // merge the operand set pair
    AM::vLogOpr(SL_INF,this,OA_OP_INF,
                "P:%lld Merging Dangling of Mags %lld %lld"_Stz, //!format-LogOpr
                m_nSlot, iOperandMag[0], iOperandMag[1]);
    
    SrtBuffer* pSrtBuffer = pSuperMergeOperands(pOperandPair[0], pOperandPair[1]);
    for (i_Index = 0; i_Index < AM::nSlotsPerNode(); i_Index++) {
      if ( ! m_pOperand[i_Index] ) {
        m_pOperand[i_Index] = pSrtBuffer;
        m_iOperands++;
        break;
      }
    }
  }
}

/*----------------------------------------------------------------------------*/
/* the merge operator loop function                                           */
/*----------------------------------------------------------------------------*/
eOR SuperMergeOp::eMergeLoop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SUPERMERGEOP,_FOOTER_);
  
  for (;;) {
    
    Operand* pOperand = pPopQueue();
    
    if (! pOperand) {
      ASSERT(bIsLocked());
      return OR_BIDE;
    }
    
    if (CN_ENDMARKER == pOperand->eClass()) {
      delete (EndMarker*)pOperand;
      if (--m_nLeftEndMarkers == 0) {
        break;
      }
      continue;
    }
    
    if (CN_STREAMDEF == pOperand->eClass()) {
      if ( ! m_pStreamDef ) {
        m_pStreamDef = new StreamDef(_pStreamDef(pOperand));
        vPipeOut(pOperand);
      }
      else
        delete _pStreamDef(pOperand);
      continue;
    }
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // save the buffer
    m_pOperand[m_iOperands++] = _pSrtBuffer(pOperand);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // should have reacieved a buffer set from each partition
  RASSERT(m_iOperands == AM::nSlotsPerNode());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // merge all operand sets into a single operand set
  vMergeDangling();
  RASSERT(m_iOperands <=1);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // find the one remaining operand set
  i8 i_Index;
  for (i_Index = 0; i_Index < C_OPERANDS; i_Index++)
    if (m_pOperand[i_Index])
      break;

  SrtBuffer* pMergedBuffer = _pSrtBuffer(m_pOperand[i_Index]);
  
  if (AM::bVerbose())
    vTrace("MERGE PIPING OUT %lld buffer\n"_Stz, pMergedBuffer->nItemCount());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // pipe out merge-sorted buffers
  if (pMergedBuffer) {
    SrtBuffer* pSrtBuffer = pMergedBuffer;
    while (pSrtBuffer) {
      vPipeOut(pSrtBuffer);
      pSrtBuffer = pSrtBuffer->pNextBuffer();
    }
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // pipe out an end marker
  vPipeOut_endMarker();
  
  return OR_EXIT;
}

/*----------------------------------------------------------------------------*/
/* mergeSelf                                                                  */
/*----------------------------------------------------------------------------*/
SrtBuffer* SuperMergeOp::pSuperMergeOperands(Operand* pOperand1, Operand* pOperand2)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SUPERMERGEOP,_FOOTER_);

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
  // left and right buffers should not belong to the same partition
  //RASSERT(pLeftBuffer->iPartitionNo() != pRightBuffer->iPartitionNo());
  
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
              m_nSlot, iMagnitude);
  
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
  
  return pFirstMergeBuffer;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__aFooterFunction(SuperMergeOp);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

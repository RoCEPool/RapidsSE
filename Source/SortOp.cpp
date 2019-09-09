/*============================================================================*/
/* SortOp.cpp -- Sort                                                         */
/*============================================================================*/

#include "DataOperators.h"

OpDesc __Op_Sort (
  CN_SORTOP,          // class number
  OK_SIMPLE,          // op kind
  "Sort"_Stz,         // first word of prompt command
  "Srt"_Stz,          // prefix
  true,               // partitioned
  SortOp::pNew,       // function which parses the command and new's an op
  SortOp::eCall,      // function which calls the op loop
  nullptr,            // function which calls the drain loop

  "<help>"_Stz
  );


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
SortOp* DataOperators::_pSortOp(void* aAddr)
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
SortOp::SortOp(Stz* szOpLabel, Digest* pDigest, OpDefn* aOpDefn, bool bAppR, OpSpec* pOpSpec)
: Op(CN_SORTOP,szOpLabel,pDigest,aOpDefn,bAppR,pOpSpec)
{
  _precondition_(_MAGIC_,CN_SORTOP);
  
  m_fpCall = SortOp::eCall;
  
  _checkInitialised_(CN_SORTOP);
  
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
/* parse & new                                                                */
/*----------------------------------------------------------------------------*/
Op* SortOp::pNew(Stz* szLabel, Digest* pDigest,
                 OpDefn* aOpDefn, bool bAppR, OpSpec* pOpSpec,
                 TokenVec* pTknVec, i8& i_Tkn_, i8 i_Slot)
{
  i_Tkn_++;
  
  SortOp* pSortOp = new SortOp(szLabel,pDigest,aOpDefn,bAppR,pOpSpec);

  pSortOp->m_nSlot = i_Slot;
  
  return pSortOp;
}

/*----------------------------------------------------------------------------*/
/* call the sort loop function                                               */
/*----------------------------------------------------------------------------*/
eOR SortOp::eCall(void* aThis)
{
  eOR eCallAgain = ((SortOp*)aThis)->eSortLoop();
  
  return eCallAgain;
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* the sort operator loop function                                            */
/*----------------------------------------------------------------------------*/
eOR SortOp::eSortLoop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SORTOP,_FOOTER_);
  
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
    
    RASSERT(CN_SRTBUFFER == pOperand->eClass());
    
    SrtBuffer* pSrtBuffer = _pSrtBuffer(pOperand);
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // call quick sort (recursive call)
    QuickSort::vQuickSortBuffer(m_pStreamDef->m_oColVecAry.pGet(0), pSrtBuffer, 0, pSrtBuffer->nItems() - 1);
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // verify that the data is sorted
    //RASSERT(pSrtBuffer->bVerifyOrder());
    
    // before feeding to mergeOp, delink the buffer first
    pSrtBuffer->vNextBuffer(nullptr);
    pSrtBuffer->vPrevBuffer(nullptr);
    
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

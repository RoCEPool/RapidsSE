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
SortOp::SortOp(Stz* szOpLabel, Digest* pDigest, OpDefn* aOpDefn, bool bAppR)
: Op(CN_SORTOP,szOpLabel,pDigest,aOpDefn,bAppR)
{
  _precondition_(_MAGIC_,CN_SORTOP);
  
  m_pStreamDef = nullptr;
  m_sSortKey   = ""_Stz;
  m_iSortCol   = -1;
  
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
  
  // delete the streamDef
  delete m_pStreamDef;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* parse & new                                                                */
/*----------------------------------------------------------------------------*/
Op* SortOp::pNew(Stz* szLabel,
                 Digest* pDigest, OpDefn* aOpDefn, bool bAppR,
                 TokenVec* pTknVec, i8& i_Tkn_, i8 i_Slot)
{
  i_Tkn_++;
  
  SortOp* pSortOp = new SortOp(szLabel,pDigest,aOpDefn,bAppR);
  
  pSortOp->m_sSortKey = pTknVec->pGet(i_Tkn_++)->szText();

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
    
    // (ray added this)
    if (CN_STREAMDEF == pOperand->eClass()) {
      
      m_pStreamDef = new StreamDef(_pStreamDef(pOperand));
      
      // figure out the key idx
      for (i8 i_Col = 0; i_Col < m_pStreamDef->m_oColAry.nItems(); i_Col++) {
        Column* pCol = m_pStreamDef->m_oColAry.pGet(i_Col);
        if (StrLib::bEqualTo(m_sSortKey.aChar(),pCol->sColumnName().aChar(),C_NO_CASE)) {
          m_iSortCol = i_Col;
          break;
        }
        RASSERT(m_iSortCol != -1);
      }
      
      delete _pStreamDef(pOperand);
      
      continue;
    }
    
    RASSERT(CN_IRWBUFFER == pOperand->eClass());
    
    IntBuffer* pIntBuffer = _pIntBuffer(pOperand);
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // create a Low cursor pointing to the first element in the array
    BufferCursor* pLowCursor = new BufferCursor(pIntBuffer, 0, pIntBuffer->aGetElement(0));
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // create a High cursor pointing to the last element in the array
    BufferCursor* pHighCursor = new BufferCursor(pIntBuffer, 0, pIntBuffer->aGetElement(pIntBuffer->nCount() - 1));
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // call quick sort (recursive call)
    BufferSort::vQuickSortBuffer(pIntBuffer, pLowCursor, pHighCursor);
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // verify that the data is sorted
    RASSERT(pIntBuffer->bVerifyOrder());
    
    // before feeding to mergeOp, delink the buffer first
    pIntBuffer->vNextBuffer(nullptr);
    pIntBuffer->vPrevBuffer(nullptr);
    
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

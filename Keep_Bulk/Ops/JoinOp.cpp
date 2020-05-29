/*============================================================================*/
/* JoinOp.cpp -- Join                                                         */
/*============================================================================*/

#include "DataOperators.h"

OpDesc __Op_Join (
  CN_JOINOP,          // class number
  OK_DUPLEX,          // op kind
  "Join"_Stz,         // first word of prompt command
  "Joi"_Stz,          // prefix
                  
  true,               // partitioned
  false,              // compiler op

  false,              // accepts CrwBuffer?
  true,               // accepts EntBuffer?
  false,              // accepts IrwBuffer?
  false,              // accepts SrtBuffer?
                  
  false,              // accepts CrwBuffer?
  false,              // accepts EntBuffer?
  false,              // accepts IrwBuffer?
  true,               // accepts SrtBuffer?
  
  CN_NONE,            // what kind of output
                  
  JoinOp::pNew,       // function which parses the command and new's an op
  JoinOp::eCall,      // function which calls the op loop
  nullptr,            // function which calls the drain loop

  "join"_Stz
);


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* new                                                                        */
/*----------------------------------------------------------------------------*/
void* JoinOp::operator new(size_t)
{
  JoinOp* pJoinOp =
  static_cast<JoinOp*>(HeapMem::pAllocate(CN_JOINOP));
  
  return pJoinOp;
}

/*----------------------------------------------------------------------------*/
/* delete                                                                     */
/*----------------------------------------------------------------------------*/
void JoinOp::operator delete(void* aAddress)
{
  JoinOp* pJoinOp = static_cast<JoinOp*>(aAddress);
  
  HeapMem::vFree(pJoinOp);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
JoinOp::JoinOp(OpSpec* pOpSpec, Pipeline* pPipeline)
: Op(CN_JOINOP,pOpSpec,pPipeline)
{
  _precondition_(_MAGIC_,CN_JOINOP);
  
  m_pLeftBuffer  = nullptr;
  m_pRightBuffer = nullptr;
  
  m_aLeftEntry  = nullptr;
  m_aRightEntry = nullptr;
  
  m_bEndLeftOperands  = false;
  m_bEndRightOperands = false;
  
  m_pSrtBuffer = nullptr;
  
  m_eNextEntry = NE_NEXT_LEFT_RIGHT;
  m_bGotLeft = false;
  
  m_pStreamDef = new StreamDef;
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
JoinOp::~JoinOp(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_JOINOP);

}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* parse & new                                                                */
/*----------------------------------------------------------------------------*/
Op* JoinOp::pNew(OpSpec* pOpSpec, Pipeline* pPipeline)
{  
  JoinOp* pJoinOp = new JoinOp(pOpSpec,pPipeline);
  
  return pJoinOp;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* call the join loop function                                                */
/*----------------------------------------------------------------------------*/
eOR JoinOp::eCall(void* aThis)
{
  eOR eCallAgain = ((JoinOp*)aThis)->eJoinLoop();
  
  return eCallAgain;
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add row refs                                                               */
/*----------------------------------------------------------------------------*/
void JoinOp::vAddRowRefs(Ent* aLeftEntry, Ent* aRightEntry)
{
  
  i8 nRowRefs = 2;
  
  // allot a refSet
  if ( ! m_pSrtBuffer ) {
    m_pSrtBuffer = new SrtBuffer(nRowRefs, TablePtn::iPartitionToSlot(m_nPartition));
    
    // pipe out the stream def's
    RASSERT(m_pStreamDef);
    vPipeOut(m_pStreamDef);
    m_pStreamDef = nullptr;
  }
  
  SrtSet* pSrtSet = m_pSrtBuffer->aAllotSrtSet();
  
  if ( ! pSrtSet )  {
    // Sort Buffer is full
    // pipe out the SrtBuffer
    vPipeOut(m_pSrtBuffer);
    
    // create another SrtBuffer for the refEnt's
    m_pSrtBuffer = new SrtBuffer(nRowRefs, TablePtn::iPartitionToSlot(m_nPartition));
    
    // insert row from entry in temporary buffer
    pSrtSet = m_pSrtBuffer->aAllotSrtSet();
    RASSERT(pSrtSet);
  }
  
  // get the row buffers
  CrwBuffer* pLeftCrwBuffer = aLeftEntry ? CrwBuffer::pRefdCrwBuffer(aLeftEntry->xRef()) : nullptr;
  CrwBuffer* pRightCrwBuffer = aRightEntry ? CrwBuffer::pRefdCrwBuffer(aRightEntry->xRef()) : nullptr;
  
  // set the rowRefs
  Crw* aLeftRowData = aLeftEntry ? pLeftCrwBuffer->pCrw(aLeftEntry->xRef().iItem()) : nullptr;
  Crw* aRightRowData = aRightEntry ? pRightCrwBuffer->pCrw(aRightEntry->xRef().iItem()) : nullptr;
  
  // set the rowData in the refSet
  pSrtSet->vRowRef(0, (i8)aLeftRowData);
  pSrtSet->vRowRef(1, (i8)aRightRowData);
}

#define TRACE_JOIN

/*----------------------------------------------------------------------------*/
/* the join operator loop function                                            */
/*----------------------------------------------------------------------------*/
eOR JoinOp::eJoinLoop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOINOP,_FOOTER_);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get first entries from left and right buffers
  //Ent* aLeftEntry  = pLeftBuffer ? pLeftBuffer->aLeadEntry() : nullptr;
  //Ent* aRightEntry = pRightBuffer ? pRightBuffer->aLeadEntry() : nullptr;
  
  Ent* aFirstRightMatch = nullptr;
  Ent* aFirstLeftMatch  = nullptr;
  EntBuffer* pFirstRightBuffer = nullptr;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // join loop
  
  while (NE_NEXT_EXIT != m_eNextEntry) {
    
    // get next left entry, right, both, or none
    switch (m_eNextEntry) {
        
      case NE_NEXT_LEFT: // get next left entry
        m_aLeftEntry  = m_pLeftBuffer ? m_pLeftBuffer->aNextEntry(m_aLeftEntry, 0) : NULL;
        
        while ( ! m_aLeftEntry && ! m_bEndLeftOperands) {
          if (OR_BIDE == ePopLeftOperand())
            return OR_BIDE;
          m_aLeftEntry  = m_pLeftBuffer ? m_pLeftBuffer->aLeadEntry() : NULL;
        }
      
        // check if it matches a first left
        if (m_aLeftEntry && aFirstLeftMatch) {
          eKC eCompare = EntWrk::eCompare(m_aLeftEntry,aFirstLeftMatch);
          
          // if matches a first left start with teh first right
          if (eCompare == KC_SAME) {
            m_aRightEntry = aFirstRightMatch;
            m_pRightBuffer = pFirstRightBuffer;
          }
          else {
            aFirstLeftMatch = nullptr;
            aFirstRightMatch = nullptr;
          }
        }
        break;
        
      case NE_NEXT_RIGHT: // get next right entry
        m_aRightEntry  = m_pRightBuffer ? m_pRightBuffer->aNextEntry(m_aRightEntry, 0) : NULL;
        
        while ( ! m_aRightEntry && ! m_bEndRightOperands) {
          if (OR_BIDE == ePopRightOperand())
            return OR_BIDE;
          m_aRightEntry  = m_pRightBuffer ? m_pRightBuffer->aLeadEntry() : NULL;
        }
        break;
        
      case NE_NEXT_LEFT_RIGHT: // get next left and right entry
        
        if ( ! m_bGotLeft) {
          
          if (m_pLeftBuffer) {
            m_aLeftEntry = m_pLeftBuffer->aNextEntry(m_aLeftEntry, 0);
          }
          
          while ( ! m_aLeftEntry && ! m_bEndLeftOperands) {
            if (OR_BIDE == ePopLeftOperand())
              return OR_BIDE;
            m_aLeftEntry = m_pLeftBuffer ? m_pLeftBuffer->aLeadEntry() : NULL;
          }
        }
        
        m_bGotLeft = true;
        
        if (m_pRightBuffer) {
          m_aRightEntry  = m_pRightBuffer->aNextEntry(m_aRightEntry, 0);
        }
        
        while ( ! m_aRightEntry && ! m_bEndRightOperands) {
          if (OR_BIDE == ePopRightOperand())
            return OR_BIDE;
          m_aRightEntry  = m_pRightBuffer ? m_pRightBuffer->aLeadEntry() : NULL;
        }
  
        m_bGotLeft = false;
        
        break;
        
      case NE_NEXT_NONE: // get neither, fall thru
        break;
        
      default:
        NEVER_REACH;
    }
    
    // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
    
    // join action based on whether there is a left and/or right entry
    
    // (a) both left and right entry
    if (m_aLeftEntry && m_aRightEntry) {
      
      // compare the two keys
      eKC eCompare = EntWrk::eCompare(m_aLeftEntry,m_aRightEntry);
      
      // how do they compare?
      switch (eCompare) {
        case KC_SAME:   // leftKey == rightKey
#ifdef TRACE_JOIN
          if (AM::bVerbose())
            vTrace("Joining: L(%p) ⋈ R(%p)\n"_Stz,m_aLeftEntry,m_aRightEntry);
#endif
          vAddRowRefs(m_aLeftEntry, m_aRightEntry);
          m_eNextEntry = NE_NEXT_RIGHT;
          
          // save first right
          if ( ! aFirstRightMatch ) {
            aFirstRightMatch = m_aRightEntry;
            pFirstRightBuffer = m_pRightBuffer;
          }
          
          // save first left
          if ( ! aFirstLeftMatch )
            aFirstLeftMatch = m_aLeftEntry;
          
          break;
          
        case KC_BEFORE: // leftKey < rightKey
          if ( ! aFirstRightMatch ) {
#ifdef TRACE_JOIN
            if (AM::bVerbose())
              vTrace("Joining: L(%p) ⋈ R(NULL)\n"_Stz,m_aLeftEntry);
#endif
            vAddRowRefs(m_aLeftEntry, nullptr);
          }
          m_eNextEntry = NE_NEXT_LEFT;
          break;
          
        case KC_AFTER:  // leftKey > rightKey
#ifdef TRACE_JOIN
          if (AM::bVerbose())
            vTrace("Joining: L(NULL) ⋈ R(%p)\n"_Stz,m_aRightEntry);
#endif
          vAddRowRefs(nullptr, m_aRightEntry);
          m_eNextEntry = NE_NEXT_RIGHT;
          break;
          
        default:
          NEVER_REACH;
      }
      
      continue;
    }
    
    // (b) left entry only
    if (m_aLeftEntry) {
      
      // no previous first right ?
      if ( ! aFirstRightMatch ) {
#ifdef TRACE_JOIN
        if (AM::bVerbose())
          vTrace("Joining: L(%p) ⋈ R(NULL)\n"_Stz,m_aLeftEntry);
#endif
        vAddRowRefs(m_aLeftEntry, nullptr);
      }
      
      // get next left
      m_eNextEntry = NE_NEXT_LEFT;
      continue;
    }
    
    // (c) right entry only
    if (m_aRightEntry) {
#ifdef TRACE_JOIN
      if (AM::bVerbose())
        vTrace("Joining: L(NULL) ⋈ R(%p)\n"_Stz,m_aRightEntry);
#endif
      vAddRowRefs(nullptr, m_aRightEntry);
      
      // get next right
      m_eNextEntry = NE_NEXT_RIGHT;
      continue;
    }
    
    // (d) neither
    m_eNextEntry = NE_NEXT_EXIT;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // join completed
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // pipe out the joined buffer
  if (m_pSrtBuffer)
    vPipeOut(m_pSrtBuffer);
  
  if (AM::bVerbose())
    vTrace("JOIN COMPLETE\n"_Stz);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // pipe out an end marker
  vPipeOut_endMarker();
  
  return OR_EXIT;
}

/*----------------------------------------------------------------------------*/
/* pop left operand                                                           */
/*----------------------------------------------------------------------------*/
eOR JoinOp::ePopLeftOperand()
{
  Operand* pLeftOperand = nullptr;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get a right operand
  for (;;) {
    
    pLeftOperand = pPopQueueLeft();
    
    if (! pLeftOperand) {
      ASSERT(bIsLocked());
      return OR_BIDE;
    }
    
    if (CN_STREAMDEF == pLeftOperand->eClass()) {
      StreamDef* pStreamDef = _pStreamDef(pLeftOperand);
      m_pStreamDef->vAdd(pStreamDef);
      delete pStreamDef;
      continue;
    }
    
    if (CN_ENDMARKER == pLeftOperand->eClass()) {
      
      EndMarker* pEndMarker = _pEndMarker(pLeftOperand);
      
      if (--m_nLeftEndMarkers == 0) {
        m_bEndLeftOperands = true;
        delete pEndMarker;
        m_pLeftBuffer = nullptr;
        return OR_NONE;
      }
      else {
        delete pEndMarker;
        continue;
      }
    }
    
    break;
  }
  
  m_pLeftBuffer = _pEntBuffer(pLeftOperand);
  
  return OR_NONE;
}
  
/*----------------------------------------------------------------------------*/
/* pop right operand                                                          */
/*----------------------------------------------------------------------------*/
eOR JoinOp::ePopRightOperand()
{
  Operand* pRightOperand = nullptr;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get a right operand
  for (;;) {
    
    pRightOperand = pPopQueueRight();
    
    if (! pRightOperand) {
      ASSERT(bIsLocked());
      return OR_BIDE;
    }
    
    if (CN_STREAMDEF == pRightOperand->eClass()) {
      StreamDef* pStreamDef = _pStreamDef(pRightOperand);
      m_pStreamDef->vAdd(pStreamDef);
      delete pStreamDef;
      continue;
    }
    
    if (CN_ENDMARKER == pRightOperand->eClass()) {
      EndMarker* pEndMarker = _pEndMarker(pRightOperand);
      
      if (--m_nRightEndMarkers == 0) {
        m_bEndRightOperands = true;
        delete pEndMarker;
        m_pRightBuffer = nullptr;
        return OR_NONE;
      }
      else {
        delete pEndMarker;
        continue;
      }
    }
    
    break;
  }
  
  m_pRightBuffer = _pEntBuffer(pRightOperand);
  
  return OR_NONE;
}

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
JoinOp* DataOperators::_pJoinOp(void* aAddr)
{
  JoinOp* pJoinOp = reinterpret_cast<JoinOp*>(aAddr);
  
  return pJoinOp;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__aFooterFunction(JoinOp);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

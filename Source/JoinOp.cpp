/*============================================================================*/
/* JoinOp.cpp -- Join                                                         */
/*============================================================================*/

#include "PushDown.h"

OpDesc __Op_Join (
   CN_JOINOP,          // class number
   OK_SIMPLE,          // op kind
   "Join"_Stz,         // first word of prompt command
   "Joi"_Stz,          // prefix
   
   JoinOp::pNew,       // function which parses the command and new's an op
   JoinOp::eCall,      // function which calls the op loop
   nullptr,            // function which calls the drain loop
   
   "<help>"_Stz
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
JoinOp::JoinOp(Stz* szOpLabel, Digest* pDigest, OpDefn* aOpDefn, bool bAppR)
: Op(CN_JOINOP,szOpLabel,pDigest,aOpDefn,bAppR)
{
  _precondition_(_MAGIC_,CN_JOINOP);
  
  m_pLeftBuffer  = nullptr;
  m_pRightBuffer = nullptr;
  
  m_aLeftEntry  = nullptr;
  m_aRightEntry = nullptr;
  
  m_bEndLeftOperands  = false;
  m_bEndRightOperands = false;
  
  m_pRefBuffer = new RefBuffer;
  
  m_eNextEntry = NE_NEXT_LEFT_RIGHT;
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
Op* JoinOp::pNew(Session* pSession, Stz* szLabel,
                 Digest* pDigest, OpDefn* aOpDefn, bool bAppR,
                 TokenVec* pTknVec, i8& i_Tkn_)
{
  i_Tkn_++;
  
  JoinOp* pJoinOp = new JoinOp(szLabel,pDigest,aOpDefn,bAppR);
  
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
  
  // allot a refSet
  RefSet* pRefSet = m_pRefBuffer->aAllotRefSet(2);
  
  if ( ! pRefSet )  {
    // Reference Buffer is full
    
    // pipe outt the refBuffer
    vPipeOut(m_pRefBuffer);
    
    // create another refBuffer for the refEnt's
    m_pRefBuffer = new RefBuffer;
    
    // insert row from entry in temporary buffer
    pRefSet = m_pRefBuffer->aAllotRefSet(2);
    RASSERT(pRefSet);
  }
  
  // get the row buffers
  CrwBuffer* pLeftCrwBuffer = aLeftEntry ? CrwBuffer::pRefdCrwBuffer(aLeftEntry->xRef()) : nullptr;
  CrwBuffer* pRightCrwBuffer = aRightEntry ? CrwBuffer::pRefdCrwBuffer(aRightEntry->xRef()) : nullptr;
  
  // set the rowRefs
  Crw* aLeftRowData = aLeftEntry ? pLeftCrwBuffer->pCrw(aLeftEntry->xRef().iItem()) : nullptr;
  Crw* aRightRowData = aRightEntry ? pRightCrwBuffer->pCrw(aRightEntry->xRef().iItem()) : nullptr;
  
  // set the rowData in the refSet
  pRefSet->vRowRef(0, (i8)aLeftRowData);
  pRefSet->vRowRef(1, (i8)aRightRowData);
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
        
        if (m_pLeftBuffer) {
          m_aLeftEntry = m_pLeftBuffer->aNextEntry(m_aLeftEntry, 0);
        }
          
        while ( ! m_aLeftEntry && ! m_bEndLeftOperands) {
          if (OR_BIDE == ePopLeftOperand())
            return OR_BIDE;
          m_aLeftEntry = m_pLeftBuffer ? m_pLeftBuffer->aLeadEntry() : NULL;
        }
          
        if (m_pRightBuffer) {
          m_aRightEntry  = m_pRightBuffer->aNextEntry(m_aRightEntry, 0);
        }
        
        while ( ! m_aRightEntry && ! m_bEndRightOperands) {
          if (OR_BIDE == ePopRightOperand())
            return OR_BIDE;
          m_aRightEntry  = m_pRightBuffer ? m_pRightBuffer->aLeadEntry() : NULL;
        }
        
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
            vTrace("Joining: L(%p) ⋈ R(NULL)\n"_Stz,m_aLeftEntry);
#endif
            vAddRowRefs(m_aLeftEntry, nullptr);
          }
          m_eNextEntry = NE_NEXT_LEFT;
          break;
          
        case KC_AFTER:  // leftKey > rightKey
#ifdef TRACE_JOIN
          vTrace("Joining: L(NULL) ⋈ R(%p)\n"_Stz,m_aRightEntry);
#endif
          vAddRowRefs(m_aLeftEntry, m_aRightEntry);
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
  vPipeOut(m_pRefBuffer);
  
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
      continue;
    }
    
    if (CN_ENDMARKER == pLeftOperand->eClass()) {
      EndMarker* pEndMarker = _pEndMarker(pLeftOperand);
      m_bEndLeftOperands = true;
      delete pEndMarker;
      m_pLeftBuffer = nullptr;
      return OR_NONE;
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
      continue;
    }
    
    if (CN_ENDMARKER == pRightOperand->eClass()) {
      EndMarker* pEndMarker = _pEndMarker(pRightOperand);
      m_bEndRightOperands = true;
      delete pEndMarker;
      m_pRightBuffer = nullptr;
      return OR_NONE;
    }
    
    break;
  }
  
  m_pRightBuffer = _pEntBuffer(pRightOperand);
  
  return OR_NONE;
}

/*----------------------------------------------------------------------------*/
/* index join                                                                 */
/*----------------------------------------------------------------------------*/
void JoinOp::vIndexJoin(EntBuffer* pLeftBuffer, EntBuffer* pRightBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOINOP,_FOOTER_);

  
  vTrace("Joined\n"_Stz);
  
  
  vTrace("\n"_Stz);
  vTrace("=================================================="
         "==================================================\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
JoinOp* PushDown::_pJoinOp(void* aAddr)
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

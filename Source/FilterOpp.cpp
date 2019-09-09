/*============================================================================*/
/* FilterOp.cpp -- Where                                                       */
/*============================================================================*/

#include "DataOperators.h"

OpDesc __Op_FilterOp (
  CN_FILTEROP,          // class number
  OK_SIMPLE,            // op kind
  "Filter"_Stz,         // first word of prompt command
  "Ftr"_Stz,            // prefix
  true,                 // partitioned
  FilterOp::pNew,       // function which parses the command and new's an op
  FilterOp::eCall,      // function which calls the op loop
  FilterOp::eDrain,     // function which calls the drain loop
  
  "<help>"_Stz
);

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
FilterOp* DataOperators::_pFilterOp(void* aAddr)
{
  FilterOp* pFilterOp = reinterpret_cast<FilterOp*>(aAddr);
  
  return pFilterOp;
}

/*----------------------------------------------------------------------------*/
/* new                                                                        */
/*----------------------------------------------------------------------------*/
void* FilterOp::operator new(size_t)
{
  FilterOp* pFilterOp =
    static_cast<FilterOp*>(HeapMem::pAllocate(CN_FILTEROP));
  
  return pFilterOp;
}

/*----------------------------------------------------------------------------*/
/* delete                                                                     */
/*----------------------------------------------------------------------------*/
void FilterOp::operator delete(void* aAddress)
{
  FilterOp* pFilterOp = static_cast<FilterOp*>(aAddress);
  
  HeapMem::vFree(pFilterOp);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
FilterOp::FilterOp(OpSpec* pOpSpec, Digest* pDigest, Stz* szOpLabel, OpDefn* aOpDefn, bool bAppR)
: Op(CN_FILTEROP,aOpDefn,pDigest,szOpLabel,bAppR,pOpSpec)
{
  _precondition_(_MAGIC_,CN_FILTEROP);
  
  m_pCalculator = new Calculator;
  m_pIrwBuffer  = new IrwBuffer;
  
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
FilterOp::~FilterOp(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_FILTEROP);
  
  delete m_pCalculator;
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* parse & new                                                                */
/*----------------------------------------------------------------------------*/
Op* FilterOp::pNew(OpSpec* pOpSpec, Digest* pDigest,
                   Stz* szOpLabel, OpDefn* aOpDefn, bool bAppR,
                   TokenVec* pTknVec, i8& i_Tkn_, i8 i_Slot)
{
  i_Tkn_++;
  
  FilterOp* pFilterOp = new FilterOp(pOpSpec,pDigest,szOpLabel,aOpDefn,bAppR);
  pFilterOp->m_iStartTkn = i_Tkn_;
  pFilterOp->m_pTokenVec = pTknVec;
  
  while ( ! pTknVec->pGet(i_Tkn_)->bSame("|"_Stz) ) {
    i_Tkn_++;
  }
  pFilterOp->m_iEndTkn = i_Tkn_ - 1;
  
  pFilterOp->m_nSlot = i_Slot;
  
  return pFilterOp;
}

/*----------------------------------------------------------------------------*/
/* call loop                                                                  */
/*----------------------------------------------------------------------------*/
eOR FilterOp::eCall(void* aThisOp)
{
  eOR eCallAgain = ((FilterOp*)aThisOp)->eOpLoop();
  
  return eCallAgain;
}

/*----------------------------------------------------------------------------*/
/* call drain loop                                                            */
/*----------------------------------------------------------------------------*/
eOR FilterOp::eDrain(void* aThisOp)
{
  eOR eCallAgain = ((FilterOp*)aThisOp)->eDrainLoop();
  
  return eCallAgain;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* op loop                                                                    */
/*----------------------------------------------------------------------------*/
eOR FilterOp::eOpLoop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FILTEROP,_FOOTER_);
  
  for (;;) {
    
    Operand* pOperand = pPopQueue();
    
    if (! pOperand) {
      ASSERT(bIsLocked());
      return OR_BIDE;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    switch (pOperand->eClass()) {
        
      case CN_ENDMARKER: {
        if (--m_nLeftEndMarkers == 0) {
          if (m_pIrwBuffer)
            vPipeOut(m_pIrwBuffer);
          vPipeOut(pOperand);
          goto L_EXIT;
        }
        else
          delete (EndMarker*)pOperand;
        break;
      }
        
        // -- -- -- -- -- --
        
      case CN_CRWBUFFER: {
        CrwBuffer* pCrwBuffer = _pCrwBuffer(pOperand);
        i8 nRows = pCrwBuffer->nRowCount();
        
        HV_Row hRow(m_pStreamDef->pColumnVec(0));
        
        for (i8 i_Row = 0; i_Row < nRows; i_Row++) {

          pCrwBuffer->vGetRow(hRow, i_Row);
          
          i8 iRes = m_pCalculator->iEvaluate(hRow);
          
          if (iRes) {
            Row* pNewRow = m_pIrwBuffer->aAllotRow(m_pStreamDef->pColumnVec(0));
            
            if ( ! pNewRow ) {
              vPipeOut(m_pIrwBuffer);
              m_pIrwBuffer = new IrwBuffer;
              pNewRow = m_pIrwBuffer->aAllotRow(m_pStreamDef->pColumnVec(0));
            }
            pNewRow->vCopy(hRow);
          }
          
        }
        break;
      }
        
      // -- -- -- -- -- --
        
      case CN_ENTBUFFER: {
        EntBuffer* pEntBuffer = _pEntBuffer(pOperand);
        i8 nRows = pEntBuffer->nItems();
        
        HV_Row hRow(m_pStreamDef->pColumnVec(0));
        
        Ent* aEnt = pEntBuffer->aLeadEntry();
        
        while (aEnt) {
          
          CrwBuffer* pCrwBuffer = CrwBuffer::pRefdCrwBuffer(aEnt->xRef());
          Crw* aRowData = pCrwBuffer->pCrw(aEnt->xRef().iItem());
          RowInflator::vInflateRow(hRow,aRowData);
          
          i8 iRes = m_pCalculator->iEvaluate(hRow);
          
          if (iRes) {
            Row* pNewRow = m_pIrwBuffer->aAllotRow(m_pStreamDef->pColumnVec(0));
            
            if ( ! pNewRow ) {
              vPipeOut(m_pIrwBuffer);
              m_pIrwBuffer = new IrwBuffer;
              pNewRow = m_pIrwBuffer->aAllotRow(m_pStreamDef->pColumnVec(0));
            }
            pNewRow->vCopy(hRow);
          }
          
          aEnt = pEntBuffer->aNextEntry(aEnt, 0);
        }
        
        break;
      }
        
        
      // -- -- -- -- -- --
        
      case CN_IRWBUFFER: {
        IrwBuffer* pIrwBuffer = _pIrwBuffer(pOperand);
        
        Row* pRow = pIrwBuffer->aFirstRow();
        while (pRow) {
          i8 iRes = m_pCalculator->iEvaluate(pRow);
          if (iRes) {
            Row* pNewRow = m_pIrwBuffer->aAllotRow(m_pStreamDef->pColumnVec(0));
            
            if ( ! pNewRow ) {
              vPipeOut(m_pIrwBuffer);
              m_pIrwBuffer = new IrwBuffer;
              pNewRow = m_pIrwBuffer->aAllotRow(m_pStreamDef->pColumnVec(0));
            }
            pNewRow->vCopy(pRow);
          }
          pRow = pIrwBuffer->aNextRow(pRow);
        }
        delete pIrwBuffer;
        break;
      }
        
      case CN_SRTBUFFER: {
        SrtBuffer* pSrtBuffer = _pSrtBuffer(pOperand);
        delete pSrtBuffer;
        RASSERT(0);
        break;
      }
        
        // -- -- -- -- -- --
        
      case CN_STREAMDEF: {
        StreamDef* pStreamDef = _pStreamDef(pOperand) ;
        m_pStreamDef = new StreamDef(pStreamDef);
        bool bResult = m_pCalculator->bCompile(m_pTokenVec, m_pStreamDef, m_iStartTkn, m_iEndTkn);
        vPipeOut(pOperand);
        
        break;
      }
        
        // -- -- -- -- -- --
        
      default:
        RASSERT(0);
        break;
    }
    
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
L_EXIT:
  return OR_EXIT;
}

/*----------------------------------------------------------------------------*/
/* drain loop                                                                 */
/*----------------------------------------------------------------------------*/
eOR FilterOp::eDrainLoop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FILTEROP,_FOOTER_);
  
  for (;;) {
    
    Operand* pOperand = pPopQueue();
    
    if (! pOperand) {
      ASSERT(bIsLocked());
      return OR_BIDE;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    switch (pOperand->eClass()) {
        
      case CN_ENDMARKER:
        vPipeOut(pOperand);
        goto L_EXIT;
        
        // -- -- -- -- -- --
        
      case CN_PAUSEMARKER:
        vPipeOut(pOperand);
        break;
        
        // -- -- -- -- -- --
        
      case CN_INTBUFFER:
        //vPipeOut(pOperand);
        break;
        
      default:
        break;
    }
    
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
L_EXIT:
  return OR_EXIT;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__aFooterFunction(FilterOp);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

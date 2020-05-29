/*============================================================================*/
/* FilterOp.cpp -- Where                                                       */
/*============================================================================*/

#include "DataOperators.h"

OpDesc __Op_FilterOp (
  CN_FILTEROP,          // class number
  OK_SINGLE,            // op kind
  "Filter"_Stz,         // first word of prompt command
  "Ftr"_Stz,            // prefix
                      
  true,                 // partitioned
  false,                // compiler op

  true,                 // accepts CrwBuffer?
  true,                 // accepts EntBuffer?
  true,                 // accepts IrwBuffer?
  false,                // accepts SrtBuffer?
                      
  false,                // accepts CrwBuffer?
  false,                // accepts EntBuffer?
  true,                 // accepts IrwBuffer?
  false,                // accepts SrtBuffer?
                      
  CN_NONE,              // what kind of output
                      
  FilterOp::pNew,       // function which parses the command and new's an op
  FilterOp::eCall,      // function which calls the op loop
  FilterOp::eDrain,     // function which calls the drain loop
  
  "filter <rpnExp>"_Stz
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
FilterOp::FilterOp(OpSpec* pOpSpec, Pipeline* pPipeline)
: Op(CN_FILTEROP,pOpSpec,pPipeline)
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
Op* FilterOp::pNew(OpSpec* pOpSpec, Pipeline* pPipeline)
{
  FilterOp* pFilterOp = new FilterOp(pOpSpec,pPipeline);
  
  RASSERT(0);
//  i_Tkn_++;
//  EnameSet* pEnameSet = pFilterOp->m_pOpSpec->pEnameSet();
//  pEnameSet->vPrint();
//
//  pFilterOp->m_iStartTkn = i_Tkn_;
//  pFilterOp->m_pTokenVec = pTknVec;
//
//  // gather the filter expression as enmPairs
//  s4K sExpr = ""_Stz;
//  while (! pTknVec->pGet(i_Tkn_)->bSame("|"_Stz)) {
//    sExpr.vConcat(pTknVec->pGet(i_Tkn_++)->szText());
//    sExpr.vConcat(" "_Stz);
//  }
//
//  // add ename pair for expression
//  pFilterOp->m_pOpSpec->vAddEnamePair("Filter"_Stz,
//                                    sExpr,
//                                    TN_STRING);
//
//  pFilterOp->m_iEndTkn = i_Tkn_ - 1;
//
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
        
        // create a column vec from each row ref
        ColumnVec* pColumnVec = new ColumnVec;
        m_pStreamDef->vAddColumns(pColumnVec);
        
        HV_Row hRow(pColumnVec);
        
        for (i8 i_Row = 0; i_Row < nRows; i_Row++) {

          pCrwBuffer->vGetRow(hRow, i_Row);
          
          i8 iRes = m_pCalculator->iEvaluate(hRow);
          
          if (iRes) {
            Row* pNewRow = m_pIrwBuffer->aAllotRow(pColumnVec);
            
            if ( ! pNewRow ) {
              vPipeOut(m_pIrwBuffer);
              m_pIrwBuffer = new IrwBuffer;
              pNewRow = m_pIrwBuffer->aAllotRow(pColumnVec);
            }
            pNewRow->vCopy(hRow);
          }
          
        }
        break;
      }
        
      // -- -- -- -- -- --
        
      case CN_ENTBUFFER: {
        EntBuffer* pEntBuffer = _pEntBuffer(pOperand);
        
        // create a column vec from each row ref
        ColumnVec* pColumnVec = new ColumnVec;
        m_pStreamDef->vAddColumns(pColumnVec);
        
        HV_Row hRow(pColumnVec);
        
        Ent* aEnt = pEntBuffer->aLeadEntry();
        
        while (aEnt) {
          
          CrwBuffer* pCrwBuffer = CrwBuffer::pRefdCrwBuffer(aEnt->xRef());
          Crw* aRowData = pCrwBuffer->pCrw(aEnt->xRef().iItem());
          RowInflator::vInflateRow(hRow,aRowData);
          
          i8 iRes = m_pCalculator->iEvaluate(hRow);
          
          if (iRes) {
            Row* pNewRow = m_pIrwBuffer->aAllotRow(pColumnVec);
            
            if ( ! pNewRow ) {
              vPipeOut(m_pIrwBuffer);
              m_pIrwBuffer = new IrwBuffer;
              pNewRow = m_pIrwBuffer->aAllotRow(pColumnVec);
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
        
        // create a column vec from each row ref
        ColumnVec* pColumnVec = new ColumnVec;
        m_pStreamDef->vAddColumns(pColumnVec);
        
        Row* pRow = pIrwBuffer->aFirstRow();
        while (pRow) {
          i8 iRes = m_pCalculator->iEvaluate(pRow);
          if (iRes) {
            Row* pNewRow = m_pIrwBuffer->aAllotRow(pColumnVec);
            
            if ( ! pNewRow ) {
              vPipeOut(m_pIrwBuffer);
              m_pIrwBuffer = new IrwBuffer;
              pNewRow = m_pIrwBuffer->aAllotRow(pColumnVec);
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

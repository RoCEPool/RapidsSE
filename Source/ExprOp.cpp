/*============================================================================*/
/* ExprOp.cpp -- Extression                                                   */
/*============================================================================*/

#include "DataOperators.h"

OpDesc __Op_Expr (
   CN_EXPROP,        // class number
   OK_SIMPLE,        // op kind
   "Expr"_Stz,       // first word of prompt command
   "Exp"_Stz,       // prefix
   true,             // partitioned
   ExprOp::pNew,     // function which parses the command and new's an op
   ExprOp::eCall,    // function which calls the op loop
   ExprOp::eDrain,   // function which calls the drain loop
   
   "<help>"_Stz
);

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
ExprOp* DataOperators::_pExprOp(void* aAddr)
{
  ExprOp* pExprOp = reinterpret_cast<ExprOp*>(aAddr);
  
  return pExprOp;
}

/*----------------------------------------------------------------------------*/
/* new                                                                        */
/*----------------------------------------------------------------------------*/
void* ExprOp::operator new(size_t)
{
  ExprOp* pExprOp =
  static_cast<ExprOp*>(HeapMem::pAllocate(CN_COUNTOP));
  
  return pExprOp;
}

/*----------------------------------------------------------------------------*/
/* delete                                                                     */
/*----------------------------------------------------------------------------*/
void ExprOp::operator delete(void* aAddress)
{
  ExprOp* pExprOp = static_cast<ExprOp*>(aAddress);
  
  HeapMem::vFree(pExprOp);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ExprOp::ExprOp(Stz* szOpLabel, Digest* pDigest, OpDefn* aOpDefn, bool bAppR, OpSpec* pOpSpec)
: Op(CN_EXPROP,szOpLabel,pDigest,aOpDefn,bAppR,pOpSpec)
{
  _precondition_(_MAGIC_,CN_EXPROP);
  
  m_fpCall     = ExprOp::eCall;
  m_pStreamDef = new StreamDef;
  
  m_pOriginalStreamDef = nullptr;
  m_pCalculator        = new Calculator;
  m_pIrwBuffer         = new IrwBuffer;
  
  for (i8 i_ColVec = 0; i_ColVec < C_MAX_COLUMNVECS; i_ColVec++)
    m_iWhichVec[i_ColVec] = C_UNKNOWN;
  
  for (i8 i_Col = 0; i_Col < C_MAX_COLUMNS; i_Col++)
    m_iWhichCol[i_Col] = C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
ExprOp::~ExprOp(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_EXPROP);
  
  delete m_pOriginalStreamDef;
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* parse & new                                                                */
/*----------------------------------------------------------------------------*/
Op* ExprOp::pNew(Stz* szLabel, Digest* pDigest,
                 OpDefn* aOpDefn, bool bAppR, OpSpec* pOpSpec,
                 TokenVec* pTknVec, i8& i_Tkn_, i8 i_Slot)
{
  i_Tkn_++;
  ASSERT(i_Slot != -1);
  
  ExprOp* pExprOp = new ExprOp(szLabel,pDigest,aOpDefn,bAppR);
  
  // create a columnVec in the stream def
  pExprOp->m_pStreamDef->m_oColVecAry.vItems(1);
  ColumnVec* pColVec = pExprOp->m_pStreamDef->pColumnVec(0);
  
  // add columns to the columnVec
  i8 i_Column = 0;
  pExprOp->m_iStartTkn = i_Tkn_;
  pExprOp->m_pTokenVec = pTknVec;
  while ( ! pTknVec->pGet(i_Tkn_)->bSame("|"_Stz) ) {
    
    if ( pTknVec->pGet(i_Tkn_)->bSame(","_Stz)) {
      pExprOp->m_iEndTkn = ++i_Tkn_;
      i_Tkn_++;
    }
      
    if ( ! pTknVec->pGet(i_Tkn_)->bIsName()) {
      i_Tkn_++;
      continue;
    }
    
    // create a column
    Column* pColumn = new Column();
    pColumn->vColumnNo(i_Column);
    pColumn->vColumnName(pTknVec->pGet(i_Tkn_++)->szText());
    pColVec->vAdd(pColumn);
    
    i_Column++;
      
  }
  pExprOp->m_iEndTkn = ++i_Tkn_;
  return pExprOp;
}

/*----------------------------------------------------------------------------*/
/* call loop                                                                  */
/*----------------------------------------------------------------------------*/
eOR ExprOp::eCall(void* aThisOp)
{
  eOR eCallAgain = ((ExprOp*)aThisOp)->eOpLoop();
  
  return eCallAgain;
}

/*----------------------------------------------------------------------------*/
/* call drain loop                                                            */
/*----------------------------------------------------------------------------*/
eOR ExprOp::eDrain(void* aThisOp)
{
  eOR eCallAgain = ((ExprOp*)aThisOp)->eDrainLoop();
  
  return eCallAgain;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* op loop                                                                    */
/*----------------------------------------------------------------------------*/
eOR ExprOp::eOpLoop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_EXPROP,_FOOTER_);
  
  for (;;) {
    
    Operand* pOperand = pPopQueue();
    
    if (! pOperand) {
      ASSERT(bIsLocked());
      return OR_BIDE;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    switch (pOperand->eClass()) {
        
      case CN_ENDMARKER:
        if (--m_nLeftEndMarkers == 0) {
          vPipeOut(m_pIrwBuffer);
          vPipeOut(pOperand);
          goto L_EXIT;
        }
        else
          delete (EndMarker*)pOperand;
        
        break;
        
      // -- -- -- -- -- --
        
      case CN_CRWBUFFER: {
        CrwBuffer* pCrwBuffer = _pCrwBuffer(pOperand) ;
        ColumnVec* pColumnVec = m_pStreamDef->m_oColVecAry.pGet(0);
        
        RASSERT(m_pOriginalStreamDef->m_oColVecAry.nItems() == 1);
        
        HV_Row hRow(m_pOriginalStreamDef->m_oColVecAry.pGet(0));
        i8 nRows = pCrwBuffer->nRowCount();
        
        for (i8 i_Row = 0; i_Row < nRows; i_Row++) {
          
          Row* pNewRow = m_pIrwBuffer->aAllotRow(m_pStreamDef->pColumnVec(0));
          
          if ( ! pNewRow ) {
            vPipeOut(m_pIrwBuffer);
            m_pIrwBuffer = new IrwBuffer;
            pNewRow = m_pIrwBuffer->aAllotRow(m_pStreamDef->pColumnVec(0));
          }
          
          pCrwBuffer->vGetRow(hRow, i_Row);
          
          i8 iRes = m_pCalculator->iEvaluate(hRow);
          pNewRow->m_waColVal[0].m_iInt = iRes;
        }
        
        break;
      }
        
      // -- -- -- -- -- --
        
      case CN_ENTBUFFER: {
        EntBuffer* pEntBuffer = _pEntBuffer(pOperand) ;
        ColumnVec* pColumnVec = m_pStreamDef->m_oColVecAry.pGet(0);
        
        RASSERT(m_pOriginalStreamDef->m_oColVecAry.nItems() == 1);
        
        Ent* aEnt = pEntBuffer->aLeadEntry();
        while (aEnt) {
          Row* pRow = m_pIrwBuffer->aAllotRow(m_pStreamDef->pColumnVec(0));
          
          if ( ! pRow ) {
            vPipeOut(m_pIrwBuffer);
            m_pIrwBuffer = new IrwBuffer;
            pRow = m_pIrwBuffer->aAllotRow(m_pStreamDef->pColumnVec(0));
          }
          
          CrwBuffer* pCrwBuffer = CrwBuffer::pRefdCrwBuffer(aEnt->xRef());
          Crw* aRowData = pCrwBuffer->pCrw(aEnt->xRef().iItem());
          
          for (i8 i_IntoCol = 0; i_IntoCol < pColumnVec->nItems(); i_IntoCol++) {
            i8 iBaseOffset = RowCoder::iBaseOffset(m_pOriginalStreamDef->m_oColVecAry.pGet(m_iWhichVec[i_IntoCol]));
                                                   
            RowInflator::vInflateColumn(pRow, i_IntoCol, aRowData, iBaseOffset, m_iWhichCol[i_IntoCol],
                                        m_pOriginalStreamDef->m_oColVecAry.pGet(m_iWhichVec[i_IntoCol]));
            
          }
          
          aEnt = pEntBuffer->aNextEntry(aEnt,0);
        }
        
        break;
      }
      
      // -- -- -- -- -- --
        
      case CN_IRWBUFFER: {
        IrwBuffer* pIrwBuffer = _pIrwBuffer(pOperand) ;
        ColumnVec* pColumnVec = m_pStreamDef->m_oColVecAry.pGet(0);
        
        RASSERT(m_pOriginalStreamDef->m_oColVecAry.nItems() == 1);
        
        Row* pIrwRow = pIrwBuffer->aFirstRow();
        while (pIrwRow) {
          Row* pRow = m_pIrwBuffer->aAllotRow(m_pStreamDef->pColumnVec(0));
          
          if ( ! pRow ) {
            vPipeOut(m_pIrwBuffer);
            m_pIrwBuffer = new IrwBuffer;
            pRow = m_pIrwBuffer->aAllotRow(m_pStreamDef->pColumnVec(0));
          }
          
          for (i8 i_IntoCol = 0; i_IntoCol < pColumnVec->nItems(); i_IntoCol++) {
            pRow->vCopy(pIrwRow, m_iWhichCol[i_IntoCol], i_IntoCol);
          }
          
          pIrwRow = pIrwBuffer->aNextRow(pIrwRow);
        }
        
        break;
      }
        
      // -- -- -- -- -- --
        
      case CN_SRTBUFFER: {
        SrtBuffer* pSrtBuffer = _pSrtBuffer(pOperand);
        ColumnVec* pColumnVec = m_pStreamDef->m_oColVecAry.pGet(0);
        
        SrtSet* aSrtSet = pSrtBuffer->aFirstSrtSet();
        while (aSrtSet) {
          Row* pRow = m_pIrwBuffer->aAllotRow(m_pStreamDef->pColumnVec(0));
          
          if ( ! pRow ) {
            vPipeOut(m_pIrwBuffer);
            m_pIrwBuffer = new IrwBuffer;
            pRow = m_pIrwBuffer->aAllotRow(m_pStreamDef->pColumnVec(0));
          }
          
          for (i8 i_IntoCol = 0; i_IntoCol < pColumnVec->nItems(); i_IntoCol++) {
            
            Crw* aRowData = (Crw*)(aSrtSet->iRowRef(m_iWhichVec[i_IntoCol]));
            
            if (aRowData) {
              i8 iBaseOffset = RowCoder::iBaseOffset(m_pOriginalStreamDef->m_oColVecAry.pGet(m_iWhichVec[i_IntoCol]));
              
              RowInflator::vInflateColumn(pRow, i_IntoCol, aRowData, iBaseOffset, m_iWhichCol[i_IntoCol],
                                          m_pOriginalStreamDef->m_oColVecAry.pGet(m_iWhichVec[i_IntoCol]));
            }
            else
              pRow->m_waColVal[i_IntoCol] = C_DATUM_NULL;
          }
          
          aSrtSet = pSrtBuffer->aNextSrtSet(aSrtSet);
        }
        
        delete pSrtBuffer;
        break;
      }
      
      // -- -- -- -- -- --
        
      case CN_STREAMDEF: {
      
        StreamDef* pStreamDef = _pStreamDef(pOperand);
        
        // save the original stream def
        m_pOriginalStreamDef = new StreamDef(pStreamDef);
        
        // update the streamDef of this project Op
        vUpdateStreamDef();
        
        bool bResult = m_pCalculator->bCompile(m_pTokenVec, m_pStreamDef, m_iStartTkn, m_iEndTkn);
        
        // pipe out the streamdef
        vPipeOut(new StreamDef(m_pStreamDef));
        
        delete pStreamDef;
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
eOR ExprOp::eDrainLoop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_EXPROP,_FOOTER_);
  
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
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* update the stream def columns of the project op                            */
/*----------------------------------------------------------------------------*/
void ExprOp::vUpdateStreamDef(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_EXPROP,_FOOTER_);
  RASSERT(m_pStreamDef->m_oColVecAry.nItems() == 1);
  ColumnVec* pExprColumnVec = m_pStreamDef->m_oColVecAry.pGet(0);
  i8 nExprColumns = pExprColumnVec->nItems();
  
  // Find the expression col in the streamDef's that came in
  
  for (i8 i_ExprCol = 0; i_ExprCol < nExprColumns; i_ExprCol++) {
    
    // find the project column in the original Stream
    Column* pExprCol = pExprColumnVec->pGet(i_ExprCol);
    m_iWhichCol[i_ExprCol] =
    m_pOriginalStreamDef->iFindColumn(pExprCol->sColumnName().aChar(),m_iWhichVec[i_ExprCol]);
    
    // set the column and datum type in the project column
    ColumnVec* pColumnVec = m_pOriginalStreamDef->m_oColVecAry.pGet(m_iWhichVec[i_ExprCol]);
    Column* pCol = pColumnVec->pGet(m_iWhichCol[i_ExprCol]);
    pExprCol->vColumnType(pCol->eColumnType());
    pExprCol->vDatumType(pCol->eDatumType());
  }
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__aFooterFunction(ExprOp);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

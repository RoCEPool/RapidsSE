/*============================================================================*/
/* ExprOp.cpp -- Extression                                                   */
/*============================================================================*/

#include "DataOperators.h"

OpDesc __Op_Expr (
  CN_EXPROP,        // class number
  OK_SINGLE,        // op kind
  "Expr"_Stz,       // first word of prompt command
  "Exp"_Stz,        // prefix
                
  true,             // partitioned
  false,            // compiler op
                  
  true,             // accepts CrwBuffer?
  true,             // accepts EntBuffer?
  true,             // accepts IrwBuffer?
  true,             // accepts SrtBuffer?
                  
  false,            // accepts CrwBuffer?
  false,            // accepts EntBuffer?
  true,             // accepts IrwBuffer?
  false,            // accepts SrtBuffer?

  CN_NONE,          // what kind of output
                
  ExprOp::pNew,     // function which parses the command and new's an op
  ExprOp::eCall,    // function which calls the op loop
  ExprOp::eDrain,   // function which calls the drain loop

  "expr <rpnExp>"_Stz
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
  static_cast<ExprOp*>(HeapMem::pAllocate(CN_EXPROP));
  
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
ExprOp::ExprOp(OpSpec* pOpSpec, Pipeline* pPipeline)
: Op(CN_EXPROP,pOpSpec,pPipeline)
{
  _precondition_(_MAGIC_,CN_EXPROP);
  
  m_fpCall     = ExprOp::eCall;
  m_pStreamDef = new StreamDef;
  
  m_pOriginalStreamDef = nullptr;
  m_pIrwBuffer         = new IrwBuffer;
  m_pTknVec            = nullptr;
  m_iTkn               = C_UNKNOWN;
  
  for (i8 i_Col = 0; i_Col < C_MAX_COLUMNS; i_Col++) {
    m_iWhichCol[i_Col] = C_UNKNOWN;
    m_iWhichVec[i_Col] = C_UNKNOWN;
    m_pCalculator[i_Col] = nullptr;
  }
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
Op* ExprOp::pNew(OpSpec* pOpSpec, Pipeline* pPipeline)
{
  ExprOp* pExprOp = new ExprOp(pOpSpec,pPipeline);
  
  RASSERT(0);
//  i_Tkn_++;
//  pExprOp->m_iTkn = i_Tkn_;
//  pExprOp->m_pTknVec = pTknVec;
//
//  s4K sExpr = ""_Stz;
//  i8 iExpr = 0;
//
//  // gather the expressions as enmPairs
//  while (true) {
//
//    while ( ! pTknVec->pGet(i_Tkn_)->bSame(","_Stz)  &&
//            ! pTknVec->pGet(i_Tkn_)->bSame("|"_Stz)) {
//      sExpr.vConcat(pTknVec->pGet(i_Tkn_++)->szText());
//      sExpr.vConcat(" "_Stz);
//    }
//    s32 sExprNm;
//    StrLib::vSprint(sExprNm,"Expr%lld"_Stz,iExpr++);
//
//    // add ename pair for expression
//    pExprOp->m_pOpSpec->vAddEnamePair(sExprNm,
//                                     sExpr,
//                                     TN_STRING);
//
//    if (pTknVec->pGet(i_Tkn_)->bSame("|"_Stz))
//      break;
//
//    sExpr=""_Stz;
//    i_Tkn_++;
//  }
  
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
        
// ALKA_TO_REVIEW
//        CrwBuffer* pCrwBuffer = _pCrwBuffer(pOperand) ;
//        ColumnVec* pColumnVec = m_pStreamDef->m_oColVecAry.pGet(0);
//
//        RASSERT(m_pOriginalStreamDef->m_oColVecAry.nItems() == 1);
//
//        HV_Row hRow(m_pOriginalStreamDef->m_oColVecAry.pGet(0));
//        i8 nRows = pCrwBuffer->nRowCount();
//
//        for (i8 i_Row = 0; i_Row < nRows; i_Row++) {
//
//          Row* pNewRow = m_pIrwBuffer->aAllotRow(m_pStreamDef->pColumnVec(0));
//
//          if ( ! pNewRow ) {
//            vPipeOut(m_pIrwBuffer);
//            m_pIrwBuffer = new IrwBuffer;
//            pNewRow = m_pIrwBuffer->aAllotRow(m_pStreamDef->pColumnVec(0));
//          }
//
//          pCrwBuffer->vGetRow(hRow, i_Row);
//
//          for (i8 i_Col = 0; i_Col < m_pStreamDef->pColumnVec(0)->nItems(); i_Col++) {
//            i8 iRes = m_pCalculator[i_Col]->iEvaluate(hRow);
//            pNewRow->m_waColVal[i_Col].m_iInt = iRes;
//          }
//        }
//
        break;
      }
        
      // -- -- -- -- -- --
        
      case CN_ENTBUFFER: {
// ALKA_TO_REVIEW
//        EntBuffer* pEntBuffer = _pEntBuffer(pOperand) ;
//        ColumnVec* pColumnVec = m_pStreamDef->m_oColVecAry.pGet(0);
//
//        RASSERT(m_pOriginalStreamDef->m_oColVecAry.nItems() == 1);
//
//        Ent* aEnt = pEntBuffer->aLeadEntry();
//        while (aEnt) {
//          Row* pRow = m_pIrwBuffer->aAllotRow(m_pStreamDef->pColumnVec(0));
//
//          if ( ! pRow ) {
//            vPipeOut(m_pIrwBuffer);
//            m_pIrwBuffer = new IrwBuffer;
//            pRow = m_pIrwBuffer->aAllotRow(m_pStreamDef->pColumnVec(0));
//          }
//
//          CrwBuffer* pCrwBuffer = CrwBuffer::pRefdCrwBuffer(aEnt->xRef());
//          Crw* aRowData = pCrwBuffer->pCrw(aEnt->xRef().iItem());
//
//          for (i8 i_IntoCol = 0; i_IntoCol < pColumnVec->nItems(); i_IntoCol++) {
//            i8 iBaseOffset = RowCoder::iBaseOffset(m_pOriginalStreamDef->m_oColVecAry.pGet(m_iWhichVec[i_IntoCol]));
//
//            RowInflator::vInflateColumn(pRow, i_IntoCol, aRowData, iBaseOffset, m_iWhichCol[i_IntoCol],
//                                        m_pOriginalStreamDef->m_oColVecAry.pGet(m_iWhichVec[i_IntoCol]));
//
//          }
//
//          aEnt = pEntBuffer->aNextEntry(aEnt,0);
//        }
        
        break;
      }
      
      // -- -- -- -- -- --
        
      case CN_IRWBUFFER: {
// ALKA_TO_REVIEW
//        IrwBuffer* pIrwBuffer = _pIrwBuffer(pOperand) ;
//        ColumnVec* pColumnVec = m_pStreamDef->m_oColVecAry.pGet(0);
//
//        RASSERT(m_pOriginalStreamDef->m_oColVecAry.nItems() == 1);
//
//        Row* pIrwRow = pIrwBuffer->aFirstRow();
//        while (pIrwRow) {
//          Row* pRow = m_pIrwBuffer->aAllotRow(m_pStreamDef->pColumnVec(0));
//
//          if ( ! pRow ) {
//            vPipeOut(m_pIrwBuffer);
//            m_pIrwBuffer = new IrwBuffer;
//            pRow = m_pIrwBuffer->aAllotRow(m_pStreamDef->pColumnVec(0));
//          }
//
//          for (i8 i_IntoCol = 0; i_IntoCol < pColumnVec->nItems(); i_IntoCol++) {
//            pRow->vCopy(pIrwRow, m_iWhichCol[i_IntoCol], i_IntoCol);
//          }
//
//          pIrwRow = pIrwBuffer->aNextRow(pIrwRow);
//        }
        
        break;
      }
        
      // -- -- -- -- -- --
        
      case CN_SRTBUFFER: {
// ALKA_TO_REVIEW
//        SrtBuffer* pSrtBuffer = _pSrtBuffer(pOperand);
//        ColumnVec* pColumnVec = m_pStreamDef->m_oColVecAry.pGet(0);
//
//        SrtSet* aSrtSet = pSrtBuffer->aFirstSrtSet();
//        while (aSrtSet) {
//          Row* pRow = m_pIrwBuffer->aAllotRow(m_pStreamDef->pColumnVec(0));
//
//          if ( ! pRow ) {
//            vPipeOut(m_pIrwBuffer);
//            m_pIrwBuffer = new IrwBuffer;
//            pRow = m_pIrwBuffer->aAllotRow(m_pStreamDef->pColumnVec(0));
//          }
//
//          for (i8 i_IntoCol = 0; i_IntoCol < pColumnVec->nItems(); i_IntoCol++) {
//
//            Crw* aRowData = (Crw*)(aSrtSet->iRowRef(m_iWhichVec[i_IntoCol]));
//
//            if (aRowData) {
//              i8 iBaseOffset = RowCoder::iBaseOffset(m_pOriginalStreamDef->m_oColVecAry.pGet(m_iWhichVec[i_IntoCol]));
//
//              RowInflator::vInflateColumn(pRow, i_IntoCol, aRowData, iBaseOffset, m_iWhichCol[i_IntoCol],
//                                          m_pOriginalStreamDef->m_oColVecAry.pGet(m_iWhichVec[i_IntoCol]));
//            }
//            else
//              pRow->m_waColVal[i_IntoCol] = C_DATUM_NULL;
//          }
//
//          aSrtSet = pSrtBuffer->aNextSrtSet(aSrtSet);
//        }
//
//        delete pSrtBuffer;
        break;
      }
      
      // -- -- -- -- -- --
        
      case CN_STREAMDEF: {
      
        StreamDef* pStreamDef = _pStreamDef(pOperand);
        
        // save the original stream def
        m_pOriginalStreamDef = new StreamDef(pStreamDef);
        
        // update the streamDef of this project Op
        vUpdateStreamDef();
        
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
Column* ExprOp::pCreateColumn(i8 i_ExprCol, Stz* sColumnNm, i8 iStartTkn, i8 iEndTkn)
{
  // create a column
  Column* pExprColumn = new Column();
  pExprColumn->vColumnNo(i_ExprCol);
  pExprColumn->vColumnNm(sColumnNm);

  // find the expression column in the original Stream
  m_iWhichCol[i_ExprCol] =
  m_pOriginalStreamDef->iFindColumn(pExprColumn->szColNm()->azChar(),m_iWhichVec[0]);

  // set the column and datum type in the expression column
// ALKA_TO_REVIEW
//  ColumnVec* pColumnVec = m_pOriginalStreamDef->m_oColVecAry.pGet(m_iWhichVec[0]);
//  Column* pCol = pColumnVec->pGet(m_iWhichCol[i_ExprCol]);
//  pExprColumn->vColumnType(pCol->eColumnType());
//  pExprColumn->vDatumType(pCol->eDatumType());
  
  return pExprColumn;
}

/*----------------------------------------------------------------------------*/
/* update the stream def columns of the project op                            */
/*----------------------------------------------------------------------------*/
void ExprOp::vUpdateStreamDef(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_EXPROP,_FOOTER_);
  
  // create a columnVec in the stream def
// ALKA_TO_REVIEW
//  m_pStreamDef->m_oColVecAry.vItems(1);
//  ColumnVec* pColVec = m_pStreamDef->pColumnVec(0);
  
//  // decipher the expressions
//  i8 i_ExprCol = 0;
//  i8 iStartTkn = m_iTkn;
//  i8 iEndTkn = C_UNKNOWN;
//  Stz* sName = ""_Stz;
//  while ( ! m_pTknVec->pGet(m_iTkn)->bSame("|"_Stz) ) {
//
//    if ( m_pTknVec->pGet(m_iTkn)->bSame(","_Stz)) {
//      iEndTkn = m_iTkn - 1;
//
//      Column* pExprColumn = pCreateColumn(i_ExprCol, sName, iStartTkn, iEndTkn);
//
//      pColVec->vAdd(pExprColumn);
//
//      m_pCalculator[i_ExprCol] = new Calculator;
//      bool bResult = m_pCalculator[i_ExprCol]->bCompile(m_pTknVec, m_pOriginalStreamDef, iStartTkn, iEndTkn);
//      RASSERT(bResult);
//      iStartTkn = ++m_iTkn;
//
//      // move to the next expression column
//      i_ExprCol++;
//    }
//
//    if ( ! m_pTknVec->pGet(m_iTkn)->bIsName()) {
//      m_iTkn++;
//      continue;
//    }
//
//    sName = m_pTknVec->pGet(m_iTkn++)->sText();
//  }
//
//  iEndTkn = m_iTkn - 1;
//
//  Column* pExprColumn = pCreateColumn(i_ExprCol, sName, iStartTkn, iEndTkn);
//
//  pColVec->vAdd(pExprColumn);
//
//  m_pCalculator[i_ExprCol] = new Calculator;
//  bool bResult = m_pCalculator[i_ExprCol]->bCompile(m_pTknVec, m_pOriginalStreamDef, iStartTkn, iEndTkn);
//  RASSERT(bResult);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__aFooterFunction(ExprOp);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

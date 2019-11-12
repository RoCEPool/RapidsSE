/*============================================================================*/
/* ProjectOp.cpp -- Project                                                   */
/*============================================================================*/

#include "DataOperators.h"

OpDesc __Op_Project (
  CN_PROJECTOP,       // class number
  OK_SINGLE,          // op kind
  "Project"_Stz,      // first word of prompt command
  "Prj"_Stz,          // prefix
                   
  true,               // partitioned
  false,              // compiler op

  true,               // accepts CrwBuffer?
  true,               // accepts EntBuffer?
  true,               // accepts IrwBuffer?
  true,               // accepts SrtBuffer?

  false,              // accepts CrwBuffer?
  false,              // accepts EntBuffer?
  true,               // accepts IrwBuffer?
  false,              // accepts SrtBuffer?

  CN_NONE,            // what kind of output
                   
  ProjectOp::pNew,    // function which parses the command and new's an op
  ProjectOp::eCall,   // function which calls the op loop
  ProjectOp::eDrain,  // function which calls the drain loop

  "project <colNm>... "_Stz
);

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
ProjectOp* DataOperators::_pProjectOp(void* aAddr)
{
  ProjectOp* pProjectOp = reinterpret_cast<ProjectOp*>(aAddr);
  
  return pProjectOp;
}

/*----------------------------------------------------------------------------*/
/* new                                                                        */
/*----------------------------------------------------------------------------*/
void* ProjectOp::operator new(size_t)
{
  ProjectOp* pProjectOp =
  static_cast<ProjectOp*>(HeapMem::pAllocate(CN_PROJECTOP));
  
  return pProjectOp;
}

/*----------------------------------------------------------------------------*/
/* delete                                                                     */
/*----------------------------------------------------------------------------*/
void ProjectOp::operator delete(void* aAddress)
{
  ProjectOp* pProjectOp = static_cast<ProjectOp*>(aAddress);
  
  HeapMem::vFree(pProjectOp);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ProjectOp::ProjectOp(OpSpec* pOpSpec, Pipeline* pPipeline)
: Op(CN_PROJECTOP,pOpSpec,pPipeline)
{
  _precondition_(_MAGIC_,CN_PROJECTOP);
  
  m_fpCall     = ProjectOp::eCall;
  m_pStreamDef = new StreamDef;
  
  m_pOriginalStreamDef = nullptr;
  m_pIrwBuffer         = new IrwBuffer;
  
  for (i8 i_Col = 0; i_Col < C_MAX_COLUMNS; i_Col++) {
    m_iWhichVec[i_Col] = C_UNKNOWN;
    m_iWhichCol[i_Col] = C_UNKNOWN;
  }
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
ProjectOp::~ProjectOp(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_PROJECTOP);
  
  delete m_pOriginalStreamDef;
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* parse & new                                                                */
/*----------------------------------------------------------------------------*/
Op* ProjectOp::pNew(OpSpec* pOpSpec, Pipeline* pPipeline)
{
  ProjectOp* pProjectOp = new ProjectOp(pOpSpec,pPipeline);
  
  EnameSet* pEnameSet = pProjectOp->m_pOpSpec->pEnameSet();
  i8 iEnms = pEnameSet->nEnames();
  pEnameSet->vPrint();
  
  pProjectOp->m_pStreamDef->m_nDefSets++;
  pProjectOp->m_pStreamDef->m_i_DefStart[0] = 0;
  
  for (i8 i_Column = 0; i_Column < iEnms; i_Column++) {
    
    // create a column
    Column oColumn = pProjectOp->m_pStreamDef->m_xaColumn[i_Column];
    oColumn.vColumnNo(i_Column);
    oColumn.vColumnNm(pEnameSet->szValue((i8)i_Column));
    pProjectOp->m_pStreamDef->m_nDefColumns[0]++;
    
  }
  
  return pProjectOp;
}

/*----------------------------------------------------------------------------*/
/* call loop                                                                  */
/*----------------------------------------------------------------------------*/
eOR ProjectOp::eCall(void* aThisOp)
{
  eOR eCallAgain = ((ProjectOp*)aThisOp)->eOpLoop();
  
  return eCallAgain;
}

/*----------------------------------------------------------------------------*/
/* call drain loop                                                            */
/*----------------------------------------------------------------------------*/
eOR ProjectOp::eDrain(void* aThisOp)
{
  eOR eCallAgain = ((ProjectOp*)aThisOp)->eDrainLoop();
  
  return eCallAgain;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* op loop                                                                    */
/*----------------------------------------------------------------------------*/
eOR ProjectOp::eOpLoop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PROJECTOP,_FOOTER_);
  
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
//  ALKA_TO_REVIEW
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
//          Row* pRow = m_pIrwBuffer->aAllotRow(m_pStreamDef->pColumnVec(0));
//
//          if ( ! pRow ) {
//            vPipeOut(m_pIrwBuffer);
//            m_pIrwBuffer = new IrwBuffer;
//            pRow = m_pIrwBuffer->aAllotRow(m_pStreamDef->pColumnVec(0));
//          }
//
//          // get compact row
//          Crw* aCrw = (Crw*)pCrwBuffer->m_oRowTray.pNub()->aGetHead(i_Row);
//
//          for (i8 i_IntoCol = 0; i_IntoCol < pColumnVec->nItems(); i_IntoCol++) {
//            i8 iBaseOffset = RowCoder::iBaseOffset(m_pOriginalStreamDef->m_oColVecAry.pGet(m_iWhichVec[i_IntoCol]));
//
//            RowInflator::vInflateColumn(pRow, i_IntoCol, aCrw, iBaseOffset, m_iWhichCol[i_IntoCol],
//                                        m_pOriginalStreamDef->m_oColVecAry.pGet(m_iWhichVec[i_IntoCol]));
//
//
//          }
//        }
        
        break;
      }
        
      // -- -- -- -- -- --
        
      case CN_ENTBUFFER: {
//  ALKA_TO_REVIEW
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
//  ALKA_TO_REVIEW
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
//
//        break;
      }
        
      // -- -- -- -- -- --
        
      case CN_SRTBUFFER: {
//  ALKA_TO_REVIEW
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
eOR ProjectOp::eDrainLoop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PROJECTOP,_FOOTER_);
  
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
void ProjectOp::vUpdateStreamDef(void)
{
//  ALKA_TO_REVIEW
//  _precondition_(_ADDRESS_,_MAGIC_,CN_PROJECTOP,_FOOTER_);
//  RASSERT(m_pStreamDef->m_oColVecAry.nItems() == 1);
//  ColumnVec* pProjColumnVec = m_pStreamDef->m_oColVecAry.pGet(0);
//  i8 nProjColumns = pProjColumnVec->nItems();
//
//  // Find the project col in the streamDef's that came in
//
//  for (i8 i_ProjCol = 0; i_ProjCol < nProjColumns; i_ProjCol++) {
//
//    // find the project column in the original Stream
//    Column* pProjCol = pProjColumnVec->pGet(i_ProjCol);
//    m_iWhichCol[i_ProjCol] =
//    m_pOriginalStreamDef->iFindColumn(pProjCol->sColumnName().azChar(),m_iWhichVec[i_ProjCol]);
//
//    // set the column and datum type in the project column
//    ColumnVec* pColumnVec = m_pOriginalStreamDef->m_oColVecAry.pGet(m_iWhichVec[i_ProjCol]);
//    Column* pCol = pColumnVec->pGet(m_iWhichCol[i_ProjCol]);
//    pProjCol->vColumnType(pCol->eColumnType());
//    pProjCol->vDatumType(pCol->eDatumType());
//  }
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__aFooterFunction(ProjectOp);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

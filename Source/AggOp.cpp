/*============================================================================*/
/* AggOp.cpp -- Aggregator                                                    */
/*============================================================================*/

#include "DataOperators.h"

OpDesc __Op_Agg (
  CN_AGGOP,           // class number
  OK_SINGLE,          // op kind
  "Agg"_Stz,          // first word of prompt command
  "Agg"_Stz,          // prefix
                 
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
                 
  AggOp::pNew,        // function which parses the command and new's an op
  AggOp::eCall,       // function which calls the op loop
  AggOp::eDrain,      // function which calls the drain loop

  "agg <colNm>"_Stz
  );

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
AggOp* DataOperators::_pAggOp(void* aAddr)
{
  AggOp* pAggOp = reinterpret_cast<AggOp*>(aAddr);
  
  return pAggOp;
}

/*----------------------------------------------------------------------------*/
/* new                                                                        */
/*----------------------------------------------------------------------------*/
void* AggOp::operator new(size_t)
{
  AggOp* pAggOp =
  static_cast<AggOp*>(HeapMem::pAllocate(CN_AGGOP));
  
  return pAggOp;
}

/*----------------------------------------------------------------------------*/
/* delete                                                                     */
/*----------------------------------------------------------------------------*/
void AggOp::operator delete(void* aAddress)
{
  AggOp* pAggOp = static_cast<AggOp*>(aAddress);
  
  HeapMem::vFree(pAggOp);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
AggOp::AggOp(OpSpec* pOpSpec, Pipeline* pPipeline)
: Op(CN_AGGOP,pOpSpec,pPipeline)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_AGGOP,_FOOTER_);
  
  m_fpCall = AggOp::eCall;
  m_nSum = 0;
  m_pOriginalStreamDef = nullptr;
  m_pStreamDef = new StreamDef;
  
  for (i8 i_Col = 0; i_Col < C_MAX_COLUMNS; i_Col++) {
    m_iWhichCol[i_Col] = C_UNKNOWN;
    m_iWhichVec[i_Col] = C_UNKNOWN;
  }
  
  _checkInitialised_(CN_AGGOP);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_AGGOP,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
AggOp::~AggOp(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_AGGOP,_FOOTER_);
  
  delete m_pOriginalStreamDef;
}

/*----------------------------------------------------------------------------*/
/* parse & new                                                                */
/*----------------------------------------------------------------------------*/
Op* AggOp::pNew(OpSpec* pOpSpec, Pipeline* pPipeline)
{  
  AggOp* pAggOp = new AggOp(pOpSpec,pPipeline);
  
  // create a column
  Column oColumn = pAggOp->m_pStreamDef->m_xaColumn[0];
  oColumn.vColumnNo(0);
  oColumn.vColumnNm(pAggOp->m_pOpSpec->pEnameSet()->szValue((i8)0));
  
  return pAggOp;
}

/*----------------------------------------------------------------------------*/
/* call loop                                                                  */
/*----------------------------------------------------------------------------*/
eOR AggOp::eCall(void* aThisOp)
{
  eOR eCallAgain = ((AggOp*)aThisOp)->eOpLoop();
  
  return eCallAgain;
}

/*----------------------------------------------------------------------------*/
/* call drain loop                                                            */
/*----------------------------------------------------------------------------*/
eOR AggOp::eDrain(void* aThisOp)
{
  eOR eCallAgain = ((AggOp*)aThisOp)->eDrainLoop();
  
  return eCallAgain;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* op loop                                                                    */
/*----------------------------------------------------------------------------*/
eOR AggOp::eOpLoop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_AGGOP,_FOOTER_);
  
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
          
          // create a column vec from each row ref
          ColumnVec* pColumnVec = new ColumnVec;
          m_pStreamDef->vAddColumns(pColumnVec);
          
          IrwBuffer* pIrwBuffer = new IrwBuffer();
          Row* pRow = pIrwBuffer->aAllotRow(pColumnVec);
          pRow->m_waColVal[0] = m_nSum;
          
          // pipe out the buffer with the count
          vPipeOut(pIrwBuffer);
          
          // now pipe out the end marker
          vPipeOut(pOperand);
          goto L_EXIT;
        }
        else
          delete (EndMarker*)pOperand;
        
        break;
        
      // -- -- -- -- -- --
      case CN_CRWBUFFER: {
        CrwBuffer* pCrwBuffer = _pCrwBuffer(pOperand) ;
        
        // get the columnVec defining the rowref
        ColumnVec* pColumnVec = new ColumnVec;
        m_pOriginalStreamDef->vAddColumns(pColumnVec);
        
        HV_Row hRow(pColumnVec);
        i8 nRows = pCrwBuffer->nRowCount();
        
        for (i8 i_Row = 0; i_Row < nRows; i_Row++) {
          
          // get compact row
          Crw* aCrw = (Crw*)pCrwBuffer->m_oRowTray.pNub()->aGetHead(i_Row);
          
          // get the key from the row
          RowInflator::vInflateColumn(hRow, aCrw, m_iWhichCol[0]);

          m_nSum += hRow->m_waColVal[m_iWhichCol[0]].m_iI8;
        }
        delete pColumnVec;
        break;
      }
        
      case CN_ENTBUFFER: {
        EntBuffer* pEntBuffer = _pEntBuffer(pOperand) ;
        
        // get the columnVec defining the rowref
        ColumnVec* pColumnVec = new ColumnVec;
        m_pOriginalStreamDef->vAddColumns(pColumnVec);
        
        HV_Row hRow(pColumnVec);
// ALKA_TO_REVIEW
//        CrwBuffer* pCrwBuffer = nullptr;
//
//        Ent* aEnt = pEntBuffer->aLeadEntry();
//
//        i8 iBaseOffset = RowCoder::iBaseOffset(m_pOriginalStreamDef->m_oColVecAry.pGet(m_iWhichVec[0]));
//
//        while (aEnt) {
//
//          DtmVal oDtmVal;
//
//          pCrwBuffer = CrwBuffer::pRefdCrwBuffer(aEnt->xRef());
//
//          Crw* aRowData = pCrwBuffer->pCrw(aEnt->xRef().iItem());
//
//          RowInflator::vInflateColumn(hRow, 0, aRowData, iBaseOffset, m_iWhichCol[0],
//                                        m_pOriginalStreamDef->m_oColVecAry.pGet(m_iWhichVec[0]));
//
//          m_nSum += hRow->m_waColVal[0].m_iI8;
//
//          aEnt = pEntBuffer->aNextEntry(aEnt,0);
//        }
        delete pColumnVec;
        break;
      }
        
      case CN_IRWBUFFER: {
        IrwBuffer* pIrwBuffer = _pIrwBuffer(pOperand) ;
        
        Row* pRow = pIrwBuffer->aFirstRow();
        while (pRow) {
          m_nSum += pRow->m_waColVal[m_iWhichCol[0]].m_iI8;
          pRow = pIrwBuffer->aNextRow(pRow);
        }
        delete pIrwBuffer;
        break;
      }

        
      case CN_SRTBUFFER: {
        SrtBuffer* pSrtBuffer = _pSrtBuffer(pOperand);
        SrtSet* aSrtSet = pSrtBuffer->aFirstSrtSet();
        
        // get the columnVec defining the rowref
        ColumnVec* pColumnVec = new ColumnVec;
        m_pOriginalStreamDef->vAddColumns(pColumnVec);
        
// ALKA_TO_REVIEW
//        while (aSrtSet) {
//
//          // get the dataRow of rowref
//          Crw* aCrw = (Crw*)(aSrtSet->iRowRef(m_iWhichVec[0]));
//
//          if ( aCrw ) {
//            // inflate the column
//            i8 iBaseOffset = RowCoder::iBaseOffset(m_pOriginalStreamDef->m_oColVecAry.pGet(m_iWhichVec[0]));
//
//            RowInflator::vInflateColumn(hRow, 0, aCrw, iBaseOffset, m_iWhichCol[0],
//                                        m_pOriginalStreamDef->m_oColVecAry.pGet(m_iWhichVec[0]));
//            if (hRow->m_waColVal[0].m_iI8 != C_DATUM_NULL)
//              m_nSum += hRow->m_waColVal[0].m_iI8;
//          }
//
//          aSrtSet = pSrtBuffer->aNextSrtSet(aSrtSet);
//        }
        delete pSrtBuffer;
        delete pColumnVec;
        break;
      }
        
      case CN_STREAMDEF: {
        
        // figure out the columns to be projected
        StreamDef* pStreamDef = _pStreamDef(pOperand);
        m_pOriginalStreamDef = new StreamDef(pStreamDef);
        
        // update the streamDef of this agg Op
        vUpdateStreamDef();
        
        // pipe out the streamdef
        vPipeOut(new StreamDef(m_pStreamDef));
        
        delete _pStreamDef(pOperand);
        break;
      }
        
      default:
        vPipeOut(pOperand);
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
eOR AggOp::eDrainLoop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_AGGOP,_FOOTER_);
  
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
/* update the stream def columns of the agg op                                */
/*----------------------------------------------------------------------------*/
void AggOp::vUpdateStreamDef(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_AGGOP,_FOOTER_);
 
// ALKA_TO_REVIEW
//  RASSERT(m_pStreamDef->m_oColVecAry.nItems() == 1);
//  ColumnVec* pAggColumnVec = m_pStreamDef->m_oColVecAry.pGet(0);
//  i8 nAggColumns = pAggColumnVec->nItems();
//
//  // Find the project col in the streamDef's that came in
//
//  for (i8 i_AggCol = 0; i_AggCol < nAggColumns; i_AggCol++) {
//
//    // find the project column in the original Stream
//    Column* pAggCol = pAggColumnVec->pGet(i_AggCol);
//    m_iWhichCol[i_AggCol] =
//    m_pOriginalStreamDef->iFindColumn(pAggCol->sColumnName().azChar(),m_iWhichVec[i_AggCol]);
//
//    // get the agg column from the original Stream
//    ColumnVec* pColumnVec = m_pOriginalStreamDef->m_oColVecAry.pGet(m_iWhichVec[i_AggCol]);
//    Column* pCol = pColumnVec->pGet(m_iWhichCol[i_AggCol]);
//
//    // set the column and datum type in the agg column
//    pAggCol->vColumnType(pCol->eColumnType());
//    pAggCol->vDatumType(pCol->eDatumType());
//  }
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__aFooterFunction(AggOp);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

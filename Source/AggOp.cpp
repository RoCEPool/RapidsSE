/*============================================================================*/
/* AggOp.cpp -- Aggregator                                                    */
/*============================================================================*/

#include "DataOperators.h"

OpDesc __Op_Agg (
  CN_AGGOP,           // class number
  OK_SIMPLE,          // op kind
  "Agg"_Stz,          // first word of prompt command
  "Agg"_Stz,          // prefix
  true,               // partitioned
  AggOp::pNew,        // function which parses the command and new's an op
  AggOp::eCall,       // function which calls the op loop
  AggOp::eDrain,      // function which calls the drain loop

  "<help>"_Stz
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
AggOp::AggOp(OpSpec* pOpSpec, Digest* pDigest, Stz* szOpLabel, OpDefn* aOpDefn, bool bAppR)
: Op(CN_AGGOP,szOpLabel,pDigest,aOpDefn,bAppR,pOpSpec)
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
  
  _checkInitialised_(CN_COUNTOP);
  
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
Op* AggOp::pNew(OpSpec* pOpSpec, Digest* pDigest, Stz* szLabel,
                OpDefn* aOpDefn, bool bAppR,
                TokenVec* pTknVec, i8& i_Tkn_, i8 i_Slot)
{
  i_Tkn_++;
  ASSERT(i_Slot != -1);
  
  AggOp* pAggOp = new AggOp(pOpSpec,pDigest,szLabel,aOpDefn,bAppR);
  
  pAggOp->m_nSlot = i_Slot;
  
  pAggOp->m_pStreamDef->m_oColVecAry.vItems(1);
  ColumnVec* pColVec = pAggOp->m_pStreamDef->pColumnVec(0);
  
  // create a column
  Column* pColumn = new Column();
  pColumn->vColumnNo(0);
  pColumn->vColumnName(pTknVec->pGet(i_Tkn_++)->szText());
  pColVec->vAdd(pColumn);
  
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
          IrwBuffer* pIrwBuffer = new IrwBuffer();
          Row* pRow = pIrwBuffer->aAllotRow(m_pStreamDef->pColumnVec(0));
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
        
        HV_Row hRow(m_pOriginalStreamDef->m_oColVecAry.pGet(0));
        i8 nRows = pCrwBuffer->nRowCount();
        
        for (i8 i_Row = 0; i_Row < nRows; i_Row++) {
          
          // get compact row
          Crw* aCrw = (Crw*)pCrwBuffer->m_oRowTray.pNub()->aGetHead(i_Row);
          
          // get the key from the row
          RowInflator::vInflateColumn(hRow, aCrw, m_iWhichCol[0]);

          m_nSum += hRow->m_waColVal[m_iWhichCol[0]].m_iI8;
        }
        
        break;
      }
        
      case CN_ENTBUFFER: {
        EntBuffer* pEntBuffer = _pEntBuffer(pOperand) ;
        
        HV_Row hRow(m_pOriginalStreamDef->m_oColVecAry.pGet(0));
        
        CrwBuffer* pCrwBuffer = nullptr;
        
        Ent* aEnt = pEntBuffer->aLeadEntry();
        
        i8 iBaseOffset = RowCoder::iBaseOffset(m_pOriginalStreamDef->m_oColVecAry.pGet(m_iWhichVec[0]));
        
        while (aEnt) {
          
          DtmVal oDtmVal;
          
          pCrwBuffer = CrwBuffer::pRefdCrwBuffer(aEnt->xRef());
          
          Crw* aRowData = pCrwBuffer->pCrw(aEnt->xRef().iItem());
    
          RowInflator::vInflateColumn(hRow, 0, aRowData, iBaseOffset, m_iWhichCol[0],
                                        m_pOriginalStreamDef->m_oColVecAry.pGet(m_iWhichVec[0]));
          
          m_nSum += hRow->m_waColVal[0].m_iI8;
          
          aEnt = pEntBuffer->aNextEntry(aEnt,0);
        }
        
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
        ColumnVec* pStreamColVec = m_pOriginalStreamDef->m_oColVecAry.pGet(0);
        HV_Row hRow(pStreamColVec);
        
        while (aSrtSet) {
            
          // get the dataRow of rowref
          Crw* aCrw = (Crw*)(aSrtSet->iRowRef(m_iWhichVec[0]));
          
          if ( aCrw ) {
            // inflate the column
            i8 iBaseOffset = RowCoder::iBaseOffset(m_pOriginalStreamDef->m_oColVecAry.pGet(m_iWhichVec[0]));
            
            RowInflator::vInflateColumn(hRow, 0, aCrw, iBaseOffset, m_iWhichCol[0],
                                        m_pOriginalStreamDef->m_oColVecAry.pGet(m_iWhichVec[0]));
            if (hRow->m_waColVal[0].m_iI8 != C_DATUM_NULL)
              m_nSum += hRow->m_waColVal[0].m_iI8;
          }
          
          aSrtSet = pSrtBuffer->aNextSrtSet(aSrtSet);
        }
        delete pSrtBuffer;
        break;
      }
        
      case CN_STREAMDEF: {
        
        // figure out the columns to be projected
        StreamDef* pStreamDef = _pStreamDef(pOperand);
        m_pOriginalStreamDef = new StreamDef(pStreamDef);
        
        RASSERT(m_pStreamDef->m_oColVecAry.nItems() == 1);
        ColumnVec* pProjColumnVec = m_pStreamDef->m_oColVecAry.pGet(0);
        i8 nProjColumns = pProjColumnVec->nItems();
        
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
  
  RASSERT(m_pStreamDef->m_oColVecAry.nItems() == 1);
  ColumnVec* pAggColumnVec = m_pStreamDef->m_oColVecAry.pGet(0);
  i8 nAggColumns = pAggColumnVec->nItems();
  
  // Find the project col in the streamDef's that came in
  
  for (i8 i_AggCol = 0; i_AggCol < nAggColumns; i_AggCol++) {
    
    // find the project column in the original Stream
    Column* pAggCol = pAggColumnVec->pGet(i_AggCol);
    m_iWhichCol[i_AggCol] =
    m_pOriginalStreamDef->iFindColumn(pAggCol->sColumnName().aChar(),m_iWhichVec[i_AggCol]);
    
    // get the agg column from the original Stream
    ColumnVec* pColumnVec = m_pOriginalStreamDef->m_oColVecAry.pGet(m_iWhichVec[i_AggCol]);
    Column* pCol = pColumnVec->pGet(m_iWhichCol[i_AggCol]);
    
    // set the column and datum type in the agg column
    pAggCol->vColumnType(pCol->eColumnType());
    pAggCol->vDatumType(pCol->eDatumType());
  }
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__aFooterFunction(AggOp);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* AggOp.cpp -- Aggregator                                                    */
/*============================================================================*/

#include "DataOperators.h"

OpDesc __Op_Agg (
   CN_AGGOP,           // class number
   OK_SIMPLE,          // op kind
   "Agg"_Stz,          // first word of prompt command
   "Agg"_Stz,          // prefix
   false,              // partitioned
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
  static_cast<AggOp*>(HeapMem::pAllocate(CN_COUNTOP));
  
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
AggOp::AggOp(Stz* szOpLabel, Digest* pDigest, OpDefn* aOpDefn, bool bAppR)
: Op(CN_AGGOP,szOpLabel,pDigest,aOpDefn,bAppR)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_AGGOP,_FOOTER_);
  
  m_fpCall = AggOp::eCall;
  m_nSum = 0;
  m_pOriginalStreamDef = new StreamDef;
  m_pStreamDef = new StreamDef;
  
  for (i8 i_ColVec = 0; i_ColVec < C_MAX_COLUMNVECS; i_ColVec++)
    m_iWhichVec[i_ColVec] = C_UNKNOWN;
  
  for (i8 i_Col = 0; i_Col < C_MAX_COLUMNS; i_Col++)
    m_iWhichCol[i_Col] = C_UNKNOWN;
  
  _checkInitialised_(CN_COUNTOP);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_AGGOP,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
AggOp::~AggOp(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_AGGOP,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* parse & new                                                                */
/*----------------------------------------------------------------------------*/
Op* AggOp::pNew(Stz* szLabel,
                  Digest* pDigest, OpDefn* aOpDefn, bool bAppR,
                  TokenVec* pTknVec, i8& i_Tkn_, i8 i_Slot)
{
  i_Tkn_++;
  ASSERT(i_Slot == -1);
  
  AggOp* pAggOp = new AggOp(szLabel,pDigest,aOpDefn,bAppR);
  
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
          
          // pipe out the streamdef
          vPipeOut(new StreamDef(m_pStreamDef));
          
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
        
      case CN_ENTBUFFER: {
        EntBuffer* pEntBuffer = _pEntBuffer(pOperand) ;
        ColumnVec* pColumnVec = m_pStreamDef->m_oColVecAry.pGet(0);
        
        Ent* aEnt = pEntBuffer->aLeadEntry();
        while (aEnt) {
          
          DtmVal oDtmVal;
          
          CrwBuffer* pCrwBuffer = CrwBuffer::pRefdCrwBuffer(aEnt->xRef());
          Crw* aRowData = pCrwBuffer->pCrw(aEnt->xRef().iItem());
          
          HV_Row hRow(m_pOriginalStreamDef->m_oColVecAry.pGet(0));
          //for (i8 i_IntoCol = 0; i_IntoCol < pColumnVec->nItems(); i_IntoCol++) {
            i8 iBaseOffset = RowCoder::iBaseOffset(m_pOriginalStreamDef->m_oColVecAry.pGet(m_iWhichVec[0]));
          
            RowInflator::vInflateColumn(hRow, 0, aRowData, iBaseOffset, m_iWhichCol[0],
                                        m_pOriginalStreamDef->m_oColVecAry.pGet(m_iWhichVec[0]));
          //}
          m_nSum += hRow->m_waColVal[0].m_iI8;
          
          aEnt = pEntBuffer->aGetNextEntry(&pEntBuffer, aEnt);
        }
        
        break;
      }
        
      case CN_IRWBUFFER: {
        IrwBuffer* pIrwBuffer = _pIrwBuffer(pOperand) ;
        ColumnVec* pColumnVec = m_pStreamDef->m_oColVecAry.pGet(0);
        
        Row* pRow = pIrwBuffer->aFirstRow();
        while (pRow) {
          m_nSum += pRow->m_waColVal[0].m_iI8;
          pRow = pIrwBuffer->aNextRow(pRow);
        }
        
        break;
      }
      case CN_INTBUFFER: {
        IntBuffer* pIntBuffer = _pIntBuffer(pOperand) ;
        m_nSum += pIntBuffer->nCount();
        break;
      }
        
      case CN_REFBUFFER: {
        RefBuffer* pRefBuffer = _pRefBuffer(pOperand) ;
        m_nSum += pRefBuffer->nItems();
        delete pRefBuffer;
        break;
      }
        
      case CN_SRTBUFFER: {
        SrtBuffer* pSrtBuffer = _pSrtBuffer(pOperand);
        m_nSum += pSrtBuffer->nItems();
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
        
        // Find the project col in the streamDef's that came in
        for (i8 i_ProjCol = 0; i_ProjCol < nProjColumns; i_ProjCol++) {
          Column* pProjCol = pProjColumnVec->pGet(i_ProjCol);
          Column* pCol = nullptr;
          
          for (i8 i_ColVec = 0; i_ColVec < m_pOriginalStreamDef->m_oColVecAry.nItems(); i_ColVec++) {
            ColumnVec* pColumnVec = m_pOriginalStreamDef->m_oColVecAry.pGet(i_ColVec);
            i8 nColumns = pColumnVec->nItems();
            
            for (i8 i_Col = 0; i_Col < nColumns; i_Col++) {
              pCol = pColumnVec->pGet(i_Col);
              if (pProjCol->sColumnName().bEqual(pCol->sColumnName())) {
                m_iWhichVec[i_ProjCol] = i_ColVec;
                m_iWhichCol[i_ProjCol] = i_Col;
                pProjCol->vColumnType(pCol->eColumnType());
                pProjCol->vDatumType(pCol->eDatumType());
                break;
              }
            }
          }
        }
        
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
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__aFooterFunction(AggOp);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

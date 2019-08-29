/*============================================================================*/
/* ProjectOp.cpp -- Project                                                   */
/*============================================================================*/

#include "DataOperators.h"

OpDesc __Op_Project (
   CN_PROJECTOP,       // class number
   OK_SIMPLE,          // op kind
   "Project"_Stz,      // first word of prompt command
   "Prj"_Stz,          // prefix
   true,               // partitioned
   ProjectOp::pNew,    // function which parses the command and new's an op
   ProjectOp::eCall,   // function which calls the op loop
   ProjectOp::eDrain,  // function which calls the drain loop
   
   "<help>"_Stz
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
  static_cast<ProjectOp*>(HeapMem::pAllocate(CN_COUNTOP));
  
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
ProjectOp::ProjectOp(Stz* szOpLabel, Digest* pDigest, OpDefn* aOpDefn, bool bAppR)
: Op(CN_PROJECTOP,szOpLabel,pDigest,aOpDefn,bAppR)
{
  _precondition_(_MAGIC_,CN_PROJECTOP);
  
  m_pStreamDef = new StreamDef;
  m_pOriginalStreamDef = nullptr;
  m_pIrwBuffer = new IrwBuffer;
  
  for (i8 i_ColVec = 0; i_ColVec < C_MAX_COLUMNVECS; i_ColVec++)
    m_iWhichVec[i_ColVec] = C_UNKNOWN;
  
  for (i8 i_Col = 0; i_Col < C_MAX_COLUMNS; i_Col++)
    m_iWhichCol[i_Col] = C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
ProjectOp::~ProjectOp(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_PROJECTOP);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* parse & new                                                                */
/*----------------------------------------------------------------------------*/
Op* ProjectOp::pNew(Stz* szLabel,
                    Digest* pDigest, OpDefn* aOpDefn, bool bAppR,
                    TokenVec* pTknVec, i8& i_Tkn_, i8 i_Slot)
{
  i_Tkn_++;
  ASSERT(i_Slot != -1);
  
  ProjectOp* pProjectOp = new ProjectOp(szLabel,pDigest,aOpDefn,bAppR);
  
  pProjectOp->m_pStreamDef->m_oColVecAry.vItems(1);
  ColumnVec* pColVec = pProjectOp->m_pStreamDef->pColumnVec(0);
  
  i8 i_Column = 0;
  while ( ! pTknVec->pGet(i_Tkn_)->bSame("|"_Stz) ) {
    
    if (pTknVec->pGet(i_Tkn_)->bSame(","_Stz)) {
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
        
      case CN_ENTBUFFER: {
        EntBuffer* pEntBuffer = _pEntBuffer(pOperand) ;
        ColumnVec* pColumnVec = m_pStreamDef->m_oColVecAry.pGet(0);
        
        Ent* aEnt = pEntBuffer->aLeadEntry();
        while (aEnt) {
          Row* pRow = m_pIrwBuffer->aAllotRow(m_pStreamDef->pColumnVec(0));
          
          if ( ! pRow ) {
            vPipeOut(m_pIrwBuffer);
            m_pIrwBuffer = new IrwBuffer;
            pRow = m_pIrwBuffer->aAllotRow(m_pStreamDef->pColumnVec(0));
          }
          
          DtmVal oDtmVal;
          
          CrwBuffer* pCrwBuffer = CrwBuffer::pRefdCrwBuffer(aEnt->xRef());
          Crw* aRowData = pCrwBuffer->pCrw(aEnt->xRef().iItem());
          
          HV_Row hRow(m_pOriginalStreamDef->m_oColVecAry.pGet(0));
          for (i8 i_Col = 0; i_Col < pColumnVec->nItems(); i_Col++) {
            RowInflator::vInflateColumn(hRow, aRowData, m_iWhichCol[i_Col]);
          }
          for (i8 i_Col = 0; i_Col < pColumnVec->nItems(); i_Col++) {
            pRow->m_waColVal[i_Col] = hRow->m_waColVal[m_iWhichCol[i_Col]];
          }
          aEnt = pEntBuffer->aGetNextEntry(&pEntBuffer, aEnt);
        }
        
        break;
      }
        
      case CN_REFBUFFER: {
        RefBuffer* pRefBuffer = _pRefBuffer(pOperand) ;
        delete pRefBuffer;
        break;
      }
        
      case CN_SRTBUFFER: {
        SrtBuffer* pSrtBuffer = _pSrtBuffer(pOperand);
        delete pSrtBuffer;
        break;
      }
        
      case CN_STREAMDEF: {
        
        // figure out the columns to be projected
        StreamDef* pStreamDef = _pStreamDef(pOperand);
        m_pOriginalStreamDef = new StreamDef(pStreamDef);
        
        ColumnVec* pColumnVec = pStreamDef->m_oColVecAry.pGet(0);
        i8 nColumns = pColumnVec->nItems();
        
        for (i8 i_ColVec = 0; i_ColVec < m_pStreamDef->m_oColVecAry.nItems(); i_ColVec++) {
          ColumnVec* pProjColumnVec = m_pStreamDef->m_oColVecAry.pGet(i_ColVec);
          i8 nProjColumns = pProjColumnVec->nItems();
          
          // for each column in the project stream def
          for (i8 i_ProjCol = 0; i_ProjCol < nProjColumns; i_ProjCol++) {
            Column* pProjCol = pProjColumnVec->pGet(i_ProjCol);
            
            Column* pCol = nullptr;
            
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
        
        delete pStreamDef;
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
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__aFooterFunction(ProjectOp);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

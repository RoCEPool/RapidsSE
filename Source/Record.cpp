/*============================================================================*/
/* Record.cpp -- Record                                                       */
/*============================================================================*/

#include "Transform.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Record* Records::_pRecord(void* aAddr)
{
  Record* pRecord = reinterpret_cast<Record*>(aAddr);
  
  return pRecord;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Record::operator new(size_t)
{
  void* pObject = HeapMem::pAllocate(CN_RECORD);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void Record::operator delete(void* aAddr)
{
  Record* pRecord = reinterpret_cast<Record*>(aAddr);
  
  HeapMem::vFree(pRecord);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Record::Record(void)
//: Object(CN_RECORD)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD,_FOOTER_);
  
  _checkInitialised_(CN_RECORD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_RECORD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Record::Record(StreamDef* pStreamDef, i8 iSubSet)
//: Object(CN_RECORD)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD,_FOOTER_);
  
  m_pStreamDef     = pStreamDef;
  m_iSubSet        = iSubSet;
  
  _checkInitialised_(CN_RECORD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_RECORD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Record::~Record(void) noexcept(false)
{
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* decode and sprint row into record                                          */
/*----------------------------------------------------------------------------*/
void Record::vAppend(Row* pRow)
{
  vReset();
  
  if (m_iSubSet == C_ALL) {
    
    for (i8 i_DefSet = 0; i_DefSet < m_pStreamDef->m_nDefSets; i_DefSet++) {
      
      // get no of columns in this stream def
      i8 nColumns = m_pStreamDef->m_nDefColumns[i_DefSet];
    
      // get start of stream def columns
      i8 iDefStart = m_pStreamDef->m_i_DefStart[i_DefSet];
    
      // for each column
      for (i8 i_Col = 0; i_Col < nColumns; i_Col++) {
        
        // get the column at index
        Column xCol = m_pStreamDef->m_xaColumn[i_Col + iDefStart];
        
        // comma separate column values
        if (i_Col)
          StrLib::vAppendString(&m_azAppendCursor,",");
        
        vAppend(&m_pStreamDef->m_xaColumn[i_Col + iDefStart],pRow);
        //vTrace("%s\n"_Stz,pRec->szRecStr());
        
      }
    }
  }
  else {
    // get no of columns in this stream def
     i8 nColumns = m_pStreamDef->m_nDefColumns[m_iSubSet];

     // get start of stream def columns
     i8 iDefStart = m_pStreamDef->m_i_DefStart[m_iSubSet];

     // for each column
     for (i8 i_Col = 0; i_Col < nColumns; i_Col++) {
       
       // get the column at index
       Column xCol = m_pStreamDef->m_xaColumn[i_Col + iDefStart];
       
       // comma separate column values
       if (i_Col)
         StrLib::vAppendString(&m_azAppendCursor,",");
       
       //! ALKA: Revert once Column not newable
       // append column value
       //vAppend(&xCol,pRow);
       vAppend(&m_pStreamDef->m_xaColumn[i_Col + iDefStart],pRow);
     }
   }
}

/*----------------------------------------------------------------------------*/
/* append ascii of datum to record                                            */
/*----------------------------------------------------------------------------*/
void Record::vAppend(Column* pCol, Row* pRow)
{
  Datum oDatum;

  i8 i_Col = pCol->iColumnNo();
  oDatum = pRow->m_waColVal[i_Col].m_iWrd;
  
  if (oDatum.bIsNull()) {
    StrLib::vAppendString(&m_azAppendCursor,"null");
    return;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  eDM eDatum = pCol->eDatum();
 
  switch (eDatum) {
    case DM_BCD: vAppendBcd(pCol,&oDatum); break;
    case DM_BOL: vAppendBol(pCol,&oDatum); break;
    case DM_DTE: vAppendDte(pCol,&oDatum); break;
    case DM_FLT: vAppendFlt(pCol,&oDatum); break;
    case DM_INT: vAppendInt(pCol,&oDatum); break;
    case DM_IVL: vAppendIvl(pCol,&oDatum); break;
    case DM_TME: vAppendTme(pCol,&oDatum); break;
    case DM_TSP: vAppendTsp(pCol,&oDatum); break;
      
    case DM_VCH:
      oDatum.vStz(pRow->m_waColVal[i_Col].m_szVch);
      //oDatum.vStzDump();
      
      vAppendVch(pCol,&oDatum);
      break;
      
    default:
      NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* print record string as row                                                 */
/*----------------------------------------------------------------------------*/
void Record::vPrintRow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD);
  
  vTrace("row: (%s)\n"_Stz,azRecord());
}

/*----------------------------------------------------------------------------*/
/* print record string as row                                                 */
/*----------------------------------------------------------------------------*/
void Record::vPrintPartialRow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD);
  
  vTrace("%s"_Stz,azRecord());
}

/*----------------------------------------------------------------------------*/
/* get record string                                                          */
/*----------------------------------------------------------------------------*/
Stz* Record::szRecStr(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD);
  
  return sz();
}

/*----------------------------------------------------------------------------*/
/* set record string                                                          */
/*----------------------------------------------------------------------------*/
void Record::vRecStr(Stz* szStr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD);
  
  StrLib::vCopy(m_caRecord,(char*)szStr);
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Record::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

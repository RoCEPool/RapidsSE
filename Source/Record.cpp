/*============================================================================*/
/* Record.cpp -- Record                                                       */
/*============================================================================*/

#include "Records.h"

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
void Record::vAppend(RowSingle* pRow)
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
          StrLib::vAppendString(&m_azCursor,",");
        
        vAppendCol(&m_pStreamDef->m_xaColumn[i_Col + iDefStart],pRow);
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
         StrLib::vAppendString(&m_azCursor,",");
       
       //! ALKA: Revert once Column not newable
       // append column value
       vAppendCol(&m_pStreamDef->m_xaColumn[i_Col + iDefStart],pRow);
     }
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
__aFooterFunction(Record);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

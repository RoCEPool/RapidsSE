/*============================================================================*/
/* JobEncodeRow.cpp -- Row part for a Job                                     */
/*============================================================================*/

#include "Jobs.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* JobEncodeRow::operator new(size_t)
{
  vFatal("fatal: JobEncodeRow class cannot be new'd\n"_Stz);
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
JobEncodeRow::JobEncodeRow(void)
{
  //m_oRowParts;
  //m_oFormatters;
  
  m_pRowCoder_Encode = new RowCoder;
  for (i8 i_Ptn = 0; i_Ptn < C_MAX_PARTITIONS; i_Ptn++) {
    m_paRowCoder_Copy[i_Ptn] = nullptr;
  }
  for (i8 i_Ptn = 0; i_Ptn < gpAttachMap->m_pGrid->m_nPartitions; i_Ptn++) {
    m_paRowCoder_Copy[i_Ptn] = new RowCoder;
  }
  
  //ColumnVec* pColVec = pTable()->pColumnVec();
  m_pRow = nullptr; //new Row(pColVec);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
JobEncodeRow::~JobEncodeRow(void) noexcept(false)
{
  for (i8 i_Ptn = 0; i_Ptn < gpAttachMap->m_pGrid->m_nPartitions; i_Ptn++) {
    
#ifdef WITH_ANALYSE_FREE
    m_paRowCoder_Copy[i_Ptn]->vReport(m_pRowCoder_Encode);
#endif
    
    delete m_paRowCoder_Copy[i_Ptn];
  }
  delete m_pRowCoder_Encode;
  
  delete m_pRow;
}

/*----------------------------------------------------------------------------*/
/*-- Public Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Encode row from given record string                                        */
/*----------------------------------------------------------------------------*/
bool JobEncodeRow::bEncodeRow(Stz* szRecStr, i8 nBytes, bool &bBlankLine)
{
  ColumnVec* pColVec = pTable()->pColumnVec();
  
  char* acRecCrs = szRecStr->aChar();
  
  bool bResult = true;
  
  if (m_pRow)
    m_pRow->vResetValues();
  else
    m_pRow = new Row(pColVec);
  
  bBlankLine = false;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // leading blanks and blank lines
  
  // skip leading spaces
  if (m_oFormatters.m_cRecordTerminator)
    StrLib::vSkipWhite(&acRecCrs);
  else
    StrLib::vSkipSpaces(&acRecCrs);
  
  // ignore blank lines
  if (! Extractor::bIsThereRecordData(acRecCrs,&m_oFormatters) ) {
    bBlankLine = true;
    return false;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // debug
  
#ifdef WITH_TRACE_ENCODING
  char caRecord[C_16KB];
  StrLib::vCopyTill(caRecord,aSrc,'\n');
  vTrace("\n--------------------------------------------------------------------------\n"_Stz);
  vTrace("\nFormatting record: %s\n"_Stz,caRecord);
#endif
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // extract row from record
  
  m_pRowCoder_Encode->vEncodeBegin(m_oRowParts.pNub());
  
  Column* pBadCol = Extractor::pExtractRow(m_pRow,&acRecCrs,pColVec,&m_oFormatters);

  // any invalid field?
  if (pBadCol) {
    if (! gpAttachMap->m_bMetadata) {
      vWarn("Skipping row - invalid field %s in record: "_Stz,
            pBadCol->szColNm());
      for (char* a = (char*)szRecStr; ( *a && '\n' != *a && '\r' != *a ); a++)
        vTrace("%c"_Stz,*a);
      vTrace("\n"_Stz);
    }
    return false;
  }
  
  hpRow hp_Row;
  vTrace("%s\n"_Stz,hp_Row.sz(m_pRow));
  
  m_pRowCoder_Encode->vEncodeRow(m_pRow);
  
  m_pRowCoder_Encode->vEncodeEnd();

  return bResult;
}

/*----------------------------------------------------------------------------*/
/* get table                                                                  */
/*----------------------------------------------------------------------------*/
Table* JobEncodeRow::pTable(void)
{
  Table* pTable = m_oRowParts.pNub()->pDesc();
  
  return pTable;
}

/*----------------------------------------------------------------------------*/
/* set table                                                                  */
/*----------------------------------------------------------------------------*/
void JobEncodeRow::vTable(Table* pTable)
{
  m_oRowParts.pNub()->vDesc(pTable);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

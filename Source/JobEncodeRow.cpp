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
}

/*----------------------------------------------------------------------------*/
/*-- Public Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Encode row from given record string                                        */
/*----------------------------------------------------------------------------*/
bool JobEncodeRow::bEncodeRow(Stz* szRecStr, i8 nBytes, bool &bBlankLine)
{
  bBlankLine = false;
  
#ifdef WITH_ANALYSE_FREE
  static Peruse xRows("Rows:"_Stz);
  xRows.vAdd(1,"\n"_Stz);
#endif
  
  bool bResult = true;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  char* aSrc = szRecStr->aChar();

  // skip leading spaces
  if (m_oFormatters.m_cRecordTerminator)
    StrLib::vSkipWhite(&aSrc);
  else
    StrLib::vSkipSpaces(&aSrc);
  
  // ignore blank lines
  if (! Extractor::bIsThereRecordData(aSrc,&m_oFormatters) ) {
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
  
  ColumnVec* pColVec = pTable()->pColumnVec();
  i8 nColumns = pColVec->nItems();

  
  //! this should probably a member in JobEncodeRow
  HV_Row hvRow(pColVec);
  
  
  m_pRowCoder_Encode->vEncodeBegin(m_oRowParts.pNub());
  
  for (i8 i_Column = 0; i_Column < nColumns; i_Column++) {
    
    Datum oDatum;
    Column* pCol = pColVec->pGet(i_Column);
    
    // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
    // extract datum from record

    bool bValidDatum = Extractor::bExtract(hvRow,&oDatum,&aSrc,pCol,&m_oFormatters);

    // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
    // is the datum valid ?
    
    if (! bValidDatum) {
      if (! gpAttachMap->m_bMetadata) {
        vWarn("Skipping row - invalid field %s in record: "_Stz,
              pCol->sColumnName().aChar());
        for (char* a = (char*)szRecStr; *a && '\n' != *a && '\r' != *a; a++)
          vTrace("%c"_Stz,*a);
        vTrace("\n"_Stz);
      }
      return false;
    }

    // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
    // append datum into row
    
    hvRow->vAppendValue(&oDatum,pCol);
    
    
    // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
    // encode datum into cache line
    m_pRowCoder_Encode->vEncodeDatum(&oDatum,pCol);
    
    //if (m_pRowCoder_Encode->m_i_IntoOffset > m_pRowCoder_Encode->nHeadBytes(pColVec)) {
    //  bResult = false;
    //  break;
    //}
      
  } // for each column
  
  //hpRow hp_Row;
  //vTrace("%s\n"_Stz,hp_Row.sz(hvRow));
  
  m_pRowCoder_Encode->vEncodeRow(hvRow);
  
  m_pRowCoder_Encode->vEncodeEnd();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // debug
  
//  HV_Rec hvRec;
//  RowCoder::vRecord(hvRec,m_oRowParts,0);
//  hvRec->vPrintRow();

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

/*============================================================================*/
/* JobRow.cpp -- Row part for a Job                                           */
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
void* JobRow::operator new(size_t)
{
  vFatal("fatal: JobRow class cannot be new'd\n"_Stz);
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
JobRow::JobRow(void)
{
  m_pTable = nullptr;
  
  //m_aRecord;
  m_nRowBytes = 0;
  //m_oFormatters;
  //m_oRowParts;

}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
JobRow::~JobRow(void) noexcept(false)
{
}

/*----------------------------------------------------------------------------*/
/*-- Public Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

#ifdef USE_VLQ_DATUMS
/*----------------------------------------------------------------------------*/
/* Format the row                                                             */
/*----------------------------------------------------------------------------*/
bool JobRow::bFormatRow(Stz* aRecord, i8 nBytes, bool &bBlankLine)
{
  // format into subInserters m_aRow
  bool bFormatted = bFormRow(aRecord, nBytes, bBlankLine);
  
  return bFormatted;
}


/*----------------------------------------------------------------------------*/
/*-- Private Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Format the row                                                             */
/*----------------------------------------------------------------------------*/
bool JobRow::bFormRow(Stz* aRecord, i8 nBytes, bool &bBlankLine)
{
  bBlankLine = false;
  
  bool bValidDatum;
  i8 iInt = 0;
  
  char* aSrc = aRecord->aChar();
  //  char caRow[1024];
  //  StrLib::vCopyLen(caRow,aRecord->aChar(),nBytes);
  //  vTrace("Row: %s\n"_Stz,caRow);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // skip leading spaces
  if (m_oFormatters.m_cRecordTerminator)
    StrLib::vSkipWhite(&aSrc);
  else
    StrLib::vSkipSpaces(&aSrc);
  
  // ignore blank lines
  if (! DtmLib::bIsThereRecordData(aSrc,&m_oFormatters) ) {
    bBlankLine = true;
    return false;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  RowEncoder* pRowEncoder = new RowEncoder(&m_oRowParts);
  
  ColumnVec* pColVec = m_pTable->pColumnVec();
  i8 nColumns = m_pTable->pColumnVec()->nItems() - 1;
  
  for (i8 i_Column = 0; i_Column < nColumns; i_Column++) {
    
    Column* pColumn = pColVec->pGet(i_Column);
    eDM eDatum = pColumn->eDatumType();
    bool bLastColumn = (i_Column + 1 == nColumns) ? true : false;

    s256 sDst;
    
    // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
    // extract datum from record
    
    if (eDatum == DM_INT || eDatum == DM_BCD)
      bValidDatum = DtmLib::bExtract(eDatum, (Stz*)&iInt, &aSrc,
                                     pColumn, bLastColumn, &m_oFormatters);
    else
      bValidDatum = DtmLib::bExtract(eDatum, sDst, &aSrc,
                                     pColumn, bLastColumn, &m_oFormatters);
    
    // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
    // is the datum valid ?
    
    if (! bValidDatum) {
      if (! gpAttachMap->m_bMetadata) {
        vWarn("Skipping row - invalid field %s in record: "_Stz,
              pColumn->sColumnName().aChar());
        for (char* a = (char*)aRecord; *a && '\n' != *a && '\r' != *a; a++)
          vTrace("%c"_Stz,*a);
        vTrace("\n"_Stz);
      }
      delete pRowEncoder;
      return false;
    }
    
    // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
    // encode datum into cache line
    
    if (eDatum == DM_INT || eDatum == DM_BCD)
      pRowEncoder->vEncodeDatum(pColumn,(byte*)&iInt);
    else
      pRowEncoder->vEncodeDatum(pColumn,(byte*)sDst.aChar());
    //MemLib::vWordDump(aCacheLine,sizeof(CacheLine));
    //RowEncoder::vPrint(pColumn,aCacheLine);

  } // for each column
  
  delete pRowEncoder;

  return true;
}

#else
/*----------------------------------------------------------------------------*/
/* create a row from given record                                             */
/*----------------------------------------------------------------------------*/
bool JobRow::bFormatRow(Stz* aRecord, i8 nBytes,  bool &bBlankLine)
{
  char* aSrc = aRecord->aChar();
  bBlankLine = false;
  ColumnVec* pColVec = m_pTable->pColumnVec();
  i8 nColumns = pColVec->nItems() - 1;
  
  Stz* pOffset = m_aRecord.aStz();;
  Stz* aDst = pOffset + C_COLUMN_OFFSET_SIZE * nColumns;
  aRowBytes = C_COLUMN_OFFSET_SIZE * nColumns;
  
  char* aStart = aSrc;
  
  // skip leading spaces
  if (m_oFormatters.m_cRecordTerminator) {
    //while (' ' == *aSrc || C_NL == *aSrc || C_CR == *aSrc)
    //  aSrc++;
    StrLib::vSkipWhite(&aSrc);
  }
  else {
    StrLib::vSkipSpaces(&aSrc);
  }
  
  // skip blank lines
  if (! DtmLib::bIsThereRecordData(aSrc,&m_oFormatters) ) {
    bBlankLine = true;
    return false;
  }
  
  // reset back to start
  //aSrc = aStart;
  
  //vTrace("RawData: ]%256s[\n"_Stz,aRawData);
  
  //MemLib::vHexDump(aRawData,256);
  for (i8 i_Column = 0; i_Column < nColumns; i_Column++) {
    
    Column* pColumn = pColVec->pGet(i_Column);
    
    // extract datum type
    eDM eDatumType = pColumn->eDatumType();
    bool bLastColumn = (i_Column + 1 == nColumns) ? true : false;
    bool bValid = DtmLib::bExtract(eDatumType, aDst, &aSrc,
                                   pColumn, bLastColumn,
                                   false,
                                   &m_oFormatters);
    
    // is the datum valid ?
    if ( ! bValid ) {
      if ( ! gpAttachMap->m_bMetadata ) {
        vWarn("Skipping row - invalid field %s in record: "_Stz,
              pColumn->sColumnName().aChar());
        for (char* a = (char*)aRecord; *a && '\n' != *a && '\r' != *a; a++)
          vTrace("%c"_Stz,*a);
        vTrace("\n"_Stz);
      }
      return false;
    }
    
    // save offset
    *((u2*)pOffset->aChar()) = m_nRecordBytes;
    
    i8 iColumnLength = DtmLib::iSize(eDatumType, aDst->aChar());
    
    // move to next offset location
    pOffset += C_COLUMN_OFFSET_SIZE;
    
    // increment row length
    m_nRecordBytes += iColumnLength;
    
    // move to next column location
    aDst += iColumnLength;
  }
  
  //vDump(aRowData, pColVec); // keep as debug
  
  return true;
}
#endif

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

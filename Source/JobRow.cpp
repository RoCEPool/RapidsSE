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
  
#ifdef WITH_ROWBUFFER
  for (i8 i_CacheLine = 0; i_CacheLine < C_CACHELINES; i_CacheLine++) {
    m_xaRow[i_CacheLine].vSetAll(0xFF);
    m_xaRow[i_CacheLine].v8B(0,0);
  }
  
#endif
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
JobRow::~JobRow(void) noexcept(false)
{
}

/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

#ifdef USE_VLQ_DATUMS
/*----------------------------------------------------------------------------*/
/* Format the row                                                             */
/*----------------------------------------------------------------------------*/
bool JobRow::bFormatRow(Stz* aRecord, i8 nBytes, bool &bBlankLine)
{
  // format into subInserters m_aRow
#ifdef WITH_ROWBUFFER
  return bFormatRow(aRecord, nBytes, &m_xaRow[0], m_nRowBytes, bBlankLine);
#else
  return bFormatRow(aRecord, nBytes, m_aRow, m_nRowBytes, bBlankLine);
#endif
}

/*----------------------------------------------------------------------------*/
/* Format the row                                                             */
/*----------------------------------------------------------------------------*/
#ifdef WITH_ROWBUFFER
bool JobRow::bFormatRow(Stz* aRecord, i8 nBytes,
                        CacheLine* aCacheLine, i8 &nDstRowBytes,
                        bool &bBlankLine)
{
  // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
  
  i8 i_CacheOffset = 8;
  
  // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
  char* aSrc = aRecord->aChar();
  bBlankLine = false;
  ColumnVec* pColVec = m_pTable->pColumnVec();
  i8 nColumns = m_pTable->pColumnVec()->nItems() - 1;
  
  aCacheLine->vSetAll(0xFF);
  aCacheLine->v8B(0,0);
  
//  RowEncoding::vEncodeBegin(aCacheLine,i_CacheOffset);
//  char caRow[1024];
//  StrLib::vCopyLen(caRow,aRecord->aChar(),nBytes);
  //vTrace("Row: %s\n"_Stz,caRow);
  
  //vTrace("aCacheLine %p\n"_Stz, aCacheLine);
  
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
    s256 aDst;
    
    // extract datum type
    eDM eDatumType = pColumn->eDatumType();
    bool bLastColumn = (i_Column + 1 == nColumns) ? true : false;
    bool bValid;
    i8 iInt=0;
    
    if (eDatumType == DM_INT ||
        eDatumType == DM_BCD) {
      
      bValid = DtmLib::bExtract(eDatumType, (Stz*)&iInt, &aSrc,
                                pColumn, bLastColumn,
                                &m_oFormatters);
    }
    else {
      bValid = DtmLib::bExtract(eDatumType, aDst, &aSrc,
                                pColumn, bLastColumn,
                                &m_oFormatters);
    }
    
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
    
    // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
    
    if (eDatumType == DM_INT ||
        eDatumType == DM_BCD) {
      RowEncoding::vEncode(pColumn,(byte*)&iInt,aCacheLine,i_CacheOffset);
      //MemLib::vWordDump(aCacheLine,sizeof(CacheLine));
      //RowEncoding::vPrint(pColumn,aCacheLine);
    }
    else {
      RowEncoding::vEncode(pColumn,(byte*)aDst.aChar(),aCacheLine,i_CacheOffset);
      //MemLib::vWordDump(aCacheLine,sizeof(CacheLine));
      //RowEncoding::vPrint(pColumn,aCacheLine);
    }

    // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
  }
  
  return true;
}

#else
bool JobRow::bFormatRow(Stz* aRecord, i8 nBytes, Stz* aDstRow, i8 &nDstRowBytes, bool &bBlankLine)
{
  // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
  CacheLine xCacheLine;
  i8 i_CacheOffset = C_UNKNOWN;
  
  //Datum::vBegin();
  RowEncoding::vEncodeBegin(&xCacheLine,i_CacheOffset);
//  char caRow[1024];
//  StrLib::vCopyLen(caRow,aRecord->aChar(),nBytes);
//  vTrace("Row: %s\n"_Stz,caRow);
  
  // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
  char* aSrc = aRecord->aChar();
  bBlankLine = false;
  ColumnVec* pColVec = m_pTable->pColumnVec();
  i8 nColumns = m_pTable->pColumnVec()->nItems() - 1;
  
  Stz* pOffset = aDstRow;
  Stz* aDst = pOffset + C_COLUMN_OFFSET_SIZE * nColumns;
  nDstRowBytes = C_COLUMN_OFFSET_SIZE * nColumns;
  
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
    bool bValid;
    i8 iInt=0;
    
    if (eDatumType == DM_INT ||
        eDatumType == DM_BCD) {
      
      bValid = DtmLib::bExtract(eDatumType, (Stz*)&iInt, &aSrc,
                                pColumn, bLastColumn,
                                &m_oFormatters);
    }
    else {
      bValid = DtmLib::bExtract(eDatumType, aDst, &aSrc,
                                pColumn, bLastColumn,
                                &m_oFormatters);
    }
    
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
    
    // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
    
    if (eDatumType == DM_INT ||
        eDatumType == DM_BCD) {
      //Datum::vSet(pColumn, (byte*)&iInt);
      
      RowEncoding::vEncode(pColumn,(byte*)&iInt,&xCacheLine,i_CacheOffset);
      MemLib::vWordDump(&xCacheLine,sizeof(CacheLine));
      RowEncoding::vPrint(pColumn,&xCacheLine);
    }
    else {
      //Datum::vSet(pColumn, (byte*)aDst);
      
      RowEncoding::vEncode(pColumn,(byte*)aDst,&xCacheLine,i_CacheOffset);
      MemLib::vWordDump(&xCacheLine,sizeof(CacheLine));
      RowEncoding::vPrint(pColumn,&xCacheLine);
    }

    // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~

    // save offset
    *((u2*)pOffset->aChar()) = nDstRowBytes;
    
    i8 iColumnLength;
    
    if (eDatumType == DM_INT ||
        eDatumType == DM_BCD) {
      iColumnLength = IntLib::iIntToVlq(iInt, (byte*)aDst);
    }
    else {
      iColumnLength = DtmLib::iSize(eDatumType, aDst->aChar());
    }
    
    // move to next offset location
    pOffset += C_COLUMN_OFFSET_SIZE;
    
    // increment row length
    nDstRowBytes += iColumnLength;
    
    // move to next column location
    aDst += iColumnLength;
  }
  
  return true;
}
#endif

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

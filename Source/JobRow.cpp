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
/* get table                                                                  */
/*----------------------------------------------------------------------------*/
Table* JobRow::pTable(void)
{
  Table* pTable = m_oRowParts.pDesc();
  
  return pTable;
}

/*----------------------------------------------------------------------------*/
/* set table                                                                  */
/*----------------------------------------------------------------------------*/
void JobRow::vTable(Table* pTable)
{
  m_oRowParts.vDesc(pTable);
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
  if (! Record::bIsThereRecordData(aSrc,&m_oFormatters) ) {
    bBlankLine = true;
    return false;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Datum oDatum;
  m_oRowParts.vReset();
  
  i8 i_Head = m_oRowParts.iAllotHead();
  ASSERT(0 == i_Head);

  ColumnVec* pColVec = pTable()->pColumnVec();
  i8 nColumns = pColVec->nItems();
  
  for (i8 i_Column = 0; i_Column < nColumns; i_Column++) {
    
    Column* pColumn = pColVec->pGet(i_Column);
    bool bLastColumn = (i_Column + 1 == nColumns) ? true : false;
    
    // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
    // extract datum from record
    
    bValidDatum = Record::bExtract(pColumn, &oDatum, &aSrc,
                                   bLastColumn, &m_oFormatters);
    
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
      //delete pRowEncoder;
      return false;
    }
    
    // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
    // encode datum into cache line
    
    //pDatum->vDatum((byte*)sDst.aChar());
    RowCoder::vEncode(m_oRowParts,pColumn,&oDatum);
    //MemLib::vWordDump(m_oRowParts.aGet(0),sizeof(CacheLine));
    //RowEncoder::vPrint(pColumn,aCacheLine);

  } // for each column
  
  //MemLib::vWordDump(m_oRowParts.aGet(0),sizeof(CacheLine));

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* CsvCursor.cpp -- Csv Cursor                                                */
/*============================================================================*/

#include "Records.h"

/*----------------------------------------------------------------------------*/
/*-- Public Static -----------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* extract row from record (returning first column which has invalid field)   */
/*----------------------------------------------------------------------------*/
eEC CsvCursor::eExtractNext(RowSingle* pRow, StreamDef* pStreamDef,
                             char* azRecord, Format* pFmt)
{
  _static_
  
  char* azCrs = azRecord;
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // leading blanks and blank lines
  
  // skip leading spaces
  if (pFmt->m_cRecordTerminator)
    StrLib::vSkipWhite(&azCrs);
  else
    StrLib::vSkipSpaces(&azCrs);
  
  // ignore blank lines
  if (! bIsThereRecordData(azCrs,pFmt) ) {
    return EC_BLANKLINE;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // debug
  
#ifdef WITH_ENCODING_TRACE
  char caRecord[C_16KB];
  StrLib::vCopyTill(caRecord,acRec,'\n');
  vTrace("\n--------------------------------------------------------------------------\n"_Stz);
  vTrace("\nFormatting record: %s\n"_Stz,caRecord);
#endif
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // extract row from record
  
  Column* pBadCol = CsvRecord::pExtractRow(pRow,pStreamDef,&azCrs,pFmt);
  
  // any invalid field?
  if (pBadCol) {
    if (! AM::bMetadata()) {
      vWarn("Skipping row - invalid field %s in record: "_Stz,
            pBadCol->szColNm());
      for (char* ac = azCrs; ( *ac && '\n' != *ac && '\r' != *ac ); ac++)
        vTrace("%c"_Stz,*ac);
      vTrace("\n"_Stz);
    }
    return EC_INVALID_VALUE;
  }
  
  //SP_Row spRow;
  //vTrace("%s\n"_Stz,spRow.sz(pRow));
  
  if (*azCrs == ' ' || *azCrs == C_NL || *azCrs == C_CR)
    azCrs++;
    
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/*-- Private Static ----------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* is there data?                                                             */
/*----------------------------------------------------------------------------*/
bool CsvCursor::bIsThereRecordData(char* aSrc, Format* pFmt)
{
  _static_
  
  if ( ! *aSrc
      || *aSrc == C_NL
      || *aSrc == C_CR
      )
    return false;
  else
    return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

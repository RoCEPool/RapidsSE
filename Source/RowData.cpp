/*============================================================================*/
/* RowData.cpp -- Row Data Buffer                                             */
/*============================================================================*/

#include "DBobjects.h"

extern BufferPool* gpBufferPool;

/*----------------------------------------------------------------------------*/
/* dump RowData                                                               */
/*----------------------------------------------------------------------------*/
void RowData::vDump(char* aRowData, i8 iRowLength)
{
  
  vTrace("RowData: \n"_Str);
  i8 i_RowLength = 0;
  i8* pSeparator;
  
  while (i_RowLength < iRowLength) {
    
    // get separator
    pSeparator = (i8*)aRowData;
    
    // extract length of next field from separator
    i8 iLength = *pSeparator;
    vTrace("Length %lld : value: "_Str,iLength);
    
    // move rowData nd rowLeghth by i8
    i_RowLength += sizeof(i8);
    aRowData = (char*)pSeparator + sizeof(i8);
    
    // dump the row filed data uoto iLength bytes
    for (i8 i =0; i < iLength; i++)
      vTrace("%c"_Str, *(aRowData+i));
    
    vTrace("\n"_Str);
    
    // move the row pointer ahead
    aRowData += iLength;
    i_RowLength += iLength;
  }

  vTrace("\n"_Str);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* fetch next record from buffer, read next chunk if necessary                */
/*----------------------------------------------------------------------------*/
void RowData::vFormatData(Str* aRawData, i8 iRawLength, Str* aRowData, i8& iRowLength)
{
  char* aSrc = (char*)aRawData->s();
  char* aDst = (char*)aRowData->s();
  
  i8 iColumnLength = 0;
  i8 iChar = 0;
  
  iRowLength = 0;
  while (iChar < iRawLength) {
    
    // pre-field byte length separator format
    // offset-col0-offset-col1...
    i8* pSeparator = (i8*)(aDst);
    
    // find location where to copy first field
    aDst = aDst + sizeof(i8);
    iRowLength += sizeof(i8);
    
    // collect the csv
    iColumnLength = 0;
    while ('|' != *aSrc) {
      iColumnLength++;
      *aDst++ = *(aSrc)++;
      
      iChar++;
    }
    iChar++;
    aSrc++;
    
    // now copy the length in the pre-field byte length separator
    *pSeparator = iColumnLength;
    iRowLength+= iColumnLength;
    
  }
  
  ASSERT('\n' == *aSrc || '|' == *aSrc);
  
  vDumpAscii(aRawData,iRawLength);
  vDump(aRowData->s(), iRowLength);
  
}

/*----------------------------------------------------------------------------*/
/* get pointer to where column starts                                         */
/*----------------------------------------------------------------------------*/
#ifdef _ROWFORMAT
Str* RowData::pColumn(Str* aRowData, i8 iColumnNo, i8& iLength)
{

  i8 iCurrentColumn = 0;
  Str* sCursor = aRowData;
  
  i8 iRowLength = 0;
  iLength = 0;
  while (iCurrentColumn <= iColumnNo) {
    sCursor += iRowLength;
    i8* pSeparator = (i8*)(sCursor);
    iLength = *pSeparator;
    iRowLength = iLength + sizeof(i8);
    iCurrentColumn++;
  }
  
  return sCursor;
}
#else
Str* RowData::pColumn(Str* aRowData, i8 iColumnNo, i8& iLength)
{
  return nullptr;
}
#endif

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

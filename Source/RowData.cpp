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
  
  vTrace("RowData: \n"_Stz);
  i8 i_RowLength = 0;
  char* pSeparator;
  
  while (i_RowLength < iRowLength) {
    
    // get separator
    pSeparator = (char*)aRowData;
    
    // extract length of next field from separator
    i8 iLength = *pSeparator;
    vTrace("Length %lld : value: "_Stz,iLength);
    
    // move rowData nd rowLeghth by char
    i_RowLength += sizeof(char);
    aRowData = (char*)pSeparator + sizeof(char);
    
    // dump the row data upto iLength bytes
    for (i8 i =0; i < iLength; i++)
      vTrace("%c"_Stz, *(aRowData+i));
    
    vTrace("\n"_Stz);
    
    // move the row pointer ahead
    aRowData += iLength;
    i_RowLength += iLength;
  }

  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* fetch next record from buffer, read next chunk if necessary                */
/*----------------------------------------------------------------------------*/
void RowData::vFormatData(Stz* aRawData, i8 iRawLength, Stz* aRowData, i8& iRowLength)
{
  char* aSrc = (char*)aRawData->aChar();
  char* aDst = (char*)aRowData->aChar();
  
  i8 iColumnLength = 0;
  i8 iChar = 0;
  
  iRowLength = 0;
  while (iChar < iRawLength) {
    
    // pre-field byte length separator format
    // offset-col0-offset-col1...
    char* pSeparator = (char*)(aDst);
    
    // find location where to copy first field
    aDst       += sizeof(char);
    iRowLength += sizeof(char);
    
    // collect the csv
    iColumnLength = 0;
    while ('|' != *aSrc && '\n' != *aSrc) {
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
  
  //ASSERT('\n' == *aSrc || '|' == *aSrc);
  
  //vDumpAscii(aRawData,iRawLength);
  vDump(aRowData->aChar(), iRowLength);
  
}

/*----------------------------------------------------------------------------*/
/* get pointer to where column starts                                         */
/*----------------------------------------------------------------------------*/
#ifdef _ROWFORMAT
char* RowData::pColumn(char* aRowData, i8 iColumnNo, i8& iRowLength)
{

  i8 iCurrentColumn = 0;
  char* sCursor = aRowData;
  
  iRowLength = 0;

  while (iCurrentColumn <= iColumnNo) {
    sCursor += iRowLength;
    char* pSeparator = (char*)(sCursor);
    iRowLength = *pSeparator;
    sCursor += sizeof(char);
    iCurrentColumn++;
  }
  
  return sCursor;
}
#else
char* RowData::pColumn(char* aRowData, i8 iColumnNo, i8& iLength)
{
  return nullptr;
}
#endif

/*----------------------------------------------------------------------------*/
/* show RowData                                                               */
/*----------------------------------------------------------------------------*/
void RowData::vGetDataString(s256& sDataString, char* aRowData, i8 iRowLength)
{  
  i8 i_RowLength = 0;
  char* pSeparator;
  char* s = sDataString.aChar();
  
  while (i_RowLength < iRowLength) {
    
    if (i_RowLength)
      *s++ = ',';
    
    // get separator
    pSeparator = (char*)aRowData;
    
    // extract length of next field from separator
    i8 iLength = *pSeparator;
    
    // move rowData nd rowLeghth by char
    i_RowLength += sizeof(char);
    aRowData = (char*)pSeparator + sizeof(char);
    
    // dump the row data upto iLength bytes
    for (i8 i =0; i < iLength; i++)
      *s++ = *(aRowData+i);
    
    // move the row pointer ahead
    aRowData += iLength;
    i_RowLength += iLength;
  }
}

/*----------------------------------------------------------------------------*/
/* show RowData                                                               */
/*----------------------------------------------------------------------------*/
void RowData::vShow(char* aRowData, i8 iRowLength)
{
  
  i8 i_RowLength = 0;
  char* pSeparator;
  
  while (i_RowLength < iRowLength) {
    
    // get separator
    pSeparator = (char*)aRowData;
    
    // extract length of next field from separator
    i8 iLength = *pSeparator;
    vTrace("%lld "_Stz,iLength);
    
    // move rowData nd rowLeghth by char
    i_RowLength += sizeof(char);
    aRowData = (char*)pSeparator + sizeof(char);
    
    // dump the row data upto iLength bytes
    for (i8 i =0; i < iLength; i++)
      vTrace("%c"_Stz, *(aRowData+i));
    
    vTrace(" "_Stz);
    
    // move the row pointer ahead
    aRowData += iLength;
    i_RowLength += iLength;
  }
  
}

/*----------------------------------------------------------------------------*/
/* show RowData                                                               */
/*----------------------------------------------------------------------------*/
void RowData::vShowData(char* aRowData, i8 iRowLength)
{
  
  i8 i_RowLength = 0;
  char* pSeparator;
  
  while (i_RowLength < iRowLength) {
    
    if (i_RowLength)
      vTrace(","_Stz);
    
    // get separator
    pSeparator = (char*)aRowData;
    
    // extract length of next field from separator
    i8 iLength = *pSeparator;
    
    // move rowData nd rowLeghth by char
    i_RowLength += sizeof(char);
    aRowData = (char*)pSeparator + sizeof(char);
    
    // dump the row data upto iLength bytes
    for (i8 i =0; i < iLength; i++)
      vTrace("%c"_Stz, *(aRowData+i));
    
    // move the row pointer ahead
    aRowData += iLength;
    i_RowLength += iLength;
  }
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

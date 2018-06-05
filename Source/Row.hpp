/*============================================================================*/
/* Row.hpp -- row                                                             */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  const i8 C_COLUMN_OFFSET_SIZE = 1;
  
  class Row // is not a proper object
  {
  protected:
  // ..data..   // variable data area

  public:
    void* operator new(size_t);
  
    Row(void);
    ~Row(void);
  
    static bool bFormatRow(ColumnVec* pColVec, Formatters* pFormatters,
                           Stz* aRawData, i8 iRawLength,
                           Stz* aRowData, i8& iRowLength,
                           bool& bBlankLine,
                           bool bAllowNonPrintable);
  

    void vAsBinaryString(s48K& sDataString, ColumnVec* pColVec);
    void vAsBinaryStringVLQ(s48K& sDataString, ColumnVec* pColVec);
    void vAsEscapedString(s48K& sDataString, ColumnVec* pColVec);
    char* aColumnDatum(i8 iColumnNo);
    void vDump(ColumnVec* pColVec);
    void vShow(ColumnVec* pColVec, bool bCompressed);
  };
  
  Row* _pRow(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

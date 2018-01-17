/*============================================================================*/
/* Row.hpp -- row                                                             */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class Row // is not a proper object
  {
  protected:

  public:
    void* operator new(size_t);
  
    Row(void);
    ~Row(void);

    char* aColumnDatum(i8 iColumnNo);
    void vAsBinaryString(s48K& sDataString, ColumnVec* pColVec);
    void vAsEscapedString(s48K& sDataString, ColumnVec* pColVec);
    void vDump(ColumnVec* pColVec);
    bool bFormatData(ColumnVec* pColVec, Formatters* pFormatters,
                     Stz* aRawData, i8 iRawLength, bool& bBlankLine);
    void vShow(ColumnVec* pColVec);
  };
  
  Row* _pRow(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

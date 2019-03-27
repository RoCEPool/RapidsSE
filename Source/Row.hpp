/*============================================================================*/
/* Row.hpp -- Row                                                             */
/*============================================================================*/
#pragma once

namespace Data
{
  static const i8 C_COLUMN_OFFSET_SIZE = 2;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class RowData
  {
  private:
  // u2 .._uOffsetVec[]
  // y1 .._yByteVec[]

  public:
    void* operator new(size_t);
  
    RowData(void);
    ~RowData(void) noexcept(false);
  
    void vAsBinaryString(s48K& sDataString, ColumnVec* pColVec);
    void vAsEscapedString(s48K& sDataString, ColumnVec* pColVec);
    char* aColumnDatum(i8 iColumnNo);
    void vDump(ColumnVec* pColVec);
    void vShow(ColumnVec* pColVec);
    
  private:
    byte* yByteVec(void);
    u2    iDatumOffset(i8 iColumnNo);
    u2*   auOffsetVec(void);
  };
  
  RowData* _aRowData(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

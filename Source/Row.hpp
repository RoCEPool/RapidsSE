/*============================================================================*/
/* Row.hpp -- Row                                                             */
/*============================================================================*/
#pragma once

namespace Data
{
  static const i8 C_COLUMN_OFFSET_SIZE = 2;
  static const i8 C_MAX_RECORD = C_16KB;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class RowData
  {
  protected:
  // ..data..   // variable data area

  public:
    void* operator new(size_t);
  
    RowData(void);
    ~RowData(void) noexcept(false);
  
    void vAsBinaryStringVLQ(s48K& sDataString, ColumnVec* pColVec);
    void vAsEscapedString(s48K& sDataString, ColumnVec* pColVec);
    char* aColumnDatum(i8 iColumnNo);
    void vDump(ColumnVec* pColVec);
    void vShow(ColumnVec* pColVec, bool bCompressed);
    
  public:
    u2 iColumnOffset(i8 iColumnNo);
  };
  
  RowData* _aRowData(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

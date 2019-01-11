/*============================================================================*/
/* Row.hpp -- Row                                                             */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  static const i8 C_COLUMN_OFFSET_SIZE = 2;
  static const i8 C_MAX_RECORD = C_16KB;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Row // is not a proper object
  {
  protected:
  // ..data..   // variable data area

  public:
    void* operator new(size_t);
  
    Row(void);
    ~Row(void) noexcept(false);
  
    void vAsBinaryString(s48K& sDataString, ColumnVec* pColVec);
    void vAsBinaryStringVLQ(s48K& sDataString, ColumnVec* pColVec);
    void vAsEscapedString(s48K& sDataString, ColumnVec* pColVec);
    char* aColumnDatum(i8 iColumnNo);
    void vDump(ColumnVec* pColVec);
    void vShow(ColumnVec* pColVec, bool bCompressed);
    
  public:
    u2 iColumnOffset(i8 iColumnNo);
  };
  
  Row* _pRow(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

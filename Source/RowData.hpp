/*============================================================================*/
/* RowData.hpp -- Row Data helper functions                                   */
/*============================================================================*/
#pragma once

namespace Structures
{
  const i8 C_COLUMN_OFFSET_SIZE = 1;

  class RowData
  {
  private:
    
  public:

    static char* pColumn(char* aRowData, i8 iColumnNo);
    static void vDump(Stz* aRowData, ColumnVec* pColVec);
    static bool bFormatData(ColumnVec* pColVec, Stz* aRawData, i8 iRawLength, Stz* aRowData, i8& iRowLength);
    static void vRowAsString(s48K& sDataString, Stz* aRowData, ColumnVec* pColVec);
    static void vShow(Stz* aRowData, ColumnVec* pColVec);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* RowData.hpp -- Row Data helper functions                                   */
/*============================================================================*/
#pragma once

namespace Structures
{
  
  class RowData
  {
  private:
  
  public:

    static Str* pColumn(Str* aRowData, i8 iColumnNo, i8& iLength);
    static void vDump(char* aRowData, i8 iRowLength);
    static void vFormatData(Str* aRawData, i8 iRawLength, Str* aRowData, i8& iRowLength);
    static void vGetDataString(s256& sDataString, char* aRowData, i8 iRowLength);
    static void vShow(char* aRowData, i8 iRowLength);
    static void vShowData(char* aRowData, i8 iRowLength);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

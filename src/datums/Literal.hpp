/*============================================================================*/
/* Literal.hpp -- Literal Value                                               */
/*============================================================================*/
#pragma once

namespace Datums
{
  
  class Literal
  {
  public:
    static bool bToCell(Dtm* aCell, Column* pCol,
                        char** aazCrs, Format* pFmt);

    static void vSprint(Stz* szTgt, Dtm* avSrc, Column* pCol);

   private:
    static bool bIsSomeNull(char** aazCrs, Format* pFmt, bool bSkipTrailingSpaces);
    static bool bIsEnclosedString(char** aazCrs, Format* pFmt);
    static bool bIsThereFieldData(char* aSrc, Format* pFmt);
  
    static bool bToCellBcd(Dtm* aCell, Column* pCol,
                           char** aazCrs, Format* pFmt);
    static bool bToCellBol(Dtm* aCell, Column* pCol,
                           char** aazCrs, Format* pFmt);
    static bool bToCellDte(Dtm* aCell, Column* pCol,
                           char** aazCrs, Format* pFmt);
    static bool bToCellFlt(Dtm* aCell, Column* pCol,
                           char** aazCrs, Format* pFmt);
    static bool bToCellInt(Dtm* aCell, Column* pCol,
                           char** aazCrs, Format* pFmt);
    static bool bToCellIvl(Dtm* aCell, Column* pCol,
                           char** aazCrs, Format* pFmt);
    static bool bToCellTme(Dtm* aCell, Column* pCol,
                           char** aazCrs, Format* pFmt);
    static bool bToCellTsp(Dtm* aCell, Column* pCol,
                           char** aazCrs, Format* pFmt);
    static bool bToCellVch(Dtm* aCell, Column* pCol,
                           char** aazCrs, Format* pFmt);

    static bool bValidFieldEnd(char* aSrc, Format* pFmt);
    static bool bValidRecordEnd(char* aSrc, Format* pFmt);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

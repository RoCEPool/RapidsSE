/*============================================================================*/
/* RowEncoding.hpp -- Row Encoding                                            */
/*============================================================================*/
#pragma once

namespace Data
{
  class RowEncoding
  {
  public:
  
  public:
    static i8 iEncodeBcd(Column* pColumn,byte* aDatum, WordView& wEncoded);
    static i8 iEncodeBol(Column* pColumn,byte* aDatum, WordView& wEncoded);
    static i8 iEncodeDte(Column* pColumn,byte* aDatum, WordView& wEncoded);
    static i8 iEncodeFlt(Column* pColumn,byte* aDatum, WordView& wEncoded);
    static i8 iEncodeInt(Column* pColumn,byte* aDatum, WordView& wEncoded);
    static i8 iEncodeTme(Column* pColumn,byte* aDatum, WordView& wEncoded);
    static i8 iEncodeTsp(Column* pColumn,byte* aDatum, WordView& wEncoded);
    static i8 iEncodeVch(Column* pColumn,byte* aDatum, WordView& wEncoded);
 };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

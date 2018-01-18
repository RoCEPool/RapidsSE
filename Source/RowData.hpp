/*============================================================================*/
/* RowData.hpp -- Row Data helper functions                                   */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  const i8 C_COLUMN_OFFSET_SIZE = 1;

  class RowData
  {
  private:
    
  public:

    static char* aColumnDatum(char* aRowData, i8 iColumnNo);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

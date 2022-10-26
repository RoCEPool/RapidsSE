/*============================================================================*/
/* Xcreate.hpp --                                                       */
/*============================================================================*/
#pragma once

#include "Xtest.hpp"

namespace STORE
{
  class Table;

  class Xcreate : public Xtest
  {
  public:

    Card_IndexList* A_pProduce_indexList_ofTable(Table* pTable);
    Card_Size*      A_pProduce_size_ofTable(Table* pTable);
    Card_Stream*    A_pProduce_stream_ofTable(Table* pTable);
    Card_TablePtn*  A_pProduce_tablePtn_ofTable(Table* pTable, char cTableSyn);

    Card_BoolCalc*  X_pConvert_rpnCalc_To_boolCalc(Card_RpnCalc* p_rpnCalc);

    // must be last
    Footer m_xFooter;

  public:
    Xcreate(eCN eClass);
   ~Xcreate(void) noexcept(false);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

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

    Card_BoolCalc*   C_pConvert_rpnCalc_To_boolCalc(Card_RpnCalc* p_rpnCalc);

    Card_StmColList* C_pDerive_streamColList(Card_ELIST* p_ELIST);

    Card_IndexList*  C_pProduce_indexList_ofTable(Table* pTable);
    Card_Size*       C_pProduce_size_ofTable(Table* pTable);
    Card_Stream*     C_pProduce_stream_ofTable(Table* pTable);
    Card_TablePtn*   C_pProduce_tablePtn_ofTable(Card_Table* p_table);

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

/*============================================================================*/
/* TransformBase.hpp --                                                       */
/*============================================================================*/
#pragma once

#include "Bench.hpp"
#include "BoardIter.hpp"


namespace STORE
{
  class Table;

  class TransformBase : public Object
  {
  public:

    // must be last
    Footer m_xFooter;


  public:
    void* operator new(size_t);
    void  operator delete(void*);

    TransformBase(eCN eClass);
   ~TransformBase(void) noexcept(false);

    Table* A_pAccess_Table(Stz* szTableNm);

    Card_IndexList* A_pProduce_indexList_ofTable(Table* pTable);
    Card_Size*      A_pProduce_size_ofTable(Table* pTable);
    Card_Stream*    A_pProduce_stream_ofTable(Table* pTable);
    Card_TablePtn*  A_pProduce_tablePtn_ofTable(Table* pTable, char cTableSyn);

    Footer* aFooter(void);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

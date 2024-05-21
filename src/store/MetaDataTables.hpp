/*============================================================================*/
/* MetaDataTables.hpp -- MetaData Tables                                      */
/*============================================================================*/
#pragma once

#include "eMD.h"

namespace STORE
{
  class MetaDataTables : public Object
  {
  private:

    StreamDef* m_apStreamDef[MD_ENUMS];

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    MetaDataTables(void);
    ~MetaDataTables(void) noexcept(false);

    static bool bDeleteFromTableOfTables(Stz* szTableNm);
    static void vDeleteAllRowsFromTable(Table* pTable, i8 iCol, Stz* szColVal);
    static void vDeleteRowFromTable(Table* pTable, i8 iCol1, Stz* szCol1Val, i8 iCol2, Stz* szCol2Val);
    static Table* pTableFind(Stz* szTableNm);

    StreamDef* pStreamDef(eMD eTable);
    void vStreamDef(eMD eTable, StreamDef* pStreamDef);

  private:

    Footer* aFooter(void);
  };

  MetaDataTables* _pMetaDataTables(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* Loader3.hpp -- Table Loader                                                */
/*============================================================================*/

#pragma once

#include "DBobjects.h"

namespace DBobjects
{
  class Loader3 : public Object
  {
  private:
    Table* m_pTable;   // table being load
    File   m_xFile;    // from which file
    i8     m_iSeek;    // seek before next read
    TxRef* m_pTxRef;   // tx
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, Loader3* pLoader3);
    void  operator delete(void* pLoader3Address);
 
    Loader3(void);
    ~Loader3(void);
    
    static void vDump(eCN eClass, Object* pObject, const char* pTab);
    
    Table* pTable(void);
    void   vTable(Table* pTable);
    
    void   vPath(s& sPath);
    void   vTxRef(TxRef* pTxRef);
    
    bool   bOpenFile(void);
    void   vBegin(void);
    bool   bFetchChunk(LoadBuffer* pLoadBuffer);
    bool   bInsertRow(i8 iPartn, StrVal* pRecord);
    void   vEnd(void);
    void   vCloseFile(void);
    
    Footer* pFooter(void);
  };
  
  Loader3* _pLoader3(void* pAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
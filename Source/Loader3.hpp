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
    
    Lock   m_xLock;    // lock
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, Loader3* pLoader3);
    void  operator delete(void* pLoader3Address);
 
    Loader3(void);
    ~Loader3(void);
    
    static void vDump(eCN eClass, Object* pObject, const char* pTab);
    
    void   vCloseFile(void);
    bool   bOpenFile(void);
    void   vPath(s& sPath);
    Table* pTable(void);
    void   vTable(Table* pTable);
    void   vTxRef(TxRef* pTxRef);    
    
    bool   bFetchChunk(LoadBuffer* pLoadBuffer);
    bool   bInsertRow(i8 iPartn, StrVal* pRecord);

    Footer* pFooter(void);
  };
  
  Loader3* _pLoader3(void* pAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
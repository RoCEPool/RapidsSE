/*============================================================================*/
/* Loader2.hpp -- Table Loader                                                */
/*============================================================================*/

#pragma once

#include "DBobjects.h"

namespace DBobjects
{
  class Loader2 : public Object
  {
  private:
    Table* m_pTable;   // table being load
    File   m_xFile;    // from which file
    i8     m_iSeek;    // seek before next read
    char*  m_pRecord;  // record from file
    i8     m_iLength;  // length of record
    TxRef* m_pTxRef;   // tx
    
    LoadBuffer* m_pLoadBuffer; // loading buffer
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, Loader2* pLoader2);
    void  operator delete(void* pLoader2Address);
 
    Loader2(void);
    ~Loader2(void);
    
    static void vDump(eCN eClass, Object* pObject, const char* pTab);
    
    Table* pTable(void);
    void   vTable(Table* pTable);
    
    void   vPath(s& sPath);
    void   vTxRef(TxRef* pTxRef);
    
    bool   bOpenFile(void);
    void   vBegin(void);
    bool   bFetchRecord(void);
    bool   bFetchChunk(void);
    void   vGetRecord(StrVal* pStrVal);
    bool   bInsertRow(i8 iPartn);
    void   vEnd(void);
    void   vCloseFile(void);
    
    Footer* pFooter(void);
  };
  
  Loader2* _pLoader2(void* pAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
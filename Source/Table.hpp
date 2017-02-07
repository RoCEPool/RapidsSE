/*============================================================================*/
/* Table.hpp - Table                                                          */
/*============================================================================*/

#pragma once

#include "DBobjects.h"

namespace DBobjects
{  
  enum eTT // Table Type
  {
    /* 0 */ TT_NONE=0,
    /* 1 */ TT_NORMAL
  };

  static const char* const pTT[]
  {
    /* 0 */ "NONE",
    /* 1 */ "NORMAL"
  };
    
  class Table : public Object
  {
  private:
    s32 m_sTableName;
    eTT m_eTableType;
    
    Dic   m_xSubDic;
  //Ring  m_xRing;
    Slots m_xSlots;
    
    Atomic m_zRefCount;    // reference count
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, Table* pTable);
    void  operator delete(void*);
    
    Table(void);
    Table(const char* pTableName);
    ~Table(void);
    
    static void vDump(eCN eClass, Object* pObject, const char* pTab);
    
    void vAnalyze(void);
    void vAppendBuffer(i8 iPartition, RowBuffer* pRowBuffer);
    Dic* pSubDic(void);
    void vInsertRow(i8 iPartition, TxRef* pTxRef, char* pRecord, i8 iLength);
    i8   nRowBuffers(void);
    void vTruncate(void);
        
    char* pTableName(void);
    void  vTableName(s& sName);
    void  vContentDump(void);
    
    void  vRefDec(void);
    void  vRefInc(void);
    bool  bRefOut(void);
    
    
    //--------------------------------------------------------------------------
    
    Partition* pPartition(i8 iSlot);
    
    Footer* pFooter(void);
  };
  
  Table* _pTable(void* pAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

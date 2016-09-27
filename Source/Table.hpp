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
    
    Dictionary m_xDictionary;
    //Ring       m_xRing;
    Partitions m_xPartitions;
        
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
    
    Dictionary* pDic(void);
    void vInsertRow(i8 iPartition, TxRef* pTxRef, char* pRecord, i8 iLength);
    i8   iRowBuffers(void);
    void vTruncate(void);
        
    char* pTableName(void);
    void  vTableName(s& sName);
    void  vDump(void);
    
    //--------------------------------------------------------------------------
    
    Partition* pPartition(i8);
    
    Footer* pFooter(void);
  };
  
  Table* _pTable(void* pAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
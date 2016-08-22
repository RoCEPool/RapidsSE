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

  static const char *pTT[]
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
    
    Ring m_xRing;
    Partitions m_xPartitions;
        
  public:
    void* operator new(size_t);
    void* operator new(size_t, Table* pTable);
    void  operator delete(void*);
    
    Table(void);
    Table(const char* pTableName);
    ~Table(void);
    
    void vCopy(Table* pTable);
    Dictionary* pDic(void);
    void vExport(File* pFile);
    SegmentBuffer* pFirstBuffer(void);
    void vInsertRow(StrVal* pRow);
    void vInsertRow2(StrVal* pRow);
    void vLoad(File* pFile);
    void vLoad2(File* pFile);
    void vList(void);
    i8   iRowBuffers(void);
    void vTruncate(void);
        
    char* pTableName(void);
    void  vTableName(s& sName);
    void  vDump(void);
    
    //--------------------------------------------------------------------------
    
    Partition* pPartition(i8);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
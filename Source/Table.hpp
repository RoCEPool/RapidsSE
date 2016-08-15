/*============================================================================*/
/* Table.hpp - Table                                                          */
/*============================================================================*/

#pragma once

#include "DBobjects.h"

namespace DBobjects {
  
  enum eTT // Table Type
  {
    /* 0 */ TT_NONE=0,
    /* 1 */ TT_XYZ
  };

  static const char *pTT[]
  {
    /* 0 */ "NONE",
    /* 1 */ "XYZ"
  };
    
  class Table : public Segment {
    
  private:
    i8    m_iTxId;
    s32   m_sTableName;
    eTT   m_eTableType;
    Row*  m_pCurrentRow;
    i8    m_nParitionMgr;
    i8    m_nColumnMgr;
    i8    m_nIndexMgr;
        
  public:
    void* operator new(size_t);
    void* operator new(size_t, Table* pTable);
    void  operator delete(void*);
    
    Table(void);
    Table(i8 iTxId, const char* pTableName);
    ~Table(void);
    
    void vCopy(Table* pTable);
    void vExport(File* pFile);
    SegmentBuffer* pFirstBuffer(void);
    void vInsertRow(StrVal* pRow);
    void vLoad(File* pFile);
    void vList(void);
    void vTruncate(void);
    
    i8   iTxId(void) const;
    void vTxId(i8 iTxId);
    
    char* pTableName(void);
    void  vTableName(s& sName);
    void  vDump(void);
  };
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
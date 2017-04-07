/*============================================================================*/
/* Table.hpp - Table                                                          */
/*============================================================================*/
#pragma once

namespace DBobjects
{  
  enum eTT // Table Type
  {
    /* 0 */ TT_NONE=0,
    /* 1 */ TT_NORMAL
  };

  static Str* aTT[]
  {
    /* 0 */ "NONE"_Str,
    /* 1 */ "NORMAL"_Str
  };
    
  class Table : public Object
  {
  private:
    s32    m_sTableName; // table name
    eTT    m_eTableType; // table type
    Dic    m_xSubDic;    // sub dictionary for this table
  //Ring   m_xRing;
    Slots  m_xSlots;     // slots
    Atomic m_zRefCount;  // reference count

    ColumnVec* m_pColumnVec; // colummn vector
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, Table* pTable);
    void  operator delete(void*);
    
    Table(void);
    Table(Str* aTableName);
    ~Table(void);
    
    static void vDump(eCN eClass, Object* pObject, Str* aTab);
    
    Str* pAnalyze(bool bDetailed);
    void vAppendBuffer(i8 iPartition, RowBuffer* pRowBuffer);
    ColumnVec* pColumnVec(void);
    i8   iRowPartition(Str* pRow);
    Dic* pSubDic(void);
    void vInsertRow(i8 iPartition, TxRef* pTxRef, Str* pRecord, i8 iLength);
    i8   nRowBuffers(void);
    void vTruncate(void);
        
    Str*  aTableName(void);
    void  vTableName(StrVal& sName);
    void  vContentDump(void);
    
    void  vRefDec(void);
    void  vRefInc(void);
    bool  bRefOut(void);
    
    
    //--------------------------------------------------------------------------
    
    Partition* pPartition(i8 iSlot);
    
    Footer* pFooter(void);
  };
  
  Table* _pTable(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

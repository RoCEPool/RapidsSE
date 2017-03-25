/*============================================================================*/
/* Index.hpp - index portion of table partition                               */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  enum eIT // Index Type
  {
    /* 0 */ IT_NONE=0,
    /* 1 */ IT_SKIP_LIST
  };
  
  static Str* aIT[]
  {
    /* 0 */ "NONE"_Str,
    /* 1 */ "SKIP_LIST"_Str
  };

  class Table;
  class Partition;
  
  class Index : public Object
  {
  private:
    i8           m_iIndexNo;
    s32          m_sIndexName;
    eIT          m_eIndexType;
    Table*       m_pTable;
    IndexLevels* m_pIndexLevels;
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, Index* pIndex);
    void  operator delete(void*);
    
    static void vDump(eCN eClass, Object* pObject, Str* aTab);
    
    Index(void);
    ~Index(void);
  
    void vDump(void);
    IndexBufferSeq* pFirstBuffer(void);
    void vPopulate(void);
    void vTable(Table* pTable);
    void vTruncate(void);
    
    Footer* pFooter(void);
  };
  
  Index* _pIndex(void* pAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

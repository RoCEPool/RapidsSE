/*============================================================================*/
/* Index.hpp - index portion of table partition                               */
/*============================================================================*/
#pragma once

namespace DBobjects
{

  class Table;
  class Partition;
  class Index;
  
  class IndexData : public Object
  {
  private:
    Index*       m_pIndex;
    IndexLevels* m_pIndexLevels;
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, IndexData* pIndexData);
    void  operator delete(void*);
    
    static void vDump(eCN eClass, Object* pObject, Str* aTab);
    static void vFinalize(eCN eClass, Object* pObject);
    
    IndexData(Index* pIndex);
    ~IndexData(void);
  
    Str*   pAnalyze(void);
    void   vDump(void);
    Index* pIndex(void);
    void   vInsertEntry(Row* pRow, i8 nRowNo, bn nBufferNo);
    void   vPopulate(void);
    void   vTidyUp(void);
    void   vTruncate(void);
  
    Footer* pFooter(void);
  };
  
  IndexData* _pIndexData(void* pAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

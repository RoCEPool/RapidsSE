/*============================================================================*/
/* TableRef.hpp - a named table                                               */
/*============================================================================*/
#pragma once

namespace Parser
{  
  class TableRef : public Object
  {
  private:
    Text* m_pCatalogName;
    Text* m_pSchemaName;
    Text* m_pTableName;
  
    Footer m_xFooter;
    
   public:
    
    void* operator new(size_t);
    void  operator delete(void*);
    
    TableRef(Text* pCatalogName, Text* pSchemaName, Text* pTableName);
    ~TableRef(void);
    
    void vPrintInto(s256& buf);
  
    Footer* aFooter(void);
  };
  
  TableRef* _pTableRef(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

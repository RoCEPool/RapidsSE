/*============================================================================*/
/* ExprColumnRef.hpp - an reference to a column name.                         */
/*============================================================================*/
#pragma once

namespace Parser
{  
  class ExprColumnRef : public ExprAtom
  {
  private:
    Text*  m_pCatalogName;
    Text*  m_pSchemaName;
    Text*  m_pTableName;
    Text*  m_pColumnName;
    Footer m_xFooter;
    
   public:
    
    void* operator new(size_t);
    void  operator delete(void*);
    
    ExprColumnRef(Text* pCatalogName, Text* pSchemaName,
                  Text* pTableName, Text* pColumnName);
    virtual ~ExprColumnRef(void);
    
    Footer* aFooter(void);
    
    virtual void vDescribe(s2K& buf) override;
  };
  
  ExprColumnRef* _pExprColumnRef(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

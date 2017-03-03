/*============================================================================*/
/* Index.hpp - index portion of table partition                               */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class Index : public Object
  {
  private:

    IndexDefn*   m_pIndexDefn;  
    IndexLevels* m_pIndexLevels;
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, Index* pIndex);
    void  operator delete(void*);
    
    static void vDump(eCN eClass, Object* pObject, const char* aTab);
    
    Index(void);
    ~Index(void);
    
    void vIndexDefn(IndexDefn* pIndexDefn);
    
    Footer* pFooter(void);
  };
  
  Index* _pIndex(void* pAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

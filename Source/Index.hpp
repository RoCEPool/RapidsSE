/*============================================================================*/
/* Index.hpp - index portion of table partition                               */
/*============================================================================*/

#pragma once

#include "DBobjects.h"

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
    void  operator delete(void*);
    
    static void vDump(eCN eClass, Object* pObject, const char* aTab);
    
    Index(void);
    ~Index(void);
    
    Footer* pFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

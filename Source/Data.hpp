/*============================================================================*/
/* Data.hpp - data portion of table partition                                 */
/*============================================================================*/

#pragma once

#include "DBobjects.h"

namespace DBobjects
{
  class Data : public Object
  {
  private:
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    static void vDump(eCN eClass, Object* pObject, const char* pTab);
    
    Data(void);
    ~Data(void);
    
    Footer* pFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

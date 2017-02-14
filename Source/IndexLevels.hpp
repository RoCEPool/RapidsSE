/*============================================================================*/
/* IndexLevels.hpp - set of index buffers by level for an index partition     */
/*============================================================================*/

#pragma once

#include "DBobjects.h"

namespace DBobjects
{
  class IndexLevels : public Object
  {
  private:
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    static void vDump(eCN eClass, Object* pObject, const char* pTab);
    
    IndexLevels(void);
    ~IndexLevels(void);
    
    Footer* pFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

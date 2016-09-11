/*============================================================================*/
/* Index.hpp - Index                                                          */
/*============================================================================*/

#pragma once

#include "DBobjects.h"

namespace DBobjects
{
  enum eIT // Index Type
  {
      /* 0 */ IT_NONE=0,
      /* 1 */ IT_XYZ
  };
  
  static const char *pIT[]
  {
    /* 0 */ "NONE",
    /* 1 */ "SKIP_SEQUENTIAL"
  };

  class Index : public Object
  {
  private:
    i8    m_iIndexNo;
    s32   m_sIndexName;
    eIT   m_eIndexType;
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    static void vDump(eCN eClass, Object* pObject, const char* pTab);
    
    Index(const char* pIndexName);
    ~Index(void);
    
    char* pIndexName(void);
    Footer* pFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

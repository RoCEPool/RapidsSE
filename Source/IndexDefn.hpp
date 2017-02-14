/*============================================================================*/
/* IndexDefn.hpp - index definition                                           */
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
  
  static const char* const pIT[]
  {
    /* 0 */ "NONE",
    /* 1 */ "SKIP_SEQUENTIAL"
  };

  class IndexDefn : public Object
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
    
    IndexDefn(const char* pIndexName);
    ~IndexDefn(void);
    
    char* pIndexName(void);
    Footer* pFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* Index.hpp - index portion of table partition                               */
/*============================================================================*/

#pragma once

#include "DBobjects.h"

namespace DBobjects
{
  enum eIT // Index Type
  {
    /* 0 */ IT_NONE=0,
    /* 1 */ IT_SKIP_LIST
  };
  
  static const char* const pIT[]
  {
    /* 0 */ "NONE",
    /* 1 */ "SKIP_LIST"
  };

  class Index : public Object
  {
  private:

    i8           m_iIndexNo;
    s32          m_sIndexName;
    eIT          m_eIndexType;
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
  
    Footer* pFooter(void);
  };
  
  Index* _pIndex(void* pAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

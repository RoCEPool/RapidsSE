/*============================================================================*/
/* Index.hpp - Index                                                          */
/*============================================================================*/

#pragma once

#include "DBobjects.h"

namespace DBobjects {
  
enum eIT // Index Type
{
    /* 0 */ IT_NONE=0,
    /* 1 */ IT_XYZ
};

class Index : public Object {

private:
  i8    m_iTxId;
  i8    m_iIndexNo;
  s32   m_sIndexName;
  eIT   m_eIndexType;
  i8    m_nIndexColumnMgr;
  i8    m_nIndexMgr;
  
public:
  void* operator new(size_t);
  void  operator delete(void*);
  
  Index(i8 iTxId, const char* pIndexName);
  ~Index(void);
  
  i8   iTxId(void) const;
  void vTxId(i8 iTxId);
  
  char* pIndexName(void);
  void vDump(void);
};
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

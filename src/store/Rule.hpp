/*============================================================================*/
/* Rule.hpp -- Abstract Rule                                                  */
/*============================================================================*/
#pragma once

#include "BASE.h"
#include "eRK.h"

namespace STORE
{
  class Rule : public Object
  {
  protected:
    eRK  m_eRule;

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Rule(eRK eRule);
   ~Rule(void) noexcept(false);

    eRK  eRule(void);

    // ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
  protected:
    Footer* aFooter(void);
  };

  Rule* _pRule(void* aAddr);
}

/*----------------------------------------------------------------------------*/
/* RASSERT_RULE macro                                                         */
/*----------------------------------------------------------------------------*/
#define RASSERT_RULE(pOBJECT,eRULE) \
  \
  if (pOBJECT && CN_RULE != pOBJECT->eGetClassNo()) \
    BASE::vThrowType(FK_TYPE, \
                     __func__,__FILE__,__LINE__, \
                     pOBJECT,CN_RULE); \
  \
  if (pOBJECT && eRULE != ((Rule*)pOBJECT)->eRule()) \
    BASE::vThrowType(FK_TYPE,\
                     __func__,__FILE__,__LINE__, \
                     pOBJECT,CN_RULE); \
  \
EndDefine

/*----------------------------------------------------------------------------*/
/* macro for standard safe cast function                                      */
/*----------------------------------------------------------------------------*/
#define __pCastFunction_Rule(SPACENM,CLASSNM,RK) \
  CLASSNM* SPACENM::_p##CLASSNM(void* aAddr) \
  { \
    CLASSNM* pObject = reinterpret_cast<CLASSNM*>(aAddr); \
    \
    RASSERT_RULE(pObject,RK); \
    \
    return pObject; \
  } \
EndDefine

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

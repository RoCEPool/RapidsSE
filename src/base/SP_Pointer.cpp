/*============================================================================*/
/* SP_Pointer.cpp - Pointer sprint helper                        (on c stack) */
/*============================================================================*/

#include "BASE.h"

#include "SP_Pointer.hpp"

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SP_Pointer::SP_Pointer(void)
: SP(CN_SPID)
{
  _precondition_(_MAGIC_,CN_SPID,_FOOTER_);
  
  _checkInitialised_(CN_SPID);
  
  _postcondition_(_MAGIC_,CN_SPID,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SP_Pointer::~SP_Pointer(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_SPID,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* test and sprint pointer details                                            */
/*----------------------------------------------------------------------------*/
Stz* SP_Pointer::sz(Object* pPointer)
{
  _precondition_(_MAGIC_,CN_SPID,_FOOTER_);
    
  if (! pPointer) {
    StrLib::vSprint(m_caResult,"nullptr"_Stz);
    return (Stz*) &m_caResult[0];
  }
  
  eEC eError = AM::OBJECT::eValidObject(pPointer);
  if (EC_NONE != eError) {
    StrLib::vSprint(m_caResult
                    ,"Pointer %p INVALID eError=%lld %s"_Stz
                    
                    ,pPointer
                    ,(i8)eError
                    ,aEC[eError]
                    );
    return (Stz*) &m_caResult[0];
  }

  StrLib::vSprint(m_caResult
                  ,"Pointer %p class %lld %lls"_Stz
                  
                  ,pPointer
                  ,pPointer->eGetClassNo()
                  ,pPointer->szClassNm()
                 );
  
  return (Stz*) &m_caResult[0];
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(SP_Pointer)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

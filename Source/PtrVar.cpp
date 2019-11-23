/*============================================================================*/
/* PtrVar.cpp - Pointer Sen Variable                                          */
/*============================================================================*/

#include "SenVM.h"

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* PtrVar::operator new(size_t)
{
  vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
         "fatal: PtrVar class cannot be new'd\n"_Stz);
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
PtrVar::PtrVar(void)
: Object(CN_PTRVAR)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTRVAR,_FOOTER_);

  m_pObj = nullptr;

  _checkInitialised_(CN_PTRVAL);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_PTRVAR,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                */
/*----------------------------------------------------------------------------*/
PtrVar::~PtrVar(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTRVAR,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* assign a value                                                             */
/*----------------------------------------------------------------------------*/
void PtrVar::operator =(Object* pObject)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTRVAR,_FOOTER_);
  
  m_pObj = pObject;
}

/*----------------------------------------------------------------------------*/
/* as integer                                                                 */
/*----------------------------------------------------------------------------*/
i8 PtrVar::iAsInt(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTRVAR,_FOOTER_);
  
  return (i8)m_pObj;
}

/*----------------------------------------------------------------------------*/
/* get pointer to proper object                                               */
/*----------------------------------------------------------------------------*/
Object* PtrVar::pObj(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTRVAR,_FOOTER_);
  
  return pPoolCheck<Object*>(m_pObj);
}

/*----------------------------------------------------------------------------*/
/* get size in bytes                                                          */
/*----------------------------------------------------------------------------*/
i8 PtrVar::nSize(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTRVAR,_FOOTER_);
  
  return sizeof(PtrVar);
}

/*----------------------------------------------------------------------------*/
/* convert to string                                                          */
/*----------------------------------------------------------------------------*/
void PtrVar::vToString(Stz* szStz)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTRVAR,_FOOTER_);

  * szStz = reinterpret_cast<void*>(m_pObj);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

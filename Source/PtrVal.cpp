/*============================================================================*/
/* PtrVal.cpp - pointer value                                                 */
/*============================================================================*/

#include "Values.h"
#include "Dictionaries.h"

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* PtrVal::operator new(size_t iSize)
{
  vFatal("fatal: PtrVal class cannot be new'd\n");
  
  //vExitFatal();
  
  exit(EXIT_FAILURE);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
PtrVal::PtrVal(void)
: Value(CN_PTRVAL)
{
  m_pPtr = nullptr;
}

/*----------------------------------------------------------------------------*/
/* assign a value                                                             */
/*----------------------------------------------------------------------------*/
void PtrVal::operator =(void* pPtr)
{
  m_pPtr = pPtr;
}

/*----------------------------------------------------------------------------*/
/* get size in bytes                                                          */
/*----------------------------------------------------------------------------*/
i8 PtrVal::iSize(void) const
{
  return sizeof(PtrVal);
}

/*----------------------------------------------------------------------------*/
/* get pVal                                                                   */
/*----------------------------------------------------------------------------*/
void* PtrVal::pPtr(void) const
{
  return pPoolCheck<void*>(m_pPtr);
}

/*----------------------------------------------------------------------------*/
/* as pointer to in memory pool object                                                              */
/*----------------------------------------------------------------------------*/
Object* PtrVal::pObject(void) const
{
  return pPoolCheck<Object*>(m_pPtr);
}

/*----------------------------------------------------------------------------*/
/* fetch value at pointer                                                     */
/*----------------------------------------------------------------------------*/
void PtrVal::vFetch(ValWsp& xValWsp)
{
  Value* pValue = reinterpret_cast<Value*>(m_pPtr);
  
  eCN eClass = pValue->eClass();
  
  switch (eClass) {
    case CN_INTVAL:
      xValWsp = pValue->pAsIntVal()->iInt();
      break;
      
    case CN_STRVAL:
      xValWsp = pValue->pAsStrVal()->sStr();
      break;
      
    case CN_PTRVAL:
      xValWsp = pValue->pAsPtrVal()->pPtr();
      break;
      
    case CN_WRDVAL:
      xValWsp = pValue->pAsWrdVal()->pWord();
      break;
      
    default:
      ASSERT(0);
  }
}

/*----------------------------------------------------------------------------*/
/* store value at pointer                                                     */
/*----------------------------------------------------------------------------*/
void PtrVal::vStore(ValWsp& xValWsp)
{
  // m_pPtr should point to a value, same as xValWsp
  Value* pValue = reinterpret_cast<Value*>(m_pPtr);

  ASSERT(pValue->eClass() == xValWsp.eClass());
  
  eCN eClass = pValue->eClass();
  
  switch (eClass) {
    case CN_INTVAL:
      *pValue->pAsIntVal() = xValWsp.iInt();
      break;
      
    case CN_STRVAL:
      *pValue->pAsStrVal() = xValWsp.sStr();
      break;
      
    case CN_PTRVAL:
      *pValue->pAsPtrVal() = xValWsp.pPtr();
      break;
      
    case CN_WRDVAL:
      *pValue->pAsWrdVal() = xValWsp.pWord();
      break;
      
    default:
      ASSERT(0);
  }
}

/*----------------------------------------------------------------------------*/
/* convert to command                                                         */
/*----------------------------------------------------------------------------*/
void PtrVal::vToCommand(char* aWorkspace)
{
  ASSERT(0);
}

/*----------------------------------------------------------------------------*/
/* convert to string                                                          */
/*----------------------------------------------------------------------------*/
void PtrVal::vToString(char* aWorkspace)
{
  String::wToPointer(m_pPtr,aWorkspace);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
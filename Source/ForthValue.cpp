/*============================================================================*/
/* ForthValue.cpp --Base Class for Self-Describing Values                          */
/*============================================================================*/

#include "Values.h"
#include "StorageEngine.h"

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* ForthValue::operator new(size_t)
{
  vFatal("fatal: ForthValue class cannot be new'd\n"_Stz);
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ForthValue::ForthValue(eCN eClass)
//: Header(eClass)
{
  m_xHeader.vSetClass(eClass);
  
  _checkInitialisedSize_(sizeof(ForthValue));
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get size (in bytes) of value                                               */
/*----------------------------------------------------------------------------*/
eCN ForthValue::eClass(void)
{
  eCN eClass = m_xHeader.eClass();
  
  return eClass;
}

/*----------------------------------------------------------------------------*/
/* get size (in bytes) of value                                               */
/*----------------------------------------------------------------------------*/
i8 ForthValue::nSize(void)
{
  i8 nBytes = 0;
  
  eCN eClassNo = eClass();
  
  switch (eClassNo) {
      
    case CN_INTVAL: {
      IntVal* aIntVal = _aIntVal(this);
      nBytes = aIntVal->nSize();
      break;
    }
      
    case CN_FIPVAL: {
      FipVal* aFipVal = _aFipVal(this);
      nBytes = aFipVal->nSize();
      break;
    }
      
    case CN_PTRVAL: {
      PtrVal* aPtrVal = _aPtrVal(this);
      nBytes = aPtrVal->nSize();
      break;
    }
      
    case CN_STRVAL: {
      StrVal* aStrVal = _aStrVal(this);
      nBytes = aStrVal->nSize();
      break;
    }
      
    default:
      vTrace("ForthValue::nSize -- invalid eClass %lld %s\n"_Stz,
             eClassNo,aCN[eClassNo]);
      NEVER_REACH;
      break;
  }
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* convert to string                                                          */
/*----------------------------------------------------------------------------*/
void ForthValue::vToString(Stz* aString)
{
  eCN eClass = this->eClass();
  
  switch (eClass) {
            
    case CN_FIPVAL: {
      FipVal* aFipVal = _aFipVal(this);
      aFipVal->vToString(aString);
      break;
    }
      
    case CN_INTVAL: {
      IntVal* aIntVal = _aIntVal(this);
      aIntVal->vToString(aString);
      break;
    }
      
    case CN_PTRVAL: {
      PtrVal* aPtrVal = _aPtrVal(this);
      aPtrVal->vToString(aString);
      break;
    }
      
    case CN_STRVAL: {
      StrVal* aStrVal = _aStrVal(this);
      aStrVal->vToString(aString);
      break;
    }
            
    default:
      vTrace("ForthValue::vToString -- invalid eClass %lld"_Stz,eClass);
      NEVER_REACH;
      break;
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* Value.cpp --Base Class for Self-Describing Values                          */
/*============================================================================*/

#include "Values.h"
#include "StorageEngine.h"

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Value::operator new(size_t)
{
  vFatal("fatal: Value class cannot be new'd\n"_Stz);
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Value::Value(eCN eClass)
: Header(eClass)
{
  _checkInitialisedSize_(sizeof(Value));
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get size (in bytes) of value                                               */
/*----------------------------------------------------------------------------*/
i8 Value::nSize(void)
{
  i8 nBytes = 0;
  
  eCN eClassNo = this->eClass();
  
  switch (eClassNo) {
      
    case CN_INTVAL: {
      IntVal* aIntVal = _aIntVal(this);
      nBytes = aIntVal->nSize();
      break;
    }
      
    case CN_FIPVAL: {
      IpVal* aIpVal = _aIpVal(this);
      nBytes = aIpVal->nSize();
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
      vTrace("Value::nSize -- invalid eClass %lld %s\n"_Stz,
             eClassNo,aCN[eClassNo]);
      NEVER_REACH;
      break;
  }
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* convert to string                                                          */
/*----------------------------------------------------------------------------*/
void Value::vToString(Stz* aString)
{
  eCN eClass = this->eClass();
  
  switch (eClass) {
            
    case CN_INTVAL: {
      IntVal* aIntVal = _aIntVal(this);
      aIntVal->vToString(aString);
      break;
    }
      
    case CN_FIPVAL: {
      IpVal* aIpVal = _aIpVal(this);
      aIpVal->vToString(aString);
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
      vTrace("Value::vToString -- invalid eClass %lld"_Stz,eClass);
      NEVER_REACH;
      break;
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

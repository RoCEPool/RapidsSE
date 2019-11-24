/*============================================================================*/
/* StkVal.cpp -- Base Class for Self-Describing Values on Stack               */
/*============================================================================*/

#include "SenVM.h"

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* StkVal::operator new(size_t)
{
  vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
         "fatal: StkVal class cannot be new'd\n"_Stz);
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
StkVal::StkVal(eCN eClass)
//: Header(eClass)
{
  m_xHeader.vSetClass(eClass);
  
  _checkInitialisedSize_(sizeof(StkVal));
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get size (in bytes) of value                                               */
/*----------------------------------------------------------------------------*/
eCN StkVal::eClass(void)
{
  eCN eClass = m_xHeader.eClass();
  
  return eClass;
}

/*----------------------------------------------------------------------------*/
/* get size (in bytes) of value                                               */
/*----------------------------------------------------------------------------*/
i8 StkVal::nStackBytes(void)
{
  i8 nBytes = 0;
  
  eCN eClassNo = eClass();
  
  switch (eClassNo) {
      
    case CN_INTVAL: {
      IntVal* aIntVal = _aIntVal(this);
      nBytes = aIntVal->nStackBytes();
      break;
    }
      
    case CN_FIPVAL: {
      FipVal* aFipVal = _aFipVal(this);
      nBytes = aFipVal->nStackBytes();
      break;
    }
      
    case CN_PTRVAL: {
      PtrVal* aPtrVal = _aPtrVal(this);
      nBytes = aPtrVal->nStackBytes();
      break;
    }
      
    case CN_STRVAL: {
      StrVal* aStrVal = _aStrVal(this);
      nBytes = aStrVal->nStackBytes();
      break;
    }
      
    default:
      vTrace("StkVal::nStackBytes -- invalid eClass %lld %s\n"_Stz,
             eClassNo,aCN[eClassNo]);
      NEVER_REACH;
      break;
  }
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* convert to string                                                          */
/*----------------------------------------------------------------------------*/
void StkVal::vToString(Stz* aString)
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
      vTrace("StkVal::vToString -- invalid eClass %lld"_Stz,eClass);
      NEVER_REACH;
      break;
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

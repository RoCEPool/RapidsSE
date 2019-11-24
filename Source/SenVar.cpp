/*============================================================================*/
/* SenVar.cpp -- Sen Variable                                                 */
/*============================================================================*/

#include "SenVM.h"

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* SenVar::operator new(size_t)
{
  vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
         "fatal: SenVar class cannot be new'd\n"_Stz);
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* get size (in bytes) of value                                               */
/*----------------------------------------------------------------------------*/
i8 SenVar::nStackBytes(void)
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
      vTrace("SenVar::nStackBytes -- invalid eClass %lld %s\n"_Stz,
             eClassNo,aCN[eClassNo]);
      NEVER_REACH;
      break;
  }
  
  return nBytes;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

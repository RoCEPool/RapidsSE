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
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* copy source stack value into this one                                      */
/*----------------------------------------------------------------------------*/
void StkVal::vCopy(StkVal* aSrcVal, i8 nBytes)
{
  if (! nBytes)
    nBytes = aSrcVal->nStackBytes();
  
  MemLib::vCopy(this,aSrcVal,nBytes,CN_STACK);
}



/*----------------------------------------------------------------------------*/
/* get azChar                                                                 */
/*----------------------------------------------------------------------------*/
char* StkVal::azChar(void)
{
  _magic_();
  
  char* az = _aStrVal(this)->azChar();
  
  return az;
}

/*----------------------------------------------------------------------------*/
/* get forth instruction pointer                                              */
/*----------------------------------------------------------------------------*/
Cell* StkVal::aFip(void)
{
  _magic_();
  
  Cell* aFip = _aFipVal(this)->aFip();
  
  return aFip;
}

/*----------------------------------------------------------------------------*/
/* set forth instruction pointer                                              */
/*----------------------------------------------------------------------------*/
void StkVal::vFip(Cell* aFip)
{
  _magic_();
  
  *_aFipVal(this) = aFip;
}

/*----------------------------------------------------------------------------*/
/* inspect the memory pattern that is a stack value                           */
/*----------------------------------------------------------------------------*/
void StkVal::vInspect(void)
{
  _magic_();
  
  i8 nBytes = nStackBytes();
  
  s256 sLead;
  sLead.vSprint("%p %s %02lldB: "_Stz,this,aCN[eClass()],nBytes);
  
  vTrace("%s"_Stz,sLead.szStz());
  byte* ayByte = (byte*)this;
  
  i8 i_Byte = 0;
  for (; i_Byte < 4; i_Byte++)
    vTrace("%02x"_Stz,*ayByte++);
  vTrace("|"_Stz);

  for (; i_Byte < 6; i_Byte++)
    vTrace("%02x"_Stz,*ayByte++);
  vTrace("|"_Stz);
  
  for (; i_Byte < 8; i_Byte++)
    vTrace("%02x"_Stz,*ayByte++);
  vTrace("|"_Stz);

  for (; i_Byte < 16; i_Byte++)
    vTrace("%02x"_Stz,*ayByte++);
  vTrace("|"_Stz);

  for (; i_Byte < nBytes; i_Byte++)
    vTrace("%02x"_Stz,*ayByte++);
  vTrace("\n"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  s256 sSpaces;
  for (i8 i_Space = 0; i_Space < sLead.nLength(); i_Space++)
    sSpaces.vConcat(" "_Stz);
  vTrace("%s"_Stz,sSpaces.szStz());

  char* acChar = (char*)this;
  i8 i_Char = 0;
  
  for (; i_Char < 4; i_Char++) {
    char cChr = *acChar++;
    if (cChr == 0)
      vTrace("⬜️"_Stz);
    else
    if (cChr < C_ASCII_MIN_VIZ || cChr > C_ASCII_MAX_VIZ)
      vTrace("%2c"_Stz, '.');
    else
      vTrace("%2c"_Stz, cChr);
  }
  vTrace("|"_Stz);

  vTrace("%04lld"_Stz,eClass());
  i_Char += 2;
  acChar += 2;
  vTrace("|"_Stz);
  
  vTrace("%04lld"_Stz,0);
  i_Char += 2;
  acChar += 2;
  vTrace("|"_Stz);

  vTrace("%16lld"_Stz,_aAnyVal(this)->iInt());
  i_Char += 8;
  acChar += 8;
  vTrace("|"_Stz);

  for (; i_Char < nBytes; i_Char++) {
    char cChr = *acChar++;
    if (cChr == 0)
      vTrace("⬜️"_Stz);
    else
    if (cChr < C_ASCII_MIN_VIZ || cChr > C_ASCII_MAX_VIZ)
      vTrace("%2c"_Stz, '.');
    else
      vTrace("%2c"_Stz, cChr);
  }

  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* get integer                                                                */
/*----------------------------------------------------------------------------*/
i8 StkVal::iInt(void)
{
  _magic_();
  
  i8 iInt = _aIntVal(this)->iInt();
  
  return iInt;
}

/*----------------------------------------------------------------------------*/
/* set integer                                                                */
/*----------------------------------------------------------------------------*/
void StkVal::vInt(i8 iInt)
{
  _magic_();
  
  *_aIntVal(this) = iInt;
}

/*----------------------------------------------------------------------------*/
/* test if nested value is an integer                                         */
/*----------------------------------------------------------------------------*/
bool StkVal::bIsInt(void)
{
  _magic_();
  
  if (CN_INTVAL == eClass())
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* get object address                                                         */
/*----------------------------------------------------------------------------*/
Object* StkVal::pObj(void)
{
  _magic_();
  
  Object* pObj = _aPtrVal(this)->pObj();
  
  return pObj;
}

/*----------------------------------------------------------------------------*/
/* set object pointer                                                         */
/*----------------------------------------------------------------------------*/
void StkVal::vObj(Object* pObj)
{
  _magic_();
  
  *_aPtrVal(this) = pObj;
}

/*----------------------------------------------------------------------------*/
/* get size of string in nested value                                         */
/*----------------------------------------------------------------------------*/
i8 StkVal::nStzSize(void)
{
  _magic_();
  
  i8 nSize = _aStrVal(this)->nSize();
  
  return nSize;
}

/*----------------------------------------------------------------------------*/
/* get stz                                                                    */
/*----------------------------------------------------------------------------*/
Stz* StkVal::szStz(void)
{
  _magic_();
  
  Stz* sz = _aStrVal(this)->szStz();
  
  return sz;
}

/*----------------------------------------------------------------------------*/
/* set object pointer                                                         */
/*----------------------------------------------------------------------------*/
void StkVal::vStz(Stz* szStz)
{
  _magic_();
  
  _aStrVal(this)->vAssign(szStz);
}




/*----------------------------------------------------------------------------*/
/* get how many bytes are needed to push a StkVal on stack                    */
/*----------------------------------------------------------------------------*/
i8 StkVal::nStackBytes(void)
{
  i8 nBytes = 0;
  
  eCN eClassNo = eClass();
  
  switch (eClassNo) {
      
    case CN_FIPVAL: {
      FipVal* aFipVal = _aFipVal(this);
      nBytes = aFipVal->nStackBytes();
      break;
    }
      
    case CN_INTVAL: {
      IntVal* aIntVal = _aIntVal(this);
      nBytes = aIntVal->nStackBytes();
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
      
    // -- -- -- -- -- --
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
void StkVal::vToString(Stz* szString)
{
  eCN eClass = this->eClass();
  
  switch (eClass) {
            
    case CN_FIPVAL: {
      FipVal* aFipVal = _aFipVal(this);
      aFipVal->vToString(szString);
      break;
    }
      
    case CN_INTVAL: {
      IntVal* aIntVal = _aIntVal(this);
      aIntVal->vToString(szString);
      break;
    }
      
    case CN_PTRVAL: {
      PtrVal* aPtrVal = _aPtrVal(this);
      aPtrVal->vToString(szString);
      break;
    }
      
    case CN_STRVAL: {
      StrVal* aStrVal = _aStrVal(this);
      aStrVal->vToString(szString);
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

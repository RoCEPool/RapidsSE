/*============================================================================*/
/* AnyVal.cpp -- Base Class for Self-Describing Values                        */
/*============================================================================*/

#include "SqlValues.h"

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* AnyVal::operator new(size_t)
{
  vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
         "fatal: AnyVal class cannot be new'd\n"_Stz);
  
  NEVER_REACH;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* copy source stack value into this one                                      */
/*----------------------------------------------------------------------------*/
void AnyVal::vCopy(AnyVal* aSrcVal, i8 nBytes)
{
  if (! nBytes)
    nBytes = aSrcVal->nStackUsage();
  
  MemLib::vCopy(this,aSrcVal,nBytes,CN_STACK);
}

/*----------------------------------------------------------------------------*/
/* get integer                                                                */
/*----------------------------------------------------------------------------*/
i8 AnyVal::iAsInt(void)
{
  _magic_();
  
  return m_iInt;
}

/*----------------------------------------------------------------------------*/
/* get bcd                                                                    */
/*----------------------------------------------------------------------------*/
Bcd AnyVal::wBcd(void)
{
  _magic_();
  
  return m_wBcd;
}

/*----------------------------------------------------------------------------*/
/* set bcd                                                                    */
/*----------------------------------------------------------------------------*/
void AnyVal::vBcd(Bcd wBcd)
{
  _magic_();
  
  m_wBcd = wBcd;
}

/*----------------------------------------------------------------------------*/
/* get bol                                                                    */
/*----------------------------------------------------------------------------*/
Bol AnyVal::wBol(void)
{
  _magic_();
  
  return m_wBol;
}

/*----------------------------------------------------------------------------*/
/* set bol                                                                    */
/*----------------------------------------------------------------------------*/
void AnyVal::vBol(Bol wBol)
{
  _magic_();
  
  m_wBol = wBol;
}

/*----------------------------------------------------------------------------*/
/* get azChar                                                                 */
/*----------------------------------------------------------------------------*/
char* AnyVal::azChar(void)
{
  _magic_();
  
  char* az = _aTypedCell(this)->azChar();
  
  return az;
}

/*----------------------------------------------------------------------------*/
/* get forth instruction pointer                                              */
/*----------------------------------------------------------------------------*/
Dtm* AnyVal::aCell(void)
{
  _magic_();
    
  return (Dtm*) & m_iInt;
}

/*----------------------------------------------------------------------------*/
/* get dte                                                                    */
/*----------------------------------------------------------------------------*/
Dte AnyVal::wDte(void)
{
  _magic_();
  
  return m_wDte;
}

/*----------------------------------------------------------------------------*/
/* set dte                                                                    */
/*----------------------------------------------------------------------------*/
void AnyVal::vDte(Dte wDte)
{
  _magic_();
  
  m_wDte = wDte;
}

/*----------------------------------------------------------------------------*/
/* get edt                                                                    */
/*----------------------------------------------------------------------------*/
Edt AnyVal::wEdt(void)
{
  _magic_();
  
  return m_wEdt;
}

/*----------------------------------------------------------------------------*/
/* set edt                                                                    */
/*----------------------------------------------------------------------------*/
void AnyVal::vEdt(Edt wEdt)
{
  _magic_();
  
  m_wEdt = wEdt;
}

/*----------------------------------------------------------------------------*/
/* get forth instruction pointer                                              */
/*----------------------------------------------------------------------------*/
ForthCell* AnyVal::aFip(void)
{
  _magic_();
    
  return m_aFip;
}

/*----------------------------------------------------------------------------*/
/* set forth instruction pointer                                              */
/*----------------------------------------------------------------------------*/
void AnyVal::vFip(ForthCell* aFip)
{
  _magic_();
  
  m_aFip = aFip;
}

/*----------------------------------------------------------------------------*/
/* inspect the memory pattern that is a stack value                           */
/*----------------------------------------------------------------------------*/
void AnyVal::vInspect(void)
{
  _magic_();
  
  i8 nBytes = nStackUsage();
  
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
i8 AnyVal::iInt(void)
{
  _magic_();
    
  return m_iInt;
}

/*----------------------------------------------------------------------------*/
/* set integer                                                                */
/*----------------------------------------------------------------------------*/
void AnyVal::vInt(i8 iInt)
{
  _magic_();
  
  m_iInt = iInt;
}

/*----------------------------------------------------------------------------*/
/* get ivl                                                                    */
/*----------------------------------------------------------------------------*/
Ivl AnyVal::wIvl(void)
{
  _magic_();
  
  return m_wIvl;
}

/*----------------------------------------------------------------------------*/
/* set ivl                                                                    */
/*----------------------------------------------------------------------------*/
void AnyVal::vIvl(Ivl wIvl)
{
  _magic_();
  
  m_wIvl = wIvl;
}

/*----------------------------------------------------------------------------*/
/* test if nested value is an integer                                         */
/*----------------------------------------------------------------------------*/
bool AnyVal::bIsInt(void)
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
Object* AnyVal::pObj(void)
{
  _magic_();
  
  return m_pObj;
}

/*----------------------------------------------------------------------------*/
/* set object pointer                                                         */
/*----------------------------------------------------------------------------*/
void AnyVal::vObj(Object* pObj)
{
  _magic_();
  
  m_pObj = pObj;
}

/*----------------------------------------------------------------------------*/
/* get size of string in nested value                                         */
/*----------------------------------------------------------------------------*/
i8 AnyVal::nStzSize(void)
{
  _magic_();
  
  i8 nSize = _aTypedCell(this)->nStzSize();
  
  return nSize;
}

/*----------------------------------------------------------------------------*/
/* get stz                                                                    */
/*----------------------------------------------------------------------------*/
Stz* AnyVal::szStz(void)
{
  _magic_();
  
  Stz* sz = _aTypedCell(this)->szStz();
  
  return sz;
}

/*----------------------------------------------------------------------------*/
/* set object pointer                                                         */
/*----------------------------------------------------------------------------*/
void AnyVal::vStz(Stz* szStz)
{
  _magic_();
  
  _aTypedCell(this)->vStz(szStz);
}


/*----------------------------------------------------------------------------*/
/* get how many bytes are needed to push a AnyVal on stack                    */
/*----------------------------------------------------------------------------*/
i8 AnyVal::nStackUsage(void)
{
  i8 nBytes = 0;
  
  eCN eClassNo = eClass();
  
  switch (eClassNo) {
      
    case CN_FIPVAL: {
//      FipVal* aFipVal = _aFipVal(this);
//      nBytes = aFipVal->nStackUsage();
      TypedCell* aTypedCell = _aTypedCell(this);
      nBytes = aTypedCell->nStackUsage();
      break;
    }
      
    case CN_INTVAL: {
//      IntVal* aIntVal = _aIntVal(this);
//      nBytes = aIntVal->nStackUsage();
      TypedCell* aTypedCell = _aTypedCell(this);
      nBytes = aTypedCell->nStackUsage();
      break;
    }
      
    case CN_PTRVAL: {
//      PtrVal* aPtrVal = _aPtrVal(this);
//      nBytes = aPtrVal->nStackUsage();
      TypedCell* aTypedCell = _aTypedCell(this);
      nBytes = aTypedCell->nStackUsage();
      break;
    }
      
    case CN_STZVAL: {
      TypedCell* aTypedCell = _aTypedCell(this);
      nBytes = aTypedCell->nStackUsage();
      break;
    }
      
    // -- -- -- -- -- --
    default:
      vTrace("AnyVal::nStackUsage -- invalid eClass %lld %s\n"_Stz,
             eClassNo,aCN[eClassNo]);
      NEVER_REACH;
      break;
  }
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* get tme                                                                    */
/*----------------------------------------------------------------------------*/
Tme AnyVal::wTme(void)
{
  _magic_();
  
  return m_wTme;
}

/*----------------------------------------------------------------------------*/
/* set tme                                                                    */
/*----------------------------------------------------------------------------*/
void AnyVal::vTme(Tme wTme)
{
  _magic_();
  
  m_wTme = wTme;
}

/*----------------------------------------------------------------------------*/
/* convert to string                                                          */
/*----------------------------------------------------------------------------*/
void AnyVal::vToString(Stz* szStz)
{
  eCN eClass = this->eClass();
  
  switch (eClass) {
            
    case CN_FIPVAL: {
//      FipVal* aFipVal = _aFipVal(this);
//      aFipVal->vToStz(szStz);
      TypedCell* aTypedCell = _aTypedCell(this);
      aTypedCell->vToStz(szStz);
      break;
    }
      
    case CN_INTVAL: {
//      IntVal* aIntVal = _aIntVal(this);
//      aIntVal->vToStz(szStz);
      TypedCell* aTypedCell = _aTypedCell(this);
      aTypedCell->vToStz(szStz);
      break;
    }
      
    case CN_PTRVAL: {
//      PtrVal* aPtrVal = _aPtrVal(this);
//      aPtrVal->vToStz(szStz);
      TypedCell* aTypedCell = _aTypedCell(this);
      aTypedCell->vToStz(szStz);
      break;
    }
      
    case CN_STZVAL: {
      TypedCell* aTypedCell = _aTypedCell(this);
      aTypedCell->vToStz(szStz);
      break;
    }
            
    default:
      vTrace("AnyVal::vToString -- invalid eClass %lld"_Stz,eClass);
      NEVER_REACH;
      break;
  }
}

/*----------------------------------------------------------------------------*/
/* get tsp                                                                    */
/*----------------------------------------------------------------------------*/
Tsp AnyVal::wTsp(void)
{
  _magic_();
  
  return m_wTsp;
}

/*----------------------------------------------------------------------------*/
/* set tsp                                                                    */
/*----------------------------------------------------------------------------*/
void AnyVal::vTsp(Tsp wTsp)
{
  _magic_();
  
  m_wTsp = wTsp;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

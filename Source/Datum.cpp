/*============================================================================*/
/* Datum.cpp -- Datum                                                         */
/*============================================================================*/

#include "Datums.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Datum* Datums::_pDatum(void* aAddr)
{
  Datum* pDatum = reinterpret_cast<Datum*>(aAddr);
  
  return pDatum;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Datum::operator new(size_t)
{
  void* pObject = HeapMem::pAllocate(CN_DATUM);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void Datum::operator delete(void* aAddr)
{
  Datum* pDatum = reinterpret_cast<Datum*>(aAddr);
  
  HeapMem::vFree(pDatum);
}


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Datum::Datum(void)
: Object(CN_DATUM)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  u.m_xAnyVal.m_iInt   = C_UNKNOWN;
  

  u.m_xAnyVal.vMagic();
  u.m_xAnyVal.vClass(CN_NONE);

  u.m_xStzVal.vStz(""_Stz);
  //vStzDump();

  //m_sVch
  
  _checkInitialised_(CN_DATUM);
  
  _postcondition_(_MAGIC_,CN_DATUM,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Datum::Datum(AnyVal* aAnyVal)
: Object(CN_DATUM)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  u.m_xAnyVal.m_iInt   = C_UNKNOWN;

  u.m_xAnyVal.vMagic();
  u.m_xAnyVal.vClass(CN_NONE);
  
  eCN eClassNo = aAnyVal->eClass();
  
  switch (eClassNo) {
    case CN_FIPVAL: u.m_xFipVal.vFip(aAnyVal->aFip());    break;
    case CN_INTVAL: u.m_xIntVal.vInt(aAnyVal->iInt());    break;
    case CN_PTRVAL: u.m_xPtrVal.vObj(aAnyVal->pObj());    break;
    case CN_STZVAL: u.m_xStzVal.vStz(aAnyVal->szStz());   break;
      
    default:
      vThrow(FK_CHECK,VL_FATAL,__func__,__FILE__,__LINE__,
             "Invalid eClass %lld"_Stz,eClassNo);
  }
  
  _postcondition_(_MAGIC_,CN_DATUM,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Datum::~Datum(void) noexcept(false)
{
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* auto conversion operator : as AnyVal*                                      */
/*----------------------------------------------------------------------------*/
Datum::operator AnyVal*(void)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  return & u.m_xAnyVal;
}






/*----------------------------------------------------------------------------*/
/* operator =                                                                 */
/*----------------------------------------------------------------------------*/
void Datum::operator =(AnyVal* aAnyVal)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  eCN eClass = aAnyVal->eClass();
  
  switch (eClass) {
    case CN_FIPVAL: u.m_xFipVal.vFip(aAnyVal->aFip());    break;
    case CN_INTVAL: u.m_xIntVal.vInt(aAnyVal->iInt());    break;
    case CN_PTRVAL: u.m_xPtrVal.vObj(aAnyVal->pObj());    break;
    case CN_STZVAL: u.m_xStzVal.vStz(aAnyVal->szStz());   break;
      
    default:
      vThrow(FK_CHECK,VL_FATAL,__func__,__FILE__,__LINE__,
             "Invalid eClass %lld"_Stz,eClass);
  }
  
  NEVER_REACH;
}



/*----------------------------------------------------------------------------*/
/* assign/copy a forth instruction pointer                                    */
/*----------------------------------------------------------------------------*/
void Datum::operator =(Cell* aFip)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  u.m_xFipVal.vFip(aFip);
}

/*----------------------------------------------------------------------------*/
/* assign/copy an integer                                                     */
/*----------------------------------------------------------------------------*/
void Datum::operator =(i8 iInt)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  u.m_xIntVal.vInt(iInt);
}

/*----------------------------------------------------------------------------*/
/* assign/copy a pointer to a proper object                                   */
/*----------------------------------------------------------------------------*/
void Datum::operator =(Object* pObj)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  u.m_xPtrVal.vObj(pObj);
}

/*----------------------------------------------------------------------------*/
/* assign/copy a string                                                       */
/*----------------------------------------------------------------------------*/
void Datum::operator =(Stz* szStz)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  u.m_xStzVal.vStz(szStz);
  
  _postcondition_(_MAGIC_,CN_DATUM,_FOOTER_);
}






/*----------------------------------------------------------------------------*/
/* test null                                                                  */
/*----------------------------------------------------------------------------*/
bool Datum::bIsNull(void)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);

  if (u.m_xAnyVal.m_iInt == C_DATUM_NULL)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* set null                                                                   */
/*----------------------------------------------------------------------------*/
void Datum::vSetNull(void)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);

  u.m_xAnyVal.m_iInt = C_DATUM_NULL;
}






/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get aAnyVal                                                                */
/*----------------------------------------------------------------------------*/
AnyVal* Datum::aAnyVal(void)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  return & u.m_xAnyVal;
}

/*----------------------------------------------------------------------------*/
/* get decimal                                                                */
/*----------------------------------------------------------------------------*/
i8 Datum::iBcd(void)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  i8 iBcd = u.m_xAnyVal.m_iBcd;
  
  return iBcd;
}

/*----------------------------------------------------------------------------*/
/* get boolean                                                                */
/*----------------------------------------------------------------------------*/
i8 Datum::iBol(void)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  i8 iBol = u.m_xAnyVal.m_iBol;
  
  return iBol;
}

/*----------------------------------------------------------------------------*/
/* get azChar                                                                 */
/*----------------------------------------------------------------------------*/
char* Datum::azChar(void)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  char* az = u.m_xStzVal.azChar();
  
  return az;
}

/*----------------------------------------------------------------------------*/
/* copy given source value into this var                                      */
/*----------------------------------------------------------------------------*/
void Datum::vCopy(AnyVal* aAnyVal)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  u.m_xAnyVal.vCopy(aAnyVal);
}

/*----------------------------------------------------------------------------*/
/* get day of date                                                            */
/*----------------------------------------------------------------------------*/
i8 Datum::iDte_day(void)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  i8 iDay = u.m_xAnyVal.m_xDte.m_uDay;
  
  return iDay;
}

/*----------------------------------------------------------------------------*/
/* get month of date                                                          */
/*----------------------------------------------------------------------------*/
i8 Datum::iDte_month(void)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  i8 iMonth = u.m_xAnyVal.m_xDte.m_uMonth;
  
  return iMonth;
}

/*----------------------------------------------------------------------------*/
/* get year of date                                                           */
/*----------------------------------------------------------------------------*/
i8 Datum::iDte_year(void)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  i8 iYear = u.m_xAnyVal.m_xDte.m_uYearDelta + C_YEAR_BASE;
  
  return iYear;
}

/*----------------------------------------------------------------------------*/
/* get address of the nested value                                            */
/*----------------------------------------------------------------------------*/
AnyVal* Datum::aEmbeddedAnyVal(void)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  return & u.m_xAnyVal;
}

/*----------------------------------------------------------------------------*/
/* get forth instruction pointer                                              */
/*----------------------------------------------------------------------------*/
Cell* Datum::aFip(void)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  Cell* aFip = u.m_xFipVal.aFip();
  
  return aFip;
}

/*----------------------------------------------------------------------------*/
/* get float                                                                  */
/*----------------------------------------------------------------------------*/
d8 Datum::dFlt(void)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  d8 dFlt = u.m_xAnyVal.m_dFlt;
  
  return dFlt;
}

/*----------------------------------------------------------------------------*/
/* get integer                                                                */
/*----------------------------------------------------------------------------*/
i8 Datum::iInt(void)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  i8 iInt = u.m_xIntVal.iInt();
  
  return iInt;
}

/*----------------------------------------------------------------------------*/
/* test if nested value is an integer                                         */
/*----------------------------------------------------------------------------*/
bool Datum::bIsInt(void)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  if (CN_INTVAL == u.m_xAnyVal.eClass())
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* get interval                                                               */
/*----------------------------------------------------------------------------*/
i8 Datum::iIvl(void)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  i8 iIvl = u.m_xAnyVal.m_iIvl;
  
  return iIvl;
}

/*----------------------------------------------------------------------------*/
/* get object address                                                         */
/*----------------------------------------------------------------------------*/
Object* Datum::pObj(void)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  Object* pObj = u.m_xPtrVal.pObj();
  
  return pObj;
}

/*----------------------------------------------------------------------------*/
/* get bytes required to push value on stack                                  */
/*----------------------------------------------------------------------------*/
i8 Datum::nStackUsage(void)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  i8 nBytes = u.m_xStzVal.nStackUsage();
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* get stz                                                                    */
/*----------------------------------------------------------------------------*/
Stz* Datum::szStz(void)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  Stz* sz = u.m_xStzVal.szStz();
  
  return sz;
}

/*----------------------------------------------------------------------------*/
/* set stz                                                                    */
/*----------------------------------------------------------------------------*/
void Datum::vStz(Stz* szStz)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  u.m_xStzVal.vStz(szStz);
}

/*----------------------------------------------------------------------------*/
/* set stz                                                                    */
/*----------------------------------------------------------------------------*/
void Datum::vStz(char* acnChar, i8 nLength)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  u.m_xStzVal.vStz(acnChar,nLength);
}

/*----------------------------------------------------------------------------*/
/* set stz                                                                    */
/*----------------------------------------------------------------------------*/
void Datum::vStzDump(void)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  vTrace("vStzDump() : "_Stz);
    
  vTrace("|%02lld|%02lld|%s|\n"_Stz,
         nStzSize(),
         szStz()->iLength(),
         szStz());
}

/*----------------------------------------------------------------------------*/
/* get size of nested string value                                            */
/*----------------------------------------------------------------------------*/
i8 Datum::nStzSize(void)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  i8 nSize = u.m_xStzVal.nStzSize();
  
  return nSize;
}

/*----------------------------------------------------------------------------*/
/* set size of nested string value                                            */
/*----------------------------------------------------------------------------*/
void Datum::vStzSize(i8 nSize)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  u.m_xStzVal.vStzSize(nSize);
}

/*----------------------------------------------------------------------------*/
/* get space allowed for nested string value                                  */
/*----------------------------------------------------------------------------*/
i8 Datum::nStzSpace(void)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  i8 nSize = sizeof(u.m_yaSpace);
  
  return nSize;
}

/*----------------------------------------------------------------------------*/
/* get time                                                                   */
/*----------------------------------------------------------------------------*/
i8 Datum::iTme(void)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  i8 iTme = u.m_xAnyVal.m_iTme;
  
  return iTme;
}

/*----------------------------------------------------------------------------*/
/* convert to stz string                                                      */
/*----------------------------------------------------------------------------*/
void Datum::vToString(Stz* szString)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  u.m_xAnyVal.vToString(szString);
}

/*----------------------------------------------------------------------------*/
/* get timestamp                                                              */
/*----------------------------------------------------------------------------*/
i8 Datum::iTsp(void)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  i8 iTsp = u.m_xAnyVal.m_iTsp;
  
  return iTsp;
}

/*----------------------------------------------------------------------------*/
/* get class of embedded value                                                */
/*----------------------------------------------------------------------------*/
eCN Datum::eValClass(void)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  return u.m_xAnyVal.eClass();
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(Datum)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

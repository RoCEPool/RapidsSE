/*============================================================================*/
/* Datum.cpp -- Datum                                                         */
/*============================================================================*/

#include "Datums.h"

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
_Datum::_Datum(eCN eClass)
: Object(eClass)
{
  
}





/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
_Datum_::_Datum_(eCN eClass)
: _Datum(eClass)
{
  
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* auto conversion operator : as AnyVal*                                      */
/*----------------------------------------------------------------------------*/
_Datum_::operator AnyVal*(void)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  return & m_xAnyVal;
}

/*----------------------------------------------------------------------------*/
/* operator =                                                                 */
/*----------------------------------------------------------------------------*/
void _Datum_::operator =(AnyVal* aAnyVal)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  eCN eClass = aAnyVal->eClass();
  
  switch (eClass) {
    case CN_FIPVAL: vFip(aAnyVal->aFip());    break;
    case CN_INTVAL: vInt(aAnyVal->iInt());    break;
    case CN_PTRVAL: vObj(aAnyVal->pObj());    break;
    case CN_STZVAL: vStz(aAnyVal->szStz());   break;
      
    default:
      vThrow(FK_CHECK,VL_FATAL,__func__,__FILE__,__LINE__,
             "Invalid eClass %lld"_Stz,eClass);
  }
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* assign/copy a forth instruction pointer                                    */
/*----------------------------------------------------------------------------*/
void _Datum_::operator =(Cell* aFip)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  vFip(aFip);
}

/*----------------------------------------------------------------------------*/
/* assign/copy an integer                                                     */
/*----------------------------------------------------------------------------*/
void _Datum_::operator =(i8 iInt)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  vInt(iInt);
}

/*----------------------------------------------------------------------------*/
/* assign/copy a pointer to a proper object                                   */
/*----------------------------------------------------------------------------*/
void _Datum_::operator =(Object* pObj)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  vObj(pObj);
}

/*----------------------------------------------------------------------------*/
/* assign/copy a string                                                       */
/*----------------------------------------------------------------------------*/
void _Datum_::operator =(Stz* szStz)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  vStz(szStz);
  
  _postcondition_(_MAGIC_,CN_DATUM,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* test null                                                                  */
/*----------------------------------------------------------------------------*/
bool _Datum_::bIsNull(void)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);

  if (m_xIntVal.m_iInt == C_DATUM_NULL)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* set null                                                                   */
/*----------------------------------------------------------------------------*/
void _Datum_::vSetNull(void)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);

  m_xIntVal.m_iInt = C_DATUM_NULL;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get class of embedded value                                                */
/*----------------------------------------------------------------------------*/
eCN _Datum_::eAnyClass(void)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  return m_xAnyVal.eClass();
}

/*----------------------------------------------------------------------------*/
/* get aAnyVal                                                                */
/*----------------------------------------------------------------------------*/
AnyVal* _Datum_::aAnyVal(void)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  return & m_xAnyVal;
}

/*----------------------------------------------------------------------------*/
/* assign datum from token                                                    */
/*----------------------------------------------------------------------------*/
bool _Datum_::bAssign(Token* pTkn, Column* pCol)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);

  char* azCrs = pTkn->azChar();
  
  Format* pFmt = AM::pFmt();
  
  bool b = CsvValue::bToDtm((Datum*)this,pCol,&azCrs,pFmt);
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* get decimal                                                                */
/*----------------------------------------------------------------------------*/
i8 _Datum_::iBcd(void)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  i8 iBcd = m_xBcdVal.m_iBcd;
  
  return iBcd;
}

/*----------------------------------------------------------------------------*/
/* set decimal                                                                */
/*----------------------------------------------------------------------------*/
void _Datum_::vBcd(i8 iBcd)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  m_xBcdVal.m_iBcd = iBcd;
}

/*----------------------------------------------------------------------------*/
/* get boolean                                                                */
/*----------------------------------------------------------------------------*/
i8 _Datum_::iBol(void)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  i8 iBol = m_xBolVal.m_iBol;
  
  return iBol;
}

/*----------------------------------------------------------------------------*/
/* set boolean                                                                */
/*----------------------------------------------------------------------------*/
void _Datum_::vBol(i8 iBol)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  m_xBolVal.m_iBol = iBol;
}

/*----------------------------------------------------------------------------*/
/* get azChar                                                                 */
/*----------------------------------------------------------------------------*/
char* _Datum_::azChar(void)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  char* az = m_xStzVal.azChar();
  
  return az;
}

/*----------------------------------------------------------------------------*/
/* copy given source value into this var                                      */
/*----------------------------------------------------------------------------*/
void _Datum_::vCopy(AnyVal* aAnyVal)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  m_xAnyVal.vCopy(aAnyVal);
}

/*----------------------------------------------------------------------------*/
/* get day of date                                                            */
/*----------------------------------------------------------------------------*/
i8 _Datum_::iDte_day(void)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  i8 iDay = m_xDteVal.m_wDte.m_uDay;
  
  return iDay;
}

/*----------------------------------------------------------------------------*/
/* get month of date                                                          */
/*----------------------------------------------------------------------------*/
i8 _Datum_::iDte_month(void)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  i8 iMonth = m_xDteVal.m_wDte.m_uMonth;
  
  return iMonth;
}

/*----------------------------------------------------------------------------*/
/* get year of date                                                           */
/*----------------------------------------------------------------------------*/
i8 _Datum_::iDte_year(void)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  i8 iYear = m_xDteVal.m_wDte.m_uYearDelta + C_YEAR_BASE;
  
  return iYear;
}

/*----------------------------------------------------------------------------*/
/* set date from parts                                                        */
/*----------------------------------------------------------------------------*/
void _Datum_::vDte(i8 iDte)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  m_xDteVal.m_iDte = iDte;
}

/*----------------------------------------------------------------------------*/
/* set date from parts                                                        */
/*----------------------------------------------------------------------------*/
void _Datum_::vDte(i8 iYear, i8 iMonth, i8 iDay)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  m_xDteVal.m_wDte.m_uYearDelta = iYear - C_YEAR_BASE;
  m_xDteVal.m_wDte.m_uMonth     = iMonth;
  m_xDteVal.m_wDte.m_uDay       = iDay;
  m_xDteVal.m_wDte.m_uSpare     = 0;
}

/*----------------------------------------------------------------------------*/
/* get era-date                                                               */
/*----------------------------------------------------------------------------*/
i8 _Datum_::iEdt(void)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  i8 iEdt = m_xEdtVal.m_iEdt;
  
  return iEdt;
}

/*----------------------------------------------------------------------------*/
/* get forth instruction pointer                                              */
/*----------------------------------------------------------------------------*/
Cell* _Datum_::aFip(void)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  Cell* aFip = m_xFipVal.aFip();
  
  return aFip;
}

/*----------------------------------------------------------------------------*/
/* set forth instruction pointer                                              */
/*----------------------------------------------------------------------------*/
void _Datum_::vFip(Cell* aFip)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  m_xFipVal.vFip(aFip);
}

/*----------------------------------------------------------------------------*/
/* get float                                                                  */
/*----------------------------------------------------------------------------*/
d8 _Datum_::dFlt(void)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  d8 dFlt = m_xFltVal.m_wFlt.m_dFlt;
  
  return dFlt;
}

/*----------------------------------------------------------------------------*/
/* set float                                                                  */
/*----------------------------------------------------------------------------*/
void _Datum_::vFlt(d8 dFlt)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  m_xFltVal.m_wFlt.m_dFlt = dFlt;
}

/*----------------------------------------------------------------------------*/
/* get integer                                                                */
/*----------------------------------------------------------------------------*/
i8 _Datum_::iInt(void)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  i8 iInt = m_xIntVal.iInt();
  
  return iInt;
}

/*----------------------------------------------------------------------------*/
/* set integer                                                                */
/*----------------------------------------------------------------------------*/
void _Datum_::vInt(i8 iInt)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  m_xIntVal.vInt(iInt);
}

/*----------------------------------------------------------------------------*/
/* test if nested value is an integer                                         */
/*----------------------------------------------------------------------------*/
bool _Datum_::bIsInt(void)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  if (CN_INTVAL == m_xAnyVal.eClass())
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* get interval                                                               */
/*----------------------------------------------------------------------------*/
i8 _Datum_::iIvl(void)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  i8 iIvl = m_xIvlVal.m_iIvl;
  
  return iIvl;
}

/*----------------------------------------------------------------------------*/
/* set interval                                                               */
/*----------------------------------------------------------------------------*/
void _Datum_::vIvl(i8 iIvl)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  m_xIvlVal.m_iIvl = iIvl;
}

/*----------------------------------------------------------------------------*/
/* set interval from parts                                                    */
/*----------------------------------------------------------------------------*/
void _Datum_::vIvl(i8 iSecond, i8 iMicro)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  m_xIvlVal.m_wIvl.m_uSecond = iSecond;
  m_xIvlVal.m_wIvl.m_uMicro  = iMicro;
}

/*----------------------------------------------------------------------------*/
/* get object address                                                         */
/*----------------------------------------------------------------------------*/
Object* _Datum_::pObj(void)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  Object* pObj = m_xPtrVal.pObj();
  
  return pObj;
}

/*----------------------------------------------------------------------------*/
/* set object address                                                         */
/*----------------------------------------------------------------------------*/
void _Datum_::vObj(Object* pObj)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  m_xPtrVal.vObj(pObj);
}

/*----------------------------------------------------------------------------*/
/* get bytes required to push value on stack                                  */
/*----------------------------------------------------------------------------*/
i8 _Datum_::nStackUsage(void)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  i8 nBytes = m_xStzVal.nStackUsage();
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* get stz                                                                    */
/*----------------------------------------------------------------------------*/
Stz* _Datum_::szStz(void)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  Stz* sz = m_xStzVal.szStz();
  
  return sz;
}

/*----------------------------------------------------------------------------*/
/* set stz                                                                    */
/*----------------------------------------------------------------------------*/
void _Datum_::vStz(Stz* szStz)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  m_xStzVal.vStz(szStz);
}

/*----------------------------------------------------------------------------*/
/* set stz                                                                    */
/*----------------------------------------------------------------------------*/
void _Datum_::vStz(char* acnChar, i8 nLength)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  m_xStzVal.vStz(acnChar,nLength);
}

/*----------------------------------------------------------------------------*/
/* set stz                                                                    */
/*----------------------------------------------------------------------------*/
void _Datum_::vStzDump(void)
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
i8 _Datum_::nStzSize(void)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  i8 nSize = m_xStzVal.nStzSize();
  
  return nSize;
}

/*----------------------------------------------------------------------------*/
/* set size of nested string value                                            */
/*----------------------------------------------------------------------------*/
void _Datum_::vStzSize(i8 nSize)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  m_xStzVal.vStzSize(nSize);
}

/*----------------------------------------------------------------------------*/
/* get space allowed for nested string value                                  */
/*----------------------------------------------------------------------------*/
i8 _Datum_::nStzSpace(void)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  i8 nSize = sizeof(m_yaSpace);
  
  return nSize;
}

/*----------------------------------------------------------------------------*/
/* get time                                                                   */
/*----------------------------------------------------------------------------*/
i8 _Datum_::iTme(void)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  i8 iTme = m_xTmeVal.m_iTme;
  
  return iTme;
}

/*----------------------------------------------------------------------------*/
/* set time                                                                   */
/*----------------------------------------------------------------------------*/
void _Datum_::vTme(i8 iTme)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  m_xTmeVal.m_iTme = iTme;
}

/*----------------------------------------------------------------------------*/
/* set time from parts                                                        */
/*----------------------------------------------------------------------------*/
void _Datum_::vTme(i8 iHour, i8 iMinute, i8 iSecond, i8 iMicro)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  m_xTmeVal.m_wTme.m_uHour   = iHour;
  m_xTmeVal.m_wTme.m_uMinute = iMinute;
  m_xTmeVal.m_wTme.m_uSecond = iSecond;
  m_xTmeVal.m_wTme.m_uMicro  = iMicro;
  m_xTmeVal.m_wTme.m_uSpare  = 0;
}

/*----------------------------------------------------------------------------*/
/* convert to stz string                                                      */
/*----------------------------------------------------------------------------*/
void _Datum_::vToString(Stz* szString)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  m_xAnyVal.vToString(szString);
}

/*----------------------------------------------------------------------------*/
/* get timestamp                                                              */
/*----------------------------------------------------------------------------*/
i8 _Datum_::iTsp(void)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  i8 iTsp = m_xTspVal.m_iTsp;
  
  return iTsp;
}

/*----------------------------------------------------------------------------*/
/* set timestamp                                                              */
/*----------------------------------------------------------------------------*/
void _Datum_::vTsp(i8 iTsp)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  m_xTspVal.m_iTsp = iTsp;
}

/*----------------------------------------------------------------------------*/
/* set timestamp from parts                                                   */
/*----------------------------------------------------------------------------*/
void _Datum_::vTsp(i8 iYear, i8 iMonth,  i8 iDay,
                 i8 iHour, i8 iMinute, i8 iSecond, i8 iMicro)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  m_xTspVal.m_wTsp.m_uYearDelta = iYear - C_YEAR_BASE;
  m_xTspVal.m_wTsp.m_uMonth     = iMonth;
  m_xTspVal.m_wTsp.m_uDay       = iDay;
  m_xTspVal.m_wTsp.m_uHour      = iHour;
  m_xTspVal.m_wTsp.m_uMinute    = iMinute;
  m_xTspVal.m_wTsp.m_uSecond    = iSecond;
  m_xTspVal.m_wTsp.m_uMicro     = iMicro;
  m_xTspVal.m_wTsp.m_uLocal     = C_TSP_LOCAL;
  m_xTspVal.m_wTsp.m_uSpare     = 0;
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(_Datum_)


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
: _Datum_(CN_DATUM)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  m_xIntVal.vMagic();
  m_xIntVal.vClass(CN_INTVAL);
  
  m_xIntVal.vInt(C_UNKNOWN);
  
  m_xStzVal.vStz(""_Stz);
  //vStzDump();
  
  //m_sVch
  
  _checkInitialised_(CN_DATUM);
  
  _postcondition_(_MAGIC_,CN_DATUM,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Datum::Datum(AnyVal* aAnyVal)
: _Datum_(CN_DATUM)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  m_xIntVal.vMagic();
  m_xIntVal.vClass(CN_INTVAL);
  
  m_xIntVal.vInt(C_UNKNOWN);
  
  eCN eClassNo = aAnyVal->eClass();
  
  switch (eClassNo) {
    case CN_FIPVAL: m_xFipVal.vFip(aAnyVal->aFip());    break;
    case CN_INTVAL: m_xIntVal.vInt(aAnyVal->iInt());    break;
    case CN_PTRVAL: m_xPtrVal.vObj(aAnyVal->pObj());    break;
    case CN_STZVAL: m_xStzVal.vStz(aAnyVal->szStz());   break;
      
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
/* assign/copy a forth instruction pointer                                    */
/*----------------------------------------------------------------------------*/
void Datum::operator =(Cell* aFip)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  vFip(aFip);
}

/*----------------------------------------------------------------------------*/
/* assign/copy an integer                                                     */
/*----------------------------------------------------------------------------*/
void Datum::operator =(i8 iInt)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  vInt(iInt);
}

/*----------------------------------------------------------------------------*/
/* assign/copy a pointer to a proper object                                   */
/*----------------------------------------------------------------------------*/
void Datum::operator =(Object* pObj)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  vObj(pObj);
}

/*----------------------------------------------------------------------------*/
/* assign/copy a string                                                       */
/*----------------------------------------------------------------------------*/
void Datum::operator =(Stz* szStz)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  vStz(szStz);
  
  _postcondition_(_MAGIC_,CN_DATUM,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

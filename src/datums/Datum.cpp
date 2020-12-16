/*============================================================================*/
/* Datum.cpp -- Datum                                                         */
/*============================================================================*/

#include "Datums.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Datum_h::operator new(size_t)
{
  void* pObject = HeapMem::pAllocate(CN_DATUM);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void Datum_h::operator delete(void* aAddr)
{
  Datum* pDatum = reinterpret_cast<Datum*>(aAddr);
  
  HeapMem::vFree(pDatum);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Datum_h::Datum_h(i8 nSizeSpace)
: Object(CN_DATUM)
{
  _precondition_(_MAGIC_,CN_DATUM);
  
  m_nSizeSpace = nSizeSpace;
  _setFooter_();
  
  MemLib::vZero(ayaSpace(),nSizeSpace,CN_DATUM);

  m_xIntVal.vMagic();
  m_xIntVal.vClass(CN_INTVAL);
  m_xIntVal.vInt(C_DATUM_NULL);
  
  m_xStzVal.vStz(""_Stz);
  //vStzDump();
  
  //m_sVch
  
  _checkInitialised_(CN_DATUM);
  
  _postcondition_(_MAGIC_,CN_DATUM,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Datum_h::~Datum_h(void) noexcept(false)
{
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* auto conversion operator : as AnyVal*                                      */
/*----------------------------------------------------------------------------*/
Datum_h::operator AnyVal*(void)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  return & m_xAnyVal;
}

/*----------------------------------------------------------------------------*/
/* operator =                                                                 */
/*----------------------------------------------------------------------------*/
void Datum_h::operator =(AnyVal* aAnyVal)
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
void Datum_h::operator =(ForthCell* aFip)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  vFip(aFip);
}

/*----------------------------------------------------------------------------*/
/* assign/copy an integer                                                     */
/*----------------------------------------------------------------------------*/
void Datum_h::operator =(i8 iInt)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  vInt(iInt);
}

/*----------------------------------------------------------------------------*/
/* assign/copy a pointer to a proper object                                   */
/*----------------------------------------------------------------------------*/
void Datum_h::operator =(Object* pObj)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  vObj(pObj);
}

/*----------------------------------------------------------------------------*/
/* assign/copy a string                                                       */
/*----------------------------------------------------------------------------*/
void Datum_h::operator =(Stz* szStz)
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
bool Datum_h::bIsNull(void)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);

  if (m_xIntVal.m_wInt.bIsNull())
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* set null                                                                   */
/*----------------------------------------------------------------------------*/
void Datum_h::vSetNull(void)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);

  m_xIntVal.vSetNull();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get class of embedded value                                                */
/*----------------------------------------------------------------------------*/
eCN Datum_h::eAnyClass(void)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  return m_xAnyVal.eClass();
}

/*----------------------------------------------------------------------------*/
/* get aAnyVal                                                                */
/*----------------------------------------------------------------------------*/
AnyVal* Datum_h::aAnyVal(void)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  return & m_xAnyVal;
}

/*----------------------------------------------------------------------------*/
/* get decimal                                                                */
/*----------------------------------------------------------------------------*/
i8 Datum_h::iBcd(void)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  i8 iBcd = m_xBcdVal.m_wBcd;
  
  return iBcd;
}

/*----------------------------------------------------------------------------*/
/* set decimal                                                                */
/*----------------------------------------------------------------------------*/
void Datum_h::vBcd(i8 iBcd)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  m_xBcdVal.m_wBcd = iBcd;
}

/*----------------------------------------------------------------------------*/
/* get boolean                                                                */
/*----------------------------------------------------------------------------*/
i8 Datum_h::iBol(void)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  i8 iBol = m_xBolVal.m_wBol;
  
  return iBol;
}

/*----------------------------------------------------------------------------*/
/* set boolean                                                                */
/*----------------------------------------------------------------------------*/
void Datum_h::vBol(i8 iBol)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  m_xBolVal.m_wBol = iBol;
}

/*----------------------------------------------------------------------------*/
/* get azChar                                                                 */
/*----------------------------------------------------------------------------*/
char* Datum_h::azChar(void)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  char* az = m_xStzVal.azChar();
  
  return az;
}

/*----------------------------------------------------------------------------*/
/* copy given source value into this var                                      */
/*----------------------------------------------------------------------------*/
void Datum_h::vCopy(AnyVal* aAnyVal)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  m_xAnyVal.vCopy(aAnyVal);
}

/*----------------------------------------------------------------------------*/
/* get date                                                                   */
/*----------------------------------------------------------------------------*/
i8 Datum_h::iDte(void)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  i8 iDay = m_xDteVal.m_wDte.m_uDay;
  
  return m_xDteVal.m_wDte;
}

/*----------------------------------------------------------------------------*/
/* get day of date                                                            */
/*----------------------------------------------------------------------------*/
i8 Datum_h::iDte_day(void)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  i8 iDay = m_xDteVal.m_wDte.m_uDay;
  
  return iDay;
}

/*----------------------------------------------------------------------------*/
/* get month of date                                                          */
/*----------------------------------------------------------------------------*/
i8 Datum_h::iDte_month(void)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  i8 iMonth = m_xDteVal.m_wDte.m_uMonth;
  
  return iMonth;
}

/*----------------------------------------------------------------------------*/
/* get year of date                                                           */
/*----------------------------------------------------------------------------*/
i8 Datum_h::iDte_year(void)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  i8 iYear = m_xDteVal.m_wDte.m_uYearDelta + C_YEAR_BASE;
  
  return iYear;
}

/*----------------------------------------------------------------------------*/
/* set date from parts                                                        */
/*----------------------------------------------------------------------------*/
void Datum_h::vDte(i8 iDte)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  m_xDteVal.m_wDte = iDte;
}

/*----------------------------------------------------------------------------*/
/* set date from parts                                                        */
/*----------------------------------------------------------------------------*/
void Datum_h::vDte(i8 iYear, i8 iMonth, i8 iDay)
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
i8 Datum_h::iEdt(void)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  i8 iEdt = m_xEdtVal.m_wEdt;
  
  return iEdt;
}

/*----------------------------------------------------------------------------*/
/* get forth instruction pointer                                              */
/*----------------------------------------------------------------------------*/
ForthCell* Datum_h::aFip(void)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  ForthCell* aFip = m_xFipVal.aFip();
  
  return aFip;
}

/*----------------------------------------------------------------------------*/
/* set forth instruction pointer                                              */
/*----------------------------------------------------------------------------*/
void Datum_h::vFip(ForthCell* aFip)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  m_xFipVal.vFip(aFip);
}

/*----------------------------------------------------------------------------*/
/* get float                                                                  */
/*----------------------------------------------------------------------------*/
d8 Datum_h::dFlt(void)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  d8 dFlt = m_xFltVal.m_wFlt.m_dFlt;
  
  return dFlt;
}

/*----------------------------------------------------------------------------*/
/* set float                                                                  */
/*----------------------------------------------------------------------------*/
void Datum_h::vFlt(d8 dFlt)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  m_xFltVal.m_wFlt.m_dFlt = dFlt;
}

/*----------------------------------------------------------------------------*/
/* get integer                                                                */
/*----------------------------------------------------------------------------*/
i8 Datum_h::iInt(void)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  i8 iInt = m_xIntVal.iInt();
  
  return iInt;
}

/*----------------------------------------------------------------------------*/
/* set integer                                                                */
/*----------------------------------------------------------------------------*/
void Datum_h::vInt(i8 iInt)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  m_xIntVal.vInt(iInt);
}

/*----------------------------------------------------------------------------*/
/* test if nested value is an integer                                         */
/*----------------------------------------------------------------------------*/
bool Datum_h::bIsInt(void)
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
i8 Datum_h::iIvl(void)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  i8 iIvl = m_xIvlVal.m_wIvl;
  
  return iIvl;
}

/*----------------------------------------------------------------------------*/
/* set interval                                                               */
/*----------------------------------------------------------------------------*/
void Datum_h::vIvl(i8 iIvl)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  m_xIvlVal.m_wIvl = iIvl;
}

/*----------------------------------------------------------------------------*/
/* set interval from parts                                                    */
/*----------------------------------------------------------------------------*/
void Datum_h::vIvl(i8 iSecond, i8 iMicro)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  m_xIvlVal.m_wIvl.m_uSecond = iSecond;
  m_xIvlVal.m_wIvl.m_uMicro  = iMicro;
}

/*----------------------------------------------------------------------------*/
/* get object address                                                         */
/*----------------------------------------------------------------------------*/
Object* Datum_h::pObj(void)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  Object* pObj = m_xPtrVal.pObj();
  
  return pObj;
}

/*----------------------------------------------------------------------------*/
/* set object address                                                         */
/*----------------------------------------------------------------------------*/
void Datum_h::vObj(Object* pObj)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  m_xPtrVal.vObj(pObj);
}

/*----------------------------------------------------------------------------*/
/* get address of space area                                                  */
/*----------------------------------------------------------------------------*/
byte* Datum_h::ayaSpace(void)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  byte* ayaSpace =
    aAdd<byte*>(this,
                 sizeof(Object) + sizeof(m_nSizeSpace) +
                 sizeof(StzVal)) ;
  

  return ayaSpace;
}

/*----------------------------------------------------------------------------*/
/* get bytes required to push value on stack                                  */
/*----------------------------------------------------------------------------*/
i8 Datum_h::nStackUsage(void)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  i8 nBytes = m_xStzVal.nStackUsage();
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* get stz                                                                    */
/*----------------------------------------------------------------------------*/
Stz* Datum_h::szStz(void)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  Stz* sz = m_xStzVal.szStz();
  
  return sz;
}

/*----------------------------------------------------------------------------*/
/* set stz                                                                    */
/*----------------------------------------------------------------------------*/
void Datum_h::vStz(Stz* szStz)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  m_xStzVal.vStz(szStz);
}

/*----------------------------------------------------------------------------*/
/* set stz                                                                    */
/*----------------------------------------------------------------------------*/
void Datum_h::vStz(char* acnChar, i8 nLength)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  m_xStzVal.vStz(acnChar,nLength);
}

/*----------------------------------------------------------------------------*/
/* set stz                                                                    */
/*----------------------------------------------------------------------------*/
void Datum_h::vStzDump(void)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  vTrace("vStzDump() : "_Stz);
    
  vTrace("|%02lld|%02lld|%s|\n"_Stz,
         nStzSize(),
         szStz()->nLength(),
         szStz());
}

/*----------------------------------------------------------------------------*/
/* get size of nested string value                                            */
/*----------------------------------------------------------------------------*/
i8 Datum_h::nStzSize(void)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  i8 nSize = m_xStzVal.nStzSize();
  
  return nSize;
}

/*----------------------------------------------------------------------------*/
/* set size of nested string value                                            */
/*----------------------------------------------------------------------------*/
void Datum_h::vStzSize(i8 nSize)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  m_xStzVal.vStzSize(nSize);
}

/*----------------------------------------------------------------------------*/
/* get space allowed for nested string value                                  */
/*----------------------------------------------------------------------------*/
i8 Datum_h::nStzSpace(void)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  return m_nSizeSpace;
}

/*----------------------------------------------------------------------------*/
/* get time                                                                   */
/*----------------------------------------------------------------------------*/
i8 Datum_h::iTme(void)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  i8 iTme = m_xTmeVal.m_wTme;
  
  return iTme;
}

/*----------------------------------------------------------------------------*/
/* set time                                                                   */
/*----------------------------------------------------------------------------*/
void Datum_h::vTme(i8 iTme)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  m_xTmeVal.m_wTme = iTme;
}

/*----------------------------------------------------------------------------*/
/* set time from parts                                                        */
/*----------------------------------------------------------------------------*/
void Datum_h::vTme(i8 iHour, i8 iMinute, i8 iSecond, i8 iMicro)
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
void Datum_h::vToString(Stz* szString)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  m_xAnyVal.vToString(szString);
}

/*----------------------------------------------------------------------------*/
/* get timestamp                                                              */
/*----------------------------------------------------------------------------*/
i8 Datum_h::iTsp(void)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  i8 iTsp = m_xTspVal.m_wTsp;
  
  return iTsp;
}

/*----------------------------------------------------------------------------*/
/* set timestamp                                                              */
/*----------------------------------------------------------------------------*/
void Datum_h::vTsp(i8 iTsp)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  m_xTspVal.m_wTsp = iTsp;
}

/*----------------------------------------------------------------------------*/
/* set timestamp from parts                                                   */
/*----------------------------------------------------------------------------*/
void Datum_h::vTsp(i8 iYear, i8 iMonth,  i8 iDay,
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
Footer* Datum_h::aFooter(void)
{
  _precondition_(_MAGIC_,CN_DATUM);
  
  Footer* aFooter_calc =
    aAdd<Footer*>(this,
                  sizeof(Object) + sizeof(m_nSizeSpace) +
                  sizeof(StzVal) + m_nSizeSpace) ;
  
  return aFooter_calc;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

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
  
  m_pCol   = nullptr;
  u.m_xAnyVal.m_iInt   = C_UNKNOWN;
  m_nChars = 0;

  u.m_xAnyVal.vMagic();
  u.m_xAnyVal.vClass(CN_NONE);

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
  
  m_pCol   = nullptr;
  u.m_xAnyVal.m_iInt   = C_UNKNOWN;
  m_nChars = 0;

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






#if 0

/*----------------------------------------------------------------------------*/
/* extract a decimal datum from csv field value                               */
/*----------------------------------------------------------------------------*/
bool Datum::bExtractBcd(char** aazCrs, Format* pFmt)
{
  SN_Bcd snBcd;
  
  Column* pColumn = pDatum->m_pCol;
  
  try {
    pDatum->u.m_xAnyVal.m_iBcd = snBcd.i(aazCrs,pColumn,pFmt);
  }
  catch (Exception xException) {
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract a boolean datum from csv field value                               */
/*----------------------------------------------------------------------------*/
bool Datum::bExtractBol(char** aazCrs, Format* pFmt)
{
  SN_Bol snBol;
  
  Column* pColumn = pDatum->m_pCol;
  
  try {
    pDatum->u.m_xAnyVal.m_iBol = snBol.i(aazCrs,pColumn,pFmt);
  }
  catch (Exception xException) {
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract a date datum from csv field value                      yyyy-mo-ddZ */
/*----------------------------------------------------------------------------*/
bool Datum::bExtractDte(char** aazCrs, Format* pFmt)
{
  SN_Dte snDte;
  
  Column* pColumn = pDatum->m_pCol;
  
  try {
    pDatum->u.m_xAnyVal.m_iDte = snDte.i(aazCrs,pColumn,pFmt);
  }
  catch (Exception xException) {
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract a float datum from csv field value                                 */
/*----------------------------------------------------------------------------*/
bool Datum::bExtractFlt(char** aazCrs, Format* pFmt)
{
  SN_Flt snFlt;
  
  Column* pColumn = pDatum->m_pCol;
  
  try {
    pDatum->u.m_xAnyVal.m_dFlt = snFlt.d(aazCrs,pColumn,pFmt);
  }
  catch (Exception xException) {
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract an integer datum from csv field value                              */
/*----------------------------------------------------------------------------*/
bool Datum::bExtractInt(char** aazCrs, Format* pFmt)
{
  SN_Int snInt;
  
  Column* pColumn = pDatum->m_pCol;
  
  try {
    pDatum->u.m_xAnyVal.m_iInt = snInt.i(aazCrs,pColumn,pFmt);
  }
  catch (Exception xException) {
    return false;
  }
  
  // ignore any trailing digits (after optional '.')
  if ('.' == **aazCrs) {
    
    // skip the '.' and any digits
    (*aazCrs)++;
    StrLib::vSkipDigits(aazCrs);
  }
  
  return true;
}


/*----------------------------------------------------------------------------*/
/* extract an interval datum from csv field value                             */
/*----------------------------------------------------------------------------*/
bool Datum::bExtractIvl(char** aazCrs, Format* pFmt)
{
  SN_Ivl snIvl;
  
  Column* pColumn = pDatum->m_pCol;
  
  try {
    pDatum->u.m_xAnyVal.m_iIvl = snIvl.i(aazCrs,pColumn,pFmt);
  }
  catch (Exception xException) {
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract a time datum from csv field value                    hh:mi:ss.fffZ */
/*----------------------------------------------------------------------------*/
bool Datum::bExtractTme(char** aazCrs, Format* pFmt)
{
  SN_Tsp snTsp;
  
  Column* pColumn = pDatum->m_pCol;
  
  try {
    pDatum->u.m_xAnyVal.m_iTsp = snTsp.i(aazCrs,pColumn,pFmt);
  }
  catch (Exception xException) {
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract a timestamp datum from csv field value    yyyy-mo-ddThh:mi:ss.fffZ */
/*----------------------------------------------------------------------------*/
bool Datum::bExtractTsp(char** aazCrs, Format* pFmt)
{
  SN_Tsp snTsp;
  
  Column* pColumn = pDatum->m_pCol;
  
  try {
    pDatum->u.m_xAnyVal.m_iTsp = snTsp.i(aazCrs,pColumn,pFmt);
  }
  catch (Exception xException) {
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract a varchar datum from csv field value                               */
/*----------------------------------------------------------------------------*/
bool Datum::bExtractVch(char** aazCrs, Format* pFmt)
{
  SN_Vch snVch;
  bool bSawEnclosedLeading = false;
  
  Column* pColumn = pDatum->m_pCol;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // test for enclosed
  bSawEnclosedLeading = bIsEnclosedString(aazCrs,pFmt);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // varchar field value processing
  
  try {
    pDatum->m_sVch = snVch.sz(aazCrs,pColumn,pFmt,
                              bSawEnclosedLeading,pDatum->m_nChars);
  }
  catch (Exception xException) {
    return false;
  }
  
  return true;
}

#endif

/*----------------------------------------------------------------------------*/
/* test null                                                                   */
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
/* get forth instruction pointer                                              */
/*----------------------------------------------------------------------------*/
Cell* Datum::aFip(void)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  Cell* aFip = u.m_xFipVal.aFip();
  
  return aFip;
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
/* get address of the nested value                                            */
/*----------------------------------------------------------------------------*/
AnyVal* Datum::aEmbeddedAnyVal(void)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  return & u.m_xAnyVal;
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
/* get size of nested string value                                            */
/*----------------------------------------------------------------------------*/
i8 Datum::nStzSize(void)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  i8 nSize = u.m_xStzVal.nStzSize();
  
  return nSize;
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

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Datum::aFooter(void)
{
  _precondition_(_MAGIC_,CN_DATUM);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

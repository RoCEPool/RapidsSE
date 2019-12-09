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
  m_wDtm.m_iInt   = C_UNKNOWN;
  m_nChars = 0;

  //m_sVch
  
  _checkInitialised_(CN_DATUM);
  
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

#if 0

/*----------------------------------------------------------------------------*/
/* extract a decimal datum from csv field value                               */
/*----------------------------------------------------------------------------*/
bool Datum::bExtractBcd(char** aazCrs, Format* pFmt)
{
  SN_Bcd snBcd;
  
  Column* pColumn = pDatum->m_pCol;
  
  try {
    pDatum->m_wDtm.m_iBcd = snBcd.i(aazCrs,pColumn,pFmt);
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
    pDatum->m_wDtm.m_iBol = snBol.i(aazCrs,pColumn,pFmt);
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
    pDatum->m_wDtm.m_iDte = snDte.i(aazCrs,pColumn,pFmt);
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
    pDatum->m_wDtm.m_dFlt = snFlt.d(aazCrs,pColumn,pFmt);
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
    pDatum->m_wDtm.m_iInt = snInt.i(aazCrs,pColumn,pFmt);
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
    pDatum->m_wDtm.m_iIvl = snIvl.i(aazCrs,pColumn,pFmt);
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
    pDatum->m_wDtm.m_iTsp = snTsp.i(aazCrs,pColumn,pFmt);
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
    pDatum->m_wDtm.m_iTsp = snTsp.i(aazCrs,pColumn,pFmt);
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

  if (m_wDtm.m_iInt == C_DATUM_NULL)
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

  m_wDtm.m_iInt = C_DATUM_NULL;
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

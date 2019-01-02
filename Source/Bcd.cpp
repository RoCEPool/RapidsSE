/*============================================================================*/
/* Bcd.cpp -- Bcd datum                                                       */
/*============================================================================*/

#include "Datums.h"

#include "StorageEngine.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Bcd::operator new(size_t)
{
  vFatal("fatal: Bcd class cannot be new'd\n"_Stz);
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Bcd::Bcd(void)
{
  vInitZero();
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Bcd::~Bcd(void) noexcept(false)
{
}

/*----------------------------------------------------------------------------*/
/*-- Operators ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* is this decimal less than given decimal?                                   */
/*----------------------------------------------------------------------------*/
bool Bcd::operator < (Bcd& xBcd)
{
  if (m_iBcd < xBcd.m_iBcd)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this decimal less than or equal to given decimal?                       */
/*----------------------------------------------------------------------------*/
bool Bcd::operator <= (Bcd& xBcd)
{
  if (m_iBcd <= xBcd.m_iBcd)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this decimal equal to given decimal?                                    */
/*----------------------------------------------------------------------------*/
bool Bcd::operator == (Bcd& xBcd)
{
  if (m_iBcd == xBcd.m_iBcd)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this decimal not equal to given decimal?                                */
/*----------------------------------------------------------------------------*/
bool Bcd::operator != (Bcd& xBcd)
{
  if (m_iBcd != xBcd.m_iBcd)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this decimal greater than or equal to given decimal?                    */
/*----------------------------------------------------------------------------*/
bool Bcd::operator >= (Bcd& xBcd)
{
  if (m_iBcd >= xBcd.m_iBcd)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this decimal greater than given decimal?                                */
/*----------------------------------------------------------------------------*/
bool Bcd::operator > (Bcd& xBcd)
{
  if (m_iBcd > xBcd.m_iBcd)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get as i8                                                                  */
/*----------------------------------------------------------------------------*/
i8 Bcd::iAsI8(void)
{
  return m_iBcd;
}

/*----------------------------------------------------------------------------*/
/* initialse to all zero                                                      */
/*----------------------------------------------------------------------------*/
void Bcd::vInitZero(void)
{
                                                                 //! to be coded
}

/*----------------------------------------------------------------------------*/
/*-- Null --------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* test if null                                                               */
/*----------------------------------------------------------------------------*/
bool Bcd::bIsNull(void)
{
  if (C_NULL_INT == m_iBcd)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* set to null                                                                */
/*----------------------------------------------------------------------------*/
void Bcd::vSetNull(void)
{
  m_iBcd = C_NULL_INT;
}

/*----------------------------------------------------------------------------*/
/*-- VLQ ---------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- Extract and Sprint ------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* copy bcd datum                                                             */
/*----------------------------------------------------------------------------*/
i8 Bcd::nCopy(void* aDst, void* aSrc)
{
  *(i8*)aDst = *(i8*)aSrc;
  
  return sizeof(i8);
}

/*----------------------------------------------------------------------------*/
/* copy bcd datum (VLQ)                                                       */
/*----------------------------------------------------------------------------*/
i8 Bcd::nCopyVLQ(void* aDst, void* aSrc)
{
  i8 iBcd = IntLib::iVlqToInt((byte*)aSrc);
  
  *(i8*)aDst = iBcd;
  
  return sizeof(i8);
}

/*----------------------------------------------------------------------------*/
/* copy bcd datum                                                             */
/*----------------------------------------------------------------------------*/
void Bcd::vCopy(void* aDst, void* aSrc)
{
  *(i8*)aDst = *(i8*)aSrc;
}

/*----------------------------------------------------------------------------*/
/* extract a decimal field from csv substr                                    */
/*----------------------------------------------------------------------------*/
bool Bcd::bExtract(char** aaSrc, Stz* aDst, DBobjects::Column* pColumn, Formatters* pFormatters)
{
  Bcd* aBcd = (Bcd*)aDst;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // head processing
  
  bool bSawEnclosedLeading =
    StrLib::bSkipLeading(aaSrc,pFormatters->m_cEnclosedBy_Leading);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // null processing
  
  bool bValidNull = false;
  if (DtmLib::bTestForNull(aaSrc,pColumn,pFormatters,bValidNull)) {
    if ( ! bValidNull )
      return false;
    aBcd->vSetNull();
    return true;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // decimal field processing
    
  if ( ! StrLib::bExtractDecimal(aaSrc,aBcd->m_iBcd,
                                 pColumn->iColumnLength(),pColumn->iColumnScale()) )
    return false;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tail processing
  
  bool bSawEnclosedTrailing =
    StrLib::bSkipTrailing(aaSrc,bSawEnclosedLeading,pFormatters->m_cEnclosedBy_Trailing);
  if (! bSawEnclosedTrailing)
    return false;
  
  // should see a valid field end
  if ( ! DtmLib::bValidFieldEnd(*aaSrc,pFormatters) )
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* sprint bcd datum                                                           */
/*----------------------------------------------------------------------------*/
void Bcd::vSprint(Stz* aDst, Stz* aSrc, DBobjects::Column* pColumn)
{
  bool bNegative = false;
  Bcd* aBcd = (Bcd*)aSrc;
  
  if (aBcd->bIsNull())
    StrLib::vSprint(aDst,"null%c"_Stz,
                    '\0');
  else {
    i8 iBcd = *(i8*)aSrc;
    i8 iDigits = 1;
    for (i8 i_Digit = 0; i_Digit < pColumn->iColumnScale(); i_Digit++)
      iDigits *= 10;
    
    i8 iBcd_Whole = iBcd / iDigits;
    i8 iBcd_Fraction = iBcd % iDigits;
    
    if (iBcd_Fraction < 0 || iBcd_Whole < 0) {
      bNegative = true;
      iBcd_Fraction = iBcd_Fraction * -1;
      iBcd_Whole = iBcd_Whole * -1;
    }
    
    if (pColumn->iColumnScale())
      StrLib::vSprint(aDst,"%s%lld.%.*lld%c"_Stz,
                      bNegative ? "-" : "",
                      iBcd_Whole,
                      pColumn->iColumnScale(),
                      iBcd_Fraction,'\0');
    else
      StrLib::vSprint(aDst,"%s%lld%c"_Stz, bNegative ? "-" : "",iBcd_Whole, '\0');
  }
}

/*----------------------------------------------------------------------------*/
/* sprint bcd datum                                                           */
/*----------------------------------------------------------------------------*/
void Bcd::vSprintVLQ(Stz* aDst, Stz* aSrc, DBobjects::Column* pColumn)
{
  
  i8 iInt = IntLib::iVlqToInt((byte*)aSrc);
  Bcd* aBcd = (Bcd*)&iInt;
  
  bool bNegative = false;
  
  if (aBcd->bIsNull())
    StrLib::vSprint(aDst,"null%c"_Stz,
                    '\0');
  else {
    i8 iBcd = iInt;
    i8 iDigits = 1;
    for (i8 i_Digit = 0; i_Digit < pColumn->iColumnScale(); i_Digit++)
      iDigits *= 10;
    
    i8 iBcd_Whole = iBcd / iDigits;
    i8 iBcd_Fraction = iBcd % iDigits;
    
    if (iBcd_Fraction < 0 || iBcd_Whole < 0) {
      bNegative = true;
      iBcd_Fraction = iBcd_Fraction * -1;
      iBcd_Whole = iBcd_Whole * -1;
    }
    
    if (pColumn->iColumnScale())
      StrLib::vSprint(aDst,"%s%lld.%.*lld%c"_Stz,
                      bNegative ? "-" : "",
                      iBcd_Whole,
                      pColumn->iColumnScale(),
                      iBcd_Fraction,'\0');
    else
      StrLib::vSprint(aDst,"%s%lld%c"_Stz, bNegative ? "-" : "",iBcd_Whole, '\0');
  }
}

/*----------------------------------------------------------------------------*/
/* is the size valid ?                                                        */
/*----------------------------------------------------------------------------*/
bool Bcd::bIsValidSize(i8 iPrecision, i8 iScale)
{
  if (iPrecision > C_MAX_PRECISION_BCD)
    return false;
  
  if (iScale > iPrecision)
    return false;
  
  if (iScale > C_MAX_PRECISION_BCD)
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/*-- Embedded Comparators ----------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* is bol argA < aArgB?                                                       */
/*----------------------------------------------------------------------------*/
bool Bcd::bLessThan(char* aArgA, char* aArgB)
{
  bool bLessThan;
  
  if (((Bcd*)aArgA)->bIsNull())
    bLessThan = true;
  
  else if (((Bcd*)aArgB)->bIsNull())
    bLessThan = false;
  
  else {
    i8 iA = *(i8*)aArgA;
    i8 iB = *(i8*)aArgB;
    
    if (iA < iB)
      bLessThan = true;
    else
      bLessThan = false;
  }
  
  return bLessThan;
}

/*----------------------------------------------------------------------------*/
/* is bol argA <= aArgB?                                                      */
/*----------------------------------------------------------------------------*/
bool Bcd::bLessEqual(char* aArgA, char* aArgB)
{
  bool bLessEqual;
  
  if (((Bcd*)aArgA)->bIsNull() && ((Bcd*)aArgB)->bIsNull())
    bLessEqual = true;
  
  else if (((Bcd*)aArgA)->bIsNull())
    bLessEqual = true;
  
  else if (((Bcd*)aArgB)->bIsNull())
    bLessEqual = false;
  
  else {
    i8 iA = *(i8*)aArgA;
    i8 iB = *(i8*)aArgB;
    
    if (iA <= iB)
      bLessEqual = true;
    else
      bLessEqual = false;
  }
  
  return bLessEqual;
}

/*----------------------------------------------------------------------------*/
/* is bol argA == aArgB?                                                      */
/*----------------------------------------------------------------------------*/
bool Bcd::bEqualTo(char* aArgA, char* aArgB)
{
  bool bEqualTo;
  
  i8 iA = *(i8*)aArgA;
  i8 iB = *(i8*)aArgB;
  
  if (iA == iB)
    bEqualTo = true;
  else
    bEqualTo = false;
  
  return bEqualTo;
}

/*----------------------------------------------------------------------------*/
/* is bol argA != aArgB?                                                      */
/*----------------------------------------------------------------------------*/
bool Bcd::bNotEqual(char* aArgA, char* aArgB)
{
  bool bNotEqual;
  
  i8 iA = *(i8*)aArgA;
  i8 iB = *(i8*)aArgB;
  
  if (iA != iB)
    bNotEqual = true;
  else
    bNotEqual = false;
  
  return bNotEqual;
}

/*----------------------------------------------------------------------------*/
/* is bol argA >= aArgB?                                                      */
/*----------------------------------------------------------------------------*/
bool Bcd::bGreaterEqual(char* aArgA, char* aArgB)
{
  bool bGreaterEqual;
  
  if (((Bcd*)aArgA)->bIsNull() && ((Bcd*)aArgB)->bIsNull())
    bGreaterEqual = true;
  
  else if (((Bcd*)aArgA)->bIsNull())
    bGreaterEqual = false;
  
  else if (((Bcd*)aArgB)->bIsNull())
    bGreaterEqual = true;
  
  else {
    i8 iA = *(i8*)aArgA;
    i8 iB = *(i8*)aArgB;
    
    if (iA >= iB)
      bGreaterEqual = true;
    else
      bGreaterEqual = false;
  }
  
  return bGreaterEqual;
}

/*----------------------------------------------------------------------------*/
/* is bol argA > aArgB?                                                       */
/*----------------------------------------------------------------------------*/
bool Bcd::bGreaterThan(char* aArgA, char* aArgB)
{
  bool bGreaterThan;
    
  if (((Bcd*)aArgA)->bIsNull())
    bGreaterThan = false;
  
  else if (((Bcd*)aArgB)->bIsNull())
    bGreaterThan = true;
  
  else {
    i8 iA = *(i8*)aArgA;
    i8 iB = *(i8*)aArgB;
    
    if (iA > iB)
      bGreaterThan = true;
    else
      bGreaterThan = false;
  }
  
  return bGreaterThan;
}

/*----------------------------------------------------------------------------*/
/* cmp argA with argB (-ve: argA < argB, 0: argA == argB, +ve: argA > argB)   */
/*----------------------------------------------------------------------------*/
i8 Bcd::iCmp(char* aArgA, char* aArgB)
{
  //----------------------------------------------------------------------------
  // check for nulls first
  if (((Bcd*)aArgA)->bIsNull() && ((Bcd*)aArgB)->bIsNull())
    return 0;
  
  if (((Bcd*)aArgA)->bIsNull())
    return -1;
  
  if (((Bcd*)aArgB)->bIsNull())
    return 1;
  
  i8 iA = *(i8*)aArgA;
  i8 iB = *(i8*)aArgB;
  
  if (iA > iB)
    return 1;
  
  if (iA < iB)
    return -1;
  
  return 0;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* Tme.cpp -- time datum                                                      */
/*============================================================================*/

#include "Datums.h"

#include "StorageEngine.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Tme::operator new(size_t)
{
  vFatal("fatal: Tme class cannot be new'd\n"_Stz);
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Tme::Tme(void)
{
  vInitZero();
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Tme::~Tme(void) noexcept(false)
{
}

/*----------------------------------------------------------------------------*/
/*-- Operators ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* is this time less than given time?                                         */
/*----------------------------------------------------------------------------*/
bool Tme::operator < (Tme& xTme)
{
  if (m_iValue < xTme.m_iValue)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this time less than or equal to given time?                             */
/*----------------------------------------------------------------------------*/
bool Tme::operator <= (Tme& xTme)
{
  if (m_iValue <= xTme.m_iValue)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this time equal to given time?                                          */
/*----------------------------------------------------------------------------*/
bool Tme::operator == (Tme& xTme)
{
  if (m_iValue == xTme.m_iValue)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this time not equal to given time?                                      */
/*----------------------------------------------------------------------------*/
bool Tme::operator != (Tme& xTme)
{
  if (m_iValue != xTme.m_iValue)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this time greater than or equal to given time?                          */
/*----------------------------------------------------------------------------*/
bool Tme::operator >= (Tme& xTme)
{
  if (m_iValue >= xTme.m_iValue)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this time greater than given time?                                      */
/*----------------------------------------------------------------------------*/
bool Tme::operator > (Tme& xTme)
{
  if (m_iValue > xTme.m_iValue)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* initialse to all zero                                                      */
/*----------------------------------------------------------------------------*/
void Tme::vInitZero(void)
{
  x.m_uHour   = 0;
  x.m_uMinute = 0;
  x.m_uSecond = 0;
  x.m_uMicro  = 0;
  x.m_uSpare  = 0;
}

/*----------------------------------------------------------------------------*/
/* get as i8                                                                  */
/*----------------------------------------------------------------------------*/
i8 Tme::iAsI8(void)
{
  return m_iValue;
}

/*----------------------------------------------------------------------------*/
/* get hour                                                                   */
/*----------------------------------------------------------------------------*/
i8 Tme::iHour(void)
{
  return x.m_uHour;
}

/*----------------------------------------------------------------------------*/
/* get minute                                                                 */
/*----------------------------------------------------------------------------*/
i8 Tme::iMinute(void)
{
  return x.m_uMinute;
}

/*----------------------------------------------------------------------------*/
/* get second                                                                 */
/*----------------------------------------------------------------------------*/
i8 Tme::iSecond(void)
{
  return x.m_uSecond;
}

/*----------------------------------------------------------------------------*/
/* get microseconds                                                           */
/*----------------------------------------------------------------------------*/
i8 Tme::iMicroseconds(void)
{
  return x.m_uMicro;
}

/*----------------------------------------------------------------------------*/
/*-- Null --------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* test if null                                                               */
/*----------------------------------------------------------------------------*/
bool Tme::bIsNull(void)
{
  if (C_NULL_INT == m_iValue)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* set to null                                                                */
/*----------------------------------------------------------------------------*/
void Tme::vSetNull(void)
{
  m_iValue = C_NULL_INT;
}

/*----------------------------------------------------------------------------*/
/*-- VLQ ---------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- Extract and Sprint ------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* copy ivl datum                                                             */
/*----------------------------------------------------------------------------*/
i8 Tme::nCopy(void* aDst, void* aSrc)
{
  *(i8*)aDst = *(i8*)aSrc;
  
  return sizeof(i8);
}

/*----------------------------------------------------------------------------*/
/* copy tme datum                                                             */
/*----------------------------------------------------------------------------*/
void Tme::vCopy(void* aDst, void* aSrc)
{
  *(i8*)aDst = *(i8*)aSrc;
}

/*----------------------------------------------------------------------------*/
/* extract a time field from csv substr                         hh:mi:ss.fffZ */
/*----------------------------------------------------------------------------*/
bool Tme::bExtract(char** aaSrc, Stz* aDst, Column* pColumn, Formatters* pFormatters)
{
  // refer: http://en.wikipedia.org/wiki/ISO_8601
  
  Tme* aTme = (Tme*)aDst;
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
    aTme->vSetNull();
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // time field processing
  
  i8 iHour, iMinute, iSecond, iMicro;
  
  if ( ! StrLib::bExtractTime(aaSrc, iHour, iMinute, iSecond, iMicro) )
    return false;
  
  aTme->x.m_uHour   = iHour;
  aTme->x.m_uMinute = iMinute;
  aTme->x.m_uSecond = iSecond;
  aTme->x.m_uMicro  = iMicro;
  aTme->x.m_uSpare  = 0;
  
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
/* extract a time field from csv substr                         hh:mi:ss.fffZ */
/*----------------------------------------------------------------------------*/
bool Tme::bExtractDefault(char** aaSrc, Stz* aDst, Column* pColumn, Formatters* pFormatters)
{
  // refer: http://en.wikipedia.org/wiki/ISO_8601
  
  Tme* aTme = (Tme*)aDst;
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // head processing
  
  StrLib::vSkipSpaces(aaSrc);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // null processing
  
  bool bValidNull = false;
  if (DtmLib::bTestForNullDefault(aaSrc,pColumn,pFormatters,bValidNull)) {
    if ( ! bValidNull )
      return false;
    aTme->vSetNull();
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // time field processing
  
  i8 iHour, iMinute, iSecond, iMicro;
  
  if ( ! StrLib::bExtractTime(aaSrc, iHour, iMinute, iSecond, iMicro) )
    return false;
  
  aTme->x.m_uHour   = iHour;
  aTme->x.m_uMinute = iMinute;
  aTme->x.m_uSecond = iSecond;
  aTme->x.m_uMicro  = iMicro;
  aTme->x.m_uSpare  = 0;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tail processing
  
  StrLib::vSkipSpaces(aaSrc);
  
  // should see a valid field end
  if ( ! DtmLib::bValidFieldEndDefault(*aaSrc,pFormatters) )
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* sprint tme datum                                                           */
/*----------------------------------------------------------------------------*/
void Tme::vSprint(Stz* aDst, Stz* aSrc)
{
  Tme* aTme = (Tme*)aSrc;
  u8 uMicro = (i8)aTme->x.m_uMicro;
  
  if (aTme->bIsNull()) {
    StrLib::vSprint(aDst,"null%c"_Stz,
                    '\0');
    return;
  }
  
  if (uMicro) {
    char sFraction[16];
    StrLib::vFormatFraction(sFraction,uMicro,6);
    
    StrLib::vSprint(aDst,"%02lld:%02lld:%02lld.%s%c"_Stz,
                    aTme->x.m_uHour,
                    aTme->x.m_uMinute,
                    aTme->x.m_uSecond,
                    sFraction,
                    '\0');
    return;
  }

  StrLib::vSprint(aDst,"%02lld:%02lld:%02lld%c"_Stz,
                  aTme->x.m_uHour,
                  aTme->x.m_uMinute,
                  aTme->x.m_uSecond,
                  '\0');
}

/*----------------------------------------------------------------------------*/
/* sprint tme datum                                                           */
/*----------------------------------------------------------------------------*/
void Tme::vSprintEscaped(Stz* aDst, Stz* aSrc)
{
  Tme* aTme = (Tme*)aSrc;
  u8 uMicro = (i8)aTme->x.m_uMicro;
  
  if (aTme->bIsNull()) {
    StrLib::vSprint(aDst,"null%c"_Stz,
                    '\0');
    return;
  }
  
  if (uMicro) {
    char sFraction[16];
    StrLib::vFormatFraction(sFraction,uMicro,6);
    
    StrLib::vSprint(aDst,"%02lld:%02lld:%02lld.%s%c"_Stz,
                    aTme->x.m_uHour,
                    aTme->x.m_uMinute,
                    aTme->x.m_uSecond,
                    sFraction,
                    '\0');
    return;
  }
  
  StrLib::vSprint(aDst,"%02lld:%02lld:%02lld%c"_Stz,
                  aTme->x.m_uHour,
                  aTme->x.m_uMinute,
                  aTme->x.m_uSecond,
                  '\0');
}

/*----------------------------------------------------------------------------*/
/*-- Embedded Comparators ----------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* is tme argA < aArgB?                                                       */
/*----------------------------------------------------------------------------*/
bool Tme::bLessThan(char* aArgA, char* aArgB)
{
  bool bLessThan;
  
  if (((Tme*)aArgA)->bIsNull())
    bLessThan = true;
  
  else if (((Tme*)aArgB)->bIsNull())
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
/* is tme argA <= aArgB?                                                      */
/*----------------------------------------------------------------------------*/
bool Tme::bLessEqual(char* aArgA, char* aArgB)
{
  bool bLessEqual;
  
  if (((Tme*)aArgA)->bIsNull() && ((Tme*)aArgB)->bIsNull())
    bLessEqual = true;
  
  else if (((Tme*)aArgA)->bIsNull())
    bLessEqual = true;
  
  else if (((Tme*)aArgB)->bIsNull())
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
/* is tme argA == aArgB?                                                      */
/*----------------------------------------------------------------------------*/
bool Tme::bEqualTo(char* aArgA, char* aArgB)
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
/* is tme argA != aArgB?                                                      */
/*----------------------------------------------------------------------------*/
bool Tme::bNotEqual(char* aArgA, char* aArgB)
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
/* is tme argA >= aArgB?                                                      */
/*----------------------------------------------------------------------------*/
bool Tme::bGreaterEqual(char* aArgA, char* aArgB)
{
  bool bGreaterEqual;
  
  if (((Tme*)aArgA)->bIsNull() && ((Tme*)aArgB)->bIsNull())
    bGreaterEqual = true;
  
  else if (((Tme*)aArgA)->bIsNull())
    bGreaterEqual = false;
  
  else if (((Tme*)aArgB)->bIsNull())
    bGreaterEqual = true;
  
  else {
    
    i8 iA = *(i8*)aArgA;
    i8 iB = *(i8*)aArgB;
    
    if (iA >= iB)
      bGreaterEqual = true;
    else
      bGreaterEqual = false;
    
//    Tme xTmeA;
//    Tme xTmeB;
//    
//    *(i8*)&xTmeA.x = iA;
//    *(i8*)&xTmeB.x = iB;
//    
//    vTrace("%d:%d:%d.%d >= %d:%d:%d.%d %s\n"_Stz,
//           xTmeA.iHour(),
//           xTmeA.iMinute(),
//           xTmeA.iSecond(),
//           xTmeA.iMicroseconds(),
//
//           xTmeB.iHour(),
//           xTmeB.iMinute(),
//           xTmeB.iSecond(),
//           xTmeA.iMicroseconds(),
//           
//           bGreaterEqual ? "true" : "false"
//           );
  }
  return bGreaterEqual;
}

/*----------------------------------------------------------------------------*/
/* is tme argA > aArgB?                                                       */
/*----------------------------------------------------------------------------*/
bool Tme::bGreaterThan(char* aArgA, char* aArgB)
{
  bool bGreaterThan;
  
  if (((Tme*)aArgA)->bIsNull())
    bGreaterThan = true;
  
  else if (((Tme*)aArgB)->bIsNull())
    bGreaterThan = false;
  
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
i8 Tme::iCmp(char* aArgA, char* aArgB)
{
  //----------------------------------------------------------------------------
  // check for nulls first
  if (((Tme*)aArgA)->bIsNull() && ((Tme*)aArgB)->bIsNull())
    return 0;
  
  if (((Tme*)aArgA)->bIsNull())
    return -1;
  
  if (((Tme*)aArgB)->bIsNull())
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

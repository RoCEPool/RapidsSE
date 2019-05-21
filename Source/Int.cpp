/*============================================================================*/
/* Int.cpp -- int datum                                                       */
/*============================================================================*/

#include "Datums.h"

#include "StorageEngine.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Int::operator new(size_t)
{
  vFatal("fatal: Int class cannot be new'd\n"_Stz);
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Int::Int(void)
{
  m_iInt = 0;
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Int::~Int(void) noexcept(false)
{
}

/*----------------------------------------------------------------------------*/
/*-- Operators ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

///*----------------------------------------------------------------------------*/
///* is this integer less than given integer?                                   */
///*----------------------------------------------------------------------------*/
//bool Int::operator < (Int& xInt)
//{
//  // NEED TO CHECK FOR NULLS
//  
//  bool bLessThan;
//  
//  if (m_iInt < xInt.m_iInt)
//    bLessThan = true;
//  else
//    bLessThan = false;
//  
//  ASSERT(0);
//  
//  return bLessThan;
//}
//
///*----------------------------------------------------------------------------*/
///* is this integer less than or equal to given integer?                       */
///*----------------------------------------------------------------------------*/
//bool Int::operator <= (Int& xInt)
//{
//  // NEED TO CHECK FOR NULLS
//  
//  bool bLessEqual;
//  
//  if (m_iInt <= xInt.m_iInt)
//    bLessEqual = true;
//  else
//    bLessEqual = false;
//  
//  ASSERT(0);
//  
//  return bLessEqual;
//}
//
///*----------------------------------------------------------------------------*/
///* is this integer equal to given integer?                                    */
///*----------------------------------------------------------------------------*/
//bool Int::operator == (Int& xInt)
//{
//  // NEED TO CHECK FOR NULLS
//  
//  bool bEqualTo;
//  
//  if (m_iInt == xInt.m_iInt)
//    bEqualTo = true;
//  else
//    bEqualTo = false;
//  
//  ASSERT(0);
//  
//  return bEqualTo;
//}
//
///*----------------------------------------------------------------------------*/
///* is this integer not equal to given integer?                                */
///*----------------------------------------------------------------------------*/
//bool Int::operator != (Int& xInt)
//{
//  // NEED TO CHECK FOR NULLS
//  
//  bool bNotEqual;
//  
//  if (m_iInt != xInt.m_iInt)
//    bNotEqual = true;
//  else
//    bNotEqual = false;
//  
//  ASSERT(0);
//  
//  return bNotEqual;
//}
//
///*----------------------------------------------------------------------------*/
///* is this integer greater than or equal to given integer?                    */
///*----------------------------------------------------------------------------*/
//bool Int::operator >= (Int& xInt)
//{
//  // NEED TO CHECK FOR NULLS
//  
//  bool bGreaterEqual;
//  
//  if (m_iInt >= xInt.m_iInt)
//    bGreaterEqual = true;
//  else
//    bGreaterEqual = false;
//  
//  ASSERT(0);
//  
//  return bGreaterEqual;
//}
//
///*----------------------------------------------------------------------------*/
///* is this integer greater than given integer?                                */
///*----------------------------------------------------------------------------*/
//bool Int::operator > (Int& xInt)
//{
//  // NEED TO CHECK FOR NULLS
//  
//  bool bGreaterThan;
//  
//  if (m_iInt > xInt.m_iInt)
//    bGreaterThan = true;
//  else
//    bGreaterThan = false;
//  
//  ASSERT(0);
//  
//  return bGreaterThan;
//}

/*----------------------------------------------------------------------------*/
/* assignment operator                                                        */
/*----------------------------------------------------------------------------*/
//void Int::operator =(i8 iArg)
//{
//  m_iInt = iArg;
//}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* initialse to all zero                                                      */
/*----------------------------------------------------------------------------*/
//void Int::vInitZero(void)
//{
//  m_iInt = 0;
//}

/*----------------------------------------------------------------------------*/
/* get as i8                                                                  */
/*----------------------------------------------------------------------------*/
//i8 Int::iAsI8(void)
//{
//  return m_iInt;
//}

/*----------------------------------------------------------------------------*/
/*-- Null --------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* test if null                                                               */
/*----------------------------------------------------------------------------*/
bool Int::bIsNull(void)
{
  if (C_DATUM_NULL == m_iInt)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* set to null                                                                */
/*----------------------------------------------------------------------------*/
//void Int::vSetNull(void)
//{
//  m_iInt = C_DATUM_NULL;
//}

/*----------------------------------------------------------------------------*/
/* sprint int datum                                                           */
/*----------------------------------------------------------------------------*/
//void Int::vSprint(Stz* aDst)
//{
//  if (bIsNull())
//    StrLib::vSprint(aDst,"null"_Stz);
//  else
//    StrLib::vSprint(aDst,"%lld"_Stz,m_iInt);
//}


/*----------------------------------------------------------------------------*/
/*-- Extract and Sprint ------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* copy int datum                                                             */
/*----------------------------------------------------------------------------*/
i8 Int::nCopy(void* aDst, void* aSrc)
{
  *(i8*)aDst = *(i8*)aSrc;
  
  return sizeof(i8);
}

/*----------------------------------------------------------------------------*/
/* copy int datum                                                             */
/*----------------------------------------------------------------------------*/
//void Int::vCopy(void* aDst, void* aSrc)
//{
//  *(i8*)aDst = *(i8*)aSrc;
//}

/*----------------------------------------------------------------------------*/
/* sprint int datum                                                           */
/*----------------------------------------------------------------------------*/
void Int::vSprint(Stz* aDst, Stz* aSrc)
{
  Int* aInt = (Int*)aSrc;
  
  if (aInt->bIsNull())
    StrLib::vSprint(aDst,"null%c"_Stz,
                    '\0');
  else
    StrLib::vSprint(aDst,"%lld%c"_Stz,*(i8*)aSrc,'\0');
}

/*----------------------------------------------------------------------------*/
/*-- Embedded Comparators ----------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* is embedded int argA < embedded aArgB?                                     */
/*----------------------------------------------------------------------------*/
bool Int::bLessThan(char* aArgA, char* aArgB)
{
  bool bLessThan;
  
  if (((Int*)aArgA)->bIsNull())
    bLessThan = true;
  
  else if (((Int*)aArgB)->bIsNull())
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
/* is embedded int argA <= embedded aArgB?                                    */
/*----------------------------------------------------------------------------*/
bool Int::bLessEqual(char* aArgA, char* aArgB)
{
  bool bLessEqual;
  
  if (((Int*)aArgA)->bIsNull() && ((Int*)aArgB)->bIsNull())
    bLessEqual = true;
  
  else if (((Int*)aArgA)->bIsNull())
    bLessEqual = true;
  
  else if (((Int*)aArgB)->bIsNull())
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
/* is embedded int argA == embedded aArgB?                                    */
/*----------------------------------------------------------------------------*/
bool Int::bEqualTo(char* aArgA, char* aArgB)
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
/* is embedded int argA != embedded aArgB?                                    */
/*----------------------------------------------------------------------------*/
bool Int::bNotEqual(char* aArgA, char* aArgB)
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
/* is embeddedint argA >= embeddedaArgB?                                      */
/*----------------------------------------------------------------------------*/
bool Int::bGreaterEqual(char* aArgA, char* aArgB)
{
  bool bGreaterEqual;
  
//  if (((Int*)aArgA)->bIsNull() && ((Int*)aArgB)->bIsNull())
//    bGreaterEqual = true;
//
//  else if (((Int*)aArgA)->bIsNull())
//    bGreaterEqual = false;
//
//  else if (((Int*)aArgB)->bIsNull())
//    bGreaterEqual = true;
  
  if (((Int*)aArgA)->bIsNull()) {
    if (((Int*)aArgB)->bIsNull())
      return true;
    else
      return false;
  }
  else if (((Int*)aArgA)->bIsNull()) {
      return true;
  }
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
/* is embedded int argA > embedded aArgB?                                     */
/*----------------------------------------------------------------------------*/
bool Int::bGreaterThan(char* aArgA, char* aArgB)
{
  bool bGreaterThan;
  
  if (((Int*)aArgA)->bIsNull())
    bGreaterThan = false;
  
  else if (((Int*)aArgB)->bIsNull())
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
//i8 Int::iCmp(char* aArgA, char* aArgB)
//{
//  //----------------------------------------------------------------------------
//  // check for nulls first
//  if (((Int*)aArgA)->bIsNull() && ((Int*)aArgB)->bIsNull())
//    return 0;
//
//  if (((Int*)aArgA)->bIsNull())
//    return -1;
//
//  if (((Int*)aArgB)->bIsNull())
//    return 1;
//
//  i8 iA = *(i8*)aArgA;
//  i8 iB = *(i8*)aArgB;
//
//  if (iA > iB)
//    return 1;
//
//  if (iA < iB)
//    return -1;
//
//  return 0;
//}
//

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

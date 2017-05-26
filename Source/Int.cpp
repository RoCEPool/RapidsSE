/*============================================================================*/
/* Int.cpp -- int datum                                                       */
/*============================================================================*/

#include "Datums.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Int::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  vFatal("fatal: Int class cannot be new'd\n"_Str);
  
  RASSERT(0);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Int::Int(void)
{
  vInitZero();
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Int::~Int(void)
{
}

/*----------------------------------------------------------------------------*/
/* is this integer less than given integer?                                   */
/*----------------------------------------------------------------------------*/
bool Int::operator < (Int& xInt)
{
  if (m_iInt < xInt.m_iInt)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this integer less than or equal to given integer?                       */
/*----------------------------------------------------------------------------*/
bool Int::operator <= (Int& xInt)
{
  if (m_iInt <= xInt.m_iInt)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this integer equal to given integer?                                    */
/*----------------------------------------------------------------------------*/
bool Int::operator == (Int& xInt)
{
  if (m_iInt == xInt.m_iInt)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this integer not equal to given integer?                                */
/*----------------------------------------------------------------------------*/
bool Int::operator != (Int& xInt)
{
  if (m_iInt != xInt.m_iInt)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this integer greater than or equal to given integer?                    */
/*----------------------------------------------------------------------------*/
bool Int::operator >= (Int& xInt)
{
  if (m_iInt >= xInt.m_iInt)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this integer greater than given integer?                                */
/*----------------------------------------------------------------------------*/
bool Int::operator > (Int& xInt)
{
  if (m_iInt > xInt.m_iInt)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* initialse to all zero                                                      */
/*----------------------------------------------------------------------------*/
void Int::vInitZero(void)
{
  m_iInt = 0;
}

/*----------------------------------------------------------------------------*/
/* get as i8                                                                  */
/*----------------------------------------------------------------------------*/
i8 Int::iAsI8(void)
{
  return m_iInt;
}

/*----------------------------------------------------------------------------*/
/* return the decile of value given its maximum                               */
/*----------------------------------------------------------------------------*/
i8 Int::iDecile(i8 iValue, i8 iMax)
{
  double dPercent = (iValue * 100) / iMax;
  double dDecile = dPercent / 10;
  
  i8 iDecile = round(dDecile);
  
  if (iDecile == 10)
    iDecile = 9;
  
  return iDecile;
}

/*----------------------------------------------------------------------------*/
/* extract int datum                                                          */
/*----------------------------------------------------------------------------*/
bool Int::bExtract(Str* aDst, Str* aSrc, i8& nBytesTaken)
{
  char* aExt = (char*)aSrc;
  
  i8 iInt;
  bool b = StrLib::bExtractI8(&aExt,&iInt);
  if (b) {
    MemLib::vCopy(aDst,&iInt,sizeof(i8));
  
    nBytesTaken = sizeof(i8);
  }
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* sprint int datum                                                           */
/*----------------------------------------------------------------------------*/
void Int::vSprint(Str* aDst, Str* aSrc, i8& nBytesTaken)
{
  StrLib::vSprint(aDst,"%lld%c"_Str,*(i8*)aSrc,0);
  
  nBytesTaken = sizeof(i8);
}

/*----------------------------------------------------------------------------*/
/* is int argA < aArgB?                                                       */
/*----------------------------------------------------------------------------*/
bool Int::bLessThan(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
{
  bool bLessThan;

  ASSERT(sizeof(i8) == nBytesA);
  ASSERT(sizeof(i8) == nBytesB);
  
  i8 iA = *(i8*)aArgA;
  i8 iB = *(i8*)aArgB;
  
  if (iA < iB)
    bLessThan = true;
  else
    bLessThan = false;
  
  return bLessThan;
}

/*----------------------------------------------------------------------------*/
/* is int argA <= aArgB?                                                      */
/*----------------------------------------------------------------------------*/
bool Int::bLessEqual(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
{
  bool bLessEqual;
  
  ASSERT(sizeof(i8) == nBytesA);
  ASSERT(sizeof(i8) == nBytesB);
  
  i8 iA = *(i8*)aArgA;
  i8 iB = *(i8*)aArgB;
  
  if (iA <= iB)
    bLessEqual = true;
  else
    bLessEqual = false;
  
  return bLessEqual;
}

/*----------------------------------------------------------------------------*/
/* is int argA == aArgB?                                                      */
/*----------------------------------------------------------------------------*/
bool Int::bEqualTo(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
{
  bool bEqualTo;
  
  ASSERT(sizeof(i8) == nBytesA);
  ASSERT(sizeof(i8) == nBytesB);
  
  i8 iA = *(i8*)aArgA;
  i8 iB = *(i8*)aArgB;
  
  if (iA == iB)
    bEqualTo = true;
  else
    bEqualTo = false;
  
  return bEqualTo;
}

/*----------------------------------------------------------------------------*/
/* is int argA != aArgB?                                                      */
/*----------------------------------------------------------------------------*/
bool Int::bNotEqual(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
{
  bool bNotEqual;
  
  ASSERT(sizeof(i8) == nBytesA);
  ASSERT(sizeof(i8) == nBytesB);
  
  i8 iA = *(i8*)aArgA;
  i8 iB = *(i8*)aArgB;
  
  if (iA != iB)
    bNotEqual = true;
  else
    bNotEqual = false;
  
  return bNotEqual;
}

/*----------------------------------------------------------------------------*/
/* is int argA >= aArgB?                                                      */
/*----------------------------------------------------------------------------*/
bool Int::bGreaterEqual(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
{
  bool bGreaterEqual;
  
  ASSERT(sizeof(i8) == nBytesA);
  ASSERT(sizeof(i8) == nBytesB);
  
  i8 iA = *(i8*)aArgA;
  i8 iB = *(i8*)aArgB;
  
  if (iA >= iB)
    bGreaterEqual = true;
  else
    bGreaterEqual = false;
  
  return bGreaterEqual;
}

/*----------------------------------------------------------------------------*/
/* is int argA > aArgB?                                                       */
/*----------------------------------------------------------------------------*/
bool Int::bGreaterThan(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
{
  bool bGreaterThan;
  
  ASSERT(sizeof(i8) == nBytesA);
  ASSERT(sizeof(i8) == nBytesB);
  
  i8 iA = *(i8*)aArgA;
  i8 iB = *(i8*)aArgB;
  
  if (iA > iB)
    bGreaterThan = true;
  else
    bGreaterThan = false;
  
  return bGreaterThan;
}

/*----------------------------------------------------------------------------*/
/* cmp argA with argB (-ve: argA < argB, 0: argA == argB, +ve: argA > argB)   */
/*----------------------------------------------------------------------------*/
i8 Int::iCmp(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
{
  ASSERT(sizeof(i8) == nBytesA);
  ASSERT(sizeof(i8) == nBytesB);
  
  i8 iA = *(i8*)aArgA;
  i8 iB = *(i8*)aArgB;

  i8 iCmp = iA - iB;
  
  return iCmp;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

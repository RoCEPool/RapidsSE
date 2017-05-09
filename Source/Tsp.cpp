/*============================================================================*/
/* Tsp.cpp -- timestamp datum                                                 */
/*============================================================================*/

#include "Datums.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Tsp::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  vFatal("fatal: Tsp class cannot be new'd\n"_Str);
  
  RASSERT(0);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Tsp::Tsp(void)
{
  vInitZero();
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Tsp::~Tsp(void)
{
}

/*----------------------------------------------------------------------------*/
/* is this timestamp less than given timestamp?                               */
/*----------------------------------------------------------------------------*/
bool Tsp::operator < (Tsp& xTsp)
{
  if (*(i8*)&x < *(i8*)&xTsp.x)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this timestamp less than or equal to given timestamp?                   */
/*----------------------------------------------------------------------------*/
bool Tsp::operator <= (Tsp& xTsp)
{
  if (*(i8*)&x <= *(i8*)&xTsp.x)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this timestamp equal to given timestamp?                                */
/*----------------------------------------------------------------------------*/
bool Tsp::operator == (Tsp& xTsp)
{
  if (*(i8*)&x == *(i8*)&xTsp.x)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this timestamp not equal to given timestamp?                            */
/*----------------------------------------------------------------------------*/
bool Tsp::operator != (Tsp& xTsp)
{
  if (*(i8*)&x != *(i8*)&xTsp.x)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this timestamp greater than or equal to given timestamp?                */
/*----------------------------------------------------------------------------*/
bool Tsp::operator >= (Tsp& xTsp)
{
  if (*(i8*)&x >= *(i8*)&xTsp.x)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this timestamp greater than given timestamp?                            */
/*----------------------------------------------------------------------------*/
bool Tsp::operator > (Tsp& xTsp)
{
  if (*(i8*)&x > *(i8*)&xTsp.x)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* initialse to all zero                                                      */
/*----------------------------------------------------------------------------*/
void Tsp::vInitZero(void)
{
  x.m_uYear   = 0;
  x.m_uMonth  = 0;
  x.m_uDay    = 0;
  x.m_uHour   = 0;
  x.m_uMinute = 0;
  x.m_uSecond = 0;
  x.m_uMilli  = 0;
}

/*----------------------------------------------------------------------------*/
/* get as i8                                                                  */
/*----------------------------------------------------------------------------*/
i8 Tsp::iAsI8(void)
{
  i8 i = *(i8*)&x;
  
  return i;
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* is tsp argA < aArgB?                                                       */
/*----------------------------------------------------------------------------*/
bool Tsp::bLessThan(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
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
/* is tsp argA <= aArgB?                                                      */
/*----------------------------------------------------------------------------*/
bool Tsp::bLessEqual(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
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
/* is tsp argA == aArgB?                                                      */
/*----------------------------------------------------------------------------*/
bool Tsp::bEqualTo(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
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
/* is tsp argA != aArgB?                                                      */
/*----------------------------------------------------------------------------*/
bool Tsp::bNotEqual(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
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
/* is tsp argA >= aArgB?                                                      */
/*----------------------------------------------------------------------------*/
bool Tsp::bGreaterEqual(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
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
/* is tsp argA > aArgB?                                                       */
/*----------------------------------------------------------------------------*/
bool Tsp::bGreaterThan(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
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
i8 Tsp::iCmp(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
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

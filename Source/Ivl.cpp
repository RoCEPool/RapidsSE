/*============================================================================*/
/* Ivl.cpp -- interval datum                                                  */
/*============================================================================*/

#include "Datums.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Ivl::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  vFatal("fatal: Ivl class cannot be new'd\n"_Str);
  
  RASSERT(0);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Ivl::Ivl(void)
{
  vInitZero();
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Ivl::~Ivl(void)
{
}

/*----------------------------------------------------------------------------*/
/* is this interval less than given interval?                                 */
/*----------------------------------------------------------------------------*/
bool Ivl::operator < (Ivl& xIvl)
{
  if (*(i8*)&x < *(i8*)&xIvl.x)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this interval less than or equal to given interval?                     */
/*----------------------------------------------------------------------------*/
bool Ivl::operator <= (Ivl& xIvl)
{
  if (*(i8*)&x <= *(i8*)&xIvl.x)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this interval equal to given interval?                                  */
/*----------------------------------------------------------------------------*/
bool Ivl::operator == (Ivl& xIvl)
{
  if (*(i8*)&x == *(i8*)&xIvl.x)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this interval not equal to given interval?                              */
/*----------------------------------------------------------------------------*/
bool Ivl::operator != (Ivl& xIvl)
{
  if (*(i8*)&x != *(i8*)&xIvl.x)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this interval greater than or equal to given interval?                  */
/*----------------------------------------------------------------------------*/
bool Ivl::operator >= (Ivl& xIvl)
{
  if (*(i8*)&x >= *(i8*)&xIvl.x)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this interval greater than given interval?                              */
/*----------------------------------------------------------------------------*/
bool Ivl::operator > (Ivl& xIvl)
{
  if (*(i8*)&x > *(i8*)&xIvl.x)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* initialse to all zero                                                      */
/*----------------------------------------------------------------------------*/
void Ivl::vInitZero(void)
{
  x.m_uSec   = 0;
  x.m_uMilli = 0;
}

/*----------------------------------------------------------------------------*/
/* get as i8                                                                  */
/*----------------------------------------------------------------------------*/
i8 Ivl::iAsI8(void)
{
  i8 i = *(i8*)&x;
  
  return i;
}

/*----------------------------------------------------------------------------*/
/* set members                                                                */
/*----------------------------------------------------------------------------*/
void Ivl::vSet(u8 uSec, u8 uMilli)
{
  x.m_uSec   = uSec;
  x.m_uMilli = uMilli;
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* is ivl argA < aArgB?                                                       */
/*----------------------------------------------------------------------------*/
bool Ivl::bLessThan(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
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
/* is ivl argA <= aArgB?                                                      */
/*----------------------------------------------------------------------------*/
bool Ivl::bLessEqual(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
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
/* is ivl argA == aArgB?                                                      */
/*----------------------------------------------------------------------------*/
bool Ivl::bEqualTo(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
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
/* is ivl argA != aArgB?                                                      */
/*----------------------------------------------------------------------------*/
bool Ivl::bNotEqual(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
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
/* is ivl argA >= aArgB?                                                      */
/*----------------------------------------------------------------------------*/
bool Ivl::bGreaterEqual(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
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
/* is ivl argA > aArgB?                                                       */
/*----------------------------------------------------------------------------*/
bool Ivl::bGreaterThan(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
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
i8 Ivl::iCmp(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
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

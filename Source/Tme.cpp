/*============================================================================*/
/* Tme.cpp -- time datum                                                      */
/*============================================================================*/

#include "Datums.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Tme::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  vFatal("fatal: Tme class cannot be new'd\n"_Str);
  
  RASSERT(0);
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
Tme::~Tme(void)
{
}

/*----------------------------------------------------------------------------*/
/* is this time less than given time?                                         */
/*----------------------------------------------------------------------------*/
bool Tme::operator < (Tme& xTme)
{
  if (*(i8*)&x < *(i8*)&xTme.x)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this time less than or equal to given time?                             */
/*----------------------------------------------------------------------------*/
bool Tme::operator <= (Tme& xTme)
{
  if (*(i8*)&x <= *(i8*)&xTme.x)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this time equal to given time?                                          */
/*----------------------------------------------------------------------------*/
bool Tme::operator == (Tme& xTme)
{
  if (*(i8*)&x == *(i8*)&xTme.x)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this time not equal to given time?                                      */
/*----------------------------------------------------------------------------*/
bool Tme::operator != (Tme& xTme)
{
  if (*(i8*)&x != *(i8*)&xTme.x)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this time greater than or equal to given time?                          */
/*----------------------------------------------------------------------------*/
bool Tme::operator >= (Tme& xTme)
{
  if (*(i8*)&x >= *(i8*)&xTme.x)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this time greater than given time?                                      */
/*----------------------------------------------------------------------------*/
bool Tme::operator > (Tme& xTme)
{
  if (*(i8*)&x > *(i8*)&xTme.x)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* initialse to all zero                                                      */
/*----------------------------------------------------------------------------*/
void Tme::vInitZero(void)
{
  x.m_uHour   = 0;
  x.m_uMinute = 0;
  x.m_uSecond = 0;
  x.m_uMilli  = 0;
}

/*----------------------------------------------------------------------------*/
/* get as i8                                                                  */
/*----------------------------------------------------------------------------*/
i8 Tme::iAsI8(void)
{
  i8 i = *(i8*)&x;
  
  return i;
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
/* get milli                                                                  */
/*----------------------------------------------------------------------------*/
i8 Tme::iMilli(void)
{
  return x.m_uMilli;
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* is tme argA < aArgB?                                                       */
/*----------------------------------------------------------------------------*/
bool Tme::bLessThan(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
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
/* is tme argA <= aArgB?                                                      */
/*----------------------------------------------------------------------------*/
bool Tme::bLessEqual(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
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
/* is tme argA == aArgB?                                                      */
/*----------------------------------------------------------------------------*/
bool Tme::bEqualTo(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
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
/* is tme argA != aArgB?                                                      */
/*----------------------------------------------------------------------------*/
bool Tme::bNotEqual(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
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
/* is tme argA >= aArgB?                                                      */
/*----------------------------------------------------------------------------*/
bool Tme::bGreaterEqual(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
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
  
  Tme xTmeA;
  Tme xTmeB;
  
  *(i8*)&xTmeA.x = iA;
  *(i8*)&xTmeB.x = iB;
  
  vTrace("%d:%d:%d.%d >= %d:%d:%d.%d %s\n"_Str,
         xTmeA.iHour(),
         xTmeA.iMinute(),
         xTmeA.iSecond(),
         xTmeA.iMilli(),

         xTmeB.iHour(),
         xTmeB.iMinute(),
         xTmeB.iSecond(),
         xTmeB.iMilli(),
         
         bGreaterEqual ? "true" : "false"
         );
  
  return bGreaterEqual;
}

/*----------------------------------------------------------------------------*/
/* is tme argA > aArgB?                                                       */
/*----------------------------------------------------------------------------*/
bool Tme::bGreaterThan(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
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
i8 Tme::iCmp(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
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

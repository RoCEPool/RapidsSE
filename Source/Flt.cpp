/*============================================================================*/
/* Flt.cpp -- float datum                                                     */
/*============================================================================*/

#include "Datums.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Flt::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  vFatal("fatal: Flt class cannot be new'd\n"_Str);
  
  RASSERT(0);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Flt::Flt(void)
{
  vInitZero();
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Flt::~Flt(void)
{
}

/*----------------------------------------------------------------------------*/
/* is this float less than given float?                                       */
/*----------------------------------------------------------------------------*/
bool Flt::operator < (Flt& xFlt)
{
  if (m_dFlt < xFlt.m_dFlt)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this float less than or equal to given float?                           */
/*----------------------------------------------------------------------------*/
bool Flt::operator <= (Flt& xFlt)
{
  if (m_dFlt <= xFlt.m_dFlt)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this float equal to given float?                                        */
/*----------------------------------------------------------------------------*/
bool Flt::operator == (Flt& xFlt)
{
  if (m_dFlt == xFlt.m_dFlt)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this float not equal to given float?                                    */
/*----------------------------------------------------------------------------*/
bool Flt::operator != (Flt& xFlt)
{
  if (m_dFlt != xFlt.m_dFlt)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this float greater than or equal to given float?                        */
/*----------------------------------------------------------------------------*/
bool Flt::operator >= (Flt& xFlt)
{
  if (m_dFlt >= xFlt.m_dFlt)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this float greater than given float?                                    */
/*----------------------------------------------------------------------------*/
bool Flt::operator > (Flt& xFlt)
{
  if (m_dFlt > xFlt.m_dFlt)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* initialse to all zero                                                      */
/*----------------------------------------------------------------------------*/
void Flt::vInitZero(void)
{
  m_dFlt = 0.0;
}

/*----------------------------------------------------------------------------*/
/* get as d8                                                                  */
/*----------------------------------------------------------------------------*/
d8 Flt::dAsD8(void)
{
  return m_dFlt;
}

/*----------------------------------------------------------------------------*/
/* test if null                                                               */
/*----------------------------------------------------------------------------*/
bool Flt::bIsNull(void)
{
  if (C_NULL_FLT == m_dFlt)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* set to null                                                                */
/*----------------------------------------------------------------------------*/
void Flt::vSetNull(void)
{
  m_dFlt = C_NULL_FLT;
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* is flt argA < aArgB?                                                       */
/*----------------------------------------------------------------------------*/
bool Flt::bLessThan(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
{
  bool bLessThan;
  
  ASSERT(sizeof(d8) == nBytesA);
  ASSERT(sizeof(d8) == nBytesB);
  
  d8 dA = *(d8*)aArgA;
  d8 dB = *(d8*)aArgB;
  
  if (dA < dB)
    bLessThan = true;
  else
    bLessThan = false;
  
  return bLessThan;
}

/*----------------------------------------------------------------------------*/
/* is flt argA <= aArgB?                                                      */
/*----------------------------------------------------------------------------*/
bool Flt::bLessEqual(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
{
  bool bLessEqual;
  
  ASSERT(sizeof(d8) == nBytesA);
  ASSERT(sizeof(d8) == nBytesB);
  
  d8 dA = *(d8*)aArgA;
  d8 dB = *(d8*)aArgB;
  
  if (dA <= dB)
    bLessEqual = true;
  else
    bLessEqual = false;
  
  return bLessEqual;
}

/*----------------------------------------------------------------------------*/
/* is flt argA == aArgB?                                                      */
/*----------------------------------------------------------------------------*/
bool Flt::bEqualTo(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
{
  bool bEqualTo;
  
  ASSERT(sizeof(d8) == nBytesA);
  ASSERT(sizeof(d8) == nBytesB);
  
  d8 dA = *(d8*)aArgA;
  d8 dB = *(d8*)aArgB;
  
  if (dA == dB)
    bEqualTo = true;
  else
    bEqualTo = false;
  
  return bEqualTo;
}

/*----------------------------------------------------------------------------*/
/* is flt argA != aArgB?                                                      */
/*----------------------------------------------------------------------------*/
bool Flt::bNotEqual(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
{
  bool bNotEqual;
  
  ASSERT(sizeof(d8) == nBytesA);
  ASSERT(sizeof(d8) == nBytesB);
  
  d8 dA = *(d8*)aArgA;
  d8 dB = *(d8*)aArgB;
  
  if (dA != dB)
    bNotEqual = true;
  else
    bNotEqual = false;
  
  return bNotEqual;
}

/*----------------------------------------------------------------------------*/
/* is flt argA >= aArgB?                                                      */
/*----------------------------------------------------------------------------*/
bool Flt::bGreaterEqual(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
{
  bool bGreaterEqual;
  
  ASSERT(sizeof(d8) == nBytesA);
  ASSERT(sizeof(d8) == nBytesB);
  
  d8 dA = *(d8*)aArgA;
  d8 dB = *(d8*)aArgB;
  
  if (dA >= dB)
    bGreaterEqual = true;
  else
    bGreaterEqual = false;
  
  return bGreaterEqual;
}

/*----------------------------------------------------------------------------*/
/* is flt argA > aArgB?                                                       */
/*----------------------------------------------------------------------------*/
bool Flt::bGreaterThan(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
{
  bool bGreaterThan;
  
  ASSERT(sizeof(d8) == nBytesA);
  ASSERT(sizeof(d8) == nBytesB);
  
  d8 dA = *(d8*)aArgA;
  d8 dB = *(d8*)aArgB;
  
  if (dA > dB)
    bGreaterThan = true;
  else
    bGreaterThan = false;
  
  return bGreaterThan;
}

/*----------------------------------------------------------------------------*/
/* cmp argA with argB (-ve: argA < argB, 0: argA == argB, +ve: argA > argB)   */
/*----------------------------------------------------------------------------*/
i8 Flt::iCmp(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
{
  ASSERT(sizeof(d8) == nBytesA);
  ASSERT(sizeof(d8) == nBytesB);
  
  d8 dA = *(d8*)aArgA;
  d8 dB = *(d8*)aArgB;
  
  i8 iCmp = dA - dB;
  
  return iCmp;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

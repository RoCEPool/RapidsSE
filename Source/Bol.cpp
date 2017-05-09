/*============================================================================*/
/* Bol.cpp -- boolean datum                                                   */
/*============================================================================*/

#include "Datums.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Bol::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  vFatal("fatal: Bol class cannot be new'd\n"_Str);
  
  RASSERT(0);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Bol::Bol(void)
{
  vInitZero();
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Bol::~Bol(void)
{
}

/*----------------------------------------------------------------------------*/
/* is this boolean less than given boolean?                                   */
/*----------------------------------------------------------------------------*/
bool Bol::operator < (Bol& xBol)
{
  if (m_iBol < xBol.m_iBol)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this boolean less than or equal to given boolean?                       */
/*----------------------------------------------------------------------------*/
bool Bol::operator <= (Bol& xBol)
{
  if (m_iBol <= xBol.m_iBol)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this boolean equal to given boolean?                                    */
/*----------------------------------------------------------------------------*/
bool Bol::operator == (Bol& xBol)
{
  if (m_iBol == xBol.m_iBol)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this boolean not equal to given boolean?                                */
/*----------------------------------------------------------------------------*/
bool Bol::operator != (Bol& xBol)
{
  if (m_iBol != xBol.m_iBol)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this boolean greater than or equal to given boolean?                    */
/*----------------------------------------------------------------------------*/
bool Bol::operator >= (Bol& xBol)
{
  if (m_iBol >= xBol.m_iBol)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this boolean greater than given boolean?                                */
/*----------------------------------------------------------------------------*/
bool Bol::operator > (Bol& xBol)
{
  if (m_iBol > xBol.m_iBol)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* initialse to all zero                                                      */
/*----------------------------------------------------------------------------*/
void Bol::vInitZero(void)
{
  m_iBol = 0;
}

/*----------------------------------------------------------------------------*/
/* get as i8                                                                  */
/*----------------------------------------------------------------------------*/
i8 Bol::iAsI8(void)
{
  return m_iBol;
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* is bol argA < aArgB?                                                       */
/*----------------------------------------------------------------------------*/
bool Bol::bLessThan(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
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
/* is bol argA <= aArgB?                                                      */
/*----------------------------------------------------------------------------*/
bool Bol::bLessEqual(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
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
/* is bol argA == aArgB?                                                      */
/*----------------------------------------------------------------------------*/
bool Bol::bEqualTo(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
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
/* is bol argA != aArgB?                                                      */
/*----------------------------------------------------------------------------*/
bool Bol::bNotEqual(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
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
/* is bol argA >= aArgB?                                                      */
/*----------------------------------------------------------------------------*/
bool Bol::bGreaterEqual(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
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
/* is bol argA > aArgB?                                                       */
/*----------------------------------------------------------------------------*/
bool Bol::bGreaterThan(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
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
i8 Bol::iCmp(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
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

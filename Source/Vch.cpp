/*============================================================================*/
/* Vch.cpp -- varchar datum                                                   */
/*============================================================================*/

#include "Datums.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Vch::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  vFatal("fatal: Vch class cannot be new'd\n"_Str);
  
  RASSERT(0);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Vch::Vch(void)
{
  vInitZero();
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Vch::~Vch(void)
{
}

/*----------------------------------------------------------------------------*/
/* is this varchar less than given varchar?                                   */
/*----------------------------------------------------------------------------*/
bool Vch::operator < (Vch& xVch)
{                                                             //! to be reviewed
  bool bLessThan;
  
  if (StrLib::iCompare((char*)this,(char*)&xVch) < 0)
    bLessThan = true;
  else
    bLessThan = false;
  
  return bLessThan;
}

/*----------------------------------------------------------------------------*/
/* is this varchar less than or equal to given varchar?                       */
/*----------------------------------------------------------------------------*/
bool Vch::operator <= (Vch& xVch)
{                                                             //! to be reviewed
  bool bLessEqual;
  
  if (StrLib::iCompare((char*)this,(char*)&xVch) <= 0)
    bLessEqual = true;
  else
    bLessEqual = false;
  
  return bLessEqual;
}

/*----------------------------------------------------------------------------*/
/* is this varchar equal to given varchar?                                    */
/*----------------------------------------------------------------------------*/
bool Vch::operator == (Vch& xVch)
{                                                             //! to be reviewed
  bool bEqualTo;
  
  if (StrLib::iCompare((char*)this,(char*)&xVch) == 0)
    bEqualTo = true;
  else
    bEqualTo = false;
  
  return bEqualTo;
}

/*----------------------------------------------------------------------------*/
/* is this varchar not equal to given varchar?                                */
/*----------------------------------------------------------------------------*/
bool Vch::operator != (Vch& xVch)
{                                                             //! to be reviewed
  bool bNotEqual;
  
  if (StrLib::iCompare((char*)this,(char*)&xVch) != 0)
    bNotEqual = true;
  else
    bNotEqual = false;
  
  return bNotEqual;
}

/*----------------------------------------------------------------------------*/
/* is this varchar greater than or equal to given varchar?                    */
/*----------------------------------------------------------------------------*/
bool Vch::operator >= (Vch& xVch)
{                                                             //! to be reviewed
  bool bGreaterEqual;
  
  if (StrLib::iCompare((char*)this,(char*)&xVch) >= 0)
    bGreaterEqual = true;
  else
    bGreaterEqual = false;
  
  return bGreaterEqual;
}

/*----------------------------------------------------------------------------*/
/* is this varchar greater than given varchar?                                */
/*----------------------------------------------------------------------------*/
bool Vch::operator > (Vch& xVch)
{                                                             //! to be reviewed
  bool bGreaterThan;
  
  if (StrLib::iCompare((char*)this,(char*)&xVch) > 0)
    bGreaterThan = true;
  else
    bGreaterThan = false;
  
  return bGreaterThan;
}

/*----------------------------------------------------------------------------*/
/* initialse to all zero                                                      */
/*----------------------------------------------------------------------------*/
void Vch::vInitZero(void)
{
                                                                 //! to be coded
}

/*----------------------------------------------------------------------------*/
/* extract varchar datum                                                      */
/*----------------------------------------------------------------------------*/
bool Vch::bExtract(Str* aDst, Str* aSrc, i8& nBytesTaken)
{
  /*
  char* aExt = (char*)aSrc;
  
  i8 iInt;
  bool b = StrLib::bExtractI8(&aExt,&iInt);
  if (b) {
    MemLib::vCopy(aDst,&iInt,sizeof(i8));
  
    nBytesTaken = sizeof(i8);
  }
  
  return b;
  */
  ASSERT(0);
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* sprint varchar datum                                                       */
/*----------------------------------------------------------------------------*/
void Vch::vSprint(Str* aDst, Str* aSrc, i8& nBytesTaken)
{
  StrLib::vSprint(aDst,"%s"_Str,aSrc->s());
  
  nBytesTaken = StrLib::iSize(aSrc->s());
}

/*----------------------------------------------------------------------------*/
/* is varchar argA < aArgB?                                                   */
/*----------------------------------------------------------------------------*/
bool Vch::bLessThan(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
{
  bool bLessThan;

  ASSERT(sizeof(aArgA) == nBytesA);
  ASSERT(sizeof(aArgB) == nBytesB);
  
  if (StrLib::iCompare(aArgA, aArgB) < 0)
    bLessThan = true;
  else
    bLessThan = false;
  
  return bLessThan;
}

/*----------------------------------------------------------------------------*/
/* is varchar argA <= aArgB?                                                  */
/*----------------------------------------------------------------------------*/
bool Vch::bLessEqual(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
{
  bool bLessEqual;
  
  ASSERT(sizeof(aArgA) == nBytesA);
  ASSERT(sizeof(aArgB) == nBytesB);
  
  if (StrLib::iCompare(aArgA, aArgB) <= 0)
    bLessEqual = true;
  else
    bLessEqual = false;
  
  return bLessEqual;
}

/*----------------------------------------------------------------------------*/
/* is varchar argA == aArgB?                                                  */
/*----------------------------------------------------------------------------*/
bool Vch::bEqualTo(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
{
  bool bEqualTo;
  
  ASSERT(sizeof(aArgA) == nBytesA);
  ASSERT(sizeof(aArgB) == nBytesB);
  
  if (StrLib::iCompare(aArgA, aArgB) == 0)
    bEqualTo = true;
  else
    bEqualTo = false;
  
  return bEqualTo;
}

/*----------------------------------------------------------------------------*/
/* is varchar argA != aArgB?                                                  */
/*----------------------------------------------------------------------------*/
bool Vch::bNotEqual(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
{
  bool bNotEqual;
  
  ASSERT(sizeof(aArgA) == nBytesA);
  ASSERT(sizeof(aArgB) == nBytesB);
  
  if (StrLib::iCompare(aArgA, aArgB) != 0)
    bNotEqual = true;
  else
    bNotEqual = false;
  
  return bNotEqual;
}

/*----------------------------------------------------------------------------*/
/* is varchar argA >= aArgB?                                                  */
/*----------------------------------------------------------------------------*/
bool Vch::bGreaterEqual(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
{
  bool bGreaterEqual;
  
  ASSERT(sizeof(aArgA) == nBytesA);
  ASSERT(sizeof(aArgB) == nBytesB);
  
  if (StrLib::iCompare(aArgA, aArgB) >= 0)
    bGreaterEqual = true;
  else
    bGreaterEqual = false;
  
  return bGreaterEqual;
}

/*----------------------------------------------------------------------------*/
/* is Vch argA > aArgB?                                                       */
/*----------------------------------------------------------------------------*/
bool Vch::bGreaterThan(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
{
  bool bGreaterThan;
  
  ASSERT(sizeof(aArgA) == nBytesA);
  ASSERT(sizeof(aArgB) == nBytesB);
  
  if (StrLib::iCompare(aArgA, aArgB) > 0)
    bGreaterThan = true;
  else
    bGreaterThan = false;
  
  return bGreaterThan;

}

/*----------------------------------------------------------------------------*/
/* cmp argA with argB (-ve: argA < argB, 0: argA == argB, +ve: argA > argB)   */
/*----------------------------------------------------------------------------*/
i8 Vch::iCmp(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
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

/*============================================================================*/
/* Vch.cpp -- varchar datum                                                   */
/*============================================================================*/

#include "Datums.h"

#include "StorageEngine.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Vch::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  vFatal("fatal: Vch class cannot be new'd\n"_Stz);
  
  NEVER_REACH;
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
/*-- Operators ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

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
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* initialse to all zero                                                      */
/*----------------------------------------------------------------------------*/
void Vch::vInitZero(void)
{
                                                                 //! to be coded
}

/*----------------------------------------------------------------------------*/
/*-- Null --------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* test if null                                                               */
/*----------------------------------------------------------------------------*/
bool Vch::bIsNull(void)
{
  if (C_NULL_VCH== *(i8*)this)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* set to null                                                                */
/*----------------------------------------------------------------------------*/
void Vch::vSetNull(void)
{
  *(i8*)this = C_NULL_VCH;
}

/*----------------------------------------------------------------------------*/
/*-- VLQ ---------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- Extract and Sprint ------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* copy str datum                                                             */
/*----------------------------------------------------------------------------*/
i8 Vch::nCopy(void* pDst, void* pSrc)
{
  i8 nBytes = StrLib::iSize((char*)pSrc);
  
  StrLib::vCopyLen((char*)pDst,(char*)pSrc, nBytes);     //! could return length
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* copy str datum                                                             */
/*----------------------------------------------------------------------------*/
void Vch::vCopy(void* pDst, void* pSrc)
{
  StrLib::vCopy((char*)pDst,(char*)pSrc);
}

/*----------------------------------------------------------------------------*/
/* extract an varchar from substr                                             */
/*----------------------------------------------------------------------------*/
bool Vch::bExtract(char** aaSrc, Vch* aVch, Column* pColumn)
{
  char* aChar = (char*)aVch;
  
  // test for null
  if ( ! **aaSrc || '|' == **aaSrc ) {
    aVch->vSetNull();
    return true;
  }
  
  // collect the csv
  while ('|' != **aaSrc)
    *aChar++ = *(*aaSrc)++;
  *aChar++ = '\0';
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* sprint tsp datum                                                           */
/*----------------------------------------------------------------------------*/
void Vch::vSprint(Stz* aDst, Stz* aSrc)
{
  StrLib::vSprint(aDst,"\"%s\"%c"_Stz,
                  aSrc->aChar(),
                  '\0');
  
}

/*----------------------------------------------------------------------------*/
/*-- Embedded Comparators ----------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* is varchar argA < aArgB?                                                   */
/*----------------------------------------------------------------------------*/
bool Vch::bLessThan(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
{
  bool bLessThan;

  ASSERT(StrLib::iSize(aArgA) == nBytesA);
  ASSERT(StrLib::iSize(aArgB) == nBytesB);
  
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
  
  ASSERT(StrLib::iSize(aArgA) == nBytesA);
  ASSERT(StrLib::iSize(aArgB) == nBytesB);
  
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
  
  ASSERT(StrLib::iSize(aArgA) == nBytesA);
  ASSERT(StrLib::iSize(aArgB) == nBytesB);
  
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
  
  ASSERT(StrLib::iSize(aArgA) == nBytesA);
  ASSERT(StrLib::iSize(aArgB) == nBytesB);
  
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
  
  ASSERT(StrLib::iSize(aArgA) == nBytesA);
  ASSERT(StrLib::iSize(aArgB) == nBytesB);
  
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
  
  ASSERT(StrLib::iSize(aArgA) == nBytesA);
  ASSERT(StrLib::iSize(aArgB) == nBytesB);
  
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
  ASSERT(StrLib::iSize(aArgA) == nBytesA);
  ASSERT(StrLib::iSize(aArgB) == nBytesB);
  
  i8 iCmp = StrLib::iCompare(aArgA, aArgB);
  
  return iCmp;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

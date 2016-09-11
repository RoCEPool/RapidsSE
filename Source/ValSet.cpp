/*============================================================================*/
/* ValSet.cpp - set of values                                                 */
/*============================================================================*/

#include "Values.h"

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* ValSet::operator new(size_t iSize)
{
  ASSERT(iSize);
  vFatal("fatal: ValSet class cannot be new'd\n");
  
  ASSERT(0);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ValSet::ValSet(void)
: Value(CN_VALSET)
{
  ASSERT(sizeof(ValSet) <= sizeof(ValWsp));
  
  Memory::vZero(m_mSpace,C_VALSET_ARRAY);
  
  m_iBytes = 0;
  
  Memory::vCheckInitialised(this,sizeof(ValSet));
}

/*----------------------------------------------------------------------------*/
/* append value to valset                                                     */
/*----------------------------------------------------------------------------*/
void ValSet::vAppend(char*& pCursor, ValWsp& xVal)
{
  i8 iSize = xVal.iSize();
  
  ASSERT(m_iBytes + iSize < C_VALSET_ARRAY);
  
  if ( ! pCursor )
    pCursor = pFirstByte();
  
  Memory::vCopy(pCursor,&xVal,iSize);
  
  m_iBytes += iSize;
  pCursor  += iSize;
}

/*----------------------------------------------------------------------------*/
/* extract next value from valset                                             */
/*----------------------------------------------------------------------------*/
bool ValSet::bExtract(char*& pCursor, ValWsp*& pVal)
{
  if ( ! pCursor )
    pCursor = pFirstByte();

  i8 iOffset = pCursor - pFirstByte();

  if (iOffset >= m_iBytes)
    return false;

  pVal = pToValWsp(pCursor);
  
  i8 iSize = pVal->iSize();
  
  pCursor += iSize;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* get size in bytes                                                          */
/*----------------------------------------------------------------------------*/
i8 ValSet::iSize(void)
{
  i8 iSize = pFirstByte() - (char*)this + m_iBytes;
  
  return iSize;
}

/*----------------------------------------------------------------------------*/
/* convert to command                                                         */
/*----------------------------------------------------------------------------*/
void ValSet::vToCommand(char* aWorkspace)
{
  ASSERT(aWorkspace);
  ASSERT("Should never be called");
}

/*----------------------------------------------------------------------------*/
/* convert to string                                                          */
/*----------------------------------------------------------------------------*/
void ValSet::vToString(char* aWorkspace)
{
  String::wToPointer(this,aWorkspace);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get ptr to first byte of space                                             */
/*----------------------------------------------------------------------------*/
char* ValSet::pFirstByte(void)
{
  return &m_mSpace[0];
}

/*----------------------------------------------------------------------------*/
/* get ptr to last bytes of space                                             */
/*----------------------------------------------------------------------------*/
char* ValSet::pLastByte(void)
{
  return &m_mSpace[C_VALSET_ARRAY-1];
}

/*----------------------------------------------------------------------------*/
/* convert cursor to valwsp*                                                  */
/*----------------------------------------------------------------------------*/
ValWsp* ValSet::pToValWsp(char* pCursor)
{
  return reinterpret_cast<ValWsp*>(pCursor);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
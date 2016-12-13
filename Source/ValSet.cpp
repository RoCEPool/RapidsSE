/*============================================================================*/
/* ValSet.cpp - set of values                                                 */
/*============================================================================*/

#include "Values.h"

#include "RowBuffer.hpp"

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* ValSet::operator new(size_t iSize)
{
  RASSERT(iSize);
  vFatal("fatal: ValSet class cannot be new'd\n");
  
  RASSERT(0);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ValSet::ValSet(void)
: Value(CN_VALSET)
{
  ASSERT(sizeof(ValSet) <= sizeof(ValWsp));
  
  Memory::vZero(m_wWorkspace,C_VALSET_WORKSPACE);
  
  m_iBytes = 0;
  
  m_pRowBuffer = nullptr;
  m_iRowBufferBytes = 0;
  
  _checkInitialised_(CN_VALSET);
}

/*----------------------------------------------------------------------------*/
/* append value to valset                                                     */
/*----------------------------------------------------------------------------*/
void ValSet::vAppend(char*& pCursor, ValWsp& xVal)
{
  i8 iSize = xVal.iSize();
  
  ASSERT(m_iBytes + iSize < C_VALSET_WORKSPACE);
  
  if ( ! pCursor )
    pCursor = pFirstByte();
  
  Memory::vCopy(pCursor,&xVal,iSize);
  
  m_iBytes += iSize;
  pCursor  += iSize;
  
  // test for a rowbuffer pointer
  if (CN_PTRVAL == xVal.eClass()) {
    
    PtrVal* pPtrVal = (PtrVal*)&xVal;
    
    if (pPtrVal->pPtr() &&
        CN_ROWBUFFER == ((Object*)pPtrVal->pPtr())->eClass()) {
      m_pRowBuffer = (RowBuffer*)pPtrVal->pPtr();
      m_iRowBufferBytes = m_pRowBuffer->iBytesUsed();
    }
  }
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
  
  // test for a rowbuffer pointer
  if (CN_PTRVAL == pVal->eClass()) {
    
    if (pVal->pPtr() &&
        CN_ROWBUFFER == ((Object*)pVal->pPtr())->eClass()) {
      *pVal = m_pRowBuffer;
    }
  }
    
  return true;
}

/*----------------------------------------------------------------------------*/
/* get row buffer                                                             */
/*----------------------------------------------------------------------------*/
DBobjects::RowBuffer* ValSet::pRowBuffer(void)
{
  return m_pRowBuffer;
}

/*----------------------------------------------------------------------------*/
/* set row buffer                                                             */
/*----------------------------------------------------------------------------*/
void ValSet::vRowBuffer(DBobjects::RowBuffer* pRowBuffer)
{
  m_pRowBuffer = pRowBuffer;
}

/*----------------------------------------------------------------------------*/
/* get size in bytes of the entire valset                                     */
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
/* get ptr to first byte of workspace                                         */
/*----------------------------------------------------------------------------*/
char* ValSet::pFirstByte(void)
{
  return &m_wWorkspace[0];
}

/*----------------------------------------------------------------------------*/
/* get ptr to last bytes of worksspace                                        */
/*----------------------------------------------------------------------------*/
char* ValSet::pLastByte(void)
{
  return &m_wWorkspace[C_VALSET_WORKSPACE-1];
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

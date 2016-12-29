/*============================================================================*/
/* ValSet.cpp - set of values                                                 */
/*============================================================================*/

#include "Values.h"

#include "RowBuffer.hpp"

#include "Arguments.hpp"
extern Arguments gxArguments;

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
  
  // test for a rowbuffer pointer
  if (CN_PTRVAL == xVal.eClass()) {
    
    PtrVal* pPtrVal = (PtrVal*)&xVal;
    
    if (pPtrVal->pPtr() &&
        CN_ROWBUFFER == ((Object*)pPtrVal->pPtr())->eClass()) {
      m_pRowBuffer = (RowBuffer*)pPtrVal->pPtr();
      m_iRowBufferBytes = m_pRowBuffer->iBytesUsed();
      
      xVal = (void*)C_ROWBUFFER_MARKER;
    }
  }
  
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
  
  // test for a rowbuffer pointer
  if (CN_PTRVAL == pVal->eClass()) {
    
    if ((i8)C_ROWBUFFER_MARKER == pVal->iAsInt()) {
      *pVal = m_pRowBuffer;
    }
  }
    
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract next value from valset                                             */
/*----------------------------------------------------------------------------*/
bool ValSet::bExtract(char*& pCursor, eCN& eClassNo, ValWsp*& pVal)
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
    
    if ((i8)C_ROWBUFFER_MARKER == pVal->iAsInt()) {
      eClassNo = CN_ROWBUFFER;
      return true;
    }
  }
  
  eClassNo = pVal->eClass();
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
/* recursively check for and get row buffer                                   */
/*----------------------------------------------------------------------------*/
DBobjects::RowBuffer* ValSet::pGetRowBuffer(void)
{
  eCN eClassNo;
  char* pCursor = nullptr;
  ValWsp* pValWsp;
  DBobjects::RowBuffer* pRowBuffer;
  
  while (bExtract(pCursor, eClassNo, pValWsp)) {
    
    if (CN_ROWBUFFER == eClassNo) {
      pRowBuffer = m_pRowBuffer;
      return pRowBuffer;
    }
    
    if (CN_VALSET == eClassNo) {
      ValSet* pValSet = (ValSet*)pValWsp;
      
      // check if this valset has a rowbuffer
      pRowBuffer = pValSet->pGetRowBuffer();
      
      if (pRowBuffer)
        return pRowBuffer;
    }
  }
  
  return nullptr;
  
}

/*----------------------------------------------------------------------------*/
/* recursively check for and get row buffer                                   */
/*----------------------------------------------------------------------------*/
bool ValSet::bHasRowBuffer(void)
{
  char* pCursor = nullptr;
  eCN eClassNo;
  ValWsp* pValWsp;
  bool bHasRowBuffer;
  
  while (bExtract(pCursor, eClassNo, pValWsp)) {
    
    if (CN_ROWBUFFER == eClassNo) {
      if (gxArguments.m_bTracepoint)
        vTrace("HAS ROW BUFFER\n");
      return true;
    }
    
    if (CN_VALSET == eClassNo) {
      ValSet* pValSet = (ValSet*)pValWsp;
      bHasRowBuffer = pValSet->bHasRowBuffer();
      if (bHasRowBuffer)
        return bHasRowBuffer;
    }
  }
  return false;

}

/*----------------------------------------------------------------------------*/
/* set row buffer                                                             */
/*----------------------------------------------------------------------------*/
void ValSet::vRowBuffer(DBobjects::RowBuffer* pRowBuffer)
{
    m_pRowBuffer = pRowBuffer;
}

/*----------------------------------------------------------------------------*/
/* set row buffer                                                             */
/*----------------------------------------------------------------------------*/
bool ValSet::bSetRowBuffer(DBobjects::RowBuffer* pRowBuffer)
{
  eCN eClassNo;
  char* pCursor = nullptr;
  ValWsp* pValWsp;
  bool bSet;
  
  while (bExtract(pCursor, eClassNo, pValWsp)) {
    
    if (CN_ROWBUFFER == eClassNo) {
      PtrVal* pPtrVal = (PtrVal*)pValWsp;
      //vTrace("SETTING ROW BUFFER\n");
      m_pRowBuffer = pRowBuffer;
      m_iRowBufferBytes = pRowBuffer->iBytesUsed();
      return true;
    }
    
    if (CN_VALSET == eClassNo) {
      ValSet* pValSet = (ValSet*)pValWsp;
      bSet = pValSet->bSetRowBuffer(pRowBuffer);
      if (bSet)
        return true;
    }
  }
  return false;
  
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

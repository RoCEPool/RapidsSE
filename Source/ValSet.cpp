/*============================================================================*/
/* ValSet.cpp - set of values                                                 */
/*============================================================================*/

#include "Values.h"

#include "StorageEngine.h"

extern bool gbTracepoint;

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
ValSet* Values::_pValSet(void* aAddr)
{
  ValSet* pValSet = reinterpret_cast<ValSet*>(aAddr);
  
  RASSERT( !pValSet || CN_VALSET == pValSet->eClass() );
  
  return pValSet;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* ValSet::operator new(size_t iSize)
{
  RASSERT(iSize);
  vFatal("fatal: ValSet class cannot be new'd\n"_Str);
  
  RASSERT(0);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ValSet::ValSet(void)
: Value(CN_VALSET)
{
  ASSERT(sizeof(ValSet) <= sizeof(ValWsp));
  
  MemLib::vZero(m_wWorkspace,C_VALSET_DATAAREA);
  
  m_iBytes = 0;
  
  m_pRowBuffer = nullptr;
  m_iRowBufferBytes = 0;
  
  _checkInitialised_(CN_VALSET);
}

/*----------------------------------------------------------------------------*/
/* append value to valset                                                     */
/*----------------------------------------------------------------------------*/
void ValSet::vAppend(Str*& pCursor, ValWsp& xVal)
{
  i8 iSize = ValLib::iSize(xVal.pNestedValueSelf());
  
  ASSERT(m_iBytes + iSize < C_VALSET_DATAAREA);
  
  if ( ! pCursor )
    pCursor = aFirstByte();
  
  // test for a rowbuffer pointer
  if (CN_PTRVAL == xVal.eClass()) {
    
    PtrVal* pPtrVal = (PtrVal*)&xVal;
    
    if (pPtrVal->pPtr() &&
        CN_ROWBUFFER == ((Object*)pPtrVal->pPtr())->eClass()) {
      m_pRowBuffer = (RowBuffer*)pPtrVal->pPtr();
      m_iRowBufferBytes = m_pRowBuffer->nUsedBytes();
      
      xVal = (Object*)C_ROWBUFFER_MARKER;
      MemLib::vCopy(pCursor,&xVal,iSize);
    }
    else {
      MemLib::vCopy(pCursor,&xVal,iSize);
    }
    
  }
  else {
    MemLib::vCopy(pCursor,&xVal,iSize);
  }
  
  m_iBytes += iSize;
  pCursor  += iSize;
  
}

/*----------------------------------------------------------------------------*/
/* extract next value from valset                                             */
/*----------------------------------------------------------------------------*/
bool ValSet::bExtract(Str*& pCursor, ValWsp*& pVal)
{
  if ( ! pCursor )
    pCursor = aFirstByte();

  i8 iOffset = pCursor - aFirstByte();

  if (iOffset >= m_iBytes)
    return false;

  pVal = pToValWsp(pCursor);
  
  i8 iSize = ValLib::iSize(pVal->pNestedValueSelf());
  
  pCursor += iSize;
  
  // test for a rowbuffer pointer
  if (CN_PTRVAL == pVal->eClass()) {
    
    if ((i8)C_ROWBUFFER_MARKER == pVal->iNestedAsInt()) {
      *pVal = m_pRowBuffer;
    }
  }
  
  // test for binary value
  if (CN_BINVAL == pVal->eClass()) {
    Object* pObjectVal = _pBinVal(pVal)->pNewObject();
    ASSERT(pObjectVal->eClass() == CN_PLAN);
    Plan* pPlan = reinterpret_cast<Plan*>(pObjectVal);
    pPlan->vNullifyPointers();
    *pVal = pObjectVal;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract next value from valset                                             */
/*----------------------------------------------------------------------------*/
bool ValSet::bExtract(Str*& pCursor, eCN& eClassNo, ValWsp*& pVal)
{
  if ( ! pCursor )
    pCursor = aFirstByte();
  
  i8 iOffset = pCursor - aFirstByte();
  
  if (iOffset >= m_iBytes)
    return false;
  
  pVal = pToValWsp(pCursor);
  
  i8 iSize = ValLib::iSize(pVal);
  
  pCursor += iSize;
  
  // test for a rowbuffer pointer
  if (CN_PTRVAL == pVal->eClass()) {
    
    if ((i8)C_ROWBUFFER_MARKER == pVal->iNestedAsInt()) {
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
  Str* pCursor = nullptr;
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
  Str* pCursor = nullptr;
  eCN eClassNo;
  ValWsp* pValWsp;
  bool bHasRowBuffer;
  
  while (bExtract(pCursor, eClassNo, pValWsp)) {
    
    if (CN_ROWBUFFER == eClassNo) {
      if (gbTracepoint)
        vTrace("HAS ROW BUFFER\n"_Str);
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
  Str* pCursor = nullptr;
  ValWsp* pValWsp;
  bool bSet;
  
  while (bExtract(pCursor, eClassNo, pValWsp)) {
    
    if (CN_ROWBUFFER == eClassNo) {
      PtrVal* pPtrVal = (PtrVal*)pValWsp;
      //vTrace("SETTING ROW BUFFER\n"_Str);
      m_pRowBuffer = pRowBuffer;
      m_iRowBufferBytes = pRowBuffer->nUsedBytes();
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
i8 ValSet::nSize(void)
{
  i8 iSize = aFirstByte()->s() - (char*)this + m_iBytes;
  
  return iSize;
}

/*----------------------------------------------------------------------------*/
/* convert to string                                                          */
/*----------------------------------------------------------------------------*/
void ValSet::vToString(Str* aWorkspace)
{
  *aWorkspace = reinterpret_cast<void*>(this);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get ptr to first byte of workspace                                         */
/*----------------------------------------------------------------------------*/
Str* ValSet::aFirstByte(void)
{
  return (Str*)&m_wWorkspace[0];
}

/*----------------------------------------------------------------------------*/
/* get ptr to last bytes of worksspace                                        */
/*----------------------------------------------------------------------------*/
Str* ValSet::aLastByte(void)
{
  return (Str*)&m_wWorkspace[C_VALSET_DATAAREA-1];
}

/*----------------------------------------------------------------------------*/
/* convert cursor to valwsp*                                                  */
/*----------------------------------------------------------------------------*/
ValWsp* ValSet::pToValWsp(Str* pCursor)
{
  return reinterpret_cast<ValWsp*>(pCursor);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

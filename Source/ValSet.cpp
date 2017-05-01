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
void ValSet::vAppend(Str*& pCursor, Value* pValue)
{
  i8 iSize = ValLib::iSize(pValue);
  
  ASSERT(m_iBytes + iSize < C_VALSET_DATAAREA);
  
  if ( ! pCursor )
    pCursor = aFirstByte();
  
  // test for a rowbuffer pointer
  if (CN_PTRVAL == pValue->eClass()) {
    
    PtrVal* pPtrVal = _pPtrVal(pValue);
    
    if (pPtrVal->pPtr() &&
        CN_ROWBUFFER == ((Object*)pPtrVal->pPtr())->eClass()) {
      m_pRowBuffer = (RowBuffer*)pPtrVal->pPtr();
      m_iRowBufferBytes = m_pRowBuffer->nUsedBytes();
      
      //xValWsp = (Object*)C_ROWBUFFER_MARKER;
      PtrVal xPtrVal;
      xPtrVal = (Object*)C_ROWBUFFER_MARKER;
      MemLib::vCopy(pCursor,&xPtrVal,sizeof(xPtrVal));
      //vTrace("APPEND C_ROWBUFFER_MARKER\n"_Str);
    }
    else {
      MemLib::vCopy(pCursor,pValue,iSize);
    }
    
  }
  else {
    MemLib::vCopy(pCursor,pValue,iSize);
  }
  
  m_iBytes += iSize;
  pCursor  += iSize;
  
}

/*----------------------------------------------------------------------------*/
/* extract next value from valset                                             */
/*----------------------------------------------------------------------------*/
#ifdef OLD_VALSET
bool ValSet::bExtractValWsp(Str*& pCursor, ValWsp*& pValWsp)
{
  if ( ! pCursor )
    pCursor = aFirstByte();

  i8 iOffset = pCursor - aFirstByte();

  if (iOffset >= m_iBytes)
    return false;

  pValWsp = pToValWsp(pCursor);
  
  i8 iSize = ValLib::iSize(pValWsp->pNestedValueSelf());
  
  pCursor += iSize;
  
  // test for a rowbuffer pointer
  if (CN_PTRVAL == pValWsp->eClass()) {
    
    if ((i8)C_ROWBUFFER_MARKER == pValWsp->iNestedAsInt()) {
      *pValWsp = m_pRowBuffer;
    }
  }
  
  // test for binary value
  if (CN_BINVAL == pValWsp->eClass()) {
    Object* pObjectVal = _pBinVal(pValWsp)->pNewObject();
    ASSERT(pObjectVal->eClass() == CN_PLAN);
    Plan* pPlan = reinterpret_cast<Plan*>(pObjectVal);
    pPlan->vNullifyPointers();
    *pValWsp = pObjectVal;
  }
  
  return true;
}

#else // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool ValSet::bExtractValue(Str*& pCursor, Value*& pValue)
{
  if ( ! pCursor )
    pCursor = aFirstByte();
  
  i8 iOffset = pCursor - aFirstByte();
  
  if (iOffset >= m_iBytes)
    return false;
  
  pValue = (Value*)pCursor;
  
  i8 iSize = ValLib::iSize(pValue);
  
  pCursor += iSize;
  
  // test for a rowbuffer pointer
  if (CN_PTRVAL == pValue->eClass()) {
    //vTrace("EXTRACT CN_PTRVAL\n"_Str);
    
    if ((i8)C_ROWBUFFER_MARKER == _pPtrVal(pValue)->iAsInt()) {
      //vTrace("EXTRACT C_ROWBUFFER_MARKER\n"_Str);
      *_pPtrVal(pValue) = m_pRowBuffer;
    }
  }
  
  // test for binary value
  if (CN_BINVAL == pValue->eClass()) {
    Object* pObjectVal = _pBinVal(pValue)->pNewObject();
    ASSERT(pObjectVal->eClass() == CN_PLAN);
    Plan* pPlan = reinterpret_cast<Plan*>(pObjectVal);
    pPlan->vNullifyPointers();
    *_pPtrVal(pValue) = pObjectVal;
  }
  
  return true;
}
#endif

/*----------------------------------------------------------------------------*/
/* extract next value from valset                                             */
/*----------------------------------------------------------------------------*/
#ifdef OLD_VALSET
bool ValSet::bExtractValWsp(Str*& pCursor, eCN& eClassNo, ValWsp*& pValWsp)
{
  if ( ! pCursor )
    pCursor = aFirstByte();
  
  i8 iOffset = pCursor - aFirstByte();
  
  if (iOffset >= m_iBytes)
    return false;
  
  pValWsp = pToValWsp(pCursor);
  
  i8 iSize = ValLib::iSize(pValWsp);
  
  pCursor += iSize;
  
  // test for a rowbuffer pointer
  if (CN_PTRVAL == pValWsp->eClass()) {
    
    if ((i8)C_ROWBUFFER_MARKER == pValWsp->iNestedAsInt()) {
      eClassNo = CN_ROWBUFFER;
      return true;
    }
  }
  
  eClassNo = pValWsp->eClass();
  return true;
}

#else // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

bool ValSet::bExtractValue(Str*& pCursor, eCN& eClassNo, Value*& pValue)
{
  if ( ! pCursor )
    pCursor = aFirstByte();
  
  i8 iOffset = pCursor - aFirstByte();
  
  if (iOffset >= m_iBytes)
    return false;
  
  pValue = (Value*)pCursor;
  
  i8 iSize = ValLib::iSize(pValue);
  
  pCursor += iSize;
  
  // test for a rowbuffer pointer
  if (CN_PTRVAL == pValue->eClass()) {
    
    if ((i8)C_ROWBUFFER_MARKER == _pPtrVal(pValue)->iAsInt()) {
      eClassNo = CN_ROWBUFFER;
      return true;
    }
  }
  
  eClassNo = pValue->eClass();
  return true;
}
#endif
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
#ifdef OLD_VALSET
DBobjects::RowBuffer* ValSet::pGetRowBuffer(void)
{
  eCN eClassNo;
  Str* pCursor = nullptr;
  ValWsp* pValWsp;
  DBobjects::RowBuffer* pRowBuffer;
  
  while (bExtractValWsp(pCursor, eClassNo, pValWsp)) {
    
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
#else // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
DBobjects::RowBuffer* ValSet::pGetRowBuffer(void)
{
  eCN eClassNo;
  Str* pCursor = nullptr;
  Value* pValue;
  DBobjects::RowBuffer* pRowBuffer;
  
  while (bExtractValue(pCursor, eClassNo, pValue)) {
    
    if (CN_ROWBUFFER == eClassNo) {
      pRowBuffer = m_pRowBuffer;
      return pRowBuffer;
    }
    
    if (CN_VALSET == eClassNo) {
      ValSet* pValSet = (ValSet*)pValue;
      
      // check if this valset has a rowbuffer
      pRowBuffer = pValSet->pGetRowBuffer();
      
      if (pRowBuffer)
        return pRowBuffer;
    }
  }
  
  return nullptr;
  
}
#endif

/*----------------------------------------------------------------------------*/
/* recursively check for and get row buffer                                   */
/*----------------------------------------------------------------------------*/
#ifdef OLD_VALSET
bool ValSet::bHasRowBuffer(void)
{
  Str* pCursor = nullptr;
  eCN eClassNo;
  ValWsp* pValWsp;
  bool bHasRowBuffer;
  
  while (bExtractValWsp(pCursor, eClassNo, pValWsp)) {
    
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
#else // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool ValSet::bHasRowBuffer(void)
{
  Str* pCursor = nullptr;
  eCN eClassNo;
  Value* pValue;
  bool bHasRowBuffer;
  
  while (bExtractValue(pCursor, eClassNo, pValue)) {
    
    if (CN_ROWBUFFER == eClassNo) {
      if (gbTracepoint)
        vTrace("HAS ROW BUFFER\n"_Str);
      return true;
    }
    
    if (CN_VALSET == eClassNo) {
      ValSet* pValSet = (ValSet*)pValue;
      bHasRowBuffer = pValSet->bHasRowBuffer();
      if (bHasRowBuffer)
        return bHasRowBuffer;
    }
  }
  return false;
  
}
#endif

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
#ifdef OLD_VALSET
bool ValSet::bSetRowBuffer(DBobjects::RowBuffer* pRowBuffer)
{
  eCN eClassNo;
  Str* pCursor = nullptr;
  ValWsp* pValWsp;
  bool bSet;
  
  while (bExtractValWsp(pCursor, eClassNo, pValWsp)) {
    
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
#else // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool ValSet::bSetRowBuffer(DBobjects::RowBuffer* pRowBuffer)
{
  eCN eClassNo;
  Str* pCursor = nullptr;
  Value* pValue;
  bool bSet;
  
  while (bExtractValue(pCursor, eClassNo, pValue)) {
    
    if (CN_ROWBUFFER == eClassNo) {
      PtrVal* pPtrVal = (PtrVal*)pValue;
      //vTrace("SETTING ROW BUFFER\n"_Str);
      m_pRowBuffer = pRowBuffer;
      m_iRowBufferBytes = pRowBuffer->nUsedBytes();
      return true;
    }
    
    if (CN_VALSET == eClassNo) {
      ValSet* pValSet = (ValSet*)pValue;
      bSet = pValSet->bSetRowBuffer(pRowBuffer);
      if (bSet)
        return true;
    }
  }
  return false;
  
}
#endif

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

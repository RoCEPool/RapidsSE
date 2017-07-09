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
  ValSet* pValSet = reinterpret_cast<ValSet*>(Heap::pAllocate(CN_VALSET));
  
  return pValSet;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void ValSet::operator delete(void* pValSetAddress)
{
  ValSet* pValSet = reinterpret_cast<ValSet*>(pValSetAddress);
  
  Heap::vFree(pValSet);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ValSet::ValSet(void)
: Value(CN_VALSET)
{
  ASSERT(sizeof(ValSet) <= sizeof(ValWrk));
  
  MemLib::vZero(m_wWorkspace,C_VALSET_DATAAREA);
  
  m_iBytes = 0;
  
  m_pRowBuffer = nullptr;
  m_iRowBufferBytes = 0;
  
  _checkInitialised_(CN_VALSET);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
ValSet::~ValSet(void)
{
  // do nothing
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
        CN_ENTRYBUFFER == ((Object*)pPtrVal->pPtr())->eClass()) {
      m_pRowBuffer = (EntryBuffer*)pPtrVal->pPtr();
      m_iRowBufferBytes = m_pRowBuffer->nUsedBytes();
      
      //xValWrk = (Object*)C_ROWBUFFER_MARKER;
      PtrVal xPtrVal;
      xPtrVal = (Object*)C_ROWBUFFER_MARKER;
      MemLib::vCopy(pCursor,&xPtrVal,sizeof(xPtrVal),CN_VALSET);
      //vTrace("APPEND C_ROWBUFFER_MARKER\n"_Str);
    }
    else {
      MemLib::vCopy(pCursor,pValue,iSize,CN_VALSET);
    }
    
  }
  else {
    MemLib::vCopy(pCursor,pValue,iSize,CN_VALSET);
  }
  
  m_iBytes += iSize;
  pCursor  += iSize;
  
}

/*----------------------------------------------------------------------------*/
/* extract next value from valset                                             */
/*----------------------------------------------------------------------------*/
bool ValSet::bExtractValue(Str*& pCursor, Value*& pValue)
{
  if ( ! pCursor )
    pCursor = aFirstByte();
  
  i8 iOffset = pCursor - aFirstByte();
  
  if (iOffset >= m_iBytes)
    return false;
  
  pValue = (Value*)pCursor;
  //vTrace("ValSet::bExtractValue: %lld %s\n"_Str,pValue->eClass(),aCN[pValue->eClass()]);
  
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
    pValue->vClass(CN_PTRVAL);
    *_pPtrVal(pValue) = pObjectVal;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract next value from valset                                             */
/*----------------------------------------------------------------------------*/
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
      eClassNo = CN_ENTRYBUFFER;
      return true;
    }
  }
  
  eClassNo = pValue->eClass();
  return true;
}

/*----------------------------------------------------------------------------*/
/* get row buffer                                                             */
/*----------------------------------------------------------------------------*/
DBobjects::EntryBuffer* ValSet::pRowBuffer(void)
{
  return m_pRowBuffer;
}

/*----------------------------------------------------------------------------*/
/* recursively check for and get row buffer                                   */
/*----------------------------------------------------------------------------*/
DBobjects::EntryBuffer* ValSet::pGetRowBuffer(void)
{
  eCN eClassNo;
  Str* pCursor = nullptr;
  Value* pValue;
  DBobjects::EntryBuffer* pRowBuffer;
  
  while (bExtractValue(pCursor, eClassNo, pValue)) {
    
    if (CN_ENTRYBUFFER == eClassNo) {
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

/*----------------------------------------------------------------------------*/
/* recursively check for and get row buffer                                   */
/*----------------------------------------------------------------------------*/
bool ValSet::bHasRowBuffer(void)
{
  Str* pCursor = nullptr;
  eCN eClassNo;
  Value* pValue;
  bool bHasRowBuffer;
  
  while (bExtractValue(pCursor, eClassNo, pValue)) {
    
    if (CN_ENTRYBUFFER == eClassNo) {
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

/*----------------------------------------------------------------------------*/
/* set row buffer                                                             */
/*----------------------------------------------------------------------------*/
void ValSet::vRowBuffer(DBobjects::EntryBuffer* pRowBuffer)
{
    m_pRowBuffer = pRowBuffer;
}

/*----------------------------------------------------------------------------*/
/* set row buffer                                                             */
/*----------------------------------------------------------------------------*/
bool ValSet::bSetRowBuffer(DBobjects::EntryBuffer* pRowBuffer)
{
  eCN eClassNo;
  Str* pCursor = nullptr;
  Value* pValue;
  bool bSet;
  
  while (bExtractValue(pCursor, eClassNo, pValue)) {
    
    if (CN_ENTRYBUFFER == eClassNo) {
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
/* convert cursor to ValWrk*                                                  */
/*----------------------------------------------------------------------------*/
ValWrk* ValSet::pToValWrk(Str* pCursor)
{
  return reinterpret_cast<ValWrk*>(pCursor);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

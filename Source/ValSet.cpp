/*============================================================================*/
/* ValSet.cpp - set of values                                                 */
/*============================================================================*/

#include "Values.h"

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
ValSet* Values::_pValSet(void* aAddr)
{
  ValSet* pValSet = reinterpret_cast<ValSet*>(aAddr);
  
  RASSERT_TYPE(pValSet,CN_VALSET);
  
  return pValSet;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* ValSet::operator new(size_t)
{
  ValSet* pValSet = reinterpret_cast<ValSet*>(HeapMem::pAllocate(CN_VALSET));
  
  return pValSet;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void ValSet::operator delete(void* pValSetAddress)
{
  ValSet* pValSet = reinterpret_cast<ValSet*>(pValSetAddress);
  
  //HeapMem::vFree(pValSet);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ValSet::ValSet(void)
: Value(CN_VALSET)
{
  ASSERT(sizeof(ValSet) <= sizeof(GenVar));
  
  _memZero_(m_wWorkspace,C_VALSET_DATAAREA,CN_VALSET);
  
  m_iBytes = 0;
  
  m_pRowBuffer = nullptr;
  m_iRowBufferBytes = 0;
  
  _checkInitialised_(CN_VALSET);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
ValSet::~ValSet(void) noexcept(false)
{
  // do nothing
}

/*----------------------------------------------------------------------------*/
/* append value to valset                                                     */
/*----------------------------------------------------------------------------*/
void ValSet::vAppend(Stz*& pCursor, Value* pValue)
{
  i8 iSize = pValue->nSize();
  
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
      
      //xVariable = (Object*)C_ENTRYBUFFER_MARKER;
      PtrVal xPtrVal;
      xPtrVal = (Object*)C_ENTRYBUFFER_MARKER;
      MemLib::vCopy(pCursor,&xPtrVal,sizeof(xPtrVal),CN_VALSET);
      //vTrace("APPEND C_ENTRYBUFFER_MARKER\n"_Stz);
    }
//    else if (pPtrVal->pPtr() &&
//        CN_WORKINGBUFFER == ((Object*)pPtrVal->pPtr())->eClass()) {
//      m_pWorkingBuffer = (WorkingBuffer*)pPtrVal->pPtr();
//      m_iWorkingBufferBytes = m_pWorkingBuffer->nUsedBytes();
//
//      //xVariable = (Object*)C_ENTRYBUFFER_MARKER;
//      PtrVal xPtrVal;
//      xPtrVal = (Object*)C_WORKINGBUFFER_MARKER;
//      MemLib::vCopy(pCursor,&xPtrVal,sizeof(xPtrVal),CN_VALSET);
//      //vTrace("APPEND C_ENTRYBUFFER_MARKER\n"_Stz);
//    }
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
bool ValSet::bExtractValue(Stz*& pCursor, Value*& pValue)
{
  if ( ! pCursor )
    pCursor = aFirstByte();
  
  i8 iOffset = pCursor - aFirstByte();
  
  if (iOffset >= m_iBytes)
    return false;
  
  pValue = (Value*)pCursor;
  //vTrace("ValSet::bExtractValue: %lld %s\n"_Stz,pValue->eClass(),aCN[pValue->eClass()]);
  
  i8 iSize = pValue->nSize();
  
  pCursor += iSize;
  
  // test for a rowbuffer pointer
  if (CN_PTRVAL == pValue->eClass()) {
    //vTrace("EXTRACT CN_PTRVAL\n"_Stz);
    
    if ((i8)C_ENTRYBUFFER_MARKER == _pPtrVal(pValue)->iAsInt()) {
      //vTrace("EXTRACT C_ENTRYBUFFER_MARKER\n"_Stz);
      *_pPtrVal(pValue) = m_pRowBuffer;
    }
    
//    if ((i8)C_WORKINGBUFFER_MARKER == _pPtrVal(pValue)->iAsInt()) {
//      //vTrace("EXTRACT C_WORKINGBUFFER_MARKER\n"_Stz);
//      *_pPtrVal(pValue) = m_pWorkingBuffer;
//    }
  }
  
  // test for binary value
  if (CN_BINVAL == pValue->eClass()) {
    Object* pObjectVal = _pBinVal(pValue)->pNewObject();
    ASSERT( (pObjectVal->eClass() == CN_PLAN) ||
            (_bIsValidJob(pObjectVal->eClass()))
           );
    
    if (pObjectVal->eClass() == CN_PLAN) {
      Plan* pPlan = reinterpret_cast<Plan*>(pObjectVal);
      pPlan->vNullifyPointers();
    }
    pValue->vSetClass(CN_PTRVAL);
    *_pPtrVal(pValue) = pObjectVal;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* extract next value from valset                                             */
/*----------------------------------------------------------------------------*/
bool ValSet::bExtractValue(Stz*& pCursor, eCN& eClassNo, Value*& pValue)
{
  if ( ! pCursor )
    pCursor = aFirstByte();
  
  i8 iOffset = pCursor - aFirstByte();
  
  if (iOffset >= m_iBytes)
    return false;
  
  pValue = (Value*)pCursor;
  
  i8 iSize = pValue->nSize();
  
  pCursor += iSize;
  
  // test for a rowbuffer pointer
  if (CN_PTRVAL == pValue->eClass()) {
    
    if ((i8)C_ENTRYBUFFER_MARKER == _pPtrVal(pValue)->iAsInt()) {
      eClassNo = CN_ENTRYBUFFER;
      return true;
    }
//    else if ((i8)C_WORKINGBUFFER_MARKER == _pPtrVal(pValue)->iAsInt()) {
//      eClassNo = CN_WORKINGBUFFER;
//      return true;
//    }
  }
  
  eClassNo = pValue->eClass();
  return true;
}

/*----------------------------------------------------------------------------*/
/* recursively check for and get row buffer                                   */
/*----------------------------------------------------------------------------*/
DBobjects::EntryBuffer* ValSet::pGetRowBuffer(void)
{
  eCN eClassNo;
  Stz* pCursor = nullptr;
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
/* recursively check for and get working buffer                               */
/*----------------------------------------------------------------------------*/
//DBobjects::WorkingBuffer* ValSet::pGetWorkingBuffer(void)
//{
//  eCN eClassNo;
//  Stz* pCursor = nullptr;
//  Value* pValue;
//  DBobjects::WorkingBuffer* pWorkingBuffer;
//
//  while (bExtractValue(pCursor, eClassNo, pValue)) {
//
//    if (CN_WORKINGBUFFER == eClassNo) {
//      pWorkingBuffer = m_pWorkingBuffer;
//      return pWorkingBuffer;
//    }
//
//    if (CN_VALSET == eClassNo) {
//      ValSet* pValSet = (ValSet*)pValue;
//      
//      // check if this valset has a rowbuffer
//      pWorkingBuffer = pValSet->pGetWorkingBuffer();
//
//      if (pWorkingBuffer)
//        return pWorkingBuffer;
//    }
//  }
//
//  return nullptr;
//
//}

/*----------------------------------------------------------------------------*/
/* recursively check for and get row buffer                                   */
/*----------------------------------------------------------------------------*/
bool ValSet::bHasRowBuffer(void)
{
  Stz* pCursor = nullptr;
  eCN eClassNo;
  Value* pValue;
  bool bHasRowBuffer;
  
  while (bExtractValue(pCursor, eClassNo, pValue)) {
    
    if (CN_ENTRYBUFFER == eClassNo) {
      if (gpAttachMap->m_bTracepoint)
        vTrace("HAS ROW BUFFER\n"_Stz);
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
/* recursively check for and get working buffer                               */
/*----------------------------------------------------------------------------*/
//bool ValSet::bHasWorkingBuffer(void)
//{
//  Stz* pCursor = nullptr;
//  eCN eClassNo;
//  Value* pValue;
//  bool bHasWorkingBuffer;
//  
//  while (bExtractValue(pCursor, eClassNo, pValue)) {
//    
//    if (CN_WORKINGBUFFER == eClassNo) {
//      if (gpAttachMap->m_bTracepoint)
//        vTrace("HAS WORKING BUFFER\n"_Stz);
//      return true;
//    }
//    
//    if (CN_VALSET == eClassNo) {
//      ValSet* pValSet = (ValSet*)pValue;
//      bHasWorkingBuffer = pValSet->bHasWorkingBuffer();
//      if (bHasWorkingBuffer)
//        return bHasWorkingBuffer;
//    }
//  }
//  return false;
//  
//}

/*----------------------------------------------------------------------------*/
/* get row buffer                                                             */
/*----------------------------------------------------------------------------*/
DBobjects::EntryBuffer* ValSet::pRowBuffer(void)
{
  return m_pRowBuffer;
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
  Stz* pCursor = nullptr;
  Value* pValue;
  bool bSet;
  
  while (bExtractValue(pCursor, eClassNo, pValue)) {
    
    if (CN_ENTRYBUFFER == eClassNo) {
      PtrVal* pPtrVal = (PtrVal*)pValue;
      //vTrace("SETTING ROW BUFFER\n"_Stz);
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
/* set working buffer                                                         */
/*----------------------------------------------------------------------------*/
//bool ValSet::bSetWorkingBuffer(DBobjects::WorkingBuffer* pWorkingBuffer)
//{
//  eCN eClassNo;
//  Stz* pCursor = nullptr;
//  Value* pValue;
//  bool bSet;
//  
//  while (bExtractValue(pCursor, eClassNo, pValue)) {
//    
//    if (CN_WORKINGBUFFER == eClassNo) {
//      PtrVal* pPtrVal = (PtrVal*)pValue;
//      //vTrace("SETTING ROW BUFFER\n"_Stz);
//      m_pWorkingBuffer = pWorkingBuffer;
//      m_iWorkingBufferBytes = pWorkingBuffer->nUsedBytes();
//      return true;
//    }
//    
//    if (CN_VALSET == eClassNo) {
//      ValSet* pValSet = (ValSet*)pValue;
//      bSet = pValSet->bSetWorkingBuffer(pWorkingBuffer);
//      if (bSet)
//        return true;
//    }
//  }
//  return false;
//  
//}

/*----------------------------------------------------------------------------*/
/* get size in bytes of the entire valset                                     */
/*----------------------------------------------------------------------------*/
i8 ValSet::nSize(void)
{
  i8 iSize = aFirstByte()->aChar() - (char*)this + m_iBytes;
  
  return iSize;
}

/*----------------------------------------------------------------------------*/
/* convert to string                                                          */
/*----------------------------------------------------------------------------*/
void ValSet::vToString(Stz* aWorkspace)
{
  *aWorkspace = reinterpret_cast<void*>(this);
}

/*----------------------------------------------------------------------------*/
/* get working buffer                                                         */
/*----------------------------------------------------------------------------*/
//DBobjects::WorkingBuffer* ValSet::pWorkingBuffer(void)
//{
//  return m_pWorkingBuffer;
//}

/*----------------------------------------------------------------------------*/
/* set working buffer                                                         */
/*----------------------------------------------------------------------------*/
//void ValSet::vWorkingBuffer(DBobjects::WorkingBuffer* pWorkingBuffer)
//{
//  m_pWorkingBuffer = pWorkingBuffer;
//}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get ptr to first byte of workspace                                         */
/*----------------------------------------------------------------------------*/
Stz* ValSet::aFirstByte(void)
{
  return (Stz*)&m_wWorkspace[0];
}

/*----------------------------------------------------------------------------*/
/* get ptr to last bytes of worksspace                                        */
/*----------------------------------------------------------------------------*/
Stz* ValSet::aLastByte(void)
{
  return (Stz*)&m_wWorkspace[C_VALSET_DATAAREA-1];
}

/*----------------------------------------------------------------------------*/
/* convert cursor to GenVar*                                                  */
/*----------------------------------------------------------------------------*/
//GenVar* ValSet::pToValWrk(Stz* pCursor)
//{
//  return reinterpret_cast<GenVar*>(pCursor);
//}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* Object.cpp -- Proper Object in system                                      */
/*============================================================================*/

#include "BASE.h"

extern SizeHeap  gxaSizeHeap[];

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Object::Object(eCN eClassNo)
: Header(eClassNo)
{
  _precondition_(_MAGIC_,eClassNo);
}

/*----------------------------------------------------------------------------*/
/* macro for standard safe cast function                                      */
/*----------------------------------------------------------------------------*/
Object* PRIMITIVES::_pObject(void* aAddr)
{
  Object* pObject = reinterpret_cast<Object*>(aAddr);

  //RASSERT_TYPE(pObject,CN);
  
  return pObject;
}


/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* inspect the object                                                         */
/*----------------------------------------------------------------------------*/
bool Object::bInspect(Object* pObject)
{
  _static_
  
  bool b = pObject->bInspectObject();
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* inspect the object                                                         */
/*----------------------------------------------------------------------------*/
void Object::vRepair(Object* pObject)
{
  _static_
  UNUSED(pObject);

  // DO NOTHING
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get class name                                                             */
/*----------------------------------------------------------------------------*/
Stz* Object::szClassNm(void)
{
  eCN eClass = this->eGetClassNo();

  Stz* szName = aCN[eClass];
  
  return szName;
}

/*----------------------------------------------------------------------------*/
/* calculate address of footer                                                */
/*----------------------------------------------------------------------------*/
Footer* Object::aCalcFooterAddress(void)
{
  eCN eClass = this->eGetClassNo();

  Footer* aFooter =
    aAdd<Footer*>(this,gxaSizeHeap[eClass].m_nClassSize-sizeof(Footer));

  if (aFooter->bValidFooter())
    return aFooter;
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* inspect object                                                             */
/*----------------------------------------------------------------------------*/
bool Object::bInspectObject(void)
{
  vTrace("Inspecting %p %-12s:\n"_Stz,this,szClassNm());
  
  vMemoryDump();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* memory copy                                                                */
/*----------------------------------------------------------------------------*/
Object* Object::pMemoryCopy(void)
{
  
  eCN eClass = this->eGetClassNo();
  i8 nSize   = gxaSizeHeap[eClass].m_nClassSize;

  // Note: When copying a proper object over to an allocated object,
  // the persistent bit in the header isn't initialized as it is part
  // of allocate(). Hence, it needs to be saved, and re added to the
  // copied object.

  Object* pCopy = (Operand*)Heaps::pAllocate(eClass);
  ASSERT(pCopy);
  bool bPersistent = pCopy->bGetPersistent();
  MemLib::vCopy(pCopy,this,nSize,eClass);
  pCopy->vSetPersistent(bPersistent);

  //TODO: If Object derives from DataBuffer, we should not overwrite DataBuffer's members
  // when we do a memcpy. Below code extracts the buffer no from the pool buffer that
  // the object lives in and then set that rowbuffer member which was overwritten.

  if (DataBuffer::bIsDataBuffer(eClass)) {
    DataBuffer* pDataBuffer = _pDataBuffer(pCopy);
    i8 iPoolBufferNo = pDataBuffer->iBufferNo();
    pDataBuffer->vSetBufferNo(iPoolBufferNo);
#ifdef WITHOUT_BUFFERIDHASHMAP
    // TODO: Hack for performance test
    pDataBuffer->vSetBufferId((void*) pDataBuffer);
#else
    pDataBuffer->vSetBufferId(
        AM::pFileMap()->oGetNewBufferId().m_pvAsAddress);
#endif
    if(eClass == BASE::CN_CRWBUFFER) {
      AM::pHashMap()->vInsertKeyValue(
          (void *) pDataBuffer->oGetBufferId().m_pvAsAddress,
          (Object *) pCopy,
          BI_CRW);
    } else if(eClass == BASE::CN_CSVBUFFER){
      AM::pHashMap()->vInsertKeyValue(
          (void *) pDataBuffer->oGetBufferId().m_pvAsAddress,
          (Object *) pCopy,
          BI_CSV);
    } else {
      ASSERT(eClass == BASE::CN_PDS);
      Pds* pPds = _pPds(pDataBuffer);
      pPds->m_fpCompare = PdsUtils::iIntCompare;
      AM::pHashMap()->vInsertKeyValue(
          (void *) pDataBuffer->oGetBufferId().m_pvAsAddress,
          (Object *) pCopy,
          BI_ROW);
    }

  }

  return pCopy;
}

/*----------------------------------------------------------------------------*/
/* memory dump                                                                */
/*----------------------------------------------------------------------------*/
void Object::vMemoryDump(void)
{
  eCN eClass = this->eGetClassNo();
  UNUSED(eClass);
  i8 nSize = nSizeOfObject();
  
  MemLib::vDumpWords(this,nSize);
}

/*----------------------------------------------------------------------------*/
/* is object to be memory logged?                                             */
/*----------------------------------------------------------------------------*/
bool Object::bMemoryLog(void)
{
  eCN eClass = this->eGetClassNo();

  i8 bDontMemLog = gxaSizeHeap[eClass].m_bDontMemLog;

  return ! bDontMemLog;
}

/*----------------------------------------------------------------------------*/
/* run post-recv() function                                                   */
/*----------------------------------------------------------------------------*/
void Object::vRunPostRecv(void)
{
  eCN eClassNo = eGetClassNo();
  
//  if (eClassNo != CN_JOB && eClassNo != CN_PARCEL)
//    vDebugPoint();
  
  fpPOSTFN* fpPostRecvFn = gxaSizeHeap[eClassNo].m_fpPostRecvFn;
  
  if (! fpPostRecvFn) {
    vSetPostRecvRun(true);
    return;
  }
  
  vSetPostRecvRun(true);
  fpPostRecvFn(this);
}

/*----------------------------------------------------------------------------*/
/* get size of object                                                         */
/*----------------------------------------------------------------------------*/
i8 Object::nSizeOfObject(void)
{
  eCN eClass = this->eGetClassNo();

  i8 nSize = gxaSizeHeap[eClass].m_nClassSize;
  
  return nSize;
}

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
eEC Object::eValidObject(void)
{
  eEC eError;
  
  if ((eError = this->eValidHeader()))
    return eError;
  
  eCN eClass = this->eGetClassNo();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Footer* aFooter =
    aAdd<Footer*>(this,gxaSizeHeap[eClass].m_nClassSize-sizeof(Footer));

  if ((eError = aFooter->eValidFooter()))
    return eError;

  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* get when allocated                                                         */
/*----------------------------------------------------------------------------*/
i8 Object::iWhenAllocated(void)
{
  Footer* aFooter = aCalcFooterAddress();
  
  i8 iWhen = aFooter ? aFooter->iWhenAllocated() : -1;
  
  return iWhen;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Object::aFooter(void)
{
  return nullptr;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

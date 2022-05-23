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
Object* BASE::_pObject(void* aAddr)
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

  Object* pCopy = (Operand*)Heaps::pAllocate(eClass);
  MemLib::vCopy(pCopy,this,nSize,eClass);

  //TODO: If Object derives from DataBuffer, we should not overwrite DataBuffer's members
  // when we do a memcpy. Below code extracts the buffer no from the pool buffer that
  // the object lives in and then set that rowbuffer member which was overwritten.
#ifdef WITH_BUFFER_ID
  if (eClass == CN_ROWBUFFER) {
    RowBuffer* pRowBuffer = _pRowBuffer(pCopy);
    i8 iBufferNo = pRowBuffer->iBufferNo();
    pRowBuffer->vSetBufferNo(iBufferNo);
    pRowBuffer->vSetBufferId(
        AM::pFileMap()->oGetNewBufferId());
  }
#endif

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

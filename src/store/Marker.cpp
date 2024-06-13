/*============================================================================*/
/* Marker.cpp -- Marker                                                        */
/*============================================================================*/

#include "BASE.h"

#include "Marker.hpp"
#include "MarkerMgr.hpp"

#include "eCF.h"
#include "Job.hpp"
#include "AM.hpp"
#include "Socket.hpp"

extern SizeHeap  gxaSizeHeap[];

ClassFn __Marker_fn (CN_MARKER,CF_INSPECT,(fpCLASSFN*)Marker::bInspect,-1);

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Marker,CN_MARKER)
__vDeleteFunction(Marker,CN_MARKER)
__aFooterFunction(Marker)

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
//Marker* STORE::_pMarker(void* aAddr)
//{
//  Marker* pMarker = reinterpret_cast<Marker*>(aAddr);
//
//  return pMarker;
//}
//
/*----------------------------------------------------------------------------*/
/* allocate from heap                                                         */
/*----------------------------------------------------------------------------*/
void* Marker::operator new(size_t, eMG eMarker)
{
  UNUSED(eMarker);
  //RASSERT(MG_CHAIN == eMarker || MG_MANAGER == eMarker);
  
  Marker* pMarker =
    static_cast<Marker*>(Heaps::pAllocate(CN_MARKER));
  
  return pMarker;
}

/*----------------------------------------------------------------------------*/
/* allocate from job marker chain                                             */
/*----------------------------------------------------------------------------*/
void* Marker::operator new(size_t, Job* pJob)
{
  RASSERT(pJob);
  
  //Marker* pMarker = pJob->pNewMarker();
  
  //if (! pMarker)
    
  Marker* pMarker =  static_cast<Marker*>(Heaps::pAllocate(CN_MARKER));
  
  return pMarker;
}

/*----------------------------------------------------------------------------*/
/* free back to the heap                                                      */
/*----------------------------------------------------------------------------*/
//void Marker::operator delete(void* aAddress)
//{
//  Object* pObject = static_cast<Object*>(aAddress);
//
//  Heaps::vFree(pObject);
//}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Marker::Marker(eOM eOpMarker)
: Operand(CN_MARKER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_MARKER,_FOOTER_);
  
  m_eOpMarker  = eOpMarker;
  m_eMarker    = MG_ORDINARY;
  m_pNextItem  = nullptr;
  m_bDeletable = false;

  _checkInitialised_(CN_MARKER);

//  vTrace("new Marker(%s) %p @%lld\n"_Stz,
//         aOM[m_eOpMarker],this,m_xFooter.iWhenAllocated());
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_MARKER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Marker::~Marker(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_);
//  vTrace("delete Marker(%s) %p @%lld\n"_Stz,
//         aOM[m_eOpMarker],this,m_xFooter.iWhenAllocated());
}

/*----------------------------------------------------------------------------*/
/* Custom receive and deserialisation from a socket                           */
/*----------------------------------------------------------------------------*/
bool Marker::bObjRecv(Socket* pSocket, Object* pObjBase, SysCall& roSysCall,
                         Object** ppObject, void* pvAuxBuf, i8 iAuxSize) {
  _static_
  ASSERT(pSocket);
  ASSERT(pObjBase);
  ASSERT(ppObject);
  ASSERT(pvAuxBuf);
  ASSERT(iAuxSize > 0);

  UNUSED(iAuxSize);
  ASSERT(iAuxSize >= (i8) sizeof(Marker));

  // copy the object header into the LowBuffer
  MemLib::vCopy(pvAuxBuf,pObjBase,sizeof(Object),CN_MARKER);

  // receive body into the LowBuffer
  i8 nRemainingBytes = gxaSizeHeap[CN_MARKER].m_nClassSize - sizeof(Object);
  void* aLowBody = aAdd<void*>(pvAuxBuf,sizeof(Object));
  bool bResult = pSocket->bBytesRecv(aLowBody,nRemainingBytes,roSysCall);
  RASSERT(bResult);

  // new a marker using info from received marker
  Marker* pReceivedMarker = (Marker*)pvAuxBuf;
  Marker* pMarker = AM::MARKER::pNewMarker(pReceivedMarker->eOpMarker(), nullptr);
  (*ppObject) = pMarker;

  return true;
}

/*----------------------------------------------------------------------------*/
/* post recv function                                                         */
/*----------------------------------------------------------------------------*/
void Marker::vPostRecv(void* aMarker)
{
  _static_
  UNUSED(aMarker);
  
  //Marker* pMarker = _pMarker(aMarker);
}


/*----------------------------------------------------------------------------*/
/* inspect the marker                                                         */
/*----------------------------------------------------------------------------*/
bool Marker::bInspect(Object* pObject)
{
  _static_
  
  Marker* pMarker = _pMarker(pObject);
  
  vTrace("Marker { %s } %p @%lld\n"_Stz,
         aOM[pMarker->m_eOpMarker],
         pMarker,
         pMarker->m_xFooter.iWhenAllocated());

  return true;
}

/*----------------------------------------------------------------------------*/
/* get deletable?                                                             */
/*----------------------------------------------------------------------------*/
bool Marker::bDeletable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_MARKER,_FOOTER_);

  return m_bDeletable;
}

/*----------------------------------------------------------------------------*/
/* set deletable?                                                             */
/*----------------------------------------------------------------------------*/
void Marker::vDeletable(bool bDeletable)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_MARKER,_FOOTER_);

  m_bDeletable = bDeletable;
}

/*----------------------------------------------------------------------------*/
/* get marker category                                                        */
/*----------------------------------------------------------------------------*/
eMG Marker::eMarker(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_MARKER,_FOOTER_);

  return m_eMarker;
}

/*----------------------------------------------------------------------------*/
/* set marker category                                                        */
/*----------------------------------------------------------------------------*/
void Marker::vMarker(eMG eMarker)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_MARKER,_FOOTER_);

  m_eMarker = eMarker;
}

/*----------------------------------------------------------------------------*/
/* get the next chain item                                                    */
/*----------------------------------------------------------------------------*/
Marker* Marker::pNextItem(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_MARKER,_FOOTER_);

  return m_pNextItem;
}

/*----------------------------------------------------------------------------*/
/* set the next chain item                                                    */
/*----------------------------------------------------------------------------*/
void Marker::vNextItem(Marker* pNextItem)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_MARKER,_FOOTER_);

  m_pNextItem = pNextItem;
}

/*----------------------------------------------------------------------------*/
/* get the Op marker type                                                     */
/*----------------------------------------------------------------------------*/
eOM Marker::eOpMarker(void)
{
  //_precondition_(_ADDRESS_,_MAGIC_,CN_MARKER,_FOOTER_);

  return m_eOpMarker;
}

/*----------------------------------------------------------------------------*/
/* set the Op marker type                                                     */
/*----------------------------------------------------------------------------*/
void Marker::vOpMarker(eOM eOpMarker)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_MARKER,_FOOTER_);
  
  m_eOpMarker = eOpMarker;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

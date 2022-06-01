/*============================================================================*/
/* socket.cpp - tcp/ip sockets                                                */
/*============================================================================*/

#include "BASE.h"

#include "Socket.hpp"
#include "Resources.hpp"
#include "Marker.hpp"
#include "AM.hpp"

//#include <pcap/pcap.h>
#include <arpa/inet.h>
//#include <netinet/in.h>

extern Stz* C_LOOPBACK_IP;

Atomic gixSocket;

extern SizeHeap  gxaSizeHeap[];

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(BASE,Socket,CN_SOCKET)
__aNewFunction(Socket,CN_SOCKET)
__vDeleteFunction(Socket,CN_SOCKET)
__aFooterFunction(Socket)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
//Socket::Socket(Object* pIsFor)
//: Port(pIsFor)
//{
//  _precondition_(_MAGIC_,CN_SOCKET,_FOOTER_);
//
//  m_pReceivedObject = nullptr;
//
//  MemLib::vZero(m_caObjectHeader,sizeof(m_caObjectHeader),CN_SOCKET);
//  m_aHeader = (Header*) & m_caObjectHeader[0];
//  m_pObject = reinterpret_cast<Object*>(m_aHeader);
//
//  m_aLowBuff   = MemLib::aMallocBlock(C_SIZE_POOL_BUFFER);
//  m_pRejectJob = new RejectJob;
//
//  _checkInitialised_(CN_SOCKET);
//
//  ++ gixSocket;
//
//  _postcondition_(_MAGIC_,CN_SOCKET,_FOOTER_);
//}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Socket::Socket(Object* pIsFor, int iSocketId)
: Port(CN_SOCKET,pIsFor,iSocketId)
//, m_okLock(this)
{
  _precondition_(_MAGIC_,CN_SOCKET,_FOOTER_);
  
  m_pReceivedObject = nullptr;
  
  MemLib::vZero(m_caObjectHeader,sizeof(m_caObjectHeader),CN_SOCKET);
  m_aHeader = (Header*) & m_caObjectHeader[0];
  m_pObject = reinterpret_cast<Object*>(m_aHeader);

  m_aLowBuff   = MemLib::aMallocBlock(C_SIZE_POOL_BUFFER);
  m_pRejectJob = new RejectJob;

  _checkInitialised_(CN_SOCKET);
  
  ++ gixSocket;

  
  _postcondition_(_MAGIC_,CN_SOCKET,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Socket::~Socket(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_SOCKET,_FOOTER_);
  
  delete m_pRejectJob;
  m_pRejectJob = nullptr;
  
  MemLib::vFreeBlock(m_aLowBuff,C_SIZE_POOL_BUFFER);
  m_aLowBuff = nullptr;
  
  -- gixSocket;
  
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get received object                                                        */
/*----------------------------------------------------------------------------*/
Object* Socket::pReceivedObject(void)
{
  _precondition_(_MAGIC_,CN_SOCKET,_FOOTER_);
  
  return m_pReceivedObject;
}

/*----------------------------------------------------------------------------*/
/* set received object                                                        */
/*----------------------------------------------------------------------------*/
void Socket::vReceivedObject(Object* pReceivedObject)
{
  _precondition_(_MAGIC_,CN_SOCKET,_FOOTER_);
  
  m_pReceivedObject = pReceivedObject;
}

/*----------------------------------------------------------------------------*/
/* recv next object from this socket                                          */
/*----------------------------------------------------------------------------*/
Object* Socket::pRecvObject(void)
{
  _precondition_(_MAGIC_,CN_SOCKET,_FOOTER_);
  
  RASSERT(! pReceivedObject());

  bool bLowMem = AM::MEMORY::bIsRunningLow();
  if (bLowMem)
     PoolHelper::vPoolAnalyze();

  m_pObject->vSetWasReceived(true);
  m_pObject->vSetPostRecvRun(false);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // receive the header of the next object
    
  if (! bHeaderRecv()) {
    vSuccess(false);
    return nullptr;
  }

  eCN eClass = m_pObject->eGetClassNo();

//  SP_Header spHeader;
//  vTrace("%lld: vSocketRecvObject: Header %s\n"_Stz,
//         AM::iThisNodeId(),
//         spHeader.sz(aHeader));
  

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // markers are special case

  if (CN_MARKER == eClass) {
    Marker* pMarker = pMarkerRecv();
    vSuccess(true);
    return pMarker;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // reject job is a special case

  // ...

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // receive object normally (or acceptable objects under low memory)

  if (! bLowMem || ! gxaSizeHeap[eClass].m_bLowMemoryDiscard) {
    Object* pReceivedObject = pObjectRecv(eClass);
    vSuccess(true);
    return pReceivedObject;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // receive and discard the body of the object (low memory condition)

  Object* pReceivedObject = pObjectRecv_lowmem(eClass);
  RASSERT(! pReceivedObject);
  
  //vReceivedObject(nullptr);
  vSuccess(true);
  
  return pReceivedObject;
}

/*----------------------------------------------------------------------------*/
/* send an object                                                             */
/*----------------------------------------------------------------------------*/
bool Socket::bSendObject(Object* pObject, SysCall& roSysCall, eSY eSendWhy)
{
  _precondition_(_MAGIC_,CN_SOCKET,_FOOTER_);
  
  roSysCall.vClear();
  
  eCN eClass = pObject->eGetClassNo();
  i8 nBytes = gxaSizeHeap[eClass].m_nClassSize;

  bool bResult = bBytesSend(pObject,nBytes,roSysCall);

  if (bResult) {
    m_pTotup->vAdd(eClass,nBytes);
    m_pWhyTot->vAdd(eSendWhy,nBytes);
  }
  
  return bResult;
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* recv next header from this socket                                          */
/*----------------------------------------------------------------------------*/
bool Socket::bHeaderRecv(void)
{
  _precondition_(_MAGIC_,CN_SOCKET,_FOOTER_);
  
  SysCall oSysCall;
  
  bool bResult = bBytesRecv(m_aHeader,sizeof(Object),oSysCall);
  if (! bResult) {
    vTrace("Socket::pRecvObject (header) : bRecvBytes returned false\n"_Stz);
    vSuccess(false);
    return false;
  }

  return true;
}

/*----------------------------------------------------------------------------*/
/* complete the recv of next marker from this socket                          */
/*----------------------------------------------------------------------------*/
Marker* Socket::pMarkerRecv(void)
{
  _precondition_(_MAGIC_,CN_SOCKET,_FOOTER_);
  
  
  SysCall oSysCall;

  // copy the object header into the LowBuffer
  MemLib::vCopy(m_aLowBuff,m_pObject,sizeof(Object),CN_SOCKET);

  // receive body into the LowBuffer
  i8 nRemainingBytes = gxaSizeHeap[CN_MARKER].m_nClassSize - sizeof(Object);
  void* aLowBody = aAdd<void*>(m_aLowBuff,sizeof(Object));
  bool bResult = bBytesRecv(aLowBody,nRemainingBytes,oSysCall);
  UNUSED(bResult);

  // new a marker using info from received marker
  Marker* pReceivedMarker = (Marker*)m_aLowBuff;
  Marker* pMarker = AM::MARKER::pNewMarker(pReceivedMarker->eOpMarker(),
                                           nullptr);

  m_pTotup->vAdd(CN_MARKER,sizeof(CN_MARKER));
  
  return pMarker;
}

/*----------------------------------------------------------------------------*/
/* complete the recv of next marker from this socket                          */
/*----------------------------------------------------------------------------*/
Object* Socket::pObjectRecv(eCN eClass)
{
  _precondition_(_MAGIC_,CN_SOCKET,_FOOTER_);
  
  
  SysCall oSysCall;

  i8 nSize = gxaSizeHeap[eClass].m_nClassSize;

  //vTrace("class:%s\n"_Stz, aCN[eClass]);
  Object* pReceivedObject = Heaps::pAllocate(eClass);
  MemLib::vCopy(pReceivedObject,m_pObject,sizeof(Object),eClass);

  void* aBody = aAdd<void*>(pReceivedObject,sizeof(Object));
  i8 nRemainingBytes = nSize - sizeof(Object);

  bool bResult = bBytesRecv(aBody,nRemainingBytes,oSysCall);
  if (! bResult) {
   //m_pDataStack->vPushInt(false);
   //vReceivedObject(nullptr);
   vTrace("Socket::pRecvObject (body) : bRecvBytes returned false\n"_Stz);
   vSuccess(false);
   return nullptr;
  }
#ifdef WITH_BUFFER_ID
  if (1 == DataBuffer::bIsDataBuffer(eClass)) {
    DataBuffer* pDataBuffer = _pDataBuffer(pReceivedObject);
    i8 iPoolBufferNo = pDataBuffer->iBufferNo();
    pDataBuffer->vSetBufferNo(iPoolBufferNo);
    pDataBuffer->vSetBufferId(
        AM::pFileMap()->oGetNewBufferId(iPoolBufferNo));
#ifdef WITH_HASHMAP
    AM::pHashMap()->vInsertKeyValue(
        (void *) pDataBuffer->oGetBufferId().m_pvAsAddress,
        (Object *) pReceivedObject);
#endif
  }
#endif
  pReceivedObject->vRunPostRecv();

  m_pTotup->vAdd(eClass,nSize);
  
  return pReceivedObject;
}

/*----------------------------------------------------------------------------*/
/* complete the recv of next marker from this socket                          */
/*----------------------------------------------------------------------------*/
Object* Socket::pObjectRecv_lowmem(eCN eClass)
{
  _precondition_(_MAGIC_,CN_SOCKET,_FOOTER_);
  
  
  SysCall oSysCall;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // receive and discard the body of the object (low memory condition)

  if (AM::SHOW::bLowMem_Recv())
    vTrace("Socket::pObjectRecv_lowmem : LOW MEMORY\n"_Stz);

  void* aLowBuff = m_aLowBuff;

  // copy the object header into the LowBuffer
  MemLib::vCopy(aLowBuff,m_pObject,sizeof(Object),eClass);

  // recv rest of the buffer into LowBuffer
  void* aLowBody = aAdd<void*>(aLowBuff,sizeof(Object));
  i8 nRemainingBytes = gxaSizeHeap[eClass].m_nClassSize - sizeof(Object);

  bool bResult = bBytesRecv(aLowBody,nRemainingBytes,oSysCall);
  UNUSED(bResult);
    
  return nullptr;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

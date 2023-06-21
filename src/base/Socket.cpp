/*============================================================================*/
/* socket.cpp - tcp/ip sockets                                                */
/*============================================================================*/

#include "BASE.h"

#include "Socket.hpp"
#include "Resources.hpp"
#include "Marker.hpp"
#include "AM.hpp"
#include "ExtendedText.hpp"

//#include <pcap/pcap.h>
#include <arpa/inet.h>
//#include <netinet/in.h>
#include "Job.hpp"
#include "CmdLineArgs.hpp"

extern Stz* C_LOOPBACK_IP;

Atomic gixSocket;

extern SizeHeap  gxaSizeHeap[];
extern CmdLineArgs  gxCmdLineArgs;

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
Socket::Socket(Object* pIsFor, int iSocketId)
: Port(pIsFor,iSocketId)
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

/* ---------------------------------------------------------------------------*/
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

  static i8 nPipeline = 0;
  bool bLowMem = false;

  m_pObject->vSetWasReceived(true);
  m_pObject->vSetPostRecvRun(false);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // receive the header of the next object
    
  if (! bHeaderRecv()) {
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
  // receive reject body into m_pRejectJob

  if (eClass == CN_REJECTJOB) {

    // copy the object header into the m_pRejectJob
    MemLib::vCopy(m_pRejectJob,m_pObject,sizeof(Object),eClass);

    // recv rest of the buffer into m_pRejectJob
    void* pRejectJob = aAdd<void*>(m_pRejectJob,sizeof(Object));
    i8 nRemainingBytes = gxaSizeHeap[eClass].m_nClassSize - sizeof(Object);

    SysCall oSysCall;

    bool bResult = bBytesRecv(pRejectJob,nRemainingBytes,oSysCall);
    UNUSED(bResult);

    vTrace("Recvd RejectJob\n"_Stz);

    // handle the received reject job
    AM::JOB::vRecvdReject(m_pRejectJob);
    nPipeline++;
    vSuccess(true);
    return nullptr;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // HACK : To simulate lowMemory
  //  if (eClass == CN_JOB) {
  //    nPipeline++;
  //    if (nPipeline == gxCmdLineArgs.m_xSysArgs.m_iSocketLowMem)
  //      bLowMem = true;
  //  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // receive object body

  try {

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // HACK : To simulate lowMemory
    //nPipeline++;
    //if (nPipeline == gxCmdLineArgs.m_xSysArgs.m_iSocketLowMem)
    //  THROW_LOWMEMORY();

    Object *pReceivedObject = pObjectRecv(eClass);
    if ( !pReceivedObject ) {
      return nullptr;
    }

    try {
      pReceivedObject->vRunPostRecv();
    }
    catch (LowMemory xLowMemory) {

      // received the object but was unable to execute postRecv

      //
      // RunPostRecv can get lowMemory only for JOB
      //

      // no other objects received allocate memory in postRecv functions
      // to handle when this changes

      RASSERT(eClass == CN_JOB);
      Job *pJob = _pJob(pReceivedObject);
      vTrace("Recvd Job %s : Unable to execute postRecv (lowmem)\n"_Stz, Job::szJobNm(pJob));

      // execute the 2PC commands directly for job received/ send reject
      vJobRecv_lowmem(pJob);

      // delete the job
      delete pJob;

      vSuccess(true);
      return nullptr;
    }

    vSuccess(true);
    return pReceivedObject;
  }
  catch (LowMemory xLowMemory) {
    // fall thru
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // receive and discard the body of the object (low memory condition)

  Object* pReceivedObject = pObjectRecv_lowmem(eClass);
  RASSERT(! pReceivedObject);

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

  if (pObject->bIsClassNo(CN_JOB)) {
    Job* pJob = _pJob(pObject);
    RASSERT(pJob->szCmdText());

    ExtendedText* pExtText = pJob->pCmdText();
    RASSERT(pExtText);
    eClass = pExtText->eGetClassNo();
    nBytes = gxaSizeHeap[eClass].m_nClassSize;
    bResult = bBytesSend(pExtText,nBytes,roSysCall);
    RASSERT(bResult);

    ExtendedTextBuffer* pExtTextBuffer = pExtText->pExtTextBuffer(0);
    RASSERT(pExtTextBuffer);
    eClass = pExtTextBuffer->eGetClassNo();
    nBytes = gxaSizeHeap[eClass].m_nClassSize;
    bResult = bBytesSend(pExtTextBuffer,nBytes,roSysCall);
    RASSERT(bResult);

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

/* ---------------------------------------------------------------------------*/
/* handle received job under low memory                                       */
/*----------------------------------------------------------------------------*/
void Socket::vJobRecv_lowmem(Job* pJob) {
  _precondition_(_MAGIC_, CN_SOCKET, _FOOTER_);

  if (Job::szJobNm(pJob)->bSame("begun"_Stz) ||
      Job::szJobNm(pJob)->bSame("executed"_Stz) ||
      Job::szJobNm(pJob)->bSame("ended"_Stz) ||
      Job::szJobNm(pJob)->bSame("cancelled"_Stz)) {

    // job has begun/executed/ended/cancelled on the participant
    // ack originating job
    AM::JOB::vAckOriginatingJob(Job::tOriginId(pJob));

  } else if (Job::szJobNm(pJob)->bSame("cancel"_Stz) ||
             Job::szJobNm(pJob)->bSame("end"_Stz) ||
             Job::szJobNm(pJob)->bSame("execute"_Stz)) {

    // trigger an execute/end of the participant pipeline job
    AM::JOB::vTriggerExecute(Job::tDistId(pJob));

  } else {

    // received a pipeline job, just reject it
    m_pRejectJob->vOriginId(Job::tOriginId(pJob));
    m_pRejectJob->vDistId(Job::tDistId(pJob));
    m_pRejectJob->vError(EC_LOW_MEMORY);

    // send a rejectJob to originating node
    AM::JOB::vSendReject(m_pRejectJob);
  }
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

  Object *pReceivedObject = nullptr;


  SysCall oSysCall;

  i8 nSize = gxaSizeHeap[eClass].m_nClassSize;

  //vTrace("class:%s\n"_Stz, aCN[eClass]);
  pReceivedObject = Heaps::pAllocate(eClass, m_pObject->bGetPersistent());
  MemLib::vCopy(pReceivedObject, m_pObject, sizeof(Object), eClass);

  void *aBody = aAdd<void *>(pReceivedObject, sizeof(Object));
  i8 nRemainingBytes = nSize - sizeof(Object);

  bool bResult = bBytesRecv(aBody, nRemainingBytes, oSysCall);
  if (!bResult) {
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

  m_pTotup->vAdd(eClass, nSize);

  if (pReceivedObject->bIsClassNo(CN_JOB)) {
    Job *pJob = _pJob(pReceivedObject);
    //pJob->vCmdText((ExtendedText*) nullptr);

    // - - - - - - - - - - - - - - - - - - - - -
    RASSERT(pJob->bRecvCmdText());

    Object* aCmdText = Heaps::pAllocate(CN_EXTENDEDTEXT, false);
    nSize = gxaSizeHeap[CN_EXTENDEDTEXT].m_nClassSize;
    bResult = bBytesRecv(aCmdText, nSize, oSysCall);
    RASSERT(bResult);
    ExtendedText* pCmdText = _pExtendedText(aCmdText);

    Object* aCmdTextBuffer = Heaps::pAllocate(CN_EXTENDEDTEXTBUFFER, false);
    nSize = gxaSizeHeap[CN_EXTENDEDTEXTBUFFER].m_nClassSize;
    bResult = bBytesRecv(aCmdTextBuffer, nSize, oSysCall);
    RASSERT(bResult);
    ExtendedTextBuffer* pCmdTextBuffer = _pExtendedTextBuffer(aCmdTextBuffer);

    pCmdText->vExtTextBuffer(0, pCmdTextBuffer);
    pJob->vCmdText(pCmdText);

    // - - - - - - - - - - - - - - - - - - - - -

    if (pJob->bRecvCodeBlock()) {
      Object* aCodeBlock = Heaps::pAllocate(CN_EXTENDEDTEXT, false);
      nSize = gxaSizeHeap[CN_EXTENDEDTEXT].m_nClassSize;
      bResult = bBytesRecv(aCodeBlock, nSize, oSysCall);
      RASSERT(bResult);
      ExtendedText* pCodeBlock = _pExtendedText(aCodeBlock);

      Object* aCodeBlockBuffer = Heaps::pAllocate(CN_EXTENDEDTEXTBUFFER, false);
      nSize = gxaSizeHeap[CN_EXTENDEDTEXTBUFFER].m_nClassSize;
      bResult = bBytesRecv(aCodeBlockBuffer, nSize, oSysCall);
      RASSERT(bResult);
      ExtendedTextBuffer* pCodeBlockBuffer = _pExtendedTextBuffer(aCodeBlockBuffer);

      pCodeBlock->vExtTextBuffer(0, pCodeBlockBuffer);
      pJob->vCodeBlock(pCodeBlock);

    }

  }

  return pReceivedObject;
}

/*----------------------------------------------------------------------------*/
/* complete the recv of next marker from this socket                          */
/*----------------------------------------------------------------------------*/
Object* Socket::pObjectRecv_lowmem(eCN eClass)
{
  _precondition_(_MAGIC_,CN_SOCKET,_FOOTER_);

  SysCall oSysCall;

  if (AM::SHOW::bLowMem_Recv())
    vTrace("Socket::pObjectRecv_lowmem : LOW MEMORY\n"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // receive and discard the body of the object (low memory condition)

  void* aLowBuff = m_aLowBuff;

  // copy the object header into the LowBuffer
  MemLib::vCopy(aLowBuff,m_pObject,sizeof(Object),eClass);

  // recv rest of the buffer into LowBuffer
  void* aLowBody = aAdd<void*>(aLowBuff,sizeof(Object));
  i8 nRemainingBytes = gxaSizeHeap[eClass].m_nClassSize - sizeof(Object);

  bool bResult = bBytesRecv(aLowBody,nRemainingBytes,oSysCall);
  UNUSED(bResult);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // JOB received under low memory

  if (eClass == CN_JOB) {

    Job* pJob = _pJob(aLowBuff);
    vTrace("Recvd Job under lowMem: %s\n"_Stz, Job::szJobNm(pJob));

    // execute the 2PC commands directly for job received
    vJobRecv_lowmem(pJob);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // Nothing to do for other objects received under low memory
    return nullptr;

  }

  return nullptr;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

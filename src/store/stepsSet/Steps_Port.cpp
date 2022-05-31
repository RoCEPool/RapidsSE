/*============================================================================*/
/* Steps_Port.cpp - port step group                                           */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "eYX.h"

#include "Marker.hpp"

#include "RowBuffer.hpp"

#include "InfoSet.hpp"

#include "StreamSet.hpp"

#include "RowCoder.hpp"
#include "HeadLine.hpp"
#include "Crw.hpp"
#include "CrwPacker.hpp"
#include "CrwBuffer.hpp"
#include "Node.hpp"
#include "Job.hpp"
#include "Socket.hpp"
#include "Parcel.hpp"
#include "Node_I.hpp"
#include "LogicUnit.hpp"
#include "OpSpec.hpp"
#include "StageSpec.hpp"

#include "SP_Header.hpp"
#include "SP_Job.hpp"

#include "Node.hpp"
#include "Grid.hpp"
#include <arpa/inet.h>
#include "TagConstants.h"
#include "MY.hpp"
#include "Op.hpp"

extern SizeHeap  gxaSizeHeap[];

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Port(void)
{
  vAddStep("listen::open"_Stz,       &Step::vSocketListen);
  vAddStep("listen.accept"_Stz,      &Step::vSocketAccept);


}

/*----------------------------------------------------------------------------*/
/* listen.accept: accept a socket connection                                  */
/*----------------------------------------------------------------------------*/
void Step::vSocketAccept(void)
{
  ENTRY();
  
  SysCall oSysCall;
  
  Socket* pListeningSocket = _pSocket(m_pDataStack->pPopPtr(__func__));
  
  // accept a connection
  int iSocketId;
  bool bResult = pListeningSocket->bAcceptClient(iSocketId, oSysCall);
  
  if ( ! bResult )
    vThrow(FK_CHECK,__func__,__FILE__,__LINE__,
           "Unable to accept connection: Errno %d"_Stz, oSysCall.iError());
  
  // return the socket
  Socket* pNewSocket = new Socket(this,iSocketId);
  
  m_pDataStack->vPushPtr(pNewSocket);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* listen::open: listen in for socket connections                           */
/*----------------------------------------------------------------------------*/
void Step::vSocketListen(void)
{
  ENTRY();
  
  SysCall oSysCall;
  
  i8 iPort = m_pDataStack->iPopInt(__func__);
  
  //- - - - - - - - - - - - - - - - - - -  - - - - - - - - - - - - - - - - - - -
  // attempt to create server-side listening socket
  
  Socket* pSocket = new Socket(this);
  bool bResult = pSocket->bCreateServer((int)iPort, oSysCall);
  
  if ( ! bResult ) {
    if (oSysCall.iError() == EADDRINUSE) {
      vThrow(FK_CHECK,__func__,__FILE__,__LINE__,
             "Address in use (port: %lld). "
             "Check if another instance of RAPIDSSE is running!"_Stz,
             iPort);
    }
    else
      vThrow(FK_CHECK,__func__,__FILE__,__LINE__,
             "Unable to listen for connections: Errno %d"_Stz, oSysCall.iError());
  }
  
  //- - - - - - - - - - - - - - - - - - -  - - - - - - - - - - - - - - - - - - -
  // test to see what happens when we attempt to create
  // server-side listening socket when the socket/port is already in use
  
//  Socket* pSocket2 = new Socket(this);
//  bool bResult2 = pSocket2->bCreateServer((int)iPort, oSysCall);
//
//  if ( ! bResult2 ) {
//    if (oSysCall.iError() == EADDRINUSE) {
//      vThrow(FK_CHECK,__func__,__FILE__,__LINE__,
//             "Address in use (port: %lld). "
//             "Check if another instance of RAPIDSSE is running!"_Stz,
//             iPort);
//    }
//    else
//      vThrow(FK_CHECK,__func__,__FILE__,__LINE__,
//             "Unable to listen for connections: Errno %d"_Stz, oSysCall.iError());
//  }

  // return socket
  m_pDataStack->vPushPtr(pSocket);
  
  EXIT();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

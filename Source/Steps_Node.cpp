/*============================================================================*/
/* Steps_Node.cpp - node step group                                           */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern Arguments       gxArguments;

extern AttachMap*      gpAttachMap;
extern BufferPool*     gpBufferPool;
extern Heap*           gpMediumHeap;
extern BPA*            gpBPA;
extern Dic*            gpMainDic;
extern Word*           gpWord;

extern Grid*           gpGrid;
extern StepArray*      gpStepArray;
extern ThreadList*     gpThreadList;
extern TaskList*       gpTaskList;
extern TxCollection*   gpTxCollection;

extern NP_Task         gnpRootTask;
extern StepBoard*      gpStepBoard;
extern Lock            gxDdlLock;
extern Lock            gxDicLock;
extern SEitfList*      gpSEitfList;

// node: ///////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Node(void)
{
  vAddPrimitive("node.init"_Str,       &vNodeInit,         false);
  vAddPrimitive("node.IP"_Str,         &vNodeIP,           false);
  vAddPrimitive("node.name"_Str,       &vNodeName,         false);
  vAddPrimitive("node.new"_Str,        &vNodeNew,          false);
  vAddPrimitive("node.no"_Str,         &vNodeNo,           false);
  vAddPrimitive("node.port"_Str,       &vNodePort,         false);
  vAddPrimitive("node.status"_Str,     &vNodeStatus,       false);
}

/*----------------------------------------------------------------------------*/
/* node.init -- initialise a node (of a grid)                                 */
/*----------------------------------------------------------------------------*/
void Task::vNodeInit(NNP_Task nnpTask)
{
ENTRY("vNodeInit"_Str);
  
  Object* pPtrNode;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrNode) )
    THROW_INVALID_DSTACK("node.init"_Str);
  
  Node* pNode = _pNode(pPtrNode);
  ASSERT(CN_NODE == pNode->eClass());
  
  new(pNode) Node;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* node.IP -- push IP of node                                                 */
/*----------------------------------------------------------------------------*/
void Task::vNodeIP(NNP_Task nnpTask)
{
  ENTRY("vNodeIP"_Str);
  
  Object* pPtrNode;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrNode) )
    THROW_INVALID_DSTACK("node.IP"_Str);
  
  Node* pNode = _pNode(pPtrNode);
  ASSERT(CN_NODE == pNode->eClass());
  
  s256 sIP;
  sIP = pNode->aNodeIP();
  nnpTask->m_pDataStack->vPush(sIP);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* node.new -- new a node (of a grid)                                         */
/*----------------------------------------------------------------------------*/
void Task::vNodeNew(NNP_Task nnpTask)
{
ENTRY("vNodeNew"_Str);
  
  i8 iNodeNo;
  
  if ( ! nnpTask->m_pDataStack->bPop(iNodeNo) )
    THROW_INVALID_DSTACK("node.new"_Str);
  
  Node* pNode = new Node(iNodeNo,
                         gxArguments.m_xNodes[iNodeNo].m_sNodeName,
                         gxArguments.m_xNodes[iNodeNo].m_sIPAddress,
                         gxArguments.m_xNodes[iNodeNo].m_iPortNo);
  ASSERT(CN_NODE == pNode->eClass());

  nnpTask->m_pDataStack->vPush(pNode);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* node.name -- push name of node                                             */
/*----------------------------------------------------------------------------*/
void Task::vNodeName(NNP_Task nnpTask)
{
  ENTRY("vNodeName"_Str);
  
  Object* pPtrNode;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrNode) )
    THROW_INVALID_DSTACK("node.name"_Str);
  
  Node* pNode = _pNode(pPtrNode);
  ASSERT(CN_NODE == pNode->eClass());
  
  s256 sName;
  sName = pNode->aNodeName();
  nnpTask->m_pDataStack->vPush(sName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* node.no -- push node number of node (of a grid)                            */
/*----------------------------------------------------------------------------*/
void Task::vNodeNo(NNP_Task nnpTask)
{
ENTRY("vNodeNo"_Str);
  
  Object* pPtrNode;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrNode) )
    THROW_INVALID_DSTACK("node.no"_Str);
  
  Node* pNode = _pNode(pPtrNode);
  ASSERT(CN_NODE == pNode->eClass());
  
  nnpTask->m_pDataStack->vPush(pNode->iNodeNo());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* node.port -- push port number of node (of a grid)                          */
/*----------------------------------------------------------------------------*/
void Task::vNodePort(NNP_Task nnpTask)
{
  ENTRY("vNodeNo"_Str);
  
  Object* pPtrNode;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrNode) )
    THROW_INVALID_DSTACK("node.port"_Str);
  
  Node* pNode = _pNode(pPtrNode);
  ASSERT(CN_NODE == pNode->eClass());
  
  nnpTask->m_pDataStack->vPush(pNode->iNodePortNo());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* node.status -- get status of this node (of a grid)                         */
/*----------------------------------------------------------------------------*/
void Task::vNodeStatus(NNP_Task nnpTask)
{
  ENTRY("vNodeStatus"_Str);

  s256 s;
  
  if (Region::bTest(gxArguments.m_sRegionName))
    StrLib::vSprint(s.aStr(),"RapidsSE instance %s running"_Str,
                    gxArguments.m_sRegionName.aStr());
  else
    StrLib::vSprint(s.aStr(),"RapidsSE instance %s not running"_Str,
                    gxArguments.m_sRegionName.aStr());
  
  nnpTask->m_pDataStack->vPush(s);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

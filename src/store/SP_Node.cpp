/*============================================================================*/
/* SP_Node.cpp - Node sprint helper                              (on c stack) */
/*============================================================================*/

#include "BASE.h"

#include "Node.hpp"
#include "SP_Node.hpp"

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SP_Node::SP_Node(void)
: SP(CN_SPID)
{
  _precondition_(_MAGIC_,CN_SPID,_FOOTER_);
  
  _checkInitialised_(CN_SPID);
  
  _postcondition_(_MAGIC_,CN_SPID,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SP_Node::~SP_Node(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_SPID,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* sprint Header and return Stz* to result                                    */
/*----------------------------------------------------------------------------*/
Stz* SP_Node::sz(Node* pNode)
{
  _precondition_(_MAGIC_,CN_SPID,_FOOTER_);
  
  StrLib::vSprint(m_caResult
                  ,"[%lld] %8s %s %lld %8s recv=%p send=%p [%s]"_Stz
                  
                  ,pNode->iNodeId()
                  ,pNode->szNodeNm()
                  ,pNode->szIPAddr()
                  ,pNode->iPortNo()
                  
                  ,aNS[pNode->eState()]
                  ,pNode->pRecvSocket()
                  ,pNode->pSendSocket()

                  ,pNode->pSendSocket()
                     ? pNode->pRecvSocket()
                         ? "cross-connected"
                         : "partially-connected"
                     : pNode->pRecvSocket()
                         ? "partially-connected"
                         : "not-connected"
                  
                  ,C_VARGS_END
                  );
  
  return (Stz*) &m_caResult[0];
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(SP_Node)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

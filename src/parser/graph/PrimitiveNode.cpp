/*============================================================================*/
/* PrimitiveNode.cpp - A node that holds definitions of base primitives       */
/*============================================================================*/

#include "../Parser.h"
#include "PrimitiveNode.hpp"



/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__pCastFunction(Parser,PrimitiveNode,CN_PRIMITIVENODE)
__aNewFunction(PrimitiveNode,CN_PRIMITIVENODE)
__vDeleteFunction(PrimitiveNode,CN_PRIMITIVENODE)
__aFooterFunction(PrimitiveNode)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
PrimitiveNode::PrimitiveNode(Stz* szName, Stz* szType, Value xValue,
                               PlanNodeList* pProperties, PlanNodeList* pChildren,
                               Stz* szPath)
    : PlanNode(CN_PRIMITIVENODE, szName, szType, xValue, pProperties, pChildren),
      m_sPath(szPath)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PRIMITIVENODE,_FOOTER_);

  _checkInitialised_(CN_PRIMITIVENODE);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_PRIMITIVENODE,_FOOTER_);

}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
PrimitiveNode::~PrimitiveNode(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PRIMITIVENODE,_FOOTER_);

}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Returns the path of this node in the hierarchy.                            */
/*----------------------------------------------------------------------------*/
Stz* PrimitiveNode::szPath(void)
{
  // Child elements are separated from parents using the '/' character.
  // Property elements are separated from their parent using the ':' character.
  return m_sPath.szStz();
}

/*----------------------------------------------------------------------------*/
/* Returns the description of this definition.                                */
/*----------------------------------------------------------------------------*/
Stz* PrimitiveNode::szDescription(void)
{
  ASSERT(m_xValue.m_pText);
  return m_xValue.m_pText->szGet();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/


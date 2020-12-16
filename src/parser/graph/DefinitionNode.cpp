/*============================================================================*/
/* DefinitionNode.cpp - A node that holds definitions of other nodes          */
/*============================================================================*/

#include "../Parser.h"
#include "DefinitionNode.hpp"



/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__pCastFunction(Parser,DefinitionNode,CN_DEFINITIONNODE)
__aNewFunction(DefinitionNode,CN_DEFINITIONNODE)
__vDeleteFunction(DefinitionNode,CN_DEFINITIONNODE)
__aFooterFunction(DefinitionNode)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
DefinitionNode::DefinitionNode(Stz* szName, Stz* szType, Value xValue,
                               PlanNodeList* pProperties, PlanNodeList* pChildren,
                               Stz* szPath)
    : PlanNode(CN_DEFINITIONNODE, szName, szType, xValue, pProperties, pChildren),
      m_sPath(szPath)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DEFINITIONNODE,_FOOTER_);

  _checkInitialised_(CN_DEFINITIONNODE);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_DEFINITIONNODE,_FOOTER_);

}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
DefinitionNode::~DefinitionNode(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DEFINITIONNODE,_FOOTER_);

}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Returns the path of this node in the hierarchy.                            */
/*----------------------------------------------------------------------------*/
Stz* DefinitionNode::szPath(void)
{
  // Child elements are separated from parents using the '/' character.
  // Property elements are separated from their parent using the ':' character.
  return m_sPath.szStz();
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/


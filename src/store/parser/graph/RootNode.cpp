/*============================================================================*/
/* RootNode.cpp - An immutable node that is the top of the plan graph         */
/*============================================================================*/

#include "BASE.h"
#include "RootNode.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__pCastFunction(Parser,RootNode,CN_ROOTNODE)
__aNewFunction(RootNode,CN_ROOTNODE)
__vDeleteFunction(RootNode,CN_ROOTNODE)
__aFooterFunction(RootNode)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
RootNode::RootNode(Stz* szName, Stz* szType, Value xValue,
                   PlanNodeList* pProperties, PlanNodeList* pChildren,
                   Stz* szPath)
    : PlanNode(CN_ROOTNODE, szName, szType, xValue, pProperties, pChildren),
      m_sPath(szPath)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROOTNODE,_FOOTER_);

  _checkInitialised_(CN_ROOTNODE);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_ROOTNODE,_FOOTER_);

}

/*----------------------------------------------------------------------------*/
/* temp constructor for use by ray to test sql compiler pipeline              */
/*----------------------------------------------------------------------------*/
RootNode::RootNode(void* aTempTest_byRay)
    : PlanNode(CN_ROOTNODE,aTempTest_byRay)
{
  
}



/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
RootNode::~RootNode(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROOTNODE,_FOOTER_);

  // Don't delete the /primitives and /definitions children because they
  // will be reused by other plan graphs.

  PlanNode* pChild;

  while ((pChild = m_pChildren->pPop()) != nullptr) {
    if (pChild->bIsClassNo(CN_DEFINITIONNODE) || pChild->bIsClassNo(CN_PRIMITIVENODE)) {
      // Don't delete these
      continue;
    }

    // Delete the other children. Could also have done this in the base destructor.
    ASSERT(pChild->bIsClassNo(CN_QUERYNODE) ||
                    pChild->bIsClassNo(CN_METADATANODE) ||
                    pChild->bIsClassNo(CN_STATISTICSNODE) );
    delete pChild;
  }
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Returns the path of this node in the hierarchy.                            */
/*----------------------------------------------------------------------------*/
Stz* RootNode::szPath(void)
{
  // Child elements are separated from parents using the '/' character.
  // Property elements are separated from their parent using the ':' character.
  return m_sPath.szStz();
}

/*----------------------------------------------------------------------------*/
/* Returns the description of this definition.                                */
/*----------------------------------------------------------------------------*/
Stz* RootNode::szDescription(void)
{
  ASSERT(m_xValue.m_eType == Value::VT_STRING);
  ASSERT(!m_xValue.m_bIsNull);
  ASSERT(m_xValue.m_pText);
  return m_xValue.m_pText->szGet();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/


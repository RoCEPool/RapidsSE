/*============================================================================*/
/* MetadataNode.cpp - An mutable node that holds metadata for the user's query*/
/*============================================================================*/

#include "BASE.h"
#include "MetadataNode.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__pCastFunction(Parser,MetadataNode,CN_METADATANODE)
__aNewFunction(MetadataNode,CN_METADATANODE)
__vDeleteFunction(MetadataNode,CN_METADATANODE)
__aFooterFunction(MetadataNode)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
MetadataNode::MetadataNode(Stz* szName, Stz* szType, Value&& xValue,
                               PlanNodeList* pProperties, PlanNodeList* pChildren)
    : MutablePlanNode(CN_METADATANODE, szName, szType, static_cast<Value&&>(xValue),
                      pProperties, pChildren)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_METADATANODE,_FOOTER_);

  _checkInitialised_(CN_METADATANODE);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_METADATANODE,_FOOTER_);

}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
MetadataNode::~MetadataNode(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_METADATANODE,_FOOTER_);

}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/


/*============================================================================*/
/* PlanNodeBuilder.cpp - A way to build PlanNodes                             */
/*============================================================================*/

#include "BASE.h"
#include "PlanNodeBuilder.hpp"
#include "PlanConstants.hpp"
#include "PlanNode.hpp"
#include "RootNode.hpp"
#include "PrimitiveNode.hpp"
#include "DefinitionNode.hpp"
#include "QueryNode.hpp"

#include "MetadataNode.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__pCastFunction(Parser,PlanNodeBuilder,CN_PLANNODEBUILDER)
__aNewFunction(PlanNodeBuilder,CN_PLANNODEBUILDER)
__vDeleteFunction(PlanNodeBuilder,CN_PLANNODEBUILDER)
__aFooterFunction(PlanNodeBuilder)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
PlanNodeBuilder::PlanNodeBuilder()
: Object(CN_PLANNODEBUILDER), m_xValue(), m_pProperties(nullptr), m_pChildren(nullptr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PLANNODEBUILDER,_FOOTER_);
  
  _checkInitialised_(CN_PLANNODEBUILDER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_PLANNODEBUILDER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
PlanNodeBuilder::~PlanNodeBuilder(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PLANNODEBUILDER,_FOOTER_);

  if (m_pProperties != nullptr)  delete m_pProperties;
  if (m_pChildren   != nullptr)  delete m_pChildren;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Creates a PlanNode* subclass based on the fields that have been set.       */
/*----------------------------------------------------------------------------*/
PlanNode* PlanNodeBuilder::pBuild(void)
{
  // Set the paths of each node if that hasn't been done
  if (m_sName.bEqual("__root__"_Stz) && m_sPath.bEqual(""_Stz)) {
    s256 sParentPath("/"_Stz);
    vSetPath(sParentPath);
  }

  RASSERT(m_sName.nLength() > 0);

  // Create the properties and children first.
  PlanNodeList *pPropertiesList = pBuildList(m_pProperties);
  PlanNodeList *pChildrenList   = pBuildList(m_pChildren);

  PlanNode* pPlanNode;

  // Create DefinitionNode or PrimitiveNode depending on current path.
  if (m_sPath.bEqual(const_cast<Stz*>(C_ROOT_PATH))) {
    pPlanNode = new RootNode(m_sName.szStz(),
                             m_sType.szStz(),
                             static_cast<Value&&>(m_xValue),
                             pPropertiesList,
                             pChildrenList,
                             m_sPath.szStz());

  } else if (m_sPath.bBeginsWith(const_cast<Stz*>(C_PRIMITIVES_PATH))) {
    pPlanNode = new PrimitiveNode(m_sName.szStz(),
                                   m_sType.szStz(),
                                   static_cast<Value&&>(m_xValue),
                                   pPropertiesList,
                                   pChildrenList,
                                   m_sPath.szStz());

  } else if (m_sPath.bBeginsWith(const_cast<Stz*>(C_DEFINITIONS_PATH))) {
    pPlanNode = new DefinitionNode(m_sName.szStz(),
                                   m_sType.szStz(),
                                   static_cast<Value&&>(m_xValue),
                                   pPropertiesList,
                                   pChildrenList,
                                   m_sPath.szStz());

  } else if (m_sPath.bBeginsWith(const_cast<Stz*>(C_PLAN_PATH))) {
    pPlanNode = new QueryNode(m_sName.szStz(),
                              m_sType.szStz(),
                              static_cast<Value&&>(m_xValue),
                              pPropertiesList,
                              pChildrenList);

  } else if (m_sPath.bEqual(const_cast<Stz*>(C_METADATA_PATH))) {
    // Shouldn't have any children at this point

    if (pPropertiesList) {
      ASSERT(pPropertiesList->nItems() == 0);
      delete pPropertiesList;
    }

    if (pChildrenList) {
      ASSERT(pChildrenList->nItems() == 0);
      delete pChildrenList;
    }

    pPlanNode = MetadataNode::pCreate();


  } else if (m_sPath.bEqual(const_cast<Stz*>(C_STATISTICS_PATH))) {
    // Shouldn't have any children at this point

    if (pPropertiesList) {
      ASSERT(pPropertiesList->nItems() == 0);
      delete pPropertiesList;
    }

    if (pChildrenList) {
      ASSERT(pChildrenList->nItems() == 0);
      delete pChildrenList;
    }

    pPlanNode = new QueryNode(m_sName.szStz(),
                              m_sType.szStz());

  } else {
    NEVER_REACH
  }

  m_xValue.vSetNull(Value::VT_UNKNOWN);
  return pPlanNode;
}

/*----------------------------------------------------------------------------*/
/* Creates a List of PlanNode*'s                                              */
/*----------------------------------------------------------------------------*/
PlanNodeList* PlanNodeBuilder::pBuildList(PlanNodeBuilderList* builderList)
{
  if (!builderList) {
    return nullptr;
  }

  PlanNodeList *pPlanNodeList = new PlanNodeList;

  PlanNodeBuilderIter* pBuilderIter = builderList->pNewIter();

  while (pBuilderIter->bHasNext()) {
    PlanNodeBuilder* pBuilder = pBuilderIter->pNext();

    PlanNode* planNode = pBuilder->pBuild();
    pPlanNodeList->vAppend(planNode);
  }
  delete pBuilderIter;

  return pPlanNodeList;
}

/*----------------------------------------------------------------------------*/
/* Sets the path of this builder and all properties and child builders        */
/*----------------------------------------------------------------------------*/
void PlanNodeBuilder::vSetPath(s256& sParentPath)
{
  if (m_sPath.nLength() == 0) {
    m_sPath.vSprint("%s%s"_Stz, sParentPath.szStz(), m_sName.szStz());
  }

  sParentPath.vConcat(m_sName.szStz());
  i8 pathSize = sParentPath.nLength();

  if (m_pProperties && m_pProperties->nItems() > 0) {

    PlanNodeBuilderIter* pBuilderIter = m_pProperties->pNewIter();

    while (pBuilderIter->bHasNext()) {
      PlanNodeBuilder* pBuilder = pBuilderIter->pNext();

      sParentPath.vClip(pathSize);
      sParentPath.vAppend(':');

      pBuilder->vSetPath(sParentPath);
    }

    delete pBuilderIter;
  }

  if (m_pChildren && m_pChildren->nItems() > 0) {

    PlanNodeBuilderIter* pBuilderIter = m_pChildren->pNewIter();

    while (pBuilderIter->bHasNext()) {
      PlanNodeBuilder* pBuilder = pBuilderIter->pNext();

      sParentPath.vClip(pathSize);
      sParentPath.vAppend('/');

      pBuilder->vSetPath(sParentPath);
    }

    delete pBuilderIter;
  }

  sParentPath.vClip(pathSize);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/


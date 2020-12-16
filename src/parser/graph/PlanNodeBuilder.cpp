/*============================================================================*/
/* PlanNodeBuilder.cpp - A way to build PlanNodes                             */
/*============================================================================*/

#include "../Parser.h"
#include "PlanNodeBuilder.hpp"



/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__pCastFunction(Parser,PlanNodeBuilder,CN_PLANNODEBUILDER)
__aFooterFunction(PlanNodeBuilder)


/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* PlanNodeBuilder::operator new(size_t)
{
  void* pObject = HeapMem::pAllocate(CN_PLANNODEBUILDER);

  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void PlanNodeBuilder::operator delete(void* pAddress)
{
  PlanNodeBuilder* pObject = reinterpret_cast<PlanNodeBuilder*>(pAddress);

  HeapMem::vFree(pObject);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
PlanNodeBuilder::PlanNodeBuilder()
: Object(CN_PLANNODEBUILDER), m_sName(""_Stz), m_sType(""_Stz),
  m_pProperties(nullptr), m_pChildren(nullptr), m_sPath(""_Stz)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PLANNODEBUILDER,_FOOTER_);
  
  _checkInitialised_(CN_PLANNODEBUILDER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_PLANNODEBUILDER,_FOOTER_);

  m_xValue.m_uInt = 0;
  m_xValue.bMustFree = false;
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

  // TODO: Create DefinitionNode or PrimitiveNode depending on current path.

  PlanNodeList *pPropertiesList = pBuildList(m_pProperties);
  PlanNodeList *pChildrenList   = pBuildList(m_pChildren);

  PlanNode* planNode = new DefinitionNode(m_sName.szStz(),
                                          m_sType.szStz(),
                                          m_xValue,
                                          pPropertiesList,
                                          pChildrenList,
                                          m_sPath.szStz());

  m_xValue.m_iInt = 0;
  m_xValue.bMustFree = false;
  return planNode;
}

/*----------------------------------------------------------------------------*/
/* Creates a List of PlanNode*'s                                              */
/*----------------------------------------------------------------------------*/
PlanNodeList* PlanNodeBuilder::pBuildList(PlanNodeBuilderList* builderList)
{
  if (!builderList) {
    return nullptr;
  }

  PlanNodeList *pPlanNodeList = new PlanNodeList();

  PlanNodeBuilder* builder = builderList->pHead();
  while (builder) {
    PlanNode* planNode = builder->pBuild();
    pPlanNodeList->vAppend(planNode);
    builder = builder->pNextItem();
  }

  return pPlanNodeList;
}

void PlanNodeBuilder::vSetPath(s256& sParentPath)
{
  if (m_sPath.nLength() == 0) {
    m_sPath.vSprint("%s%s"_Stz, sParentPath.szStz(), m_sName.szStz());
  }

  sParentPath.vConcat(m_sName.szStz());
  i8 pathSize = sParentPath.nLength();

  if (m_pProperties) {
    PlanNodeBuilder* builder = m_pProperties->pHead();
    while (builder) {
      sParentPath.vClip(pathSize);
      sParentPath.vAppend(':');

      builder->vSetPath(sParentPath);
      builder = builder->pNextItem();
    }
  }

  if (m_pChildren) {
    PlanNodeBuilder* builder = m_pChildren->pHead();
    while (builder) {
      sParentPath.vClip(pathSize);
      sParentPath.vAppend('/');

      builder->vSetPath(sParentPath);
      builder = builder->pNextItem();
    }
  }

  sParentPath.vClip(pathSize);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/


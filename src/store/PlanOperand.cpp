/*============================================================================*/
/* PlanOperand.cpp -- Plan Operand                                            */
/*============================================================================*/

#include "BASE.h"

#include "PlanOperand.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,PlanOperand,CN_PLANOPERAND)
__aNewFunction(PlanOperand,CN_PLANOPERAND)
__vDeleteFunction(PlanOperand,CN_PLANOPERAND)
__aFooterFunction(PlanOperand)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
PlanOperand::PlanOperand(Parser::PlanNode* pPlanNode)
: Operand(CN_PLANOPERAND)
{
  _precondition_(_MAGIC_,CN_PLANOPERAND);
  
  m_pPlanNode = pPlanNode;
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
PlanOperand::~PlanOperand(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_);
}

/*----------------------------------------------------------------------------*/
/* repair the plan operand                                                    */
/*----------------------------------------------------------------------------*/
void PlanOperand::vRepair(Object* pObject, eRF eRepair)
{
  _static_

  PlanOperand* pPlanOperand = _pPlanOperand(pObject);

  // delete the planOperand
  delete pPlanOperand;
}

/*----------------------------------------------------------------------------*/
/* get the object                                                             */
/*----------------------------------------------------------------------------*/
Parser::PlanNode* PlanOperand::pPlanNode(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_pPlanNode;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

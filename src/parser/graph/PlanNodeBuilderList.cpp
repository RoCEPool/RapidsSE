/*============================================================================*/
/* PlanNodeBuilderList.cpp - a list of PlanNodeBuilder objects                */
/*============================================================================*/

#include "parser/stages/parser/Parser.h"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__pCastFunction(Parser,PlanNodeBuilderList,CN_PLANNODEBUILDERLIST)
__aNewFunction(PlanNodeBuilderList,CN_PLANNODEBUILDERLIST)
__vDeleteFunction(PlanNodeBuilderList,CN_PLANNODEBUILDERLIST)
__aFooterFunction(PlanNodeBuilderList)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
PlanNodeBuilderList::PlanNodeBuilderList()
: ListNL_t<PlanNodeBuilder>(CN_PLANNODEBUILDERLIST)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PLANNODEBUILDERLIST,_FOOTER_);
  
  _checkInitialised_(CN_PLANNODEBUILDERLIST);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_PLANNODEBUILDERLIST,_FOOTER_);
  
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
PlanNodeBuilderList::~PlanNodeBuilderList(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PLANNODEBUILDERLIST,_FOOTER_);

  while (nItems() > 0) {
    PlanNodeBuilder* planNodeBuilder = pPop();
    delete planNodeBuilder;
  }
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

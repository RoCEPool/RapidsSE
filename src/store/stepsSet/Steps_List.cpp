/*============================================================================*/
/* Steps_List.cpp -- list step group                                          */
/*============================================================================*/

#include "BASE.h"
#include "STORE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "StepTiming.hpp"

#include "Node_I.hpp"
#include "LogicUnit.hpp"
#include "Response.hpp"
#include "Result.hpp"
#include "Job.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_List(void)
{
  vAddStep("List[].addRowBuffer"_Stz, &Step::vListAddRowBuffer);
  vAddStep("List[].delete"_Stz,       &Step::vListDelete);
  vAddStep("List[].new"_Stz,          &Step::vListNew);
  vAddStep("List[].print"_Stz,        &Step::vListPrint);
}

/*----------------------------------------------------------------------------*/
/* List[].addRowBuffer-- add row buffer to list                               */
/*----------------------------------------------------------------------------*/
void Step::vListAddRowBuffer(void)
{
  ENTRY();
  
  vTrace("vListAddRowBuffer\n"_Stz);
  

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* List[].delete -- delete list                                               */
/*----------------------------------------------------------------------------*/
void Step::vListDelete(void)
{
  ENTRY();
  
  vTrace("vListDelete\n"_Stz);
  

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* List[].new -- new list                                                     */
/*----------------------------------------------------------------------------*/
void Step::vListNew(void)
{
  ENTRY();
  
  vTrace("vListNew\n"_Stz);
  

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* List[].print -- print list                                                 */
/*----------------------------------------------------------------------------*/
void Step::vListPrint(void)
{
  ENTRY();
  
  vTrace("vListPrint\n"_Stz);
  

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

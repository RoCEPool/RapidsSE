/*============================================================================*/
/* Steps_Zero.cpp - zero step group                                           */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern Dic*            gpMainDic;

extern Grid*           gpGrid;
extern StepArray*      gpStepArray;
extern ThreadList*     gpThreadList;
extern TaskList*       gpTaskList;

extern NP_Task         gnpBootTask;
extern Lock            gxDdlLock;
extern Lock            gxDicLock;
extern SEitfList*      gpSEitfList;

// zero: ///////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Zero(void)
{
  vAddPrimitive("0<"_Stz,              &vZeroLess,         false);
  vAddPrimitive("0<="_Stz,             &vZeroLessEqual,    false);
  vAddPrimitive("0=="_Stz,             &vZeroEqual,        false);
  vAddPrimitive("0!="_Stz,             &vZeroNotEqual,     false);
  vAddPrimitive("0>="_Stz,             &vZeroGreaterEqual, false);
  vAddPrimitive("0>"_Stz,              &vZeroGreater,      false);
}

/*----------------------------------------------------------------------------*/
/* 0< -- test if top value is less than zero                                  */
/*----------------------------------------------------------------------------*/
void Task::vZeroLess(MP_Task mpTask)
{
  ENTRY("vZeroLess"_Stz);
  
  i8 iZero;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iZero) )
    THROW_INVALID_DSTACK("0<"_Stz);
  
  if (iZero < 0)
    mpTask->m_pDataStack->vPushInt((i8)1);
  else
    mpTask->m_pDataStack->vPushInt((i8)0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* 0<= -- test if top value is less than or equal to zero                     */
/*----------------------------------------------------------------------------*/
void Task::vZeroLessEqual(MP_Task mpTask)
{
  ENTRY("vZeroLessEqual"_Stz);
  
  i8 iZero;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iZero) )
    THROW_INVALID_DSTACK("0<="_Stz);
  
  if (iZero <= 0)
    mpTask->m_pDataStack->vPushInt((i8)1);
  else
    mpTask->m_pDataStack->vPushInt((i8)0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* 0== -- test if top value is equal to zero / nullptr                        */
/*----------------------------------------------------------------------------*/
void Task::vZeroEqual(MP_Task mpTask)
{
  ENTRY("vZeroEqual"_Stz);
  
  i8 iZero;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iZero) )
    THROW_INVALID_DSTACK("0=="_Stz);
  
  if (iZero == 0)
    mpTask->m_pDataStack->vPushInt((i8)1);
  else
    mpTask->m_pDataStack->vPushInt((i8)0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* 0!= -- test if top value is not equal to zero                              */
/*----------------------------------------------------------------------------*/
void Task::vZeroNotEqual(MP_Task mpTask)
{
  ENTRY("vZeroNotEqual"_Stz);
  
  i8 iZero;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iZero) )
    THROW_INVALID_DSTACK("0!="_Stz);
  
  if (iZero != 0)
    mpTask->m_pDataStack->vPushInt((i8)1);
  else
    mpTask->m_pDataStack->vPushInt((i8)0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* zeroGreaterEqual -- test if top value is greater than or equal to zero     */
/*----------------------------------------------------------------------------*/
void Task::vZeroGreaterEqual(MP_Task mpTask)
{
  ENTRY("vZeroGreaterEqual"_Stz);
  
  i8 iZero;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iZero) )
    THROW_INVALID_DSTACK("0>="_Stz);
  
  if (iZero >= 0)
    mpTask->m_pDataStack->vPushInt((i8)1);
  else
    mpTask->m_pDataStack->vPushInt((i8)0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* 0> -- test if top value is greater than zero                              */
/*----------------------------------------------------------------------------*/
void Task::vZeroGreater(MP_Task mpTask)
{
  ENTRY("vZeroGreater"_Stz);
  
  i8 iZero;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iZero) )
    THROW_INVALID_DSTACK("0>"_Stz);
  
  if (iZero > 0)
    mpTask->m_pDataStack->vPushInt((i8)1);
  else
    mpTask->m_pDataStack->vPushInt((i8)0);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

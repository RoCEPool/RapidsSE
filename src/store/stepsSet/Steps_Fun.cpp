/*============================================================================*/
/* Steps_Fun.cpp - fun[] step group                                           */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"
#include "Op.hpp"
#include "Stream.hpp"
#include "Stage.hpp"

#include "LogicUnit.hpp"

//#define WITH_COMBINEDOP_SE endif

#define WITH_FLAT_STAGE1 endif
//#define WITH_FLAT_STAGE2 endif
//#define WITH_FLAT_STAGE3 endif
//#define WITH_FLAT_STAGE4 endif

#ifdef WITH_COMBINEDOP_SE
  #include "../combined/FlatQuery.hpp"
#endif

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Fun(void)
{
  vAddStep("Fun0[]"_Stz,      &Step::vFun0Get);
  vAddStep("Fun0[].get"_Stz,  &Step::vFun0Get);
  vAddStep("Fun0[].set"_Stz,  &Step::vFun0Set);
  vAddStep("Fun0[].setup"_Stz,&Step::vFun0Setup);

  vAddStep("Fun1[]"_Stz,      &Step::vFun1Get);
  vAddStep("Fun1[].get"_Stz,  &Step::vFun1Get);
  vAddStep("Fun1[].set"_Stz,  &Step::vFun1Set);
  vAddStep("Fun1[].setup"_Stz,  &Step::vFun1Setup);

  vAddStep("Fun1_[]"_Stz,      &Step::vFun1_Get);
  vAddStep("Fun1_[].get"_Stz,  &Step::vFun1_Get);
  vAddStep("Fun1_[].set"_Stz,  &Step::vFun1_Set);
  vAddStep("Fun1_[].setup"_Stz,  &Step::vFun1_Setup);

  vAddStep("Fun2[]"_Stz,      &Step::vFun2Get);
  vAddStep("Fun2[].get"_Stz,  &Step::vFun2Get);
  vAddStep("Fun2[].isNotNull"_Stz,  &Step::vFun2IsNotNull);
  vAddStep("Fun2[].set"_Stz,  &Step::vFun2Set);
  vAddStep("Fun2[].setup"_Stz,&Step::vFun2Setup);

  vAddStep("Fun3[]"_Stz,      &Step::vFun3Get);
  vAddStep("Fun3[].get"_Stz,  &Step::vFun3Get);
  vAddStep("Fun3[].set"_Stz,  &Step::vFun3Set);
  vAddStep("Fun3[].setup"_Stz,&Step::vFun3Setup);

  vAddStep("Fun4[]"_Stz,      &Step::vFun4Get);
  vAddStep("Fun4[].get"_Stz,  &Step::vFun4Get);
  vAddStep("Fun4[].set"_Stz,  &Step::vFun4Set);
}

/*----------------------------------------------------------------------------*/
/* Fun0[].get -- get Fun0                                                     */
/*----------------------------------------------------------------------------*/
void Step::vFun0Get(void)
{
  ENTRY();

  m_pDataStack->vPushInt((i8)* m_pLogicUnit->m_afFun0);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Fun0[].set -- set Fun0                                                     */
/*----------------------------------------------------------------------------*/
void Step::vFun0Set(void)
{
  ENTRY();

  i8 iFun = m_pDataStack->iPopInt(__func__);

  m_pLogicUnit->m_afFun0 = (bool (*)(void*)) iFun;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Fun0[].setup -- setup Fun0                                                 */
/*----------------------------------------------------------------------------*/
void Step::vFun0Setup(void)
{
  ENTRY();

#ifdef WITH_COMBINEDOP_SE

  Stream* pStream = m_pOp->pStream();
  i8 iStage = pStream->pStage()->i_Stage();

  #ifdef WITH_FLAT_STAGE1
  if (iStage == 1)
    m_pLogicUnit->m_afFun0 =  (bool (*)(void*))stage1[0];
  #endif

  #ifdef WITH_FLAT_STAGE2
  if (iStage == 1)
    m_pLogicUnit->m_afFun0 =  (bool (*)(void*))stage1[0];
  if (iStage == 2)
    m_pLogicUnit->m_afFun0 =  (bool (*)(void*))stage2[0];
  #endif

  #ifdef WITH_FLAT_STAGE3
  if (iStage == 1)
    m_pLogicUnit->m_afFun0 =  (bool (*)(void*))stage1[0];
  if (iStage == 2)
    m_pLogicUnit->m_afFun0 =  (bool (*)(void*))stage2[0];
  if (iStage == 3)
    m_pLogicUnit->m_afFun0 =  (bool (*)(void*))stage3[0];
  #endif

#ifdef WITH_FLAT_STAGE4
  if (iStage == 1)
    m_pLogicUnit->m_afFun0 =  (bool (*)(void*))stage1[0];
  if (iStage == 2)
    m_pLogicUnit->m_afFun0 =  (bool (*)(void*))stage2[0];
  if (iStage == 3)
    m_pLogicUnit->m_afFun0 =  (bool (*)(void*))stage3[0];
  if (iStage == 4)
    m_pLogicUnit->m_afFun0 =  (bool (*)(void*))stage4[0];
#endif

#endif

  if ( m_pLogicUnit->m_afFun0 )
    m_pDataStack->vPushInt(true);
  else
    m_pDataStack->vPushInt(false);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Fun1[].get -- get Fun1                                                     */
/*----------------------------------------------------------------------------*/
void Step::vFun1Get(void)
{
  ENTRY();

  m_pDataStack->vPushInt((i8)* m_pLogicUnit->m_afFun1);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Fun1[].set -- set Fun1                                                     */
/*----------------------------------------------------------------------------*/
void Step::vFun1Set(void)
{
  ENTRY();

  i8 iFun = m_pDataStack->iPopInt(__func__);

  m_pLogicUnit->m_afFun1 = (bool (*)(void*)) iFun;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Fun1[].setup -- setup Fun1                                                 */
/*----------------------------------------------------------------------------*/
void Step::vFun1Setup(void)
{
  ENTRY();

#ifdef WITH_COMBINEDOP_SE
  Stream* pStream = m_pOp->pStream();
  i8 iStage = pStream->pStage()->i_Stage();

    #ifdef WITH_FLAT_STAGE1
      if (iStage == 1)
        m_pLogicUnit->m_afFun1 =  (bool (*)(void*))stage1[1];
    #endif

    #ifdef WITH_FLAT_STAGE2
  if (iStage == 1)
        m_pLogicUnit->m_afFun1 =  (bool (*)(void*))stage1[1];
  if (iStage == 2)
        m_pLogicUnit->m_afFun1 =  (bool (*)(void*))stage2[1];
    #endif

    #ifdef WITH_FLAT_STAGE3
  if (iStage == 1)
        m_pLogicUnit->m_afFun1 =  (bool (*)(void*))stage1[1];
  if (iStage == 2)
        m_pLogicUnit->m_afFun1 =  (bool (*)(void*))stage2[1];
  if (iStage == 3)
        m_pLogicUnit->m_afFun1 =  (bool (*)(void*))stage3[1];
    #endif

  #ifdef WITH_FLAT_STAGE4
  if (iStage == 1)
    m_pLogicUnit->m_afFun1 =  (bool (*)(void*))stage1[1];
  if (iStage == 2)
    m_pLogicUnit->m_afFun1 =  (bool (*)(void*))stage2[1];
  if (iStage == 3)
    m_pLogicUnit->m_afFun1 =  (bool (*)(void*))stage3[1];
  if (iStage == 3)
     m_pLogicUnit->m_afFun1 =  (bool (*)(void*))stage4[1];
  #endif

  #endif

  if ( m_pLogicUnit->m_afFun0 )
    m_pDataStack->vPushInt(true);
  else
    m_pDataStack->vPushInt(false);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Fun1_[].get -- get Fun1                                                    */
/*----------------------------------------------------------------------------*/
void Step::vFun1_Get(void)
{
  ENTRY();

  m_pDataStack->vPushInt((i8)* m_pLogicUnit->m_afFun1_);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Fun1_[].set -- set Fun1_                                                   */
/*----------------------------------------------------------------------------*/
void Step::vFun1_Set(void)
{
  ENTRY();

  i8 iFun = m_pDataStack->iPopInt(__func__);

  m_pLogicUnit->m_afFun1_ = (bool (*)(void*)) iFun;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Fun1_[].setup -- setup Fun1_                                               */
/*----------------------------------------------------------------------------*/
void Step::vFun1_Setup(void)
{
  ENTRY();

#ifdef WITH_COMBINEDOP_SE
  Stream* pStream = m_pOp->pStream();
  i8 iStage = pStream->pStage()->i_Stage();

    #ifdef WITH_FLAT_STAGE1
      if (iStage == 1)
        m_pLogicUnit->m_afFun1_ =  (bool (*)(void*))stage1[1];
    #endif

    #ifdef WITH_FLAT_STAGE2
  if (iStage == 1)
        m_pLogicUnit->m_afFun1_ =  (bool (*)(void*))stage1[1];
  if (iStage == 2)
        m_pLogicUnit->m_afFun1_ =  (bool (*)(void*))stage2[1];
    #endif

    #ifdef WITH_FLAT_STAGE3
  if (iStage == 1)
        m_pLogicUnit->m_afFun1_ =  (bool (*)(void*))stage1[1];
  if (iStage == 2)
        m_pLogicUnit->m_afFun1_ =  (bool (*)(void*))stage2[1];
  if (iStage == 3)
        m_pLogicUnit->m_afFun1_ =  (bool (*)(void*))stage3[1];
    #endif

  #ifdef WITH_FLAT_STAGE4
  if (iStage == 1)
    m_pLogicUnit->m_afFun1_ =  (bool (*)(void*))stage1[1];
  if (iStage == 2)
    m_pLogicUnit->m_afFun1_ =  (bool (*)(void*))stage2[1];
  if (iStage == 3)
    m_pLogicUnit->m_afFun1_ =  (bool (*)(void*))stage3[1];
  if (iStage == 3)
     m_pLogicUnit->m_afFun1_ =  (bool (*)(void*))stage4[1];
  #endif

#endif

  if ( m_pLogicUnit->m_afFun0 )
    m_pDataStack->vPushInt(true);
  else
    m_pDataStack->vPushInt(false);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Fun2[].get -- get Fun2                                                     */
/*----------------------------------------------------------------------------*/
void Step::vFun2Get(void)
{
  ENTRY();

  m_pDataStack->vPushInt((i8)* m_pLogicUnit->m_afFun2);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Fun2[].isNotNull -- fun2 is not null                                       */
/*----------------------------------------------------------------------------*/
void Step::vFun2IsNotNull(void)
{
  ENTRY();

  m_pDataStack->vPushInt(m_pLogicUnit->m_afFun2 ? 1 : 0);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Fun2[].set -- set Fun2                                                     */
/*----------------------------------------------------------------------------*/
void Step::vFun2Set(void)
{
  ENTRY();

  i8 iFun = m_pDataStack->iPopInt(__func__);

  m_pLogicUnit->m_afFun2 = (bool (*)(void*)) iFun;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Fun2[].setup -- setup Fun0                                                 */
/*----------------------------------------------------------------------------*/
void Step::vFun2Setup(void)
{
  ENTRY();

#ifdef WITH_COMBINEDOP_SE
  Stream* pStream = m_pOp->pStream();
  i8 iStage = pStream->pStage()->i_Stage();

    #ifdef WITH_FLAT_STAGE1
      if (iStage == 1)
            m_pLogicUnit->m_afFun2 =  (bool (*)(void*))stage1[2];
    #endif

    #ifdef WITH_FLAT_STAGE2
      if (iStage == 1)
            m_pLogicUnit->m_afFun2 =  (bool (*)(void*))stage1[2];
      if (iStage == 2)
            m_pLogicUnit->m_afFun2 =  (bool (*)(void*))stage2[2];
    #endif

  #ifdef WITH_FLAT_STAGE3
  if (iStage == 1)
            m_pLogicUnit->m_afFun2 =  (bool (*)(void*))stage1[2];
      if (iStage == 2)
            m_pLogicUnit->m_afFun2 =  (bool (*)(void*))stage2[2];
    if (iStage == 3)
      m_pLogicUnit->m_afFun2 =  (bool (*)(void*))stage3[2];
  #endif

#ifdef WITH_FLAT_STAGE4
  if (iStage == 1)
            m_pLogicUnit->m_afFun2 =  (bool (*)(void*))stage1[2];
      if (iStage == 2)
            m_pLogicUnit->m_afFun2 =  (bool (*)(void*))stage2[2];
    if (iStage == 3)
      m_pLogicUnit->m_afFun2 =  (bool (*)(void*))stage3[2];
    if (iStage == 4)
      m_pLogicUnit->m_afFun2 =  (bool (*)(void*))stage4[2];
#endif

#endif

  if ( m_pLogicUnit->m_afFun0 )
    m_pDataStack->vPushInt(true);
  else
    m_pDataStack->vPushInt(false);

  EXIT();
}


/*----------------------------------------------------------------------------*/
/* Fun3[].get -- get Fun3                                                     */
/*----------------------------------------------------------------------------*/
void Step::vFun3Get(void)
{
  ENTRY();

  m_pDataStack->vPushInt((i8)* m_pLogicUnit->m_afFun3);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Fun3[].set -- set Fun3                                                     */
/*----------------------------------------------------------------------------*/
void Step::vFun3Set(void)
{
  ENTRY();

  i8 iFun = m_pDataStack->iPopInt(__func__);

  m_pLogicUnit->m_afFun3 = (bool (*)(void*)) iFun;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Fun3[].setup -- setup Fun0                                                 */
/*----------------------------------------------------------------------------*/
void Step::vFun3Setup(void)
{
  ENTRY();

#ifdef WITH_COMBINEDOP_SE
  Stream* pStream = m_pOp->pStream();
  i8 iStage = pStream->pStage()->i_Stage();
    #ifdef WITH_FLAT_STAGE1
      if (iStage == 1)
            m_pLogicUnit->m_afFun3 =  (bool (*)(void*))stage1[3];
    #endif

    #ifdef WITH_FLAT_STAGE2
  if (iStage == 1)
            m_pLogicUnit->m_afFun3 =  (bool (*)(void*))stage1[3];
      if (iStage == 2)
            m_pLogicUnit->m_afFun3 =  (bool (*)(void*))stage2[3];
    #endif

    #ifdef WITH_FLAT_STAGE3
  if (iStage == 1)
            m_pLogicUnit->m_afFun3 =  (bool (*)(void*))stage1[3];
  if (iStage == 2)
            m_pLogicUnit->m_afFun3 =  (bool (*)(void*))stage2[3];
      if (iStage == 3)
        m_pLogicUnit->m_afFun3 =  (bool (*)(void*))stage3[3];
    #endif

    #ifdef WITH_FLAT_STAGE4
  if (iStage == 1)
    m_pLogicUnit->m_afFun3 =  (bool (*)(void*))stage1[3];
  if (iStage == 2)
    m_pLogicUnit->m_afFun3 =  (bool (*)(void*))stage2[3];
  if (iStage == 3)
    m_pLogicUnit->m_afFun3 =  (bool (*)(void*))stage3[3];
      if (iStage == 3)
        m_pLogicUnit->m_afFun3 =  (bool (*)(void*))stage4[3];
    #endif

#endif

  if ( m_pLogicUnit->m_afFun0 )
    m_pDataStack->vPushInt(true);
  else
    m_pDataStack->vPushInt(false);
  EXIT();
}


/*----------------------------------------------------------------------------*/
/* Fun4[].get -- get Fun4                                                     */
/*----------------------------------------------------------------------------*/
void Step::vFun4Get(void)
{
  ENTRY();

  m_pDataStack->vPushInt((i8)* m_pLogicUnit->m_afFun4);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Fun4[].set -- set Fun4                                                     */
/*----------------------------------------------------------------------------*/
void Step::vFun4Set(void)
{
  ENTRY();

  i8 iFun = m_pDataStack->iPopInt(__func__);

  m_pLogicUnit->m_afFun4 = (bool (*)(void*)) iFun;

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

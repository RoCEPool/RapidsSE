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

//#define WITH_FLAT_STAGE1 endif
//#define WITH_FLAT_STAGE2 endif
//#define WITH_FLAT_STAGE3 endif

#ifdef WITH_COMBINEDOP_SE
  #include "../combined/FlatQuery.h"
#endif

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Fun(void)
{
  vAddStep("Fun0[]"_Stz,      &Step::vFun0Get);
  vAddStep("Fun0[].call"_Stz, &Step::vFun0Call);
  vAddStep("Fun0[].get"_Stz,  &Step::vFun0Get);
  vAddStep("Fun0[].set"_Stz,  &Step::vFun0Set);
  vAddStep("Fun0[].setup"_Stz,&Step::vFun0Setup);

  vAddStep("Fun1[]"_Stz,      &Step::vFun1Get);
  vAddStep("Fun1[].call"_Stz, &Step::vFun1Call);
  vAddStep("Fun1[].get"_Stz,  &Step::vFun1Get);
  vAddStep("Fun1[].set"_Stz,  &Step::vFun1Set);
  vAddStep("Fun1[].setup"_Stz,  &Step::vFun1Setup);

  vAddStep("Fun2[]"_Stz,      &Step::vFun2Get);
  vAddStep("Fun2[].call"_Stz, &Step::vFun2Call);
  vAddStep("Fun2[].get"_Stz,  &Step::vFun2Get);
  vAddStep("Fun2[].set"_Stz,  &Step::vFun2Set);
  vAddStep("Fun2[].setup"_Stz,&Step::vFun2Setup);

  vAddStep("Fun3[]"_Stz,      &Step::vFun3Get);
  vAddStep("Fun3[].call"_Stz, &Step::vFun3Call);
  vAddStep("Fun3[].get"_Stz,  &Step::vFun3Get);
  vAddStep("Fun3[].set"_Stz,  &Step::vFun3Set);
  vAddStep("Fun3[].setup"_Stz,&Step::vFun3Setup);
}

/*----------------------------------------------------------------------------*/
/* Fun0[].call -- call Fun0                                                   */
/*----------------------------------------------------------------------------*/
void Step::vFun0Call(void)
{
  ENTRY();

  bool (*afFun)(void*) = m_p_TaskLogicUnit->m_afFun0;

  bool b = (*afFun)(nullptr);
  if (! b)
    vTrace("Fun0 returned false\n"_Stz);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Fun0[].get -- get Fun0                                                     */
/*----------------------------------------------------------------------------*/
void Step::vFun0Get(void)
{
  ENTRY();

  m_pDataStack->vPushInt((i8)* m_p_TaskLogicUnit->m_afFun0);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Fun0[].set -- set Fun0                                                     */
/*----------------------------------------------------------------------------*/
void Step::vFun0Set(void)
{
  ENTRY();

  i8 iFun = m_pDataStack->iPopInt(__func__);

  m_p_TaskLogicUnit->m_afFun0 = (bool (*)(void*)) iFun;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Fun0[].setup -- setup Fun0                                                 */
/*----------------------------------------------------------------------------*/
void Step::vFun0Setup(void)
{
  ENTRY();

#ifdef WITH_COMBINEDOP_SE

  Stream* pStream = m_p_TaskLogicUnit->m_pOp->pStream();
  i8 iStage = pStream->pStage()->iStage();

  #ifdef WITH_FLAT_STAGE1
  if (iStage == 1)
    m_p_TaskLogicUnit->m_afFun0 =  (bool (*)(void*))stage1[0];
  #endif

  #ifdef WITH_FLAT_STAGE2
  if (iStage == 2)
    m_p_TaskLogicUnit->m_afFun0 =  (bool (*)(void*))stage2[0];
  #endif

  #ifdef WITH_FLAT_STAGE3
    if (iStage == 3)
      m_p_TaskLogicUnit->m_afFun0 =  (bool (*)(void*))stage3[0];
  #endif

#endif

  if ( m_p_TaskLogicUnit->m_afFun0 )
    m_pDataStack->vPushInt(true);
  else
    m_pDataStack->vPushInt(false);

  EXIT();
}



/*----------------------------------------------------------------------------*/
/* Fun1[].call -- call Fun1                                                   */
/*----------------------------------------------------------------------------*/
void Step::vFun1Call(void)
{
  ENTRY();

  bool (*afFun)(void*) = m_p_TaskLogicUnit->m_afFun1;

  bool b = (*afFun)(nullptr);
  if (! b)
    vTrace("Fun1 returned false\n"_Stz);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Fun1[].get -- get Fun1                                                     */
/*----------------------------------------------------------------------------*/
void Step::vFun1Get(void)
{
  ENTRY();

  m_pDataStack->vPushInt((i8)* m_p_TaskLogicUnit->m_afFun1);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Fun1[].set -- set Fun1                                                     */
/*----------------------------------------------------------------------------*/
void Step::vFun1Set(void)
{
  ENTRY();

  i8 iFun = m_pDataStack->iPopInt(__func__);

  m_p_TaskLogicUnit->m_afFun1 = (bool (*)(void*)) iFun;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Fun1[].setup -- setup Fun1                                                 */
/*----------------------------------------------------------------------------*/
void Step::vFun1Setup(void)
{
  ENTRY();

#ifdef WITH_COMBINEDOP_SE
  Stream* pStream = m_p_TaskLogicUnit->m_pOp->pStream();
  i8 iStage = pStream->pStage()->iStage();

    #ifdef WITH_FLAT_STAGE1
      if (iStage == 1)
        m_p_TaskLogicUnit->m_afFun1 =  (bool (*)(void*))stage1[1];
    #endif

    #ifdef WITH_FLAT_STAGE2
      if (iStage == 2)
        m_p_TaskLogicUnit->m_afFun1 =  (bool (*)(void*))stage2[1];
    #endif

    #ifdef WITH_FLAT_STAGE3
      if (iStage == 3)
        m_p_TaskLogicUnit->m_afFun0 =  (bool (*)(void*))stage3[1];
    #endif

  #endif

  if ( m_p_TaskLogicUnit->m_afFun0 )
    m_pDataStack->vPushInt(true);
  else
    m_pDataStack->vPushInt(false);

  EXIT();
}




/*----------------------------------------------------------------------------*/
/* Fun2[].call -- call Fun2                                                   */
/*----------------------------------------------------------------------------*/
void Step::vFun2Call(void)
{
  ENTRY();

  bool (*afFun)(void*) = m_p_TaskLogicUnit->m_afFun2;

  bool b = (*afFun)(nullptr);
  if (! b)
    vTrace("Fun2 returned false\n"_Stz);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Fun2[].get -- get Fun2                                                     */
/*----------------------------------------------------------------------------*/
void Step::vFun2Get(void)
{
  ENTRY();

  m_pDataStack->vPushInt((i8)* m_p_TaskLogicUnit->m_afFun2);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Fun2[].set -- set Fun2                                                     */
/*----------------------------------------------------------------------------*/
void Step::vFun2Set(void)
{
  ENTRY();

  i8 iFun = m_pDataStack->iPopInt(__func__);

  m_p_TaskLogicUnit->m_afFun2 = (bool (*)(void*)) iFun;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Fun2[].setup -- setup Fun0                                                 */
/*----------------------------------------------------------------------------*/
void Step::vFun2Setup(void)
{
  ENTRY();

#ifdef WITH_COMBINEDOP_SE
  Stream* pStream = m_p_TaskLogicUnit->m_pOp->pStream();
  i8 iStage = pStream->pStage()->iStage();

    #ifdef WITH_FLAT_STAGE1
      if (iStage == 1)
            m_p_TaskLogicUnit->m_afFun2 =  (bool (*)(void*))stage1[2];
    #endif

    #ifdef WITH_FLAT_STAGE2
      if (iStage == 2)
            m_p_TaskLogicUnit->m_afFun2 =  (bool (*)(void*))stage2[2];
    #endif

#ifdef WITH_FLAT_STAGE3
  if (iStage == 3)
    m_p_TaskLogicUnit->m_afFun0 =  (bool (*)(void*))stage3[2];
#endif

#endif

  if ( m_p_TaskLogicUnit->m_afFun0 )
    m_pDataStack->vPushInt(true);
  else
    m_pDataStack->vPushInt(false);

  EXIT();
}





/*----------------------------------------------------------------------------*/
/* Fun3[].call -- call Fun3                                                   */
/*----------------------------------------------------------------------------*/
void Step::vFun3Call(void)
{
  ENTRY();

  bool (*afFun)(void*) = m_p_TaskLogicUnit->m_afFun3;

  bool b = (*afFun)(nullptr);
  if (! b)
    vTrace("Fun3 returned false\n"_Stz);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Fun3[].get -- get Fun3                                                     */
/*----------------------------------------------------------------------------*/
void Step::vFun3Get(void)
{
  ENTRY();

  m_pDataStack->vPushInt((i8)* m_p_TaskLogicUnit->m_afFun3);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Fun3[].set -- set Fun3                                                     */
/*----------------------------------------------------------------------------*/
void Step::vFun3Set(void)
{
  ENTRY();

  i8 iFun = m_pDataStack->iPopInt(__func__);

  m_p_TaskLogicUnit->m_afFun3 = (bool (*)(void*)) iFun;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Fun3[].setup -- setup Fun0                                                 */
/*----------------------------------------------------------------------------*/
void Step::vFun3Setup(void)
{
  ENTRY();

#ifdef WITH_COMBINEDOP_SE
  Stream* pStream = m_p_TaskLogicUnit->m_pOp->pStream();
  i8 iStage = pStream->pStage()->iStage();
    #ifdef WITH_FLAT_STAGE1
      if (iStage == 1)
            m_p_TaskLogicUnit->m_afFun3 =  (bool (*)(void*))stage1[3];
    #endif

    #ifdef WITH_FLAT_STAGE2
      if (iStage == 2)
            m_p_TaskLogicUnit->m_afFun3 =  (bool (*)(void*))stage2[3];
    #endif

    #ifdef WITH_FLAT_STAGE3
      if (iStage == 3)
        m_p_TaskLogicUnit->m_afFun0 =  (bool (*)(void*))stage3[3];
    #endif

#endif

  if ( m_p_TaskLogicUnit->m_afFun0 )
    m_pDataStack->vPushInt(true);
  else
    m_pDataStack->vPushInt(false);
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

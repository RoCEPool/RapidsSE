/*============================================================================*/
/* Steps_StreamDefOut.cpp -- Stream Definition (out-going) step group         */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_StreamDefOut(void)
{
  vAddStep("StreamDefOut[]"_Stz,            &Step::vStreamDefOutGet);
  vAddStep("StreamDefOut[].addColumn"_Stz,  &Step::vStreamDefOutAddColumn);
  vAddStep("StreamDefOut[].addColumns"_Stz, &Step::vStreamDefOutAddColumns);
  vAddStep("StreamDefOut[].delete"_Stz,     &Step::vStreamDefOutDelete);
  vAddStep("StreamDefOut[].new"_Stz,        &Step::vStreamDefOutNew);
}

/*----------------------------------------------------------------------------*/
/* StreamDefOut[].addColumn -- add a column                                   */
/*----------------------------------------------------------------------------*/
void Step::vStreamDefOutAddColumn(void)
{
  ENTRY();
    
  s32 sColNm;
  m_pDataStack->vPopStr(sColNm,__func__);

  eCT eColType = (eCT)m_pDataStack->iPopInt(__func__);

  StreamDef* pStreamDefOut = * m_pRegSet->m_xpStreamDefOut;
  
  pStreamDefOut->vAddColumn(sColNm,eColType);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamDefOut[].addColumns -- add columns from given stream def             */
/*----------------------------------------------------------------------------*/
void Step::vStreamDefOutAddColumns(void)
{
  ENTRY();
    
  StreamDef* pStreamDefSrc = _pStreamDef(m_pDataStack->pPopPtr(__func__));

  StreamDef* pStreamDefOut = * m_pRegSet->m_xpStreamDefOut;
  
  pStreamDefOut->vAddColumns(pStreamDefSrc);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamDefOut[].delete -- delete stream definition out                      */
/*----------------------------------------------------------------------------*/
void Step::vStreamDefOutDelete(void)
{
  ENTRY();
    
  StreamDef* pStreamDefOut = * m_pRegSet->m_xpStreamDefOut;
  
  m_pRegSet->m_xpStreamDefOut = nullptr;
  
  delete pStreamDefOut;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamDefOut[] -- get stream definition out                                */
/*----------------------------------------------------------------------------*/
void Step::vStreamDefOutGet(void)
{
  ENTRY();
    
  m_pDataStack->vPushPtr(* m_pRegSet->m_xpStreamDefOut);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamDefOut[].new -- new a stream definition out                          */
/*----------------------------------------------------------------------------*/
void Step::vStreamDefOutNew(void)
{
  ENTRY();
    
  m_pRegSet->m_xpStreamDefOut = new StreamDef;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/


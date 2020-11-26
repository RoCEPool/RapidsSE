/*============================================================================*/
/* Steps_Vortex.cpp - vortex step group                                       */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Vortex(void)
{
  vAddStep("vortex::new"_Stz,        &Step::vVortexNew);
  
  vAddStep("vortex.close"_Stz,       &Step::vVortexClose);
  vAddStep("vortex.delete"_Stz,      &Step::vVortexDelete);
  vAddStep("vortex.open"_Stz,        &Step::vVortexOpen);
  vAddStep("vortex.processH"_Stz,    &Step::vVortexProcessH);
  vAddStep("vortex.processHpp"_Stz,  &Step::vVortexProcessHpp);
}

/*----------------------------------------------------------------------------*/
/* vortex::new -- new Vortex                                                  */
/*----------------------------------------------------------------------------*/
void Step::vVortexNew(void)
{
  ENTRY();

  Vortex* pVortex = new Vortex;
  
  m_pDataStack->vPushPtr(pVortex);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* vortex.close -- close Vortex                                               */
/*----------------------------------------------------------------------------*/
void Step::vVortexClose(void)
{
  ENTRY();

  Vortex* pVortex = _pVortex(m_pDataStack->pPopPtr(__func__));
    
  pVortex->vClose();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* vortex.delete -- delete Vortex                                             */
/*----------------------------------------------------------------------------*/
void Step::vVortexDelete(void)
{
  ENTRY();

  Vortex* pVortex = _pVortex(m_pDataStack->pPopPtr(__func__));
  
  delete pVortex;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* vortex.open -- open Vortex                                                 */
/*----------------------------------------------------------------------------*/
void Step::vVortexOpen(void)
{
  ENTRY();

  Vortex* pVortex = _pVortex(m_pDataStack->pPopPtr(__func__));
    
  s256 sFilePath;
  m_pDataStack->vPopStr(sFilePath,__func__);  EXIT();
  
  bool bOpened = pVortex->bOpen(sFilePath);

  m_pDataStack->vPushInt(bOpened);
}

/*----------------------------------------------------------------------------*/
/* vortex::processH -- process .h files                                       */
/*----------------------------------------------------------------------------*/
void Step::vVortexProcessH(void)
{
  ENTRY();
  
  Vortex* pVortex = _pVortex(m_pDataStack->pPopPtr(__func__));

  s256 sSourcePath;
  m_pDataStack->vPopStr(sSourcePath,__func__);
  
  pVortex->vProcessSource(sSourcePath);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* vortex::processHpp -- process .hpp files                                   */
/*----------------------------------------------------------------------------*/
void Step::vVortexProcessHpp(void)
{
  ENTRY();
  
  Vortex* pVortex = _pVortex(m_pDataStack->pPopPtr(__func__));

  s256 sSourcePath;
  m_pDataStack->vPopStr(sSourcePath,__func__);
  
  pVortex->vProcessSource(sSourcePath);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

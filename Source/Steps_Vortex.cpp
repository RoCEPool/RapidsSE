/*============================================================================*/
/* Steps_Vortex.cpp - vortex step group                                       */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Vortex(void)
{
  vAddStep("vortex::processH"_Stz,    &Step::vVortexProcessH);
  vAddStep("vortex::processHpp"_Stz,  &Step::vVortexProcessHpp);
}

/*----------------------------------------------------------------------------*/
/* vortex::processH -- process .h files                                       */
/*----------------------------------------------------------------------------*/
void Step::vVortexProcessH(void)
{
  ENTRY(); // ENTRY("vVortexProcessH"_Stz);
  
  s256 sFilePath;
  m_pDataStack->vPopStr(sFilePath,__func__);
  
  File* pHFile = new File(nullptr);

  bool b = pHFile->bOpenMode(sFilePath,C_OPEN_READ);
  
  pHFile->vClose();
  
  delete pHFile;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* vortex::processHpp -- process .hpp files                                   */
/*----------------------------------------------------------------------------*/
void Step::vVortexProcessHpp(void)
{
  ENTRY(); // ENTRY("vVortexProcessHpp"_Stz);
  
  s256 sFilePath;
  m_pDataStack->vPopStr(sFilePath,__func__);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

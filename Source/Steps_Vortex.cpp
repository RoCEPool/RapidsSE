/*============================================================================*/
/* Steps_Vortex.cpp - vortex step group                                       */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Vortex(void)
{
  vAddStep("vortex::new"_Stz,         &Step::vVortexNew);
  
  vAddStep("vortex.delete"_Stz,       &Step::vVortexDelete);
  vAddStep("vortex::processH"_Stz,    &Step::vVortexProcessH);
  vAddStep("vortex::processHpp"_Stz,  &Step::vVortexProcessHpp);
}

/*----------------------------------------------------------------------------*/
/* vortex::new -- new Vortex                                                  */
/*----------------------------------------------------------------------------*/
void Step::vVortexNew(void)
{
  ENTRY(); // ENTRY("vVortexNew"_Stz);

  Vortex* pVortex = new Vortex;
  
  m_pDataStack->vPushPtr(pVortex);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* vortex.delete -- delete Vortex                                             */
/*----------------------------------------------------------------------------*/
void Step::vVortexDelete(void)
{
  ENTRY(); // ENTRY("vVortexNew"_Stz);

  Vortex* pVortex = _pVortex(m_pDataStack->pPopPtr(__func__));
  
  delete pVortex;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* vortex::processH -- process .h files                                       */
/*----------------------------------------------------------------------------*/
void Step::vVortexProcessH(void)
{
  ENTRY(); // ENTRY("vVortexProcessH"_Stz);
  
  s256 sFilePath;
  m_pDataStack->vPopStr(sFilePath,__func__);
  
//  File* pHFile = new File(nullptr);
//
//  bool b = pHFile->bOpenMode(sFilePath,C_OPEN_READ);
//
//  pHFile->vClose();
//
//  delete pHFile;
  
  Input* pInput = new Input;
  pInput->vPushFile(sFilePath);
  
  Token m_xTokenNm;
  Token m_xTokenXyz;
  while (pInput->bGetToken(&m_xTokenXyz)) {
   
    if (m_xTokenXyz.bSame("friend"_Stz)) {
      
      while (pInput->bGetToken(&m_xTokenXyz)) {
        if (m_xTokenXyz.bSame(";"_Stz))
         break;
      }
      continue;
    }

    if (m_xTokenXyz.bSame("class"_Stz)) {
      
      if (! pInput->bGetToken(&m_xTokenNm))
        break;

      if (! pInput->bGetToken(&m_xTokenXyz))
        break;

      if (m_xTokenXyz.bSame(":"_Stz) || m_xTokenXyz.bSame("{"_Stz))
        vTrace("  %s\n"_Stz,m_xTokenNm.azChar());
    }
  }

  delete pInput;
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
  
  Input* pInput = new Input;
  pInput->vPushFile(sFilePath);
  
  Token m_xTokenNm;
  Token m_xTokenXyz;
  while (pInput->bGetToken(&m_xTokenXyz)) {
    
    if (m_xTokenXyz.bSame("friend"_Stz)) {
      
      while (pInput->bGetToken(&m_xTokenXyz)) {
        if (m_xTokenXyz.bSame(";"_Stz))
         break;
      }
      continue;
    }
    
    if (m_xTokenXyz.bSame("class"_Stz)) {
      
      if (! pInput->bGetToken(&m_xTokenNm))
        break;

      if (! pInput->bGetToken(&m_xTokenXyz))
        break;

      if (m_xTokenXyz.bSame(":"_Stz) || m_xTokenXyz.bSame("{"_Stz))
        vTrace("  %s\n"_Stz,m_xTokenNm.azChar());
    }
  }
  
  delete pInput;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

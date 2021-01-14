/*============================================================================*/
/* SeApi.cpp -- Se Reentrant API                                              */
/*============================================================================*/

#include "BASE.h"

#include "SeApi.hpp"

#include "AM.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,SeApi,CN_SEAPI)
__aNewFunction(SeApi,CN_SEAPI)
__vDeleteFunction(SeApi,CN_SEAPI)
__aFooterFunction(SeApi)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SeApi::SeApi(void)
: Object(CN_SEAPI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEAPI,_FOOTER_);
  
  m_sSequence = ""_Stz;
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_SEAPI,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SeApi::~SeApi(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEAPI,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get current folder as string                                               */
/*----------------------------------------------------------------------------*/

 Stz* SeApi::szCurrentFolderNm(String* psFolderNm)
{
  _static_

  // get the current task
  Task* pTask = AM::pThisTLSTask();
  
  // get dictionary stack
  DicStack* pDicStack = pTask->pDicStack();
  
  // get dictionary
  Dic* pDic = pDicStack->pTop();
  
  // express that as a folder name string
  psFolderNm->vCopy(pDic->szDicNm());
    
  return psFolderNm->szStz();
}

/*----------------------------------------------------------------------------*/
/* get current folder path as string                                          */
/*----------------------------------------------------------------------------*/Stz* SeApi::szCurrentFolderPath(String* psFolderPath)
{
  _static_

  // get the current task
  Task* pTask = AM::pThisTLSTask();
  
  // get dictionary stack
  DicStack* pDicStack = pTask->pDicStack();
  
  // express that as a path-folder string
  pDicStack->vToPathStr(psFolderPath->szStz());
  
  return psFolderPath->szStz();
}

/*----------------------------------------------------------------------------*/
/* test if given table name is known                                          */
/*----------------------------------------------------------------------------*/
bool SeApi::bIsKnownTableNm(Stz* szTableNm)
{
  _static_
  
  // get the current task
  Task* pTask = AM::pThisTLSTask();
  
  // get dictionary stack
  DicStack* pDicStack = pTask->pDicStack();
  
  s32 sWordNm;
  sWordNm = szTableNm;

  Word* pWord = pDicStack->pFindWord(sWordNm);
 
  if (pWord)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* test if given column name is known within supplied table                   */
/*----------------------------------------------------------------------------*/
bool SeApi::bIsKnownColumnTable(Stz* szTableNm,Stz* szColumnNm)
{
  _static_
  
  // get the current task
  Task* pTask = AM::pThisTLSTask();
  
  // get dictionary stack
  DicStack* pDicStack = pTask->pDicStack();
  
  // get dictionary
  Dic* pDic = pDicStack->pTop();

  
  return false;
}

/*----------------------------------------------------------------------------*/
/* print all table names in current folder (debug help)                       */
/*----------------------------------------------------------------------------*/
void SeApi::vPrintAllTablesInCurrFolder(void)
{
  _static_
  
  // get the current task
  Task* pTask = AM::pThisTLSTask();
  
  // get dictionary stack
  DicStack* pDicStack = pTask->pDicStack();
  
  // get dictionary
  Dic* pDic = pDicStack->pTop();

  Word* pWord = nullptr;
  while ( (pWord = pDic->pEachWord(pWord)) ) {
    vTrace("  - %s\n"_Stz,pWord->szWordNm());
   }
}



/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* pipe | operator                                                            */
/*----------------------------------------------------------------------------*/
SeApi& SeApi::operator |(SeApi* pArg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEAPI,_FOOTER_);
  
  return *(SeApi*)this;
}

/*----------------------------------------------------------------------------*/
/* add operator argument                                                      */
/*----------------------------------------------------------------------------*/
SeApi* SeApi::pAdd(Stz* szOpNm, Stz* szOpArg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEAPI,_FOOTER_);
  
  m_sSequence.vCatSprint(" %s %s"_Stz,szOpArg,szOpNm);
  
  return this;
}

/*----------------------------------------------------------------------------*/
/* encode                                                                     */
/*----------------------------------------------------------------------------*/
SeApi* SeApi::pEncode(Stz* szAction)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEAPI,_FOOTER_);
  
  pEncode(szAction,""_Stz);
  
  return this;
}

/*----------------------------------------------------------------------------*/
/* encode                                                                     */
/*----------------------------------------------------------------------------*/
SeApi* SeApi::pEncode(Stz* szAction, Stz* szArgument)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEAPI,_FOOTER_);
  
  if (szAction->bSame("scan"_Stz) && szArgument->bSame("tables"_Stz)) {
    pAdd("metaScan_seapi"_Stz,""_Stz);
  }
  else
  if (szAction->bSame("into"_Stz)) {
    pAdd("rowToList_seapi"_Stz,""_Stz);
  }
  else
  if (szAction->bSame("tbd..."_Stz)) {
    vTrace("pEncode tbd : %s\n"_Stz,szArgument); \
  }
  else {
    NEVER_REACH;
  }
  
  return this;
}

/*----------------------------------------------------------------------------*/
/* get sequence                                                               */
/*----------------------------------------------------------------------------*/
Stz* SeApi::szGet(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEAPI,_FOOTER_);
    
  return m_sSequence;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

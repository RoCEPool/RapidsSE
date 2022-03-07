/*============================================================================*/
/* OptimiserCommands.cpp -- the optimiser commands                            */
/*============================================================================*/

#include "OptimiserCommands.hpp"

#include "OptimiserCatalog.hpp"
#include "Dynamo.hpp"

extern OptimiserCatalog gxOptimiserCatalog;

/*----------------------------------------------------------------------------*/
/* add                                                                        */
/*----------------------------------------------------------------------------*/
bool STORE::bOpt_add(Optimiser* pOptimiser, TokenVec* pTknVec, i8& riAction)
{
  vTrace("bOpt_add\n"_Stz);
  
  s4K sCmd;
  pTknVec->vToTknString(sCmd);
  
  vTrace("Cmd : %s\n"_Stz,sCmd.szStz());
  
  CardCtl* pCardCtl = pOptimiser->pCardCtl();

  i8 nTkns = pTknVec->nItems();
  if (nTkns < 2)
    return false;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (pTknVec->pGet(1)->bSame("*"_Stz)) {
    for (i8 i_Card = 0; i_Card < CK_ENUMS; i_Card++) {
      pCardCtl->vPrint(i_Card,1);
    }
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  for (i8 i_Tkn = 1; i_Tkn < nTkns; i_Tkn++) {
    Stz* szName = pTknVec->pGet(i_Tkn)->szChars();
    eCK eCard = Card::eToECard(szName);
    if (CK_NONE == eCard)
      continue;
    
    pCardCtl->vPrint(eCard,1);
  }
  
  return true;
}

OptimiserFun __Opt_add
(
  "add"_Stz,          // name of the prompt function
  "+"_Stz,            // command first word
  nullptr,            // command second word
  bOpt_add,           // function to call
  0,0,                // spare
  "add card print"_Stz
);

/*----------------------------------------------------------------------------*/
/* below                                                                      */
/*----------------------------------------------------------------------------*/
bool STORE::bOpt_below(Optimiser* pOptimiser, TokenVec* pTknVec, i8& riAction)
{
  vTrace("bOpt_below\n"_Stz);
  
  s4K sCmd;
  pTknVec->vToTknString(sCmd);
  
  vTrace("Cmd : %s\n"_Stz,sCmd.szStz());
  
  CardCtl* pCardCtl = pOptimiser->pCardCtl();

  i8 nTkns = pTknVec->nItems();
  if (pTknVec->nItems() < 2)
    return false;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (pTknVec->pGet(1)->bSame("*"_Stz)) {
    for (i8 i_Card = 0; i_Card < CK_ENUMS; i_Card++) {
      pCardCtl->vBelow(i_Card,1);
    }
    
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  for (i8 i_Tkn = 1; i_Tkn < nTkns; i_Tkn++) {
    Stz* szName = pTknVec->pGet(i_Tkn)->szChars();
    eCK eCard = Card::eToECard(szName);
    if (CK_NONE == eCard)
      continue;
    
    pCardCtl->vBelow(eCard,1);
  }
  
  return true;
}

OptimiserFun __Opt_below
(
  "below"_Stz,        // name of the prompt function
  "below"_Stz,        // command first word
  nullptr,            // command second word
  bOpt_below,         // function to call
  0,0,                // spare
  "set card below"_Stz
);

/*----------------------------------------------------------------------------*/
/* clear                                                                      */
/*----------------------------------------------------------------------------*/
bool STORE::bOpt_clear(Optimiser* pOptimiser, TokenVec* pTknVec, i8& riAction)
{
  Board* pBoard = pOptimiser->pHeadBoard();
  
  pBoard->vClear();

  riAction = 1;
  
  return true;
}

OptimiserFun __Opt_clear
(
  "clear"_Stz,        // name of the prompt function
  "clear"_Stz,        // command first word
  nullptr,            // command second word
  bOpt_clear,         // function to call
  0,0,                // spare
  "clear"_Stz
);

/*----------------------------------------------------------------------------*/
/* depth                                                                      */
/*----------------------------------------------------------------------------*/
bool STORE::bOpt_depth(Optimiser* pOptimiser, TokenVec* pTknVec, i8& riAction)
{
  vTrace("bOpt_depth\n"_Stz);
  
  s4K sCmd;
  pTknVec->vToTknString(sCmd);
  
  vTrace("Cmd : %s\n"_Stz,sCmd.szStz());
  
  CardCtl* pCardCtl = pOptimiser->pCardCtl();

  i8 iDepth = pCardCtl->iDepth();
  
  i8 nTkns = pTknVec->nItems();
  if (nTkns < 2) {
    vTrace("depth : %lld\n"_Stz,iDepth);
    riAction = 1;
    return true;
  }
  
  iDepth = pTknVec->pGet(1)->iToI8();
  //if (iDepth < 1)
  //  iDepth = 99;
  
  pCardCtl->vDepth(iDepth);
  
  return true;
}

OptimiserFun __Opt_depth
(
  "depth"_Stz,        // name of the prompt function
  "depth"_Stz,        // command first word
  nullptr,            // command second word
  bOpt_depth,         // function to call
  0,0,                // spare
  "set print depth limit"_Stz
);

/*----------------------------------------------------------------------------*/
/* form                                                                       */
/*----------------------------------------------------------------------------*/
bool STORE::bOpt_form(Optimiser* pOptimiser, TokenVec* pTknVec, i8& riAction)
{
  vTrace("bOpt_form\n"_Stz);
  
  s4K sCmd;
  pTknVec->vToTknString(sCmd);
  
  vTrace("Cmd : %s\n"_Stz,sCmd.szStz());
  
  CardCtl* pCardCtl = pOptimiser->pCardCtl();

  i8 nTkns = pTknVec->nItems();
  if (nTkns < 3)
    return false;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  i8 iForm = pTknVec->pGet(1)->iToI8();
  
  if (pTknVec->pGet(2)->bSame("*"_Stz)) {
    for (i8 i_Card = 0; i_Card < CK_ENUMS; i_Card++) {
      pCardCtl->vForm(i_Card,iForm);
    }
    
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  for (i8 i_Tkn = 2; i_Tkn < nTkns; i_Tkn++) {
    Stz* szName = pTknVec->pGet(i_Tkn)->szChars();
    eCK eCard = Card::eToECard(szName);
    if (CK_NONE == eCard)
      continue;
    
    pCardCtl->vForm(eCard,iForm);
  }
  
  return true;
}

OptimiserFun __Opt_form
(
  "form"_Stz,         // name of the prompt function
  "form"_Stz,         // command first word
  nullptr,            // command second word
  bOpt_form,          // function to call
  0,0,                // spare
  "set card print form"_Stz
);

/*----------------------------------------------------------------------------*/
/* hide                                                                       */
/*----------------------------------------------------------------------------*/
bool STORE::bOpt_help(Optimiser* pOptimiser, TokenVec* pTknVec, i8& riAction)
{
  gxOptimiserCatalog.vHelp();
  
  riAction = 1;
  
  return true;
}

OptimiserFun __Opt_help_0
(
  "help"_Stz,         // name of the prompt function
  "?"_Stz,            // command first word
  nullptr,            // command second word
  bOpt_help,          // function to call
  0,0,                // spare
  "help"_Stz
);

OptimiserFun __Opt_help_1
(
  "help"_Stz,         // name of the prompt function
  "help"_Stz,         // command first word
  nullptr,            // command second word
  bOpt_help,          // function to call
  0,0,                // spare
  "help"_Stz
);

/*----------------------------------------------------------------------------*/
/* hide                                                                       */
/*----------------------------------------------------------------------------*/
bool STORE::bOpt_hide(Optimiser* pOptimiser, TokenVec* pTknVec, i8& riAction)
{
  vTrace("bOpt_hide\n"_Stz);
  
  s4K sCmd;
  pTknVec->vToTknString(sCmd);
  
  vTrace("Cmd : %s\n"_Stz,sCmd.szStz());
  
  CardCtl* pCardCtl = pOptimiser->pCardCtl();

  i8 nTkns = pTknVec->nItems();
  if (nTkns < 2)
    return false;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (pTknVec->pGet(1)->bSame("*"_Stz)) {
    for (i8 i_Card = 0; i_Card < CK_ENUMS; i_Card++) {
      pCardCtl->vBelow(i_Card,0);
    }
    
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  for (i8 i_Tkn = 1; i_Tkn < nTkns; i_Tkn++) {
    Stz* szName = pTknVec->pGet(i_Tkn)->szChars();
    eCK eCard = Card::eToECard(szName);
    if (CK_NONE == eCard)
      return false;
    
    pCardCtl->vBelow(eCard,0);
  }
  
  return true;
}

OptimiserFun __Opt_hide
(
  "hide"_Stz,         // name of the prompt function
  "hide"_Stz,         // command first word
  nullptr,            // command second word
  bOpt_hide,          // function to call
  0,0,                // spare
  "clear card see"_Stz
);

/*----------------------------------------------------------------------------*/
/* load                                                                       */
/*----------------------------------------------------------------------------*/
bool STORE::bOpt_load(Optimiser* pOptimiser, TokenVec* pTknVec, i8& riAction)
{
  vTrace("bOpt_write\n"_Stz);

  Board* pBoard = pOptimiser->pHeadBoard();
  pBoard->vClear();

  s256 sPath;
  sPath.vSprint("/tmp/%s"_Stz,"unamed.txt"_Stz);
  
  File* pFile = new File(pOptimiser);

  bool bOpened = pFile->bOpenRead(sPath);
  if (! bOpened) {
    pFile->vClose();
    delete pFile;
    return false;
  }

  s48K sPipelineStmt;
  i8 nBytesRead = pFile->nReadFile(sPipelineStmt);
  if (! nBytesRead) {
    pFile->vClose();
    delete pFile;
    return false;
  }

  vTrace("Loaded (%lld bytes):\n%s\n"_Stz
         ,nBytesRead
         ,sPipelineStmt.szStz());

  pFile->vClose();
  delete pFile;
  
  PipelineSpec* pPipelineSpec =
    Dynamo::pToPipelineSpec(sPipelineStmt,
                            pOptimiser->pJobTags());

  pOptimiser->vToPipelineSpec(pPipelineSpec);

  return true;
}

OptimiserFun __Opt_load
(
  "load"_Stz,         // name of the prompt function
  "load"_Stz,         // command first word
  nullptr,            // command second word
   bOpt_load,         // function to call
  0,0,                // spare
  "load statement from file"_Stz
);

/*----------------------------------------------------------------------------*/
/* set queru name                                                             */
/*----------------------------------------------------------------------------*/
bool STORE::bOpt_name(Optimiser* pOptimiser, TokenVec* pTknVec, i8& riAction)
{
  vTrace("bOpt_name\n"_Stz);

  riAction = 1;

  Board* pBoard = pOptimiser->pHeadBoard();

  i8 nTkns = pTknVec->nItems();
  
  if (nTkns < 2) {
    vTrace("QueryNm : %s\n"_Stz,pBoard->szQueryNm());
    return true;
  }
  
  Stz* szQueryNm = pTknVec->pGet(1)->szChars();
  pBoard->vQueryNm(szQueryNm);
  
  return true;
}

OptimiserFun __Opt_name
(
  "name"_Stz,         // name of the prompt function
  "name"_Stz,          // command first word
  nullptr,            // command second word
  bOpt_name,       // function to call
  0,0,                // spare
  "get/set query name"_Stz
);

/*----------------------------------------------------------------------------*/
/* print                                                                       */
/*----------------------------------------------------------------------------*/
bool STORE::bOpt_reprint(Optimiser* pOptimiser, TokenVec* pTknVec, i8& riAction)
{
  vTrace("bOpt_reprint\n"_Stz);
  
  riAction = 0;
  
  return true;
}

OptimiserFun __Opt_reprint
(
  "reprint"_Stz,      // name of the prompt function
  "."_Stz,            // command first word
  nullptr,            // command second word
  bOpt_reprint,       // function to call
  0,0,                // spare
  "reprint board"_Stz
);

/*----------------------------------------------------------------------------*/
/* sub                                                                        */
/*----------------------------------------------------------------------------*/
bool STORE::bOpt_sub(Optimiser* pOptimiser, TokenVec* pTknVec, i8& riAction)
{
  vTrace("bOpt_sub\n"_Stz);
  
  s4K sCmd;
  pTknVec->vToTknString(sCmd);
  
  vTrace("Cmd : %s\n"_Stz,sCmd.szStz());
  
  CardCtl* pCardCtl = pOptimiser->pCardCtl();
  
  i8 nTkns = pTknVec->nItems();
  if (nTkns < 2)
    return false;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (pTknVec->pGet(1)->bSame("*"_Stz)) {
    for (i8 i_Card = 0; i_Card < CK_ENUMS; i_Card++) {
      if (Card::bIsMain((eCK)i_Card))
        continue;
      pCardCtl->vPrint(i_Card,0);
    }
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  for (i8 i_Tkn = 1; i_Tkn < nTkns; i_Tkn++) {
    Stz* szName = pTknVec->pGet(i_Tkn)->szChars();
    eCK eCard = Card::eToECard(szName);
    if (CK_NONE == eCard)
      continue;
    
    pCardCtl->vPrint(eCard,0);
  }
  
  return true;
}

OptimiserFun __Opt_sub
(
  "sub"_Stz,          // name of the prompt function
  "-"_Stz,            // command first word
  nullptr,            // command second word
  bOpt_sub,           // function to call
  0,0,                // spare
  "remove card print"_Stz
);

/*----------------------------------------------------------------------------*/
/* weave                                                                      */
/*----------------------------------------------------------------------------*/
bool STORE::bOpt_weave(Optimiser* pOptimiser, TokenVec* pTknVec, i8& riAction)
{
  vTrace("bOpt_weave\n"_Stz);
  
  s4K sCmd;
  pTknVec->vToTknString(sCmd);
  
  vTrace("%s\n"_Stz
         ,pOptimiser->szWeave()
         );
  
  riAction = 1;
  
  return true;
}

OptimiserFun __Opt_weave
(
  "weave"_Stz,        // name of the prompt function
  "weave"_Stz,        // command first word
  nullptr,            // command second word
  bOpt_weave,           // function to call
  0,0,                // spare
  "weave statement from cards"_Stz
);

/*----------------------------------------------------------------------------*/
/* write                                                                      */
/*----------------------------------------------------------------------------*/
bool STORE::bOpt_write(Optimiser* pOptimiser, TokenVec* pTknVec, i8& riAction)
{
  vTrace("bOpt_write\n"_Stz);
  
  s4K sCmd;
  pTknVec->vToTknString(sCmd);
  
  Stz* szStmt = pOptimiser->szWeave();
  Stz* szQueryNm = pOptimiser->pHeadBoard()->szQueryNm();

  s256 sPath;
  sPath.vSprint("/tmp/%s.txt"_Stz,szQueryNm);
  
  File* pFile = new File(pOptimiser);
  
  pFile->bOpenWrite(sPath);
  
  pFile->vPrint("%s"_Stz,szStmt);

  pFile->vClose();
  
  delete pFile;
  
  vTrace("%s written\n"_Stz,sPath.szStz());
  
  riAction = 1;
  
  return true;
}

OptimiserFun __Opt_write
(
  "write"_Stz,        // name of the prompt function
  "write"_Stz,        // command first word
  nullptr,            // command second word
  bOpt_write,           // function to call
  0,0,                // spare
  "write statement to file"_Stz
);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

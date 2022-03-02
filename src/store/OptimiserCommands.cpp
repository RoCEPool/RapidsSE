/*============================================================================*/
/* OptimiserCommands.cpp -- the optimiser commands                            */
/*============================================================================*/

#include "OptimiserCommands.hpp"

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

  if (pTknVec->nItems() < 2)
    return false;
  
  if (pTknVec->pGet(1)->bSame("*"_Stz)) {
    for (i8 i_Card = 0; i_Card < CK_ENUMS; i_Card++) {
      pCardCtl->vPrint(i_Card,1);
    }
  }
  else {
    Stz* szName = pTknVec->pGet(1)->szChars();
    eCK eCard = Card::eToECard(szName);
    if (CK_NONE == eCard)
      return false;
    
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
  
  if (pTknVec->nItems() < 2) {
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
/* hide                                                                        */
/*----------------------------------------------------------------------------*/
bool STORE::bOpt_hide(Optimiser* pOptimiser, TokenVec* pTknVec, i8& riAction)
{
  vTrace("bOpt_hide\n"_Stz);
  
  s4K sCmd;
  pTknVec->vToTknString(sCmd);
  
  vTrace("Cmd : %s\n"_Stz,sCmd.szStz());
  
  CardCtl* pCardCtl = pOptimiser->pCardCtl();

  if (pTknVec->nItems() < 2)
    return false;
  
  if (pTknVec->pGet(1)->bSame("*"_Stz)) {
    for (i8 i_Card = 0; i_Card < CK_ENUMS; i_Card++) {
      pCardCtl->vSee(i_Card,0);
    }
  }
  else {
    Stz* szName = pTknVec->pGet(1)->szChars();
    eCK eCard = Card::eToECard(szName);
    if (CK_NONE == eCard)
      return false;
    
    pCardCtl->vSee(eCard,0);
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
/* see                                                                        */
/*----------------------------------------------------------------------------*/
bool STORE::bOpt_see(Optimiser* pOptimiser, TokenVec* pTknVec, i8& riAction)
{
  vTrace("bOpt_see\n"_Stz);
  
  s4K sCmd;
  pTknVec->vToTknString(sCmd);
  
  vTrace("Cmd : %s\n"_Stz,sCmd.szStz());
  
  CardCtl* pCardCtl = pOptimiser->pCardCtl();

  if (pTknVec->nItems() < 2)
    return false;
  
  if (pTknVec->pGet(1)->bSame("*"_Stz)) {
    for (i8 i_Card = 0; i_Card < CK_ENUMS; i_Card++) {
      pCardCtl->vSee(i_Card,1);
    }
  }
  else {
    Stz* szName = pTknVec->pGet(1)->szChars();
    eCK eCard = Card::eToECard(szName);
    if (CK_NONE == eCard)
      return false;
    
    pCardCtl->vSee(eCard,1);
  }
  
  return true;
}

OptimiserFun __Opt_see
(
  "see"_Stz,          // name of the prompt function
  "see"_Stz,          // command first word
  nullptr,            // command second word
  bOpt_see,           // function to call
  0,0,                // spare
  "set card see"_Stz
);

/*----------------------------------------------------------------------------*/
/* sub                                                                        */
/*----------------------------------------------------------------------------*/
bool STORE::bOpt_sub(Optimiser* pOptimiser, TokenVec* pTknVec, i8& riAction)
{
  vTrace("bOptimser_b\n"_Stz);
  
  s4K sCmd;
  pTknVec->vToTknString(sCmd);
  
  vTrace("Cmd : %s\n"_Stz,sCmd.szStz());
  
  CardCtl* pCardCtl = pOptimiser->pCardCtl();
  
  if (pTknVec->nItems() < 2)
    return false;
  
  if (pTknVec->pGet(1)->bSame("*"_Stz)) {
    for (i8 i_Card = 0; i_Card < CK_ENUMS; i_Card++) {
      if (Card::bIsMain((eCK)i_Card))
        continue;
      pCardCtl->vPrint(i_Card,0);
    }
  }
  else {
    Stz* szName = pTknVec->pGet(1)->szChars();
    eCK eCard = Card::eToECard(szName);
    if (CK_NONE == eCard)
      return false;
    
    
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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

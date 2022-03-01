/*============================================================================*/
/* OptimiserCommands.cpp -- the optimiser commands                            */
/*============================================================================*/

#include "OptimiserCommands.hpp"

/*----------------------------------------------------------------------------*/
/* add                                                                        */
/*----------------------------------------------------------------------------*/
bool STORE::bOpt_add(Optimiser* pOptimiser, TokenVec* pTknVec)
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
/* sub                                                                        */
/*----------------------------------------------------------------------------*/
bool STORE::bOpt_sub(Optimiser* pOptimiser, TokenVec* pTknVec)
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

/*============================================================================*/
/* OptimiserCommands.cpp -- the optimiser commands                            */
/*============================================================================*/

#include "OptimiserCommands.hpp"

#include "OptimiserCatalog.hpp"
#include "Dynamo.hpp"
#include "CardRegs.hpp"

extern OptimiserCatalog gxOptimiserCatalog;

/*----------------------------------------------------------------------------*/
/* cardCtl: add                                                               */
/*----------------------------------------------------------------------------*/
bool STORE::bOpt_add(Optimiser* pOptimiser, TokenVec* pTknVec, i8& riAction)
{
  vTrace("bOpt_add\n"_Stz);
//  s4K sCmd;
//  pTknVec->vToTknString(sCmd);
//  vTrace("Cmd : %s\n"_Stz,sCmd.szStz());

  Drafter* pDrafter = pOptimiser->pDrafter();
  bool bResult = pDrafter->bAdd(pTknVec,riAction);
  return bResult;
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
/* cardCtl: below                                                             */
/*----------------------------------------------------------------------------*/
bool STORE::bOpt_below(Optimiser* pOptimiser, TokenVec* pTknVec, i8& riAction)
{
  vTrace("bOpt_below\n"_Stz);

  Drafter* pDrafter = pOptimiser->pDrafter();
  bool bResult = pDrafter->bBelow(pTknVec,riAction);
  return bResult;
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
/* drafter: clear                                                             */
/*----------------------------------------------------------------------------*/
bool STORE::bOpt_clear(Optimiser* pOptimiser, TokenVec* pTknVec, i8& riAction)
{
  vTrace("bOpt_clear\n"_Stz);
  
  Drafter* pDrafter = pOptimiser->pDrafter();
  bool bResult = pDrafter->bClear(pTknVec,riAction);
  return bResult;
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
/* drafter: compare cards                                                     */
/*----------------------------------------------------------------------------*/
bool STORE::bOpt_cmp(Optimiser* pOptimiser, TokenVec* pTknVec, i8& riAction)
{
  vTrace("bOpt_cut\n"_Stz);

  Drafter* pDrafter = pOptimiser->pDrafter();
  bool bResult = pDrafter->bCmp(pTknVec,riAction);
  return bResult;
}

OptimiserFun __Opt_cmp
(
  "cmp"_Stz,          // name of the prompt function
  "cmp"_Stz,          // command first word
  nullptr,            // command second word
  bOpt_cmp,           // function to call
  0,0,                // spare
  "compare cards"_Stz
);

/*----------------------------------------------------------------------------*/
/* drafter: cut card                                                          */
/*----------------------------------------------------------------------------*/
bool STORE::bOpt_cut(Optimiser* pOptimiser, TokenVec* pTknVec, i8& riAction)
{
  vTrace("bOpt_cut\n"_Stz);
  
  Drafter* pDrafter = pOptimiser->pDrafter();
  bool bResult = pDrafter->bCut(pTknVec,riAction);
  return bResult;
}

OptimiserFun __Opt_cut
(
  "cut"_Stz,          // name of the prompt function
  "cut"_Stz,          // command first word
  nullptr,            // command second word
  bOpt_cut,           // function to call
  0,0,                // spare
  "cut card"_Stz
);

/*----------------------------------------------------------------------------*/
/* cardCtl: depth                                                             */
/*----------------------------------------------------------------------------*/
bool STORE::bOpt_depth(Optimiser* pOptimiser, TokenVec* pTknVec, i8& riAction)
{
  vTrace("bOpt_depth\n"_Stz);
  
  Drafter* pDrafter = pOptimiser->pDrafter();
  bool bResult = pDrafter->bDepth(pTknVec,riAction);
  return bResult;
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
/* cardCtl: form                                                              */
/*----------------------------------------------------------------------------*/
bool STORE::bOpt_form(Optimiser* pOptimiser, TokenVec* pTknVec, i8& riAction)
{
  vTrace("bOpt_form\n"_Stz);
  
  Drafter* pDrafter = pOptimiser->pDrafter();
  bool bResult = pDrafter->bForm(pTknVec,riAction);
  return bResult;
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
/* help                                                                       */
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
/* cardCtl: hide                                                              */
/*----------------------------------------------------------------------------*/
bool STORE::bOpt_hide(Optimiser* pOptimiser, TokenVec* pTknVec, i8& riAction)
{
  vTrace("bOpt_hide\n"_Stz);
    
  Drafter* pDrafter = pOptimiser->pDrafter();
  bool bResult = pDrafter->bHide(pTknVec,riAction);
  return bResult;
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
/* drafter: load                                                              */
/*----------------------------------------------------------------------------*/
bool STORE::bOpt_load(Optimiser* pOptimiser, TokenVec* pTknVec, i8& riAction)
{
  vTrace("bOpt_write\n"_Stz);
  
  Drafter* pDrafter = pOptimiser->pDrafter();
  bool bResult = pDrafter->bLoad(pTknVec,riAction);
  return bResult;
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
/* drafter: set query name                                                    */
/*----------------------------------------------------------------------------*/
bool STORE::bOpt_name(Optimiser* pOptimiser, TokenVec* pTknVec, i8& riAction)
{
  vTrace("bOpt_name\n"_Stz);
  
  Drafter* pDrafter = pOptimiser->pDrafter();
  bool bResult = pDrafter->bName(pTknVec,riAction);
  return bResult;
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
/* print                                                                      */
/*----------------------------------------------------------------------------*/
bool STORE::bOpt_reprint(Optimiser* pOptimiser, TokenVec* pTknVec, i8& riAction)
{
  vTrace("bOpt_reprint\n"_Stz);
  
  Drafter* pDrafter = pOptimiser->pDrafter();
  bool bResult = pDrafter->bReprint(pTknVec,riAction);
  return bResult;
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
/* drafter: set card registers                                                */
/*----------------------------------------------------------------------------*/
bool STORE::bOpt_set(Optimiser* pOptimiser, TokenVec* pTknVec, i8& riAction)
{
  vTrace("bOpt_set\n"_Stz);
  
  Drafter* pDrafter = pOptimiser->pDrafter();
  bool bResult = pDrafter->bSet(pTknVec,riAction);
  return bResult;
}

OptimiserFun __Opt_set
(
  "set"_Stz,          // name of the prompt function
  "set"_Stz,          // command first word
  nullptr,            // command second word
  bOpt_set,           // function to call
  0,0,                // spare
  "set card registers"_Stz
);

/*----------------------------------------------------------------------------*/
/* cardCtl: sub                                                               */
/*----------------------------------------------------------------------------*/
bool STORE::bOpt_sub(Optimiser* pOptimiser, TokenVec* pTknVec, i8& riAction)
{
  vTrace("bOpt_sub\n"_Stz);
  
  Drafter* pDrafter = pOptimiser->pDrafter();
  bool bResult = pDrafter->bSub(pTknVec,riAction);
  return bResult;
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
/* drafter: weave                                                             */
/*----------------------------------------------------------------------------*/
bool STORE::bOpt_weave(Optimiser* pOptimiser, TokenVec* pTknVec, i8& riAction)
{
  vTrace("bOpt_weave\n"_Stz);
  
  Drafter* pDrafter = pOptimiser->pDrafter();
  bool bResult = pDrafter->bWeave(pTknVec,riAction);
  return bResult;
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
/* drafter: write                                                             */
/*----------------------------------------------------------------------------*/
bool STORE::bOpt_write(Optimiser* pOptimiser, TokenVec* pTknVec, i8& riAction)
{
  vTrace("bOpt_write\n"_Stz);
  
  Drafter* pDrafter = pOptimiser->pDrafter();
  bool bResult = pDrafter->bWrite(pTknVec,riAction);
  return bResult;
}

OptimiserFun __Opt_write
(
  "write"_Stz,        // name of the prompt function
  "write"_Stz,        // command first word
  nullptr,            // command second word
  bOpt_write,         // function to call
  0,0,                // spare
  "write statement to file"_Stz
);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

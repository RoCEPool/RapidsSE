/*============================================================================*/
/* RulesCommands.cpp -- the rules commands                                    */
/*============================================================================*/

#include "RulesCommands.hpp"

/*----------------------------------------------------------------------------*/
/* rule 0                                                                     */
/*----------------------------------------------------------------------------*/
bool STORE::bRule_r0(Optimiser* pOptimiser, TokenVec* pTknVec, i8& riAction)
{
  vTrace("bRule_r0\n"_Stz);
  
  s4K sCmd;
  pTknVec->vToTknString(sCmd);
  
  vTrace("Cmd : %s\n"_Stz,sCmd.szStz());
  
  i8 nTkns = pTknVec->nItems();
  if (nTkns < 2)
    return false;
    
  return true;
}

RulesFun __Rule_r0
(
  "r0"_Stz,  // name of the function
  "r0"_Stz,  // command first word
  nullptr,   // command second word
  bRule_r0,  // function to call
  0,0,       // spare
  "rule 0"_Stz
);

/*----------------------------------------------------------------------------*/
/* rule 1                                                                     */
/*----------------------------------------------------------------------------*/
bool STORE::bRule_r1(Optimiser* pOptimiser, TokenVec* pTknVec, i8& riAction)
{
  vTrace("bRule_r1\n"_Stz);
  
  s4K sCmd;
  pTknVec->vToTknString(sCmd);
  
  vTrace("Cmd : %s\n"_Stz,sCmd.szStz());
  
  i8 nTkns = pTknVec->nItems();
  if (nTkns < 2)
    return false;
    
  return true;
}

RulesFun __Rule_r1
(
  "r1"_Stz,  // name of the function
  "r1"_Stz,  // command first word
  nullptr,   // command second word
  bRule_r1,  // function to call
  0,0,       // spare
  "rule 1"_Stz
);

/*----------------------------------------------------------------------------*/
/* rule 2                                                                     */
/*----------------------------------------------------------------------------*/
bool STORE::bRule_r2(Optimiser* pOptimiser, TokenVec* pTknVec, i8& riAction)
{
  vTrace("bRule_r2\n"_Stz);
  
  s4K sCmd;
  pTknVec->vToTknString(sCmd);
  
  vTrace("Cmd : %s\n"_Stz,sCmd.szStz());
  
  i8 nTkns = pTknVec->nItems();
  if (nTkns < 2)
    return false;
    
  return true;
}

RulesFun __Rule_r2
(
  "r2"_Stz,  // name of the function
  "r2"_Stz,  // command first word
  nullptr,   // command second word
  bRule_r2,  // function to call
  0,0,       // spare
  "rule 2"_Stz
);

/*----------------------------------------------------------------------------*/
/* rule 3                                                                     */
/*----------------------------------------------------------------------------*/
bool STORE::bRule_r3(Optimiser* pOptimiser, TokenVec* pTknVec, i8& riAction)
{
  vTrace("bRule_r3\n"_Stz);
  
  s4K sCmd;
  pTknVec->vToTknString(sCmd);
  
  vTrace("Cmd : %s\n"_Stz,sCmd.szStz());
  
  i8 nTkns = pTknVec->nItems();
  if (nTkns < 2)
    return false;
    
  return true;
}

RulesFun __Rule_r3
(
  "r3"_Stz,  // name of the function
  "r3"_Stz,  // command first word
  nullptr,   // command second word
  bRule_r3,  // function to call
  0,0,       // spare
  "rule 3"_Stz
);

/*----------------------------------------------------------------------------*/
/* rule 4                                                                     */
/*----------------------------------------------------------------------------*/
bool STORE::bRule_r4(Optimiser* pOptimiser, TokenVec* pTknVec, i8& riAction)
{
  vTrace("bRule_r4\n"_Stz);
  
  s4K sCmd;
  pTknVec->vToTknString(sCmd);
  
  vTrace("Cmd : %s\n"_Stz,sCmd.szStz());
  
  i8 nTkns = pTknVec->nItems();
  if (nTkns < 2)
    return false;
    
  return true;
}

RulesFun __Rule_r4
(
  "r4"_Stz,  // name of the function
  "r4"_Stz,  // command first word
  nullptr,   // command second word
  bRule_r4,  // function to call
  0,0,       // spare
  "rule 4"_Stz
);

/*----------------------------------------------------------------------------*/
/* rule 5                                                                     */
/*----------------------------------------------------------------------------*/
bool STORE::bRule_r5(Optimiser* pOptimiser, TokenVec* pTknVec, i8& riAction)
{
  vTrace("bRule_r5\n"_Stz);
  
  s4K sCmd;
  pTknVec->vToTknString(sCmd);
  
  vTrace("Cmd : %s\n"_Stz,sCmd.szStz());
  
  i8 nTkns = pTknVec->nItems();
  if (nTkns < 2)
    return false;
    
  return true;
}

RulesFun __Rule_r5
(
  "r5"_Stz,  // name of the function
  "r5"_Stz,  // command first word
  nullptr,   // command second word
  bRule_r5,  // function to call
  0,0,       // spare
  "rule 5"_Stz
);

/*----------------------------------------------------------------------------*/
/* rule 6                                                                     */
/*----------------------------------------------------------------------------*/
bool STORE::bRule_r6(Optimiser* pOptimiser, TokenVec* pTknVec, i8& riAction)
{
  vTrace("bRule_r6\n"_Stz);
  
  s4K sCmd;
  pTknVec->vToTknString(sCmd);
  
  vTrace("Cmd : %s\n"_Stz,sCmd.szStz());
  
  i8 nTkns = pTknVec->nItems();
  if (nTkns < 2)
    return false;
    
  return true;
}

RulesFun __Rule_r6
(
  "r6"_Stz,  // name of the function
  "r6"_Stz,  // command first word
  nullptr,   // command second word
  bRule_r6,  // function to call
  0,0,       // spare
  "rule 6"_Stz
);

/*----------------------------------------------------------------------------*/
/* rule 7                                                                     */
/*----------------------------------------------------------------------------*/
bool STORE::bRule_r7(Optimiser* pOptimiser, TokenVec* pTknVec, i8& riAction)
{
  vTrace("bRule_r7\n"_Stz);
  
  s4K sCmd;
  pTknVec->vToTknString(sCmd);
  
  vTrace("Cmd : %s\n"_Stz,sCmd.szStz());
  
  i8 nTkns = pTknVec->nItems();
  if (nTkns < 2)
    return false;
    
  return true;
}

RulesFun __Rule_r7
(
  "r7"_Stz,  // name of the function
  "r7"_Stz,  // command first word
  nullptr,   // command second word
  bRule_r7,  // function to call
  0,0,       // spare
  "rule 7"_Stz
);

/*----------------------------------------------------------------------------*/
/* rule 8                                                                     */
/*----------------------------------------------------------------------------*/
bool STORE::bRule_r8(Optimiser* pOptimiser, TokenVec* pTknVec, i8& riAction)
{
  vTrace("bRule_r8\n"_Stz);
  
  s4K sCmd;
  pTknVec->vToTknString(sCmd);
  
  vTrace("Cmd : %s\n"_Stz,sCmd.szStz());
  
  i8 nTkns = pTknVec->nItems();
  if (nTkns < 2)
    return false;
    
  return true;
}

RulesFun __Rule_r8
(
  "r8"_Stz,  // name of the function
  "r8"_Stz,  // command first word
  nullptr,   // command second word
  bRule_r8,  // function to call
  0,0,       // spare
  "rule 8"_Stz
);

/*----------------------------------------------------------------------------*/
/* rule 9                                                                     */
/*----------------------------------------------------------------------------*/
bool STORE::bRule_r9(Optimiser* pOptimiser, TokenVec* pTknVec, i8& riAction)
{
  vTrace("bRule_r9\n"_Stz);
  
  s4K sCmd;
  pTknVec->vToTknString(sCmd);
  
  vTrace("Cmd : %s\n"_Stz,sCmd.szStz());
  
  i8 nTkns = pTknVec->nItems();
  if (nTkns < 2)
    return false;
    
  return true;
}

RulesFun __Rule_r9
(
  "r9"_Stz,  // name of the function
  "r9"_Stz,  // command first word
  nullptr,   // command second word
  bRule_r9,  // function to call
  0,0,       // spare
  "rule 9"_Stz
);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

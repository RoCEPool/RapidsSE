/*============================================================================*/
/* Steps_Operand.cpp -- Operand step group                                    */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Operand(void)
{
  vAddStep("Operand[].decode"_Stz,   &Step::vOperandDecode);
}

/*----------------------------------------------------------------------------*/
/* Operand[].decode -- decode operand into its operand kind                   */
/*----------------------------------------------------------------------------*/
void Step::vOperandDecode(void)
{
  ENTRY("vOperandDecode"_Stz);
  
  eZK eOperand = ZK_NONE;
  
  eCN eClass_op = m_pRegSet->m_pOperand->eClass();
  switch (eClass_op) {
      
    case CN_CRWBUFFER: eOperand = ZK_CRWBUFFER; break;
    case CN_CSVBUFFER: eOperand = ZK_CSVBUFFER; break;
    case CN_ENDMARKER: eOperand = ZK_ENDMARKER; break;
    case CN_ENTBUFFER: eOperand = ZK_ENTBUFFER; break;
    case CN_IRWBUFFER: eOperand = ZK_IRWBUFFER; break;
    case CN_SRTBUFFER: eOperand = ZK_SRTBUFFER; break;
    case CN_STREAMDEF: eOperand = ZK_STREAMDEF; break;
      
    default:
      RASSERT(0);
      break;
  }

  m_pDataStack->vPushI8(eOperand);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

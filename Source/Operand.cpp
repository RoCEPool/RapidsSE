/*============================================================================*/
/* Operand.cpp -- Operand Buffer                                              */
/*============================================================================*/

#include "Operands.h"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Operand* Operands::_pOperand(void* aAddress)
{
  Operand* pOperand = reinterpret_cast<Operand*>(aAddress);
  
  return pOperand;
}

/*----------------------------------------------------------------------------*/
/* is given class a valid operand?                                            */
/*----------------------------------------------------------------------------*/
bool Operands::_bIsValidOperand(eCN eClass)
{
  switch(eClass) {
    case CN_EOSMARKER:
    case CN_ERRORMARKER:
    case CN_CANCELMARKER:
    case CN_COMMENCEMARKER:
    case CN_COMMENCEDMARKER:
    case CN_COMPLETEDMARKER:
    case CN_CRWBUFFER:
    case CN_CSVBUFFER:
    case CN_IRWBUFFER:
    case CN_PULSEMARKER:
    case CN_STARTMARKER:
    case CN_STARTEDMARKER:
    case CN_STREAMDEF:
    case CN_STOPMARKER:
    case CN_STOPPEDMARKER:
      return true;
    default:
      return false;
  }
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Operand::Operand(eCN eClassNo)
: AlignedObject(eClassNo,sizeof(Operand))
{
  _precondition_(_ADDRESS_,_MAGIC_,eClassNo);
    
  MemLib::vZero(m__Padding,sizeof(m__Padding),eClassNo);

  _postcondition_(_ADDRESS_,_MAGIC_,eClassNo);  
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Operand::~Operand(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

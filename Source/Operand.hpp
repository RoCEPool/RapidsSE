/*============================================================================*/
/* Operand.hpp -- Operand                                                     */
/*============================================================================*/
#pragma once

namespace Operands
{
  class Operand : public AlignedObject,
                  public ListItem_t<Operand>
  {
  private:
    eTP        m_eTailProcessing;     // pipeline tail processing
  
    SingleWord m__PadToCacheLine[4];  // padding to cache line

  public:
    Operand(eCN eClass);
    ~Operand(void) noexcept(false);
    
    bool bIsInfoSet(void);
    bool bIsMarker(void);
    bool bIsStreamDef(void);
    void vTailProcessing(eTP eTailProcessing);
    eTP  eTailProcessing(void);
  };
  
  Operand* _pOperand(void* aAddr);
  bool _bIsValidOperand(eCN eClass);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

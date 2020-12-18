/*============================================================================*/
/* Operand.hpp -- Operand                                                     */
/*============================================================================*/
#pragma once

#include "ePT.h"
#include "eZK.h"

namespace Chunks
{
  class Operand : public AlignedObject,
                  public ListItem_t<Operand>
  {
  private:
    ePT        m_eTailProcessing;     // pipeline tail processing
  
    SingleWord m__PadToCacheLine[4];  // padding to cache line

  public:
    Operand(eCN eClass);
    ~Operand(void) noexcept(false);

    bool bIsBuffer(void);
    bool bIsInfoSet(void);
    bool bIsMarker(void);
    bool bIsProfile(void);
    bool bIsStreamDef(void);
    eZK  eZkKind(void);
    void vTailProcessing(ePT eTailProcessing);
    ePT  eTailProcessing(void);
  };
  
  Operand* _pOperand(void* aAddr);
  bool _bIsValidOperand(eCN eClass);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

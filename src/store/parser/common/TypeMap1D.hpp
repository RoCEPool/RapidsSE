/*============================================================================*/
/* TypeMap1D.hpp - A class for mapping input to output types for expressions  */
/*============================================================================*/
#pragma once

#include "TypeId.hpp"
#include "eTM.hpp"

namespace Parser
{
  class TypeMap1D : Object
  {
    private:
    i8  m_iSize;
    i1* m_aiTypeArray;

    // must be last
    Footer m_xFooter;

    public:
    TypeMap1D();
    ~TypeMap1D();

    // Add a mapping of one input operand type to one output type
    TypeMap1D* pAddMapping(TypeId xArg, TypeId xResult);
    TypeMap1D* pAddMapping(eTM eMapper, TypeId xResult);

    // Add a mapping of one input type to the same type as the output type
    TypeMap1D* pAddSelfMapping(TypeId xArg);

    // Lookup a mapping of one input operand type to one output type
    TypeId     xLookup(TypeId xArg);

    // Print the mappings that are set.
    void vPrintMap();

    Footer *aFooter(void);
  };
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

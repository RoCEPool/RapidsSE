/*============================================================================*/
/* TypeMap2D.hpp - A class for mapping input to output types for expressions  */
/*============================================================================*/
#pragma once

#include "TypeId.hpp"
#include "eTM.hpp"

namespace Parser
{
  class TypeMap2D : Object
  {
    private:
    i8  m_iDim;
    i1* m_aiTypeArray;

    // must be last
    Footer m_xFooter;

    public:
    TypeMap2D();
    ~TypeMap2D();

    enum ResultToUse {
      SAME_AS_ARG1,
      SAME_AS_ARG2
    };

    // Add a mapping of two input operand types to one output type
    TypeMap2D* pAddMapping(TypeId xArg1, TypeId xArg2, TypeId xResult);
    TypeMap2D* pAddMapping(TypeId xArg1, eTM eMapper,  TypeId xResult);
    TypeMap2D* pAddMapping(eTM eMapper,  TypeId xArg2, TypeId xResult);

    TypeMap2D* pAddMapping(TypeId xArg1, eTM eMapper,  ResultToUse eResult);
    TypeMap2D* pAddMapping(eTM eMapper,  TypeId xArg2, ResultToUse eResult);


    // Add a mapping where the input types are the same as the output type.
    TypeMap2D* pAddSelfMapping(TypeId xArg);

    // Lookup a mapping of two input operand types to one output type
    TypeId     xLookup(TypeId xArg1, TypeId xArg2);

    // Print the mappings that are set.
    void vPrintMap();

    Footer *aFooter(void);
  };
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

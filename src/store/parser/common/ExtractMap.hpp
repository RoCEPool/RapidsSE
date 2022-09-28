/*============================================================================*/
/* ExtractMap.hpp - A class for mapping input to output types for EXTRACT expr*/
/*============================================================================*/
#pragma once

#include "TypeId.hpp"
#include "eTM.hpp"
#include "ExtractField.hpp"

namespace Parser
{
  class ExtractMap : Object
  {
    private:
    i8  m_iSize;
    i1* m_aiTypeArray;

    // must be last
    Footer m_xFooter;

    public:
    ExtractMap();
    ~ExtractMap();

    // Add a mapping of two input operand types to one output type
    ExtractMap* pAddMapping(ExtractField::eEF eField, TypeId xArg, TypeId xResult);
    ExtractMap* pAddMapping(ExtractField::eEF eField, eTM eMapper, TypeId xResult);

    // Lookup a mapping of two input operand types to one output type
    TypeId     xLookup(ExtractField::eEF eField, TypeId xArg2);

    Footer *aFooter(void);
  };
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

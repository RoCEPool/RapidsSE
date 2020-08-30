/*============================================================================*/
/* Facts.h -- Facts namespace                                                 */
/*============================================================================*/
#pragma once

#include "LogicalPlans.h"

//------------------------------------------------------------------------------

namespace Facts
{
  void vCheckAssumptions(void);
}

//------------------------------------------------------------------------------

static const i8 C_MAX_FACETS     = 64;
static const i8 C_MAX_PROPERTIES = 64;

#include "Property.hpp"
typedef VectorNL_t<CN_PROPERTYVEC, Property, C_MAX_PROPERTIES> PropertyVec;

#include "Facet.hpp"
typedef VectorNL_t<CN_PROPERTYVEC, Facet, C_MAX_FACETS> FacetVec;

#include "Concept.hpp"

#include "Fact.hpp"

#include "Kbase.hpp"

#include "Question.hpp"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

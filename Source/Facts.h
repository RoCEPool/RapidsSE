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

static const i8 C_MAX_FACETS   = 64;
static const i8 C_MAX_FEATURES = 64;

#include "Feature.hpp"
typedef VectorNL_t<CN_FEATUREVEC, Feature, C_MAX_FEATURES> FeatureVec;

#include "Facet.hpp"
typedef VectorNL_t<CN_FACETVEC, Facet, C_MAX_FACETS> FacetVec;

#include "Concept.hpp"

#include "Fact.hpp"

#include "Kbase.hpp"

#include "Question.hpp"

#include "Transform.hpp"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* PlanConstants.hpp - Common constants for the plan graph                    */
/*============================================================================*/
#pragma once

namespace Parser
{
  // Separators
  static const char C_CHILD_SEPARATOR     = '/';
  static const char C_PROPERTY_SEPARATOR  = ':';
  static       Stz* C_CHILD_SEPARATOR_STZ     = "/"_Stz;
  static       Stz* C_PROPERTY_SEPARATOR_STZ  = ":"_Stz;

  // Names / identifiers
  static const Stz* C_ROOT_NAME         = "__root__"_Stz;
  static const Stz* C_PRIMITIVES_NAME   = "primitives"_Stz;
  static const Stz* C_DEFINITIONS_NAME  = "definitions"_Stz;
  static const Stz* C_PLAN_NAME         = "plan"_Stz;
  static const Stz* C_METADATA_NAME     = "metadata"_Stz;
  static const Stz* C_STATISTICS_NAME   = "statistics"_Stz;

  // Paths of nodes
  static const Stz* C_ROOT_PATH         = "/__root__"_Stz;
  static const Stz* C_PRIMITIVES_PATH   = "/__root__/primitives"_Stz;
  static const Stz* C_DEFINITIONS_PATH  = "/__root__/definitions"_Stz;
  static const Stz* C_PLAN_PATH         = "/__root__/plan"_Stz;
  static const Stz* C_METADATA_PATH     = "/__root__/metadata"_Stz;
  static const Stz* C_STATISTICS_PATH   = "/__root__/statistics"_Stz;

  // Common type definition paths
  static const Stz* C_VALUE_OBJ_DEFN = "/__root__/definitions/object/valueObject"_Stz;
  static const Stz* C_STRING_OBJ_DEFN = "/__root__/definitions/object/valueObject/stringObject"_Stz;
  static const Stz* C_STRING_OBJ_DEFN_NONNULL = "/__root__/definitions/object/valueObject/stringObject/nonNullableStringObject"_Stz;

  static const Stz* C_DATASOURCE_DEFN = "/__root__/definitions/object/planObject/sqlOperator/dataSource"_Stz;

  static const Stz* C_ROOT_DEFN_PATH  = "/__root__/primitives/root"_Stz;
  static const Stz* C_TRUNK_DEFN_PATH = "/__root__/primitives/trunk"_Stz;

  static const Stz* C_ROOT_CHILDREN_NAMES[] = { C_PRIMITIVES_NAME,
                                                C_DEFINITIONS_NAME,
                                                C_PLAN_NAME,
                                                C_METADATA_NAME,
                                                C_STATISTICS_NAME,
                                                nullptr };

  // Constraints
  static const Stz* C_CONSTRAINTS_CONTAINER_NAME  = "__constraints"_Stz;
  static const Stz* C_CONSTRAINTS_CONTAINER_PATH  = "/__root__/primitives/constraints"_Stz;

  static const Stz* C_CONSTRAINTS_CHILDTYPE_PATH  = "/__root__/primitives/constraints/childType"_Stz;
  static const Stz* C_CONSTRAINTS_VALUETYPE_PATH  = "/__root__/primitives/constraints/valueType"_Stz;
  static const Stz* C_CONSTRAINTS_CHILDCOUNT_PATH = "/__root__/primitives/constraints/childCount"_Stz;
  static const Stz* C_CONSTRAINTS_MINCHILDCOUNT_PATH = "/__root__/primitives/constraints/minChildCount"_Stz;
  static const Stz* C_CONSTRAINTS_MAXCHILDCOUNT_PATH = "/__root__/primitives/constraints/maxChildCount"_Stz;

  static const Stz* C_CONSTRAINTS_NOCHILDREN_PATH     = "/__root__/primitives/constraints/noChildren"_Stz;
  static const Stz* C_CONSTRAINTS_ALWAYSCHILDREN_PATH = "/__root__/primitives/constraints/alwaysChildren"_Stz;
  static const Stz* C_CONSTRAINTS_NULLABLE_PATH       = "/__root__/primitives/constraints/nullable"_Stz;
  static const Stz* C_CONSTRAINTS_NONNULLABLE_PATH    = "/__root__/primitives/constraints/nonNullable"_Stz;
  static const Stz* C_CONSTRAINTS_OPTIONAL_PATH       = "/__root__/primitives/constraints/optional"_Stz;
  static const Stz* C_CONSTRAINTS_MANDATORY_PATH      = "/__root__/primitives/constraints/mandatory"_Stz;

  // Primitives
  static const Stz* C_SCALAR_PRIMITIVE    = "/__root__/primitives/types/scalarType"_Stz;
  static const Stz* C_STRING_PRIMITIVE    = "/__root__/primitives/types/scalarType/string"_Stz;
  static const Stz* C_INTEGER_PRIMITIVE   = "/__root__/primitives/types/scalarType/integer"_Stz;
  static const Stz* C_FLOAT_PRIMITIVE     = "/__root__/primitives/types/scalarType/float"_Stz;
  static const Stz* C_DECIMAL_PRIMITIVE   = "/__root__/primitives/types/scalarType/decimal"_Stz;
  static const Stz* C_BOOLEAN_PRIMITIVE   = "/__root__/primitives/types/scalarType/boolean"_Stz;
  static const Stz* C_DATE_PRIMITIVE      = "/__root__/primitives/types/scalarType/date"_Stz;
  static const Stz* C_TIME_PRIMITIVE      = "/__root__/primitives/types/scalarType/time"_Stz;
  static const Stz* C_TIMESTAMP_PRIMITIVE = "/__root__/primitives/types/scalarType/timestamp"_Stz;
  static const Stz* C_INTERVAL_PRIMITIVE  = "/__root__/primitives/types/scalarType/interval"_Stz;
  static const Stz* C_UNKNOWN_PRIMITIVE   = "/__root__/primitives/types/scalarType/unknown"_Stz;

  // Misc
  static       Stz* C_SUBQUERY_NAME_PREFIX = "__subquery"_Stz;
}
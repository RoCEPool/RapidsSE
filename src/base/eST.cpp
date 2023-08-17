/*============================================================================*/
/* eST.cpp - sketch token                                                     */
/*============================================================================*/
#include "BASE.h"
#include "eST.h"

Stz* aST[ST_ENUMS]
{
  /* */ "SF_NONE"_Stz,

  /* */ "ST_AGGREGATES"_Stz       , // AGGREGATES
  /* */ "ST_AND"_Stz              , // AND
  /* */ "ST_AS"_Stz               , // AS
  /* */ "ST_ASTERISK_SIGN"_Stz    , // *
  /* */ "ST_BETWEEN"_Stz          , // BETWEEN
  /* */ "ST_BOUND"_Stz            , // BOUND
  /* */ "ST_CASE"_Stz             , // CASE
  /* */ "ST_CAST"_Stz             , // CAST
  /* */ "ST_CASTTYPE"_Stz         , // CASTTYPE
  /* */ "ST_CLOSEBRACE_SIGN"_Stz  , // }
  /* */ "ST_CLOSEPARENTH_SIGN"_Stz, // )
  /* */ "ST_COMMA_SIGN"_Stz       , // ,
  /* */ "ST_COUNT"_Stz            , // COUNT
  /* */ "ST_COUNTALL"_Stz         , // COUNTALL
  /* */ "ST_COUNTDISTINCT"_Stz    , // COUNTDISTINCT
  /* */ "ST_DATE"_Stz             , // DATE
  /* */ "ST_DESC"_Stz             , // DESC
  /* */ "ST_DIVIDE_SIGN"_Stz      , // /
  /* */ "ST_ELSE"_Stz             , // ELSE
  /* */ "ST_ENDIF"_Stz            , // ENDIF
  /* */ "ST_EQUAL_SIGN"_Stz       , // ==
  /* */ "ST_EXTRACT_DAY"_Stz      , // EXTRACT_DAY
  /* */ "ST_EXTRACT_HOUR"_Stz     , // EXTRACT_HOUR
  /* */ "ST_EXTRACT_MINUTE"_Stz   , // EXTRACT_MINUTE
  /* */ "ST_EXTRACT_MONTH"_Stz    , // EXTRACT_MONTH
  /* */ "ST_EXTRACT_SECOND"_Stz   , // EXTRACT_SECOND
  /* */ "ST_EXTRACT_YEAR"_Stz     , // EXTRACT_YEAR
  /* */ "ST_FILTER"_Stz           , // FILTER
  /* */ "ST_GREATEREQUAL_SIGN"_Stz, // >=
  /* */ "ST_GREATER_SIGN"_Stz     , // >
  /* */ "ST_GROUPBY"_Stz          , // GROUPBY
  /* */ "ST_IN"_Stz               , // IN
  /* */ "ST_INTERVAL_DT"_Stz      , // INTERVAL_DT
  /* */ "ST_INTERVAL_YM"_Stz      , // INTERVAL_YM
  /* */ "ST_ISNULL"_Stz           , // ISNULL
  /* */ "ST_LBOUND"_Stz           , // LBOUND
  /* */ "ST_LEFT_OUTER"_Stz       , // LEFT_OUTER
  /* */ "ST_LESSEQUAL_SIGN"_Stz   , // <=
  /* */ "ST_LESS_SIGN"_Stz        , // <
  /* */ "ST_LIKE2"_Stz            , // LIKE2
  /* */ "ST_LIKE3"_Stz            , // LIKE3
  /* */ "ST_MATCHLEADING"_Stz     , // MATCHLEADING
  /* */ "ST_MATCHMIDDLE"_Stz      , // MATCHMIDDLE
  /* */ "ST_MATCHMIDDLE1"_Stz     , // MATCHMIDDLE1
  /* */ "ST_MATCHMIDDLE2"_Stz     , // MATCHMIDDLE2
  /* */ "ST_MATCHTRAILING"_Stz    , // MATCHTRAILING
  /* */ "ST_MAX"_Stz              , // MAX
  /* */ "ST_MIN"_Stz              , // MIN
  /* */ "ST_MINUS_SIGN"_Stz       , // -
  /* */ "ST_NEGATE"_Stz           , // NEGATE
  /* */ "ST_NOT"_Stz              , // NOT
  /* */ "ST_NOTEQUAL_SIGN"_Stz    , // !=
  /* */ "ST_NOTFALSE_SIGN"_Stz    , // !F
  /* */ "ST_NOTTRUE_SIGN"_Stz     , // !T
  /* */ "ST_NULL"_Stz             , // NULL
  /* */ "ST_OR"_Stz               , // OR
  /* */ "ST_ORDGROUPBY"_Stz       , // ORDGROUPBY
  /* */ "ST_PERCENT_SIGN"_Stz     , // %
  /* */ "ST_PLUS_SIGN"_Stz        , // +
  /* */ "ST_SORT"_Stz             , // SORT
  /* */ "ST_SUBSTRING2"_Stz       , // SUBSTRING2
  /* */ "ST_SUBSTRING3"_Stz       , // SUBSTRING3
  /* */ "ST_SUM"_Stz              , // SUM
  /* */ "ST_THEN"_Stz             , // THEN
  /* */ "ST_TIME"_Stz             , // TIME
  /* */ "ST_TIMESTAMP"_Stz        , // TIMESTAMP
  /* */ "ST_TOP"_Stz              , // TOP
  /* */ "ST_TYPE"_Stz             , // TYPE
  /* */ "ST_UBOUND"_Stz           , // UBOUND
  /* */ "ST_WHEN"_Stz             , // WHEN
  /* */ "ST_XOR"_Stz              , // XOR

//  /* */ "SF_ENUMS"_Stz
};

Stz* aST_string[ST_ENUMS]
{
  /* */ "SF_NONE"_Stz,

  /* */ "AGGREGATES"_Stz    , // AGGREGATES
  /* */ "AND"_Stz           , // AND
  /* */ "AS"_Stz            , // AS
  /* */ "*"_Stz             , // *
  /* */ "BETWEEN"_Stz       , // BETWEEN
  /* */ "BOUND"_Stz         , // BOUND
  /* */ "CASE"_Stz          , // CASE
  /* */ "CAST"_Stz          , // CAST
  /* */ "CASTTYPE"_Stz      , // CASTTYPE
  /* */ "}"_Stz             , // }
  /* */ ")"_Stz             , // )
  /* */ "_Stz,"_Stz         , // ,
  /* */ "COUNT"_Stz         , // COUNT
  /* */ "COUNTALL"_Stz      , // COUNTALL
  /* */ "COUNTDISTINCT"_Stz , // COUNTDISTINCT
  /* */ "DATE"_Stz          , // DATE
  /* */ "DESC"_Stz          , // DESC
  /* */ "/"_Stz             , // /
  /* */ "ELSE"_Stz          , // ELSE
  /* */ "ENDIF"_Stz         , // ENDIF
  /* */ "=="_Stz            , // ==
  /* */ "EXTRACT_DAY"_Stz   , // EXTRACT_DAY
  /* */ "EXTRACT_HOUR"_Stz  , // EXTRACT_HOUR
  /* */ "EXTRACT_MINUTE"_Stz, // EXTRACT_MINUTE
  /* */ "EXTRACT_MONTH"_Stz , // EXTRACT_MONTH
  /* */ "EXTRACT_SECOND"_Stz, // EXTRACT_SECOND
  /* */ "EXTRACT_YEAR"_Stz  , // EXTRACT_YEAR
  /* */ "FILTER"_Stz        , // FILTER
  /* */ ">="_Stz            , // >=
  /* */ ">"_Stz             , // >
  /* */ "GROUPBY"_Stz       , // GROUPBY
  /* */ "IN"_Stz            , // IN
  /* */ "INTERVAL_DT"_Stz   , // INTERVAL_DT
  /* */ "INTERVAL_YM"_Stz   , // INTERVAL_YM
  /* */ "ISNULL"_Stz        , // ISNULL
  /* */ "LBOUND"_Stz        , // LBOUND
  /* */ "LEFT_OUTER"_Stz    , // LEFT_OUTER
  /* */ "<="_Stz            , // <=
  /* */ "<"_Stz             , // <
  /* */ "LIKE2"_Stz         , // LIKE2
  /* */ "LIKE3"_Stz         , // LIKE3
  /* */ "MATCHLEADING"_Stz  , // MATCHLEADING
  /* */ "MATCHMIDDLE"_Stz   , // MATCHMIDDLE
  /* */ "MATCHMIDDLE1"_Stz  , // MATCHMIDDLE1
  /* */ "MATCHMIDDLE2"_Stz  , // MATCHMIDDLE2
  /* */ "MATCHTRAILING"_Stz , // MATCHTRAILING
  /* */ "MAX"_Stz           , // MAX
  /* */ "MIN"_Stz           , // MIN
  /* */ "-"_Stz             , // -
  /* */ "NEGATE"_Stz        , // NEGATE
  /* */ "NOT"_Stz           , // NOT
  /* */ "!="_Stz            , // !=
  /* */ "!F"_Stz            , // !F
  /* */ "!T"_Stz            , // !T
  /* */ "NULL"_Stz          , // NULL
  /* */ "OR"_Stz            , // OR
  /* */ "ORDGROUPBY"_Stz    , // ORDGROUPBY
  /* */ "%"_Stz             , // %
  /* */ "+"_Stz             , // +
  /* */ "SORT"_Stz          , // SORT
  /* */ "SUBSTRING2"_Stz    , // SUBSTRING2
  /* */ "SUBSTRING3"_Stz    , // SUBSTRING3
  /* */ "SUM"_Stz           , // SUM
  /* */ "THEN"_Stz          , // THEN
  /* */ "TIME"_Stz          , // TIME
  /* */ "TIMESTAMP"_Stz     , // TIMESTAMP
  /* */ "TOP"_Stz           , // TOP
  /* */ "TYPE"_Stz          , // TYPE
  /* */ "UBOUND"_Stz        , // UBOUND
  /* */ "WHEN"_Stz          , // WHEN
  /* */ "XOR"_Stz           , // XOR

//    /* */ "SF_ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
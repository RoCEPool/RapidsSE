/*============================================================================*/
/* eST.h - sketch token                                                       */
/*============================================================================*/
#pragma once

enum eST // sketch token
{
  /* */ ST_NONE = 0,

  /* */ ST_AGGREGATES       , // AGGREGATES
  /* */ ST_AND              , // AND
  /* */ ST_AS               , // AS
  /* */ ST_ASTERISK_SIGN    , // *
  /* */ ST_BETWEEN          , // BETWEEN
  /* */ ST_BOUND            , // BOUND
  /* */ ST_CASE             , // CASE
  /* */ ST_CAST             , // CAST
  /* */ ST_CASTTYPE         , // CASTTYPE
  /* */ ST_CLOSEBRACE_SIGN  , // }
  /* */ ST_CLOSEPARENTH_SIGN, // )
  /* */ ST_COMMA_SIGN       , // ,
  /* */ ST_COUNT            , // COUNT
  /* */ ST_COUNTALL         , // COUNTALL
  /* */ ST_COUNTDISTINCT    , // COUNTDISTINCT
  /* */ ST_DATE             , // DATE
  /* */ ST_DESC             , // DESC
  /* */ ST_DIVIDE_SIGN      , // /
  /* */ ST_ELSE             , // ELSE
  /* */ ST_ENDIF            , // ENDIF
  /* */ ST_EQUAL_SIGN       , // ==
  /* */ ST_EXTRACT_DAY      , // EXTRACT_DAY
  /* */ ST_EXTRACT_HOUR     , // EXTRACT_HOUR
  /* */ ST_EXTRACT_MINUTE   , // EXTRACT_MINUTE
  /* */ ST_EXTRACT_MONTH    , // EXTRACT_MONTH
  /* */ ST_EXTRACT_SECOND   , // EXTRACT_SECOND
  /* */ ST_EXTRACT_YEAR     , // EXTRACT_YEAR
  /* */ ST_FILTER           , // FILTER
  /* */ ST_GREATEREQUAL_SIGN, // >=
  /* */ ST_GREATER_SIGN     , // >
  /* */ ST_GROUPBY          , // GROUPBY
  /* */ ST_IN               , // IN
  /* */ ST_INTERVAL_DT      , // INTERVAL_DT
  /* */ ST_INTERVAL_YM      , // INTERVAL_YM
  /* */ ST_ISNULL           , // ISNULL
  /* */ ST_LBOUND           , // LBOUND
  /* */ ST_LEFT_OUTER       , // LEFT_OUTER
  /* */ ST_LESSEQUAL_SIGN   , // <=
  /* */ ST_LESS_SIGN        , // <
  /* */ ST_LIKE2            , // LIKE2
  /* */ ST_LIKE3            , // LIKE3
  /* */ ST_MATCHLEADING     , // MATCHLEADING
  /* */ ST_MATCHMIDDLE      , // MATCHMIDDLE
  /* */ ST_MATCHMIDDLE1     , // MATCHMIDDLE1
  /* */ ST_MATCHMIDDLE2     , // MATCHMIDDLE2
  /* */ ST_MATCHTRAILING    , // MATCHTRAILING
  /* */ ST_MAX              , // MAX
  /* */ ST_MIN              , // MIN
  /* */ ST_MINUS_SIGN       , // -
  /* */ ST_NEGATE           , // NEGATE
  /* */ ST_NOT              , // NOT
  /* */ ST_NOTEQUAL_SIGN    , // !=
  /* */ ST_NOTFALSE_SIGN    , // !F
  /* */ ST_NOTTRUE_SIGN     , // !T
  /* */ ST_NULL             , // NULL
  /* */ ST_OR               , // OR
  /* */ ST_ORDGROUPBY       , // ORDGROUPBY
  /* */ ST_PERCENT_SIGN     , // %
  /* */ ST_PLUS_SIGN        , // +
  /* */ ST_SORT             , // SORT
  /* */ ST_SUBSTRING2       , // SUBSTRING2
  /* */ ST_SUBSTRING3       , // SUBSTRING3
  /* */ ST_SUM              , // SUM
  /* */ ST_THEN             , // THEN
  /* */ ST_TIME             , // TIME
  /* */ ST_TIMESTAMP        , // TIMESTAMP
  /* */ ST_TOP              , // TOP
  /* */ ST_TYPE             , // TYPE
  /* */ ST_UBOUND           , // UBOUND
  /* */ ST_WHEN             , // WHEN
  /* */ ST_XOR              , // XOR

  /* */ ST_ENUMS
};

extern Stz* aST[ST_ENUMS];
extern Stz* aST_string[ST_ENUMS];

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/


// Generated from /Users/craig/repos/rapidsse/thirdparty/antlr/sql.g4 by ANTLR 4.8

#pragma once


#include "antlr4-runtime.h"
#include "sqlVisitor.h"


namespace Parser {

/**
 * This class provides an empty implementation of sqlVisitor, which can be
 * extended to create a visitor which only needs to handle a subset of the available methods.
 */
class  sqlBaseVisitor : public sqlVisitor {
public:

  virtual antlrcpp::Any visitStatements(sqlParser::StatementsContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitStatement(sqlParser::StatementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitStatementChoice(sqlParser::StatementChoiceContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitTags(sqlParser::TagsContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitKeyValueTag(sqlParser::KeyValueTagContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitKeyOnlyTag(sqlParser::KeyOnlyTagContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitTagName(sqlParser::TagNameContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitQueryCmd(sqlParser::QueryCmdContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitQuery(sqlParser::QueryContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitQueryHint(sqlParser::QueryHintContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitHint(sqlParser::HintContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitSelectQuery(sqlParser::SelectQueryContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitSelectExpr(sqlParser::SelectExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitSetQuery(sqlParser::SetQueryContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitSetExpr(sqlParser::SetExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitCorrClause(sqlParser::CorrClauseContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitWithQuery(sqlParser::WithQueryContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitSubquery(sqlParser::SubqueryContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitNaturalJoin(sqlParser::NaturalJoinContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitJoinUsing(sqlParser::JoinUsingContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitJoinOn(sqlParser::JoinOnContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitFetch(sqlParser::FetchContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitValuesSubquery(sqlParser::ValuesSubqueryContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitCrossJoin(sqlParser::CrossJoinContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitTableReference(sqlParser::TableReferenceContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitJoinType(sqlParser::JoinTypeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitExtendedJoinType(sqlParser::ExtendedJoinTypeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitValuesExpr(sqlParser::ValuesExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitBpaCmd(sqlParser::BpaCmdContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitCompactIdxCmd(sqlParser::CompactIdxCmdContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitCountCmd(sqlParser::CountCmdContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitCreateCmd(sqlParser::CreateCmdContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitCreateTableCmd(sqlParser::CreateTableCmdContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitCreateIndexCmd(sqlParser::CreateIndexCmdContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitDeleteCmd(sqlParser::DeleteCmdContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitDloadCmd(sqlParser::DloadCmdContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitDropCmd(sqlParser::DropCmdContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitDropTableCmd(sqlParser::DropTableCmdContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitDropIndexCmd(sqlParser::DropIndexCmdContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitEmptyCmd(sqlParser::EmptyCmdContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitExplainCmd(sqlParser::ExplainCmdContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitExplainOptions(sqlParser::ExplainOptionsContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitExplainStageList(sqlParser::ExplainStageListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitExplainRange(sqlParser::ExplainRangeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitExplainRangeChoice(sqlParser::ExplainRangeChoiceContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitExplainFormat(sqlParser::ExplainFormatContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitFileCmd(sqlParser::FileCmdContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitInsertCmd(sqlParser::InsertCmdContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitLoadCmd(sqlParser::LoadCmdContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitLoadReplicatedCmd(sqlParser::LoadReplicatedCmdContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitLoadOption(sqlParser::LoadOptionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitLoadOptionKeys(sqlParser::LoadOptionKeysContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitLogCmd(sqlParser::LogCmdContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitLscolCmd(sqlParser::LscolCmdContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitLsidxCmd(sqlParser::LsidxCmdContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitLsidxcolCmd(sqlParser::LsidxcolCmdContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitLstblCmd(sqlParser::LstblCmdContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitMemoryleaksCmd(sqlParser::MemoryleaksCmdContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitNoopCmd(sqlParser::NoopCmdContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitPipelineCmd(sqlParser::PipelineCmdContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitPipelineStage(sqlParser::PipelineStageContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitPipelineOpSeq(sqlParser::PipelineOpSeqContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitPipelineOperator(sqlParser::PipelineOperatorContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitPipelineOpParam(sqlParser::PipelineOpParamContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitPipelineTypeNames(sqlParser::PipelineTypeNamesContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitSleepCmd(sqlParser::SleepCmdContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitSleepUnits(sqlParser::SleepUnitsContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitSpaceCmd(sqlParser::SpaceCmdContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitStatusCmd(sqlParser::StatusCmdContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitSzidxCmd(sqlParser::SzidxCmdContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitSztblCmd(sqlParser::SztblCmdContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitTruncateCmd(sqlParser::TruncateCmdContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitTrackmemoryCmd(sqlParser::TrackmemoryCmdContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitUpdateCmd(sqlParser::UpdateCmdContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitUnloadCmd(sqlParser::UnloadCmdContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitTableDefinition(sqlParser::TableDefinitionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitTablePartitioning(sqlParser::TablePartitioningContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitColumnDefinition(sqlParser::ColumnDefinitionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitColumnNameList(sqlParser::ColumnNameListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitIndexDefinition(sqlParser::IndexDefinitionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitIndexMethod(sqlParser::IndexMethodContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitDataType(sqlParser::DataTypeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitSqlDataType(sqlParser::SqlDataTypeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitIntervalType(sqlParser::IntervalTypeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitYearMonthIntervalType(sqlParser::YearMonthIntervalTypeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitDayTimeIntervalType(sqlParser::DayTimeIntervalTypeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitFixedLenChar(sqlParser::FixedLenCharContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitVaryingLenChar(sqlParser::VaryingLenCharContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitAsSubquery(sqlParser::AsSubqueryContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitSelectOrValues(sqlParser::SelectOrValuesContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitSelectList(sqlParser::SelectListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitSelectListItem(sqlParser::SelectListItemContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitGroupByList(sqlParser::GroupByListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitGroupByListItem(sqlParser::GroupByListItemContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitOrderByList(sqlParser::OrderByListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitOrderByListItem(sqlParser::OrderByListItemContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitJoinUsingList(sqlParser::JoinUsingListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitJoinUsingListItem(sqlParser::JoinUsingListItemContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitCorrList(sqlParser::CorrListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitCorrListItem(sqlParser::CorrListItemContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitColAliases(sqlParser::ColAliasesContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitColAliasList(sqlParser::ColAliasListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitColAliasListItem(sqlParser::ColAliasListItemContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitPropertyList(sqlParser::PropertyListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitProperty(sqlParser::PropertyContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitValueList(sqlParser::ValueListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitPredicate(sqlParser::PredicateContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitJoinPredicate(sqlParser::JoinPredicateContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitCasePredicate(sqlParser::CasePredicateContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitCountAll(sqlParser::CountAllContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitExtractFunc(sqlParser::ExtractFuncContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitScalarFunc(sqlParser::ScalarFuncContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitUnaryOp(sqlParser::UnaryOpContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitCoalesceOp(sqlParser::CoalesceOpContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitVarRef(sqlParser::VarRefContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitPositionFunc(sqlParser::PositionFuncContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitColRef(sqlParser::ColRefContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitCaseOp(sqlParser::CaseOpContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitLiteralVal(sqlParser::LiteralValContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitOverlayFunc(sqlParser::OverlayFuncContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitIfNullOp(sqlParser::IfNullOpContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitSubqueryExistsOp(sqlParser::SubqueryExistsOpContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitIsNullOp(sqlParser::IsNullOpContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitInOp(sqlParser::InOpContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitTrimFunc(sqlParser::TrimFuncContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitLikeOp(sqlParser::LikeOpContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitTypeOfOp(sqlParser::TypeOfOpContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitNullIfOp(sqlParser::NullIfOpContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitInlineCast(sqlParser::InlineCastContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitBinaryOp(sqlParser::BinaryOpContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitBetweenOp(sqlParser::BetweenOpContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitAggFunc(sqlParser::AggFuncContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitDecodeOp(sqlParser::DecodeOpContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitConcatOp(sqlParser::ConcatOpContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitSubqueryExpr(sqlParser::SubqueryExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitIfOp(sqlParser::IfOpContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitLikeEscapeOp(sqlParser::LikeEscapeOpContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitSubStringFunc(sqlParser::SubStringFuncContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitSubqueryInOp(sqlParser::SubqueryInOpContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitLogicOp(sqlParser::LogicOpContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitCastOp(sqlParser::CastOpContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitParenthetic(sqlParser::ParentheticContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitMethodCall(sqlParser::MethodCallContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitVariableName(sqlParser::VariableNameContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitColumnRef(sqlParser::ColumnRefContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitColumnWildcard(sqlParser::ColumnWildcardContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitClassName(sqlParser::ClassNameContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitFunctionName(sqlParser::FunctionNameContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitMethodName(sqlParser::MethodNameContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitCatalogName(sqlParser::CatalogNameContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitSchemaName(sqlParser::SchemaNameContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitTableName(sqlParser::TableNameContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitTableAlias(sqlParser::TableAliasContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitColumnName(sqlParser::ColumnNameContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitColumnAlias(sqlParser::ColumnAliasContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitCollationName(sqlParser::CollationNameContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitIndexName(sqlParser::IndexNameContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitPropertyName(sqlParser::PropertyNameContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitHintName(sqlParser::HintNameContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitExprHint(sqlParser::ExprHintContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitPropertyValue(sqlParser::PropertyValueContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitHintValue(sqlParser::HintValueContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitIdentifier(sqlParser::IdentifierContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitQualifier(sqlParser::QualifierContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitPrecision(sqlParser::PrecisionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitScale(sqlParser::ScaleContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitSize(sqlParser::SizeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitLiteral(sqlParser::LiteralContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitSimpleLiteral(sqlParser::SimpleLiteralContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitBooleanLiteral(sqlParser::BooleanLiteralContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitIntegerLiteral(sqlParser::IntegerLiteralContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitSignedIntegerLiteral(sqlParser::SignedIntegerLiteralContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitDecimalLiteral(sqlParser::DecimalLiteralContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitSignedDecimalLiteral(sqlParser::SignedDecimalLiteralContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitFloatLiteral(sqlParser::FloatLiteralContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitSignedFloatLiteral(sqlParser::SignedFloatLiteralContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitDateLiteral(sqlParser::DateLiteralContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitTimeLiteral(sqlParser::TimeLiteralContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitTimestampLiteral(sqlParser::TimestampLiteralContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitStringLiteral(sqlParser::StringLiteralContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitIntervalLiteral(sqlParser::IntervalLiteralContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitYearMonthInterval(sqlParser::YearMonthIntervalContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitDayTimeInterval(sqlParser::DayTimeIntervalContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitNonReservedWord(sqlParser::NonReservedWordContext *ctx) override {
    return visitChildren(ctx);
  }


};

}  // namespace Parser

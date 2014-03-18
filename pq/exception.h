#ifndef __REDFOX_PQ_EXCEPTION_H__
#define __REDFOX_PQ_EXCEPTION_H__
// Copyright (c) 2010 - Uqee, Inc.

#include <libpq-fe.h>

#include "redfox/exception.h"

namespace Redfox {
namespace PQ {

struct Exception : virtual ::Redfox::Exception
{
    Exception(const std::string &message)
        : m_message(message)
    {}
    Exception()
    {}
    ~Exception() throw () {}

    const char *what() const throw () { return m_message.c_str(); }
private:
    std::string m_message;
};

#define DEFINE_REDFOX_PQ_EXCEPTION(Exception, Base)                             \
struct Exception : Base                                                         \
{                                                                               \
    Exception()                                                                 \
    {}                                                                          \
    Exception(const std::string &message)                                       \
        : Base(message)                                                         \
    {}                                                                          \
};

DEFINE_REDFOX_PQ_EXCEPTION(ConnectionException, Exception);

DEFINE_REDFOX_PQ_EXCEPTION(DataException, Exception);
DEFINE_REDFOX_PQ_EXCEPTION(ArraySubscriptError, DataException);
DEFINE_REDFOX_PQ_EXCEPTION(CharacterNotInRepertoireException, DataException);
DEFINE_REDFOX_PQ_EXCEPTION(OverflowException, DataException);
DEFINE_REDFOX_PQ_EXCEPTION(DivisionByZeroException, DataException);
DEFINE_REDFOX_PQ_EXCEPTION(AssignmentError, DataException);
DEFINE_REDFOX_PQ_EXCEPTION(EscapeCharacterConflictException, DataException);
DEFINE_REDFOX_PQ_EXCEPTION(InvalidArgumentException, DataException);
DEFINE_REDFOX_PQ_EXCEPTION(NullValueNotAllowedException, DataException);

DEFINE_REDFOX_PQ_EXCEPTION(IntegrityConstraintViolationException, Exception)
DEFINE_REDFOX_PQ_EXCEPTION(RestrictViolationException, IntegrityConstraintViolationException)
DEFINE_REDFOX_PQ_EXCEPTION(NotNullViolationException, IntegrityConstraintViolationException)
DEFINE_REDFOX_PQ_EXCEPTION(ForeignKeyViolationException, IntegrityConstraintViolationException)
DEFINE_REDFOX_PQ_EXCEPTION(UniqueViolationException, IntegrityConstraintViolationException)
DEFINE_REDFOX_PQ_EXCEPTION(CheckViolationException, IntegrityConstraintViolationException)

DEFINE_REDFOX_PQ_EXCEPTION(InvalidTransactionStateException, Exception);
DEFINE_REDFOX_PQ_EXCEPTION(ActiveTransactionException, InvalidTransactionStateException);
DEFINE_REDFOX_PQ_EXCEPTION(BranchTransactionAlreadyActiveException, InvalidTransactionStateException);
DEFINE_REDFOX_PQ_EXCEPTION(HeldCursorRequiresSameIsolationLevelException, InvalidTransactionStateException);
DEFINE_REDFOX_PQ_EXCEPTION(InappropriateAccessModeForBranchTransactionException, InvalidTransactionStateException);
DEFINE_REDFOX_PQ_EXCEPTION(InappropriateIsolationLevelForBranchTransactionException, InvalidTransactionStateException);
DEFINE_REDFOX_PQ_EXCEPTION(NoActiveTransactionForBranchException, InvalidTransactionStateException);
DEFINE_REDFOX_PQ_EXCEPTION(ReadOnlyTransactionException, InvalidTransactionStateException);
DEFINE_REDFOX_PQ_EXCEPTION(SchemaAndDataStatementMixingNotSupportedException, InvalidTransactionStateException);
DEFINE_REDFOX_PQ_EXCEPTION(NoActiveTransactionException, InvalidTransactionStateException);
DEFINE_REDFOX_PQ_EXCEPTION(InFailedTransactionException, InvalidTransactionStateException);

DEFINE_REDFOX_PQ_EXCEPTION(TransactionRollbackException, Exception);
DEFINE_REDFOX_PQ_EXCEPTION(TransactionIntegrityConstraintViolationException, TransactionRollbackException);
DEFINE_REDFOX_PQ_EXCEPTION(SerializationFailureException, TransactionRollbackException);
DEFINE_REDFOX_PQ_EXCEPTION(StatementCompletionUnknownException, TransactionRollbackException);
DEFINE_REDFOX_PQ_EXCEPTION(DeadlockDetectedException, TransactionRollbackException);

DEFINE_REDFOX_PQ_EXCEPTION(AccessRuleViolationException, Exception);
DEFINE_REDFOX_PQ_EXCEPTION(SyntaxError, AccessRuleViolationException);
DEFINE_REDFOX_PQ_EXCEPTION(InsufficientPrivilegeException, AccessRuleViolationException);
DEFINE_REDFOX_PQ_EXCEPTION(CannotCoerceException, AccessRuleViolationException);
DEFINE_REDFOX_PQ_EXCEPTION(GroupingError, SyntaxError);
DEFINE_REDFOX_PQ_EXCEPTION(WindowingError, SyntaxError);
DEFINE_REDFOX_PQ_EXCEPTION(InvalidRecursionException, SyntaxError);
DEFINE_REDFOX_PQ_EXCEPTION(InvalidForeignKeyException, AccessRuleViolationException);
DEFINE_REDFOX_PQ_EXCEPTION(InvalidNameException, SyntaxError);
DEFINE_REDFOX_PQ_EXCEPTION(NameTooLongException, SyntaxError);
DEFINE_REDFOX_PQ_EXCEPTION(ReservedNameException, SyntaxError);
DEFINE_REDFOX_PQ_EXCEPTION(DatatypeMismatchException, AccessRuleViolationException);
DEFINE_REDFOX_PQ_EXCEPTION(IndeterminateDatatypeException, AccessRuleViolationException);
DEFINE_REDFOX_PQ_EXCEPTION(WrongObjectTypeException, AccessRuleViolationException);
DEFINE_REDFOX_PQ_EXCEPTION(UndefinedColumnException, AccessRuleViolationException);
DEFINE_REDFOX_PQ_EXCEPTION(UndefinedFunctionException, AccessRuleViolationException);
DEFINE_REDFOX_PQ_EXCEPTION(UndefinedTableException, AccessRuleViolationException);
DEFINE_REDFOX_PQ_EXCEPTION(UndefinedParameterException, AccessRuleViolationException);
DEFINE_REDFOX_PQ_EXCEPTION(UndefinedObjectException, AccessRuleViolationException);
DEFINE_REDFOX_PQ_EXCEPTION(DuplicateColumnException, AccessRuleViolationException);
DEFINE_REDFOX_PQ_EXCEPTION(DuplicateCursorException, AccessRuleViolationException);
DEFINE_REDFOX_PQ_EXCEPTION(DuplicateDatabaseException, AccessRuleViolationException);
DEFINE_REDFOX_PQ_EXCEPTION(DuplicateFunctionException, AccessRuleViolationException);
DEFINE_REDFOX_PQ_EXCEPTION(DuplicatePreparedStatementException, AccessRuleViolationException);
DEFINE_REDFOX_PQ_EXCEPTION(DuplicateSchemaException, AccessRuleViolationException);
DEFINE_REDFOX_PQ_EXCEPTION(DuplicateTableException, AccessRuleViolationException);
DEFINE_REDFOX_PQ_EXCEPTION(DuplicateAliasException, AccessRuleViolationException);
DEFINE_REDFOX_PQ_EXCEPTION(DuplicateObjectException, AccessRuleViolationException);
DEFINE_REDFOX_PQ_EXCEPTION(AmbiguousColumnException, AccessRuleViolationException);
DEFINE_REDFOX_PQ_EXCEPTION(AmbiguousFunctionException, AccessRuleViolationException);
DEFINE_REDFOX_PQ_EXCEPTION(AmbiguousParameterException, AccessRuleViolationException);
DEFINE_REDFOX_PQ_EXCEPTION(AmbiguousAliasException, AccessRuleViolationException);
DEFINE_REDFOX_PQ_EXCEPTION(InvalidColumnReferenceException, AccessRuleViolationException);
DEFINE_REDFOX_PQ_EXCEPTION(InvalidColumnDefinitionException, AccessRuleViolationException);
DEFINE_REDFOX_PQ_EXCEPTION(InvalidCursorDefinitionException, AccessRuleViolationException);
DEFINE_REDFOX_PQ_EXCEPTION(InvalidDatabaseDefinitionException, AccessRuleViolationException);
DEFINE_REDFOX_PQ_EXCEPTION(InvalidFunctionDefinitionException, AccessRuleViolationException);
DEFINE_REDFOX_PQ_EXCEPTION(InvalidPreparedStatementDefinitionException, AccessRuleViolationException);
DEFINE_REDFOX_PQ_EXCEPTION(InvalidSchemaDefinitionException, AccessRuleViolationException);
DEFINE_REDFOX_PQ_EXCEPTION(InvalidTableDefinitionException, AccessRuleViolationException);
DEFINE_REDFOX_PQ_EXCEPTION(InvalidObjectDefinitionException, AccessRuleViolationException);

DEFINE_REDFOX_PQ_EXCEPTION(InsufficientResourcesException, Exception);
DEFINE_REDFOX_PQ_EXCEPTION(DiskFullException, InsufficientResourcesException);
DEFINE_REDFOX_PQ_EXCEPTION(OutOfMemoryException, InsufficientResourcesException);
DEFINE_REDFOX_PQ_EXCEPTION(TooManyConnectionsException, InsufficientResourcesException);

DEFINE_REDFOX_PQ_EXCEPTION(ProgramLimitExceededException, Exception);
DEFINE_REDFOX_PQ_EXCEPTION(StatementTooComplexException, ProgramLimitExceededException);
DEFINE_REDFOX_PQ_EXCEPTION(TooManyColumnsException, ProgramLimitExceededException);
DEFINE_REDFOX_PQ_EXCEPTION(TooManyArgumentsException, ProgramLimitExceededException);

DEFINE_REDFOX_PQ_EXCEPTION(OperatorInterventionException, Exception);
DEFINE_REDFOX_PQ_EXCEPTION(QueryCanceledException, OperatorInterventionException);
DEFINE_REDFOX_PQ_EXCEPTION(AdminShutdownException, OperatorInterventionException);
DEFINE_REDFOX_PQ_EXCEPTION(CrashShutdownException, OperatorInterventionException);
DEFINE_REDFOX_PQ_EXCEPTION(CannotConnectNowException, OperatorInterventionException);

DEFINE_REDFOX_PQ_EXCEPTION(SystemError, Exception);
DEFINE_REDFOX_PQ_EXCEPTION(IOError, SystemError);
DEFINE_REDFOX_PQ_EXCEPTION(UndefinedFileException, SystemError);
DEFINE_REDFOX_PQ_EXCEPTION(DuplicateFileException, SystemError);

DEFINE_REDFOX_PQ_EXCEPTION(ConfigFileError, Exception);
DEFINE_REDFOX_PQ_EXCEPTION(LockFileExistsException, ConfigFileError);

DEFINE_REDFOX_PQ_EXCEPTION(InternalError, Exception);
DEFINE_REDFOX_PQ_EXCEPTION(DataCorruptedException, InternalError);
DEFINE_REDFOX_PQ_EXCEPTION(IndexCorruptedException, InternalError);

void throwException(PGconn *conn);
void throwException(PGresult *result);

}}

#endif

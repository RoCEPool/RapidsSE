class RapidsError(Exception):
    """Base class for all application-thrown errors"""
    def __init__(self, msg: str, should_print_trace: bool):
        super(RapidsError, self).__init__(msg)
        self.should_print_trace = should_print_trace

    def __reduce__(self):
        # For pickleability of the exception in a multi-process environment
        # Reference: https://stackoverflow.com/a/36342588
        return SshError, (self.args[0], self.should_print_trace)


class TracebackError(RapidsError):
    """Base class of errors that should have their traceback printed"""
    def __init__(self, msg: str):
        super(TracebackError, self).__init__(msg, True)


class NonTracebackError(RapidsError):
    """Base class of errors that shouldn't have their traceback printed"""
    def __init__(self, msg: str):
        super(NonTracebackError, self).__init__(msg, False)


class ConfigurationError(NonTracebackError):
    """Errors that come up from an incorrect configuration file"""
    def __init__(self, msg: str):
        super(ConfigurationError, self).__init__(msg)


class DatabaseError(RapidsError):
    """General errors to do with storing/retrieving results from the database"""
    def __init__(self, msg: str):
        super(DatabaseError, self).__init__(msg, True)


class DatabaseConsistencyError(DatabaseError):
    """Represents an error with the data in the database"""
    def __init__(self, msg: str):
        super(DatabaseConsistencyError, self).__init__(msg)


class CompilationError(NonTracebackError):
    """Errors that come up from compiling RapidsSE and building the installer"""
    def __init__(self, msg: str):
        super(CompilationError, self).__init__(msg)


class InstallationError(NonTracebackError):
    """Errors that come up from installing RapidsSE"""
    def __init__(self, msg: str):
        super(InstallationError, self).__init__(msg)


class SubProcessError(TracebackError):
    """Errors that come up from running commands as local subprocesses"""
    def __init__(self, msg: str, stdout: str, stderr: str):
        super(SubProcessError, self).__init__(msg)
        self.stdout = stdout
        self.stderr = stderr

    def __reduce__(self):
        # For pickleability of the exception in a multi-process environment
        # Reference: https://stackoverflow.com/a/36342588
        return SubProcessError, (self.args[0], self.stdout, self.stderr)

    def __str__(self) -> str:
        s = super(SubProcessError, self).__str__()
        if self.stdout is not None:
            s = s + f"\nStdout: {self.stdout}"

        if self.stderr is not None:
            s = s + f"\nStderr: {self.stderr}"

        return s
    

class SshError(TracebackError):
    """Errors that come up from running commands over SSH"""
    def __init__(self, msg: str, stdout: str = None, stderr: str = None, exit_code: int = -1234):
        super(SshError, self).__init__(msg)
        self.stdout = stdout
        self.stderr = stderr
        self.exit_code = exit_code

    def __reduce__(self):
        # For pickleability of the exception in a multi-process environment
        # Reference: https://stackoverflow.com/a/36342588
        return SshError, (self.args[0], self.stdout, self.stderr, self.exit_code)

    def __str__(self) -> str:
        s = super(SshError, self).__str__()
        if self.stdout is not None:
            s = s + f"\nStdout: {self.stdout}"

        if self.stderr is not None:
            s = s + f"\nStderr: {self.stderr}"

        if self.exit_code != -1234:
            s = s + f"\nExit Code: {str(self.exit_code)}"

        return s


class ClusterError(NonTracebackError):
    """Errors that come up from starting/stopping the RapidsSE cluster"""
    def __init__(self, msg: str):
        super(ClusterError, self).__init__(msg)


class Ec2Error(NonTracebackError):
    """Errors that come up from provisioning EC2 instances"""
    def __init__(self, msg: str):
        super(Ec2Error, self).__init__(msg)


class Ec2InstanceTimeoutError(Ec2Error):
    """Thrown when an operation on an EC2 instance takes too long"""
    def __init__(self, msg: str, instance_ids=None):
        super(Ec2InstanceTimeoutError, self).__init__(msg)
        if instance_ids is None:
            instance_ids = []
        self.instance_ids = instance_ids

    def __reduce__(self):
        # For pickleability of the exception in a multi-process environment
        # Reference: https://stackoverflow.com/a/36342588
        return SshError, (self.args[0], self.instance_ids)


class Ec2InsufficientCapacity(Ec2Error):
    """Errors that come up from no capacity to provision EC2 instances"""
    def __init__(self, msg: str):
        super(Ec2InsufficientCapacity, self).__init__(msg)


class NotImplementedYet(TracebackError):
    """Errors that come up from unhandled situations"""
    def __init__(self, msg: str):
        super(NotImplementedYet, self).__init__(msg)


class GitError(TracebackError):
    """Errors that come up from running git commands"""
    def __init__(self, msg: str):
        super(GitError, self).__init__(msg)

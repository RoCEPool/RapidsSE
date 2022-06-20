import string


class MakeReleaseError(Exception):
    """Base class for all errors emanating from MakeRelease"""
    def __init__(self, msg: string, should_print_trace: bool, *args, **kwargs):
        # super().__init__(*args, **kwargs)
        self.msg = msg
        self.should_print_trace = should_print_trace


class TracebackError(MakeReleaseError):
    """Base class of errors that should have their traceback printed"""
    def __init__(self, msg: string, *args, **kwargs):
        super().__init__(msg, True, *args, **kwargs)


class NonTracebackError(MakeReleaseError):
    """Base class of errors that shouldn't have their traceback printed"""
    def __init__(self, msg: string, *args, **kwargs):
        super().__init__(msg, False, *args, **kwargs)


class ConfigurationError(NonTracebackError):
    """Errors that come up from an incorrect configuration file"""
    def __init__(self, msg: string, *args, **kwargs):
        super().__init__(msg, *args, **kwargs)


class CompilationError(NonTracebackError):
    """Errors that come up from compiling RapidsSE and building the installer"""
    def __init__(self, msg: string, *args, **kwargs):
        super().__init__(msg, *args, **kwargs)


class InstallationError(NonTracebackError):
    """Errors that come up from installing RapidsSE"""
    def __init__(self, msg: string, *args, **kwargs):
        super().__init__(msg, *args, **kwargs)


class SshError(TracebackError):
    """Errors that come up from starting/stopping the RapidsSE cluster"""
    def __init__(self, msg: string, stdout: string, stderr: string, *args, **kwargs):
        super().__init__(msg, *args, **kwargs)
        self.stdout = stdout
        self.stderr = stderr

    def __str__(self) -> str:
        s = super().__str__()
        if self.stdout is not None:
            s = s + f"\nStdout: {self.stdout}"

        if self.stderr is not None:
            s = s + f"\nStderr: {self.stderr}"

        return s


class ClusterError(NonTracebackError):
    """Errors that come up from starting/stopping the RapidsSE cluster"""
    def __init__(self, msg: string, *args, **kwargs):
        super().__init__(msg, *args, **kwargs)


class Ec2Error(NonTracebackError):
    """Errors that come up from provisioning EC2 instances"""
    def __init__(self, msg: string, *args, **kwargs):
        super().__init__(msg, *args, **kwargs)


class Ec2InstanceTimeoutError(Ec2Error):
    """Thrown when an operation on an EC2 instance takes too long"""
    def __init__(self, msg: string, instance_ids=None, *args, **kwargs):
        super().__init__(msg, *args, **kwargs)
        if instance_ids is None:
            instance_ids = []
        self.instance_ids = instance_ids


class NotImplementedYet(TracebackError):
    """Errors that come up from unhandled situations"""
    def __init__(self, msg: string, *args, **kwargs):
        super().__init__(msg, *args, **kwargs)


class GitError(TracebackError):
    """Errors that come up from running git commands"""
    def __init__(self, msg: string, *args, **kwargs):
        super().__init__(msg, *args, **kwargs)

import string


class MakeReleaseError(Exception):
    """Base class for all errors emanating from MakeRelease"""
    def __init__(self, should_print_trace: bool, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.should_print_trace = should_print_trace


class NonTracebackError(MakeReleaseError):
    """Base class of errors that shouldn't have their traceback printed"""
    def __init__(self, *args, **kwargs):
        super().__init__(False, *args, **kwargs)


class ConfigurationError(NonTracebackError):
    """Errors that come up from an incorrect configuration file"""
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)


class CompilationError(NonTracebackError):
    """Errors that come up from compiling RapidsSE and building the installer"""
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)


class InstallationError(NonTracebackError):
    """Errors that come up from installing RapidsSE"""
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)


class SshError(MakeReleaseError):
    """Errors that come up from starting/stopping the RapidsSE cluster"""
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)


class ClusterError(NonTracebackError):
    """Errors that come up from starting/stopping the RapidsSE cluster"""
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)


class Ec2Error(NonTracebackError):
    """Errors that come up from provisioning EC2 instances"""
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)


class Ec2InstanceTimeoutError(Ec2Error):
    """Thrown when an operation on an EC2 instance takes too long"""
    def __init__(self, msg: string, instance_ids=None, *args, **kwargs):
        super().__init__(msg, *args, **kwargs)
        if instance_ids is None:
            instance_ids = []
        self.instance_ids = instance_ids


class NotImplementedYet(MakeReleaseError):
    """Errors that come up from unhandled situations"""
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)


class GitError(MakeReleaseError):
    """Errors that come up from running git commands"""
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)

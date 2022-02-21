
class SeptestError(Exception):
    """Base class for all errors emanating from Septest"""
    def __init__(self, should_print_trace: bool, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.should_print_trace = should_print_trace


class NonTracebackError(SeptestError):
    """Base class of errors that shouldn't have their traceback printed"""
    def __init__(self, *args, **kwargs):
        super().__init__(False, *args, **kwargs)


class ConfigurationError(NonTracebackError):
    """Errors that come up from an incorrect configuration file"""
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)


class DatabaseError(SeptestError):
    """General errors to do with storing/retrieving results from the database"""
    def __init__(self, *args, **kwargs):
        super().__init__(True, *args, **kwargs)


class DatabaseConsistencyError(DatabaseError):
    """Represents an error with the data in the database"""
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


class Ec2Error(NonTracebackError):
    """Errors that come up from provisioning EC2 instances"""
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)

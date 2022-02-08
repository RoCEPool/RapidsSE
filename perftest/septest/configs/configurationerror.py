
class ConfigurationError(Exception):
    """Errors that come up from an incorrect configuration file"""

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)

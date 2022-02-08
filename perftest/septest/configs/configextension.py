from collections import OrderedDict


# This is an extension used in ConfigParser to allow config files
# to have multiple non-unique keys and get the values for this key
# as a list.
# Source: https://stackoverflow.com/a/70375875
class ConfigParserMultiValues(OrderedDict):
    def __setitem__(self, key, value):
        if key in self and isinstance(value, list):
            self[key].extend(value)
        else:
            super().__setitem__(key, value)

    @staticmethod
    def getlist(value):
        return value.splitlines()

import string
import configparser
from .configextension import ConfigParserMultiValues


class JobConfig:
    """Encapsulates a job config file and hides how it is implemented and parsed."""

    C_SECTION = "job"
    C_DURATION = "MaxDuration"
    C_CONTINUEONFAILURE = "ContinueOnTestFailure"
    C_TESTPATH = "TestPath"

    def __init__(self, path: string):
        self.path = path
        # Method for reading multiple keys of the same name but with different values
        # comes from here: https://stackoverflow.com/a/70375875
        self.parser = configparser.ConfigParser(strict=False,
                                                empty_lines_in_values=False,
                                                dict_type=ConfigParserMultiValues,
                                                converters={"list": ConfigParserMultiValues.getlist})
        self.parser.read([path])

    def __str__(self):
        s = f"<JobConfig>\n" \
            f"  {self.C_DURATION} = {self.get_max_duration()}\n" \
            f"  {self.C_CONTINUEONFAILURE} = {self.get_continue_on_test_failure()}\n" \
            f"  {self.C_TESTPATH}s = [\n    " + \
            ("\n    ".join(self.get_test_paths())) + "\n  ]\n"
        return s

    def get_max_duration(self) -> int:
        return self.parser.getint(self.C_SECTION, self.C_DURATION)

    def get_continue_on_test_failure(self) -> bool:
        return self.parser.getboolean(self.C_SECTION, self.C_CONTINUEONFAILURE)

    def get_test_paths(self) -> list:
        if not self.parser.has_option(self.C_SECTION, self.C_TESTPATH):
            return []

        return self.parser.getlist(self.C_SECTION, self.C_TESTPATH)

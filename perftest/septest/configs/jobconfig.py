import os.path
import string
import configparser
import pathlib
from collections import OrderedDict

from septest.errors import ConfigurationError


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


class JobConfig:
    """Encapsulates a job config file and hides how it is implemented and parsed."""

    C_SECTION = "job"
    C_DURATION = "MaxDuration"
    C_CONTINUEONFAILURE = "ContinueOnTestFailure"
    C_HARDWARECONFIG = "HardwareConfigPath"
    C_CLUSTERCONFIG = "ClusterConfigPath"
    C_TESTPATH = "TestPath"

    def __init__(self, path: string):
        self.path: string = path

        # Method for reading multiple keys of the same name but with different values
        # comes from here: https://stackoverflow.com/a/70375875
        self.parser = configparser.ConfigParser(strict=False,
                                                empty_lines_in_values=False,
                                                dict_type=ConfigParserMultiValues,
                                                converters={"list": ConfigParserMultiValues.getlist})
        self.parser.read_file(open(path))

        if not self.parser.has_section(self.C_SECTION):
            raise ConfigurationError(f"Config file is missing the [{self.C_SECTION}] header: {path}")

        duration = self.get_max_duration()
        continue_test_failure = self.get_continue_on_test_failure()
        hw_path_str = self.get_hardware_config_path()
        cluster_path_str = self.get_cluster_config_path()
        test_paths = self.get_test_paths()

        if duration is None or duration <= 0:
            msg = f"Config file field {self.C_DURATION} must have a positive value in file: {path}"
            raise ConfigurationError(msg)

        if continue_test_failure is None:
            msg = f"Config file field {self.C_CONTINUEONFAILURE} must be set in file: {path}"
            raise ConfigurationError(msg)

        # Check the hardware config paths
        self.__validate_path(hw_path_str, self.C_HARDWARECONFIG, path)

        # Check the cluster config paths
        self.__validate_path(cluster_path_str, self.C_CLUSTERCONFIG, path)

        # Check the test file paths
        if len(test_paths) == 0:
            msg = f"At least one {self.C_TESTPATH} field must be specified in config file: {path}"
            raise ConfigurationError(msg)

        for test_path_str in test_paths:
            self.__validate_path(test_path_str, self.C_TESTPATH, path)

    def __str__(self):
        s = f"<JobConfig>\n" \
            f"  {self.C_DURATION} = {self.get_max_duration()}\n" \
            f"  {self.C_CONTINUEONFAILURE} = {self.get_continue_on_test_failure()}\n" \
            f"  {self.C_TESTPATH}s = [\n    " + \
            ("\n    ".join(self.get_test_paths())) + "\n  ]"
        return s

    def get_job_name(self) -> string:
        """Returns the name of the job file without the last extension"""
        return os.path.basename(self.path).rsplit(sep=".", maxsplit=1)[0]

    def get_max_duration(self) -> float:
        return self.parser.getfloat(self.C_SECTION, self.C_DURATION)

    def get_continue_on_test_failure(self) -> bool:
        return self.parser.getboolean(self.C_SECTION, self.C_CONTINUEONFAILURE)

    def get_hardware_config_path(self) -> string:
        """Returns the path of the hardware config file as stored in the job config file"""
        s = self.parser.get(self.C_SECTION, self.C_HARDWARECONFIG)

        if s.startswith('"') and s.endswith('"'):
            s = s[1:-1]
        elif s.startswith("'") and s.endswith("'"):
            s = s[1:-1]
        return s

    def get_resolved_hardware_config_path(self) -> string:
        """Returns the absolute path of the hardware config file"""
        path_str = self.get_hardware_config_path()

        path_obj = pathlib.Path(path_str)
        resolved_path = path_obj

        if not path_obj.is_absolute():
            resolved_path = (os.path.dirname(self.path) / path_obj).resolve()

        assert resolved_path.exists()
        return str(resolved_path)

    def get_cluster_config_path(self) -> string:
        """Returns the path of the cluster config file as stored in the job config file"""
        s = self.parser.get(self.C_SECTION, self.C_CLUSTERCONFIG)

        if s.startswith('"') and s.endswith('"'):
            s = s[1:-1]
        elif s.startswith("'") and s.endswith("'"):
            s = s[1:-1]
        return s

    def get_resolved_cluster_config_path(self) -> string:
        """Returns the absolute path of the cluster config file"""
        path_str = self.get_cluster_config_path()

        path_obj = pathlib.Path(path_str)
        resolved_path = path_obj

        if not path_obj.is_absolute():
            resolved_path = (os.path.dirname(self.path) / path_obj).resolve()

        assert resolved_path.exists()
        return str(resolved_path)

    def get_test_paths(self) -> list:
        """Returns the paths of the test files as stored in the job config file. May be relative to this file"""
        if not self.parser.has_option(self.C_SECTION, self.C_TESTPATH):
            return []

        # Iterate the tests and check if they have any double quotes
        tests = self.parser.getlist(self.C_SECTION, self.C_TESTPATH)

        for i in range(len(tests)):
            s = tests[i]
            if s.startswith('"') and s.endswith('"'):
                s = s[1:-1]
            elif s.startswith("'") and s.endswith("'"):
                s = s[1:-1]
            tests[i] = s

        return tests

    def get_resolved_test_paths(self) -> list:
        """Returns the absolute paths of the test files"""
        test_paths = self.get_test_paths()
        resolved_test_paths = []

        for path_str in test_paths:
            path_obj = pathlib.Path(path_str)
            resolved_path = path_obj

            if not path_obj.is_absolute():
                resolved_path = (os.path.dirname(self.path) / path_obj).resolve()

            assert resolved_path.exists()
            resolved_test_paths.append(str(resolved_path))

        return resolved_test_paths

    @staticmethod
    def __validate_path(path_str: string, key_name: string, job_cfg_file_path: string):
        """Validates that a path referenced from a config file actually exists and is a file"""
        if path_str is None:
            msg = f"Config file field {key_name} must have a valid path in file: {job_cfg_file_path}"
            raise ConfigurationError(msg)

        path_obj = pathlib.Path(path_str)
        resolved_path = path_obj

        if not path_obj.is_absolute():
            resolved_path = (os.path.dirname(job_cfg_file_path) / path_obj).resolve()

        if not resolved_path.exists():
            msg = f"The config file at {path_str} referenced from field {key_name} in " \
                  f"config file {job_cfg_file_path} does not exist"
            raise ConfigurationError(msg)

        if not resolved_path.is_file():
            msg = f"The config file at {path_str} referenced from field " \
                  f"{key_name} in config file {job_cfg_file_path} is not a file"
            raise ConfigurationError(msg)

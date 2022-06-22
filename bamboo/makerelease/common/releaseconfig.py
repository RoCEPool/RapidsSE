import os.path
import string
import configparser
import pathlib
from collections import OrderedDict

from common.errors import ConfigurationError
import common.ec2 as ec2
from clusterconfig import ClusterConfig


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


class ReleaseConfig:
    """Encapsulates a release config file and hides how it is implemented and parsed."""

    C_BUILD_SECTION = "build"
    C_TEST_SECTION = "test"
    C_INSTANCE_TYPE = "InstanceType"
    C_TEST_NAME = "TestName"
    C_IMAGE_ID = "ImageId"
    C_CLOUD_INIT = "CloudInit"
    C_CLUSTER_CONFIG = "ClusterConfig"
    C_TERMINATE = "Terminate"
    C_TERMINATE_DEFAULT_VAL = True

    def __init__(self, path: string):
        self.path: string = path

        # Method for reading multiple keys of the same name but with different values
        # comes from here: https://stackoverflow.com/a/70375875
        self.parser = configparser.ConfigParser(strict=False,
                                                empty_lines_in_values=False,
                                                dict_type=ConfigParserMultiValues,
                                                converters={"list": ConfigParserMultiValues.getlist})
        self.parser.read_file(open(path))

        for section in [self.C_TEST_SECTION, self.C_TEST_SECTION]:
            if not self.parser.has_section(section):
                raise ConfigurationError(f"Config file is missing the [{section}] header: {path}")

        build_instance = self.get_build_instance()
        build_image = self.get_build_image()
        build_cloudinit = self.get_build_cloudinit()

        test_instance = self.get_test_instance()
        test_name_list = self.get_test_names()
        test_image_list = self.get_test_images()
        test_cloudinit_list = self.get_test_cloudinits_paths()
        test_cluster_config_list = self.get_test_cluster_configs()

        if build_instance is None or len(build_instance) == 0:
            msg = f"Config file field {self.C_INSTANCE_TYPE} in the [{self.C_BUILD_SECTION}] section " \
                  f"must not be empty in the file: {path}"
            raise ConfigurationError(msg)

        # Check that build_instance is a valid instance
        if not ec2.is_valid_instance_type(build_instance):
            msg = f"Config file field {self.C_INSTANCE_TYPE} in the [{self.C_BUILD_SECTION}] section " \
                  f"does not contain a valid EC2 instance type: {build_instance}"
            raise ConfigurationError(msg)

        if build_image is None or len(build_image) == 0:
            msg = f"Config file field {self.C_IMAGE_ID} in the [{self.C_BUILD_SECTION}] section " \
                  f"must not be empty in the file: {path}"
            raise ConfigurationError(msg)

        # Check that build_image is a valid instance
        if not ec2.is_valid_image_id(build_image):
            msg = f"Config file field {self.C_IMAGE_ID} in the [{self.C_BUILD_SECTION}] section " \
                  f"does not contain a valid EC2 image id: {build_image}"
            raise ConfigurationError(msg)

        if build_cloudinit is None or len(build_cloudinit) == 0:
            msg = f"Config file field {self.C_CLOUD_INIT} in the [{self.C_BUILD_SECTION}] section " \
                  f"must not be empty in the file: {path}"
            raise ConfigurationError(msg)

        # Validate the build cloudinit file is resolvable
        self.__validate_path(build_cloudinit, self.C_BUILD_SECTION, self.C_CLOUD_INIT, self.path)

        if test_instance is None or len(test_instance) == 0:
            msg = f"Config file field {self.C_INSTANCE_TYPE} in the [{self.C_TEST_SECTION}] section " \
                  f"must not be empty in the file: {path}"
            raise ConfigurationError(msg)

        # Check that test_instance is a valid instance
        if not ec2.is_valid_instance_type(test_instance):
            msg = f"Config file field {self.C_INSTANCE_TYPE} in the [{self.C_TEST_SECTION}] section " \
                  f"does not contain a valid EC2 instance type: {test_instance}"
            raise ConfigurationError(msg)

        if test_name_list is None or len(test_name_list) == 0:
            msg = f"Config file field {self.C_TEST_NAME} in the [{self.C_TEST_SECTION}] section " \
                  f"must not be empty in the file: {path}"
            raise ConfigurationError(msg)

        if test_image_list is None or len(test_image_list) == 0:
            msg = f"Config file field {self.C_IMAGE_ID} in the [{self.C_TEST_SECTION}] section " \
                  f"must not be empty in the file: {path}"
            raise ConfigurationError(msg)

        # Check that each of the test_images is a valid instance
        for test_image in test_image_list:
            if not ec2.is_valid_image_id(test_image):
                msg = f"Config file field {self.C_IMAGE_ID} in the [{self.C_TEST_SECTION}] section " \
                      f"does not contain a valid EC2 image id: {test_image}"
                raise ConfigurationError(msg)

        if test_cloudinit_list is None or len(test_cloudinit_list) == 0:
            msg = f"Config file field {self.C_CLOUD_INIT} in the [{self.C_TEST_SECTION}] section " \
                  f"must not be empty in the file: {path}"
            raise ConfigurationError(msg)

        if test_cluster_config_list is None or len(test_cluster_config_list) == 0:
            msg = f"Config file field {self.C_CLUSTER_CONFIG} in the [{self.C_TEST_SECTION}] section " \
                  f"must not be empty in the file: {path}"
            raise ConfigurationError(msg)

        if len(test_name_list) != len(test_image_list):
            msg = f"The config file fields {self.C_TEST_NAME} and {self.C_IMAGE_ID} in the " \
                  f"[{self.C_TEST_SECTION}] section must be specified the same number of times in the file: {path}"
            raise ConfigurationError(msg)

        if len(test_image_list) != len(test_cloudinit_list):
            msg = f"The config file fields {self.C_IMAGE_ID} and {self.C_CLOUD_INIT} in the " \
                  f"[{self.C_TEST_SECTION}] section must be specified the same number of times in the file: {path}"
            raise ConfigurationError(msg)

        if len(test_image_list) != len(test_cluster_config_list):
            msg = f"The config file fields {self.C_IMAGE_ID} and {self.C_CLUSTER_CONFIG} in the " \
                  f"[{self.C_TEST_SECTION}] section must be specified the same number of times in the file: {path}"
            raise ConfigurationError(msg)

        # Check that all of the cloudinit scripts are resolvable
        for cloudinit_path in test_cloudinit_list:
            self.__validate_path(cloudinit_path, self.C_TEST_SECTION, self.C_CLOUD_INIT, self.path)

    def get_build_instance(self) -> string:
        """Returns the name of the EC2 instance to be used for building SE"""
        return self.parser.get(self.C_BUILD_SECTION, self.C_INSTANCE_TYPE)

    def get_build_image(self) -> string:
        """Returns the name of the EC2 image to be used for building SE"""
        return self.parser.get(self.C_BUILD_SECTION, self.C_IMAGE_ID)

    def get_build_cloudinit(self) -> string:
        """Returns the name of the cloudinit script to be used for building SE"""
        # Iterate the cloudinit scripts and check if they have any double quotes
        cloudinit = self.parser.get(self.C_BUILD_SECTION, self.C_CLOUD_INIT)

        if cloudinit.startswith('"') and cloudinit.endswith('"'):
            cloudinit = cloudinit[1:-1]
        elif cloudinit.startswith("'") and cloudinit.endswith("'"):
            cloudinit = cloudinit[1:-1]

        cloudinit = self.__resolve_path(cloudinit)
        return cloudinit

    def get_build_terminate(self) -> bool:
        """Do we shutdown the instance at the end of the build phase?"""
        return self.parser.getboolean(self.C_BUILD_SECTION, self.C_TERMINATE, fallback=self.C_TERMINATE_DEFAULT_VAL)

    def get_test_instance(self) -> string:
        """Returns the name of the EC2 instance to be used for testing SE"""
        return self.parser.get(self.C_TEST_SECTION, self.C_INSTANCE_TYPE)

    def get_test_names(self) -> [string]:
        """Returns a list of the names of the test configurations to be used for testing SE"""
        # Iterate the names and check if they have any double quotes
        names = self.parser.getlist(self.C_TEST_SECTION, self.C_TEST_NAME)

        for i in range(len(names)):
            s = names[i]
            if s.startswith('"') and s.endswith('"'):
                s = s[1:-1]
            elif s.startswith("'") and s.endswith("'"):
                s = s[1:-1]
            names[i] = s

        return names

    def get_test_images(self) -> [string]:
        """Returns a list of the name of the EC2 images to be used for testing SE"""
        # Iterate the images and check if they have any double quotes
        images = self.parser.getlist(self.C_TEST_SECTION, self.C_IMAGE_ID)

        for i in range(len(images)):
            s = images[i]
            if s.startswith('"') and s.endswith('"'):
                s = s[1:-1]
            elif s.startswith("'") and s.endswith("'"):
                s = s[1:-1]
            images[i] = s

        return images

    def get_test_cloudinits_paths(self) -> [pathlib.Path]:
        """Returns a list of the name of the cloudinit scripts to be used for testing SE"""
        # Iterate the cloudinit scripts and check if they have any double quotes
        cloudinits = self.parser.getlist(self.C_TEST_SECTION, self.C_CLOUD_INIT)

        for i in range(len(cloudinits)):
            s = cloudinits[i]
            if s.startswith('"') and s.endswith('"'):
                s = s[1:-1]
            elif s.startswith("'") and s.endswith("'"):
                s = s[1:-1]
            cloudinits[i] = self.__resolve_path(s)

        return cloudinits

    def get_test_cluster_configs(self) -> [ClusterConfig]:
        """Returns a list of ClusterConfig objects which have extracted out
           the details of each cluster.config file"""
        config_paths: [pathlib.Path] = self.get_test_cluster_config_paths()

        # Create a list of ClusterConfig objects
        cluster_configs: [ClusterConfig] = list(map(ClusterConfig, config_paths))
        return cluster_configs

    def get_test_cluster_config_paths(self) -> [pathlib.Path]:
        """Returns a list of the name of the cluster.config files to be used for testing SE"""
        # Iterate the cluster config files and check if they have any double quotes
        cluster_config_paths = self.parser.getlist(self.C_TEST_SECTION, self.C_CLUSTER_CONFIG)

        for i in range(len(cluster_config_paths)):
            s = cluster_config_paths[i]
            if s.startswith('"') and s.endswith('"'):
                s = s[1:-1]
            elif s.startswith("'") and s.endswith("'"):
                s = s[1:-1]
            cluster_config_paths[i] = self.__resolve_path(s)

        return cluster_config_paths

    def __resolve_path(self, path_str: string) -> string:
        path_obj = pathlib.Path(path_str)
        resolved_path = path_obj

        if not path_obj.is_absolute():
            resolved_path = (os.path.dirname(self.path) / path_obj).resolve()

        assert resolved_path.exists(), f"Path does not exist: {str(resolved_path)}"
        return str(resolved_path)

    @staticmethod
    def __validate_path(path_str: string, section_name: string, key_name: string, cfg_file_path: string):
        """Validates that a path referenced from a config file actually exists and is a file"""
        if path_str is None:
            msg = f"Config file field {key_name} in section [{section_name}] must have a valid path " \
                  f"in file: {cfg_file_path}"
            raise ConfigurationError(msg)

        path_obj = pathlib.Path(path_str)
        resolved_path = path_obj

        if not path_obj.is_absolute():
            resolved_path = (os.path.dirname(cfg_file_path) / path_obj).resolve()

        if not resolved_path.exists():
            msg = f"The file at {path_str} referenced from field {key_name} " \
                  f"in section [{section_name}] " \
                  f"in config file {cfg_file_path} does not exist"
            raise ConfigurationError(msg)

        if not resolved_path.is_file():
            msg = f"The file at {path_str} referenced from field " \
                  f"{key_name} in section [{section_name}] " \
                  f"in config file {cfg_file_path} is not a file"
            raise ConfigurationError(msg)

    def get_test_terminate(self) -> bool:
        """Do we shutdown the instance at the end of the test phase?"""
        return self.parser.getboolean(self.C_TEST_SECTION, self.C_TERMINATE, fallback=self.C_TERMINATE_DEFAULT_VAL)

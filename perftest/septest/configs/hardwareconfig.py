import string
import configparser
from .configurationerror import ConfigurationError


class HardwareConfig:
    """Encapsulates a hardware config file and hides how it is implemented and parsed."""

    C_SECTION          = "hardware"
    C_INSTANCETYPE     = "InstanceType"
    C_NUMMACHINES      = "NumMachines"
    C_USESPOTINSTANCES = "UseSpotInstances"

    def __init__(self, path: string):
        self.path = path

        # Method for reading multiple keys of the same name but with different values
        # comes from here: https://stackoverflow.com/a/70375875
        self.parser = configparser.ConfigParser(strict=True,
                                                empty_lines_in_values=False)
        self.parser.read_file(open(path))

        if not self.parser.has_section(self.C_SECTION):
            raise ConfigurationError(f"Config file is missing the [{self.C_SECTION}] header: {path}")

        instance_type = self.get_instance_type()
        num_machines = self.get_num_machines()
        use_spot_instances = self.get_use_spot_instances()

        if instance_type is None or len(instance_type) == 0:
            raise ConfigurationError(f"Config file field {self.C_INSTANCETYPE} must be set in file: {path}")

        if num_machines is None or num_machines <= 0:
            raise ConfigurationError(f"Config file field {self.C_NUMMACHINES} must have a positive value in file: {path}")

        if use_spot_instances is None:
            raise ConfigurationError(f"Config file field {self.C_USESPOTINSTANCES} must be set in file: {path}")

    def __str__(self):
        s = f"<HardwareConfig>\n" \
            f"  {self.C_INSTANCETYPE} = {self.get_instance_type()}\n" \
            f"  {self.C_NUMMACHINES} = {self.get_num_machines()}\n" \
            f"  {self.C_USESPOTINSTANCES} = {self.get_use_spot_instances()}"
        return s

    def get_instance_type(self) -> string:
        s = self.parser.get(self.C_SECTION, self.C_INSTANCETYPE)

        if s.startswith('"') and s.endswith('"'):
            s = s[1:-1]
        elif s.startswith("'") and s.endswith("'"):
            s = s[1:-1]
        return s

    def get_num_machines(self) -> int:
        return self.parser.getint(self.C_SECTION, self.C_NUMMACHINES)

    def get_use_spot_instances(self) -> bool:
        return self.parser.getboolean(self.C_SECTION, self.C_USESPOTINSTANCES)

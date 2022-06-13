# Set up the logger configuration.
# This runs before any other code

import logging.config
import os

C_LOG_CONFIG_FILE = "configs/logging.ini"

if os.path.exists(C_LOG_CONFIG_FILE):
    logging.config.fileConfig(C_LOG_CONFIG_FILE)
else:
    logging.basicConfig()
    logging.getLogger().warning("Logging config file not found. Using logging defaults")

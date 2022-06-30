# Set up the logger configuration.
# This runs before any other code

import logging.config
import os
import multiprocessing_logging

C_LOG_CONFIG_FILE = "configs/logging.ini"

if os.path.exists(C_LOG_CONFIG_FILE):
    logging.config.fileConfig(C_LOG_CONFIG_FILE)

    # This line uses the multiprocessing_logging library to funnel all logging
    # from child processes back to this parent process.
    multiprocessing_logging.install_mp_handler()

else:
    logging.basicConfig()
    logging.getLogger().warning("Logging config file not found. Using logging defaults")

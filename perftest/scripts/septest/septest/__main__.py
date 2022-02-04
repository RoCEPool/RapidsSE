"""A simple main entry point for executing the SE Performance Test script"""
import sys
import septest.runners.jobrunner as jobrunner

def main() -> int:
    """The main entry point"""
    job_path = ""
    bamboo_build_str = ""
    git_hash = ""
    tags = {}

    jobrunner.runJob(job_path, bamboo_build_str, git_hash, tags)
    return 0


if __name__ == '__main__':
    sys.exit(main())

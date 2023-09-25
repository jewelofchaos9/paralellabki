from loguru import logger
from fastapi import Request
import os
import sys


log_level = os.getenv("LOG_LEVEL", "TRACE")

def opener(file, flags):
    return os.open(file, flags, 0o777)

logger_parameters = {
    "sink": "logs/logger.log",
    "rotation": "500 MB",
    "retention": "30 days",
    "level": log_level,
    "opener": opener
}

fmt = (
    "<green>{time:YYYY-MM-DD HH:mm:ss.SSS}</green> | "
    "<level>{level: <8}</level> | "
    "<white>{extra[request_uuid]}</white> | "
    "<cyan>{name}</cyan>:<cyan>{function}</cyan>:<cyan>{line}</cyan> - "
    "<level>{message}</level>"
)





logger.remove()
logger.add(sys.stdout, format=fmt)
logger.configure(extra={"request_uuid": None})
logger.add(**logger_parameters, format=fmt)

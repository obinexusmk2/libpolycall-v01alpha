import time


def emit_sensor():
    print({"sensor": "rpi", "value": 22.4, "ts": int(time.time())})


if __name__ == "__main__":
    emit_sensor()

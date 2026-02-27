import json


def observe(guid: str, value: float):
    print(json.dumps({"guid": guid, "value": value, "binding": "python"}))


if __name__ == "__main__":
    observe("guid-demo-001", 20.5)

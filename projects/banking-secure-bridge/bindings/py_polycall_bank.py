from fastapi import FastAPI

app = FastAPI(title="polycall-bank")


@app.get("/account/{account_id}")
def read_account(account_id: int):
    return {
        "account_id": account_id,
        "status": "active",
        "binding": "SQUARE",
        "transport": "libpolycall-driver"
    }

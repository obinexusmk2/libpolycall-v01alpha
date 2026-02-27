import hashlib


def test_guid_seed_reproducible():
    payload = b"GET_ACCOUNT:10001"
    first = hashlib.sha256(payload).hexdigest()
    second = hashlib.sha256(payload).hexdigest()
    assert first == second

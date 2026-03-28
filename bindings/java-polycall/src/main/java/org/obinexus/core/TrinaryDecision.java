package org.obinexus.core;

public enum TrinaryDecision {
    NO,
    MAYBE,
    YES;

    public String wireValue() {
        return name().toLowerCase();
    }
}

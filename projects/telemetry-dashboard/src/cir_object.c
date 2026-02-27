#include "cir_object.h"

#include <string.h>

void cir_assign_float(CIR_Value* v, double input, const char* guid) {
    v->type = FLOAT64;
    v->value.f64 = input;
    v->size = sizeof(double);
    strncpy(v->guid, guid, sizeof(v->guid) - 1);
    v->guid[sizeof(v->guid) - 1] = '\0';
}

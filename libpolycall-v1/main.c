/* =============================
 * PolyCall CLI entrypoint
 * main.c is orchestration-only.
 * ============================= */

/* ---------- Includes ---------- */
#include "polycall_runtime.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

/* ---------- Types ---------- */
typedef struct {
    bool non_interactive;
    const char* config_file;
} CliOptions;

/* ---------- Command/config orchestration ---------- */
static CliOptions parse_cli_options(int argc, char* argv[]) {
    CliOptions options = {.non_interactive = false, .config_file = NULL};

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-f") == 0 && i + 1 < argc) {
            options.config_file = argv[++i];
            options.non_interactive = true;
        }
    }

    return options;
}

/* ---------- Main loop orchestration ---------- */
int main(int argc, char* argv[]) {
    CliOptions options = parse_cli_options(argc, argv);

    if (!runtime_initialize()) {
        fprintf(stderr, "Failed to initialize runtime\n");
        return 1;
    }

    bool run_ok = true;
    if (options.non_interactive) {
        run_ok = runtime_run_non_interactive(options.config_file);
    } else {
        runtime_run_interactive();
    }

    runtime_cleanup();
    printf("Goodbye!\n");
    return run_ok ? 0 : 1;
}

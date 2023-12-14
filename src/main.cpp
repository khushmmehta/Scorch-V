#define FMT_HEADER_ONLY
#include <fmt/core.h>
#include <ScorchV.h>

int main() {
    ScorchV app;

    try { app.run(); }
    catch (const std::exception& e) { fmt::print(fmterr, "{}", e.what()); return EXIT_FAILURE; }

    getchar();

    return EXIT_SUCCESS;
}

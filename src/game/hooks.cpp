// Hooks for default.xbe. A strong definition replaces the recompiled function at that address, for example:
//
// #include "xdk/hle.h"
//
// X86_EXTERN_FUNC(__imp__sub_00012345);
// X86_FUNC(sub_00012345) {
//     __imp__sub_00012345(ctx, base);  // the original
// }

#include "xdk/hle.h"
#include "core/log.h"
#include <array>
#include <unordered_map>

// Observe the original EA pad conversion without changing its inputs or result.
X86_EXTERN_FUNC(__imp__sub_00034360);
X86_FUNC(sub_00034360) {
    const uint32_t object = X86_MEM32(ctx.esp + 4);
    __imp__sub_00034360(ctx, base);
    const uint32_t raw = X86_MEM32(object + 8);
    const uint32_t output = X86_MEM32(object + 0x1c);
    if (!raw || !output) return;
    const uint32_t handle = X86_MEM32(raw + 0xd4);
    if (!handle) return;
    const std::array<uint32_t, 6> state = {
        X86_MEM16(raw), X86_MEM32(raw + 10), X86_MEM32(raw + 14),
        X86_MEM32(output + 8), X86_MEM32(output + 12), X86_MEM32(output + 16)};
    static thread_local std::unordered_map<uint32_t, std::array<uint32_t, 6>> previous_by_handle;
    auto& previous = previous_by_handle[handle];
    static thread_local unsigned reports = 0;
    if (state != previous && reports < 1024) {
        ++reports;
        LOGR("pad-convert: handle=%08X buttons=%04X sticks=%d,%d,%d,%d floats=%.5f,%.5f,%.5f,%.5f output=%08X %08X %08X",
             handle, state[0], int(int16_t(X86_MEM16(raw + 10))),
             int(int16_t(X86_MEM16(raw + 12))), int(int16_t(X86_MEM16(raw + 14))),
             int(int16_t(X86_MEM16(raw + 16))), double(X86_MEMF32(raw + 0x18)),
             double(X86_MEMF32(raw + 0x1c)), double(X86_MEMF32(raw + 0x20)),
             double(X86_MEMF32(raw + 0x24)), state[3], state[4], state[5]);
    }
    previous = state;
}

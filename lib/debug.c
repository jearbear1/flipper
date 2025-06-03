#include "libflipper.h"

int lf_debug_level = LF_DEBUG_LEVEL_OFF;

void lf_set_debug_level(int level) {
    lf_debug_level = level;
}

void lf_debug_call(struct _fmr_call *call) {
#ifndef LF_DISABLE_DEBUG
    printf("call\n");
    printf("  └─module:    0x%x\n", call->module);
    printf("  └─function:  0x%x\n", call->function);

    // Type strings for lf_type values (0–31)
    const char *typestrs[] = {
        "unknown", "unknown", "void",    "uint64",  "int",     "unknown", "ptr",     "int64",
        "unknown", "unknown", "unknown", "unknown", "unknown", "unknown", "unknown", "unknown",
        "uint8",   "uint16",  "unknown", "uint32",  "unknown", "unknown", "unknown", "unknown",
        "int8",    "int16",   "unknown", "int32",   "unknown", "unknown", "unknown", "unknown"
    };

    printf("  └─return:    %s\n", typestrs[call->ret]);
    printf("  └─argc:      0x%x (%d arguments)\n", call->argc, call->argc);
    
    // Print argument types
    printf("  └─types:     ");
    for (lf_argc i = 0; i < call->argc; i++) {
        printf("%s%s", i > 0 ? ", " : "", typestrs[call->arg_types[i]]);
    }
    printf("\n");

    printf("args\n");
    // Offset pointer for reading each argument
    uint8_t *offset = call->argv;

    for (lf_argc i = 0; i < call->argc; i++) {
        lf_type type = call->arg_types[i];
        lf_arg arg = 0;
        memcpy(&arg, offset, lf_sizeof(type));

        // Print argument with signedness prefix
        const char *prefix = (type == 3 || type == 6 || type == 16 || type == 17 || type == 19) ? "u" : "";
        printf("  └─ %s%s:   0x%llx\n", prefix, typestrs[type], arg);
        offset += lf_sizeof(type);
    }

    printf("\n");
#endif
}

void lf_debug_packet(struct _fmr_packet *packet) {
#ifndef LF_DISABLE_DEBUG
    if (lf_debug_level != LF_DEBUG_LEVEL_ALL) return;

    struct _fmr_header hdr = packet->hdr;

    if (hdr.magic == FMR_MAGIC_NUMBER) {
        printf("header\n");
        printf("  └─magic:     0x%x\n", hdr.magic);
        printf("  └─checksum:  0x%x\n", hdr.crc);
        printf("  └─length:    %d bytes (%.02f%%)\n", hdr.len, (float)hdr.len / sizeof(struct _fmr_packet) * 100);

        char *classstrs[] = { "exec", "push", "pull", "dyld", "malloc", "free" };
        printf("  └─class:     %s\n", classstrs[hdr.type]);

        /* Cast the packet to the appropriate struct to extract payload metadata. */
        struct _fmr_call_packet *call_pkt = (struct _fmr_call_packet *)packet;
        struct _fmr_push_pull_packet *pushpull = (struct _fmr_push_pull_packet *)packet;
        struct _fmr_dyld_packet *dyld = (struct _fmr_dyld_packet *)packet;
        struct _fmr_memory_packet *mem = (struct _fmr_memory_packet *)packet;

        switch (hdr.type) {
            case fmr_rpc_class:
                /* Prints detailed information about an RPC call from a call_packet. */
                lf_debug_call(&call_pkt->call);
                break;
            case fmr_push_class:
            case fmr_pull_class:
                printf("length:\n");
                printf("   └─ ptr:     0x%llx\n", pushpull->ptr);
                printf("   └─ len:     0x%x\n\n", pushpull->len);
                break;
            case fmr_dyld_class:
                printf("module:\n");
                printf("   └─ module: '%s'\n", dyld->module);
                break;
            case fmr_malloc_class:
                printf("malloc:\n");
                printf("   └─ size: '0x%x'\n", mem->size);
                break;
            case fmr_free_class:
                printf("free:\n");
                printf("   └─ ptr: '0x%llx'\n", mem->ptr);
                break;
            default:
                printf("invalid packet class.\n");
                break;
        }

        for (size_t i = 1; i <= hdr.len; i++) {
            printf("0x%02x ", ((uint8_t *)packet)[i - 1]);
            if (i % 8 == 0 && i < hdr.len - 1) printf("\n");
        }
    } else {
        printf("invalid magic number (0x%02x).\n", hdr.magic);
    }
    printf("\n\n-----------\n\n");
#endif
}

void lf_debug_result(struct _fmr_result *result) {
#ifdef __LF_DEBUG__
    if (lf_debug_level != LF_DEBUG_LEVEL_ALL) return;

    printf("response:\n");
    printf("  └─ value:    0x%llx\n", result->value);
    printf("  └─ error:    0x%x\n", result->error);
    printf("\n-----------\n\n");
#endif
}

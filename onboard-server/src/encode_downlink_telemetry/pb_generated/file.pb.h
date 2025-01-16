/* Automatically generated nanopb header */
/* Generated by nanopb-0.4.9 */

#ifndef PB_FILE_PB_H_INCLUDED
#define PB_FILE_PB_H_INCLUDED
#include <pb.h>

#if PB_PROTO_HEADER_VERSION != 40
#error Regenerate this file with the current version of nanopb generator.
#endif

/* Struct definitions */
typedef struct _File {
    char extension[17];
    pb_callback_t data;
} File;


#ifdef __cplusplus
extern "C" {
#endif

/* Initializer values for message structs */
#define File_init_default                        {"", {{NULL}, NULL}}
#define File_init_zero                           {"", {{NULL}, NULL}}

/* Field tags (for use in manual encoding/decoding) */
#define File_extension_tag                       2
#define File_data_tag                            3

/* Struct field encoding specification for nanopb */
#define File_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, STRING,   extension,         2) \
X(a, CALLBACK, SINGULAR, BYTES,    data,              3)
#define File_CALLBACK pb_default_field_callback
#define File_DEFAULT NULL

extern const pb_msgdesc_t File_msg;

/* Defines for backwards compatibility with code written before nanopb-0.4.0 */
#define File_fields &File_msg

/* Maximum encoded size of messages (where known) */
/* File_size depends on runtime parameters */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif

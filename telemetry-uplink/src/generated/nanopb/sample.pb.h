/* Automatically generated nanopb header */
/* Generated by nanopb-0.4.9 */

#ifndef PB_SAMPLE_PB_H_INCLUDED
#define PB_SAMPLE_PB_H_INCLUDED
#include <pb.h>

#if PB_PROTO_HEADER_VERSION != 40
#error Regenerate this file with the current version of nanopb generator.
#endif

/* Struct definitions */
typedef struct _Primitive {
    pb_size_t which_value;
    union {
        int32_t int_val;
        int64_t long_val;
        float float_val;
        double double_val;
        bool bool_val;
        char string_val[61];
    } value;
} Primitive;

typedef struct _File {
    char filepath[101];
    char extension[11];
} File;

typedef struct _Sample {
    char metric_id[41];
    float timestamp; /* s since last epoch */
    pb_size_t which_data;
    union {
        Primitive primitive;
        File file;
    } data;
} Sample;


#ifdef __cplusplus
extern "C" {
#endif

/* Initializer values for message structs */
#define Sample_init_default                      {"", 0, 0, {Primitive_init_default}}
#define Primitive_init_default                   {0, {0}}
#define File_init_default                        {"", ""}
#define Sample_init_zero                         {"", 0, 0, {Primitive_init_zero}}
#define Primitive_init_zero                      {0, {0}}
#define File_init_zero                           {"", ""}

/* Field tags (for use in manual encoding/decoding) */
#define Primitive_int_val_tag                    1
#define Primitive_long_val_tag                   2
#define Primitive_float_val_tag                  3
#define Primitive_double_val_tag                 4
#define Primitive_bool_val_tag                   5
#define Primitive_string_val_tag                 6
#define File_filepath_tag                        1
#define File_extension_tag                       2
#define Sample_metric_id_tag                     1
#define Sample_timestamp_tag                     2
#define Sample_primitive_tag                     3
#define Sample_file_tag                          4

/* Struct field encoding specification for nanopb */
#define Sample_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, STRING,   metric_id,         1) \
X(a, STATIC,   SINGULAR, FLOAT,    timestamp,         2) \
X(a, STATIC,   ONEOF,    MESSAGE,  (data,primitive,data.primitive),   3) \
X(a, STATIC,   ONEOF,    MESSAGE,  (data,file,data.file),   4)
#define Sample_CALLBACK NULL
#define Sample_DEFAULT NULL
#define Sample_data_primitive_MSGTYPE Primitive
#define Sample_data_file_MSGTYPE File

#define Primitive_FIELDLIST(X, a) \
X(a, STATIC,   ONEOF,    INT32,    (value,int_val,value.int_val),   1) \
X(a, STATIC,   ONEOF,    INT64,    (value,long_val,value.long_val),   2) \
X(a, STATIC,   ONEOF,    FLOAT,    (value,float_val,value.float_val),   3) \
X(a, STATIC,   ONEOF,    DOUBLE,   (value,double_val,value.double_val),   4) \
X(a, STATIC,   ONEOF,    BOOL,     (value,bool_val,value.bool_val),   5) \
X(a, STATIC,   ONEOF,    STRING,   (value,string_val,value.string_val),   6)
#define Primitive_CALLBACK NULL
#define Primitive_DEFAULT NULL

#define File_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, STRING,   filepath,          1) \
X(a, STATIC,   SINGULAR, STRING,   extension,         2)
#define File_CALLBACK NULL
#define File_DEFAULT NULL

extern const pb_msgdesc_t Sample_msg;
extern const pb_msgdesc_t Primitive_msg;
extern const pb_msgdesc_t File_msg;

/* Defines for backwards compatibility with code written before nanopb-0.4.0 */
#define Sample_fields &Sample_msg
#define Primitive_fields &Primitive_msg
#define File_fields &File_msg

/* Maximum encoded size of messages (where known) */
#define File_size                                114
#define Primitive_size                           62
#define SAMPLE_PB_H_MAX_SIZE                     Sample_size
#define Sample_size                              163

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif

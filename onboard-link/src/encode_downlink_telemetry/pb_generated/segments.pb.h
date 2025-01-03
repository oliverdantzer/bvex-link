/* Automatically generated nanopb header */
/* Generated by nanopb-0.4.9 */

#ifndef PB_SEGMENTS_SEGMENTS_PB_H_INCLUDED
#define PB_SEGMENTS_SEGMENTS_PB_H_INCLUDED
#include <pb.h>

#if PB_PROTO_HEADER_VERSION != 40
#error Regenerate this file with the current version of nanopb generator.
#endif

/* Struct definitions */
/* contains a single segment's data and its metadata */
typedef struct _segments_Segment {
    uint32_t sequence_num;
} segments_Segment;

/* contains a list of segments and their shared metadata */
typedef struct _segments_SegmentsFrame {
    uint32_t num_segments;
    pb_callback_t data_type;
    pb_callback_t segments;
} segments_SegmentsFrame;


#ifdef __cplusplus
extern "C" {
#endif

/* Initializer values for message structs */
#define segments_Segment_init_default            {0}
#define segments_SegmentsFrame_init_default      {0, {{NULL}, NULL}, {{NULL}, NULL}}
#define segments_Segment_init_zero               {0}
#define segments_SegmentsFrame_init_zero         {0, {{NULL}, NULL}, {{NULL}, NULL}}

/* Field tags (for use in manual encoding/decoding) */
#define segments_Segment_sequence_num_tag        1
#define segments_SegmentsFrame_num_segments_tag  2
#define segments_SegmentsFrame_data_type_tag     3
#define segments_SegmentsFrame_segments_tag      6

/* Struct field encoding specification for nanopb */
#define segments_Segment_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, UINT32,   sequence_num,      1)
#define segments_Segment_CALLBACK NULL
#define segments_Segment_DEFAULT NULL

#define segments_SegmentsFrame_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, UINT32,   num_segments,      2) \
X(a, CALLBACK, SINGULAR, STRING,   data_type,         3) \
X(a, CALLBACK, REPEATED, MESSAGE,  segments,          6)
#define segments_SegmentsFrame_CALLBACK pb_default_field_callback
#define segments_SegmentsFrame_DEFAULT NULL
#define segments_SegmentsFrame_segments_MSGTYPE segments_Segment

extern const pb_msgdesc_t segments_Segment_msg;
extern const pb_msgdesc_t segments_SegmentsFrame_msg;

/* Defines for backwards compatibility with code written before nanopb-0.4.0 */
#define segments_Segment_fields &segments_Segment_msg
#define segments_SegmentsFrame_fields &segments_SegmentsFrame_msg

/* Maximum encoded size of messages (where known) */
/* segments_SegmentsFrame_size depends on runtime parameters */
#define SEGMENTS_SEGMENTS_PB_H_MAX_SIZE          segments_Segment_size
#define segments_Segment_size                    6

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif

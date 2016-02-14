/*
 * Simple Dalvik Virtual Machine Project
 * This Project is created for elementary class
 * to help student to understand How it works.
 *
 * Auther : Chun-Yu Wang
 * Email  : wicanr2@gmail.com
 * Created Date : 2013/11/14
 */
#ifndef SIMPLE_DVM_H
#define SIMPLE_DVM_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef short u2;
typedef unsigned int u4;
typedef unsigned int uint;
typedef unsigned char u1;
typedef unsigned short ushort;
typedef unsigned char byte;

//---------------------------------
// map_list
typedef struct _map_item {
    ushort  type;
    ushort  unused;
    uint    size;
    uint    offset;
} map_item;
typedef struct _map_list {
    uint size ;
    map_item *map_item;
} map_list;

typedef struct _type_item {
    ushort type_idx;
} type_item;
typedef struct _type_list {
    uint size ;
    type_item *type_item;
} type_list;
//---------------------------------
// string_ids
typedef struct _string_data_item { 
    int index;
    int uleb128_len ;
    u1  data[255];
} string_data_item;

typedef struct _string_ids {
    u4  string_data_off;
} string_ids;

//---------------------------------
// type_ids
typedef struct _type_id_item {
    uint descriptor_idx;
} type_id_item;

//---------------------------------
// proto_ids
typedef struct _proto_id_item {
    uint shorty_idx; // pointer to string_data_item
    uint return_type_idx; // pointer to type_ids
    uint parameters_off;
} proto_id_item;
//---------------------------------
// field_ids
typedef struct _field_id_item {
    ushort class_idx; /*index into the type_ids list for the definer of this field.*/
    ushort type_idx;
    uint   name_idx;
} field_id_item;
//---------------------------------
// method_ids
typedef struct _method_id_tiem {
    ushort  class_idx; /*index into the type_ids list for the definer of this method.*/
    ushort  proto_idx; /*index into the proto_ids list for the prototype of this method*/
    uint    name_idx;
} method_id_item;
//---------------------------------
// class defs
typedef struct _code_item {
    ushort registers_size;
    ushort ins_size;
    ushort outs_size;
    ushort tries_size;
    uint   debug_info_off;
    uint   insns_size;
    ushort *insns;
    /*
    ushort padding;
    try_item 
    handlers
    */
} code_item;

typedef struct _encoded_method {
    uint method_idx_diff;
    uint access_flags;
    uint code_off;
    code_item code_item;
} encoded_method ;

typedef struct _class_def_item {
    uint class_idx;
    uint access_flags;
    uint superclass_idx; /* index into typeIds for superclass */
    uint interfaces_off; /* file offset to DexTypeList */
    uint source_file_idx; /* index into stringIds for source file name */
    uint annotations_off; /* file offset to annotations_directory_item */
    uint class_data_off; /* file offset to class_data_item */
    uint static_values_off; /* file offset to DexEncodedArray */
} class_def_item;

typedef struct _class_data_item {
    uint static_fields_size;
    uint instance_fields_size;
    uint direct_methods_size;
    uint virtual_methods_size;

    encoded_method *direct_methods; 
    encoded_method *virtual_methods;
}class_data_item ;
//---------------------------------


typedef struct _DexHeader {
    u1 magic[8]; /* includes version number */
    u1 checksum[4]; /* adler32 checksum */
    u1 signature[20]; /* SHA-1 hash */
    u4 fileSize; /* length of entire file */
    u4 headerSize; /* offset to start of next section */
    u4 endianTag;
    u4 linkSize;
    u4 linkOff;
    u4 mapOff;
    u4 stringIdsSize;
    u4 stringIdsOff;
    u4 typeIdsSize;
    u4 typeIdsOff;
    u4 protoIdsSize;
    u4 protoIdsOff;
    u4 fieldIdsSize;
    u4 fieldIdsOff;
    u4 methodIdsSize;
    u4 methodIdsOff;
    u4 classDefsSize;
    u4 classDefsOff;
    u4 dataSize;
    u4 dataOff;
} DexHeader;


typedef struct DexFileFormat {
    DexHeader header;
    string_ids       *string_ids;
    string_data_item *string_data_item;
    type_id_item     *type_id_item;
    proto_id_item    *proto_id_item;
    type_list        *proto_type_list;
    field_id_item    *field_id_item;
    method_id_item   *method_id_item;
    class_def_item   *class_def_item;
    class_data_item  *class_data_item;
    map_list         map_list;
    type_list        type_list;
    u1               *data;
} DexFileFormat;

//---------------------------------
/*
 * Dex File Parser
 */
int parseDexFile( char *file, DexFileFormat *dex );
void printDexFile(DexFileFormat *dex );

/*
 * map list parser
 */
void parse_map_list(DexFileFormat *dex, unsigned char *buf, int offset);
/*
 * String ids parser
 */
void parse_string_ids(DexFileFormat *dex, unsigned char *buf, int offset);
string_data_item* get_string_data_item( DexFileFormat *dex, int string_id );
char* get_string_data( DexFileFormat *dex, int string_id );
/*
 * type_ids parser
 */
void parse_type_ids(DexFileFormat *dex, unsigned char *buf, int offset);
type_id_item* get_type_item( DexFileFormat *dex, int type_id );
char * get_type_item_name( DexFileFormat *dex, int type_id );
/*
 * proto_ids parser
 */
void parse_proto_ids(DexFileFormat *dex, unsigned char *buf, int offset);
proto_id_item* get_proto_item( DexFileFormat *dex, int proto_id );
type_list * get_proto_type_list ( DexFileFormat *dex, int proto_id );
/*
 * field_ids parser
 */
void parse_field_ids(DexFileFormat *dex, unsigned char *buf, int offset);
field_id_item* get_field_item( DexFileFormat *dex, int field_id );
/*
 * method ids parser
 */
void parse_method_ids(DexFileFormat *dex, unsigned char *buf, int offset);
method_id_item* get_method_item( DexFileFormat *dex, int method_id );
/*
 * class defs parser
 */
void parse_class_defs(DexFileFormat *dex, unsigned char *buf, int offset);

int get_uleb128_len ( unsigned char *buf, int offset, int *size );
//---------------------------------
/*
 * generic parameter parser for 35c
 */
typedef struct _invoke_parameters {
    int method_id ;
    int reg_count ;
    int reg_idx[5] ; // 0-5 map C-G
} invoke_parameters ;

// Dalvik VM Register Bank
typedef struct _simple_dvm_register {
    u1 data[4];
} simple_dvm_register;
typedef struct _simple_dalvik_vm {
    u1 heap[8192];
    u1 object_ref[4];
    simple_dvm_register regs[32];
    invoke_parameters p ;
    u1 result[8];
    uint pc;
    
} simple_dalvik_vm;

// convert to int ok
void load_reg_to( simple_dalvik_vm *vm, int id, unsigned char *ptr); 
void load_reg_to_double( simple_dalvik_vm *vm, int id, unsigned char *ptr);
void load_result_to_double ( simple_dalvik_vm *vm, unsigned char *ptr);

void store_to_reg ( simple_dalvik_vm *vm, int id, unsigned char *ptr);
void store_double_to_reg ( simple_dalvik_vm *vm, int id, unsigned char *ptr);
void store_double_to_result ( simple_dalvik_vm *vm, unsigned char *ptr);

void move_top_half_result_to_reg ( simple_dalvik_vm *vm, int id);
void move_bottom_half_result_to_reg ( simple_dalvik_vm *vm, int id);

void simple_dvm_startup(DexFileFormat *dex, simple_dalvik_vm *vm, char *entry);
int find_const_string(DexFileFormat *dex, char *entry);
void runMethod (DexFileFormat *dex, simple_dalvik_vm *vm, encoded_method *m );

typedef int (*opCodeFunc) (DexFileFormat *dex, simple_dalvik_vm *vm, u1 *ptr, int *pc );

typedef struct _byteCode {
    char *name;
    unsigned char opCode;
    int offset;
    opCodeFunc func; 
} byteCode;

int is_verbose();
int enable_verbose();
int disable_verbose();
int set_verbose(int l);
#endif


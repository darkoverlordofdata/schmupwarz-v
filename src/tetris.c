 #ifndef V_COMMIT_HASH
 #define V_COMMIT_HASH "47f5e11"
 #endif
 #include <inttypes.h>
 

//#include <inttypes.h>  // int64_t etc
//#include <stdint.h>  // int64_t etc

//================================== TYPEDEFS ================================*/

typedef int64_t i64;
typedef int16_t i16;
typedef int8_t i8;
typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t byte;
typedef uint32_t rune;
typedef float f32;
typedef double f64;
typedef unsigned char* byteptr;
typedef int* intptr;
typedef void* voidptr;
typedef char* charptr;
typedef struct array array;
typedef struct map map;
typedef array array_string;
typedef array array_int;
typedef array array_byte;
typedef array array_f32;
typedef array array_f64;
typedef array array_u16;
typedef array array_u32;
typedef array array_u64;
typedef map map_int;
typedef map map_string;
#ifndef bool
	typedef int bool;
	#define true 1
	#define false 0
#endif

 

//#include <inttypes.h>  // int64_t etc
#include <stdio.h>  // TODO remove all these includes, define all function signatures and types manually
#include <stdlib.h>

//#include "fns.h"
#include <signal.h>
#include <stdarg.h> // for va_list
#include <string.h> // memcpy

#if INTPTR_MAX == INT32_MAX
    #define TARGET_IS_32BIT 1
#elif INTPTR_MAX == INT64_MAX
    #define TARGET_IS_64BIT 1
#else
    #error "The environment is not 32 or 64-bit."
#endif

#if defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__ || defined(__BYTE_ORDER) && __BYTE_ORDER == __BIG_ENDIAN || defined(__BIG_ENDIAN__) || defined(__ARMEB__) || defined(__THUMBEB__) || defined(__AARCH64EB__) || defined(_MIBSEB) || defined(__MIBSEB) || defined(__MIBSEB__)
    #define TARGET_ORDER_IS_BIG
#elif defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__ || defined(__BYTE_ORDER) && __BYTE_ORDER == __LITTLE_ENDIAN || defined(__LITTLE_ENDIAN__) || defined(__ARMEL__) || defined(__THUMBEL__) || defined(__AARCH64EL__) || defined(_MIPSEL) || defined(__MIPSEL) || defined(__MIPSEL__) || defined(_M_AMD64) || defined(_M_X64) || defined(_M_IX86)
    #define TARGET_ORDER_IS_LITTLE
#else
    #error "Unknown architecture endianness"
#endif

#ifndef _WIN32
#include <ctype.h>
#include <locale.h> // tolower
#include <sys/time.h>
#include <unistd.h> // sleep
#else
#if defined(_MSC_VER)
#pragma comment(lib, "Dbghelp.lib")
#endif
#if defined(__MSVCRT_VERSION__) && __MSVCRT_VERSION__ < __MSVCR90_DLL
#error Please upgrade your MinGW distribution to use msvcr90.dll or later.
#endif
#endif

#if defined(__CYGWIN__) && !defined(_WIN32)
#error Cygwin is not supported, please use MinGW or Visual Studio.
#endif


#ifdef __linux__
#include <sys/types.h>
#include <sys/wait.h> // os__wait uses wait on nix
#endif

#ifdef __FreeBSD__
#include <sys/types.h>
#include <sys/wait.h> // os__wait uses wait on nix
#endif

#ifdef __DragonFly__
#include <sys/types.h>
#include <sys/wait.h> // os__wait uses wait on nix
#endif

#ifdef __OpenBSD__
#include <sys/types.h>
#include <sys/resource.h>
#include <sys/wait.h> // os__wait uses wait on nix
#endif

#define EMPTY_STRUCT_DECLARATION
#define EMPTY_STRUCT_INITIALIZATION 0
// Due to a tcc bug, the length of an array needs to be specified, but GCC crashes if it is...
#define EMPTY_ARRAY_OF_ELEMS(x,n) (x[])
#define TCCSKIP(x) x

#ifdef __TINYC__
#undef EMPTY_STRUCT_DECLARATION
#undef EMPTY_STRUCT_INITIALIZATION
#define EMPTY_STRUCT_DECLARATION char _dummy
#define EMPTY_STRUCT_INITIALIZATION 0
#undef EMPTY_ARRAY_OF_ELEMS
#define EMPTY_ARRAY_OF_ELEMS(x,n) (x[n])
#undef TCCSKIP
#define TCCSKIP(x)
#endif

#define OPTION_CAST(x) (x)

#ifdef _WIN32
#define WINVER 0x0600
#ifdef _WIN32_WINNT
#undef _WIN32_WINNT
#endif
#define _WIN32_WINNT 0x0600
#define WIN32_LEAN_AND_MEAN
#define _UNICODE
#define UNICODE
#include <windows.h>

// must be included after <windows.h>
#ifndef __TINYC__
#include <shellapi.h>
#endif

#include <io.h> // _waccess
#include <fcntl.h> // _O_U8TEXT
#include <direct.h> // _wgetcwd
//#include <WinSock2.h>
#ifdef _MSC_VER
// On MSVC these are the same (as long as /volatile:ms is passed)
#define _Atomic volatile

// MSVC cannot parse some things properly
#undef EMPTY_STRUCT_DECLARATION
#undef OPTION_CAST

#define EMPTY_STRUCT_DECLARATION int ____dummy_variable
#define OPTION_CAST(x)
#endif

#else
#include <pthread.h>
#endif


//============================== HELPER C MACROS =============================*/
#define _PUSH(arr, val, tmp, tmp_typ) {tmp_typ tmp = (val); array_push(arr, &tmp);}
#define _PUSH_MANY(arr, val, tmp, tmp_typ) {tmp_typ tmp = (val); array_push_many(arr, tmp.data, tmp.len);}
#define _IN(typ, val, arr) array_##typ##_contains(arr, val)
#define _IN_MAP(val, m) map_exists(m, val)
#define DEFAULT_EQUAL(a, b) (a == b)
#define DEFAULT_NOT_EQUAL(a, b) (a != b)
#define DEFAULT_LT(a, b) (a < b)
#define DEFAULT_LE(a, b) (a <= b)
#define DEFAULT_GT(a, b) (a > b)
#define DEFAULT_GE(a, b) (a >= b)
//================================== GLOBALS =================================*/
byteptr g_str_buf;
int load_so(byteptr);
void reload_so();

 int g_test_oks = 0;
 int g_test_fails = 0;
#include <float.h>
#include "glad.h"
#include <math.h>
#include <sys/stat.h>
#include <errno.h>
#ifndef _WIN32
#include <dirent.h>
#endif
#ifndef _WIN32
#include <unistd.h>
#endif
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <GLFW/glfw3.h>
#include <time.h>
#include "ft2build.h"
#include FT_FREETYPE_H
typedef struct array array;
typedef array array_string;
typedef array array_bool;
typedef array array_byte;
typedef array array_int;
typedef array array_char;
typedef array array_T;
typedef array array_i64;
typedef array array_f32;
typedef struct hashmap hashmap;
typedef array array_hashmapentry;
typedef struct hashmapentry hashmapentry;
typedef struct map map;
typedef struct mapnode mapnode;
typedef struct Option Option;
typedef struct string string;
typedef struct ustring ustring;
typedef Option Option_int;
typedef struct gx__Color gx__Color;
typedef struct gx__TextCfg gx__TextCfg;
typedef struct gx__Image gx__Image;
typedef struct gl__Shader gl__Shader;
typedef struct strings__Builder strings__Builder;
typedef struct rand__Pcg32 rand__Pcg32;
typedef struct rand__Splitmix64 rand__Splitmix64;
typedef struct os__File os__File;
typedef struct os__FileInfo os__FileInfo;
typedef Option Option_array_byte;
typedef Option Option_string;
typedef Option Option_bool;
typedef Option Option_bool;
typedef Option Option_bool;
typedef Option Option_array_string;
typedef array array_ustring;
typedef Option Option_array_ustring;
typedef Option Option_os__File;
typedef Option Option_os__File;
typedef Option Option_os__File;
typedef struct _V_MulRet_int_V_bool _V_MulRet_int_V_bool;
typedef struct os__Result os__Result;
typedef struct varg_string varg_string;

typedef Option Option_array_string;
typedef Option Option_bool;
typedef Option Option_os__Result;
typedef struct stbi__Image stbi__Image;
typedef struct glfw__TmpGlImportHack glfw__TmpGlImportHack;
typedef struct glfw__WinCfg glfw__WinCfg;
typedef struct glfw__Window glfw__Window;
typedef struct glfw__Size glfw__Size;
typedef struct glfw__Pos glfw__Pos;
typedef  void (*clickpubfn)( void* window, int button, int action, int mods /*FFF*/ ); //type alias name="clickpubfn" parent=`fn (void*,int,int,int)`
typedef struct glm__Mat4 glm__Mat4;
typedef struct glm__Vec2 glm__Vec2;
typedef struct glm__Vec3 glm__Vec3;
typedef struct time__Time time__Time;
typedef Option Option_int;
typedef struct gg__Vec2 gg__Vec2;
typedef struct gg__Cfg gg__Cfg;
typedef struct gg__GG gg__GG;
typedef struct freetype__Character freetype__Character;
typedef struct freetype__FreeType freetype__FreeType;
typedef array array_freetype__Character;
typedef array array_array_int;
typedef array array_gx__Color;
typedef struct Block Block;
typedef struct Game Game;
typedef array array_Block;
typedef Option Option_int;
typedef int gl__TmpGlImportHack;
typedef Option Option_array_byte;
typedef Option Option_string;
typedef Option Option_bool;
typedef Option Option_bool;
typedef Option Option_bool;
typedef Option Option_array_string;
typedef Option Option_array_ustring;
typedef Option Option_os__File;
typedef Option Option_os__File;
typedef Option Option_os__File;
typedef Option Option_array_string;
typedef Option Option_bool;
typedef Option Option_os__Result;
typedef int time__FormatTime;
typedef int time__FormatDate;
typedef int time__FormatDelimiter;
typedef Option Option_int;
typedef int GameState;
struct string {
	byte* str;
	int len;
};

struct array {
	void* data;
	int len;
	int cap;
	int element_size;
};

struct map {
	int element_size;
	mapnode* root;
	int size;
};

struct Option {
	byte data  [300 ];
	string error;
	int ecode;
	bool ok;
	bool is_none;
};


//----
struct glm__Mat4 {
	f32* data;
};

struct Block {
	int x;
	int y;
};

struct Game {
	int score;
	GameState state;
	int pos_x;
	int pos_y;
	array_array_int field;
	array_Block tetro;
	array_Block tetros_cache;
	int tetro_idx;
	int rotation_idx;
	gg__GG* gg;
	freetype__FreeType* ft;
	bool font_loaded;
};

struct _V_MulRet_int_V_bool {
	int var_0;
	bool var_1;
};

struct hashmap {
	int cap;
	array_string keys;
	array_hashmapentry table;
	int elm_size;
	int nr_collisions;
};

struct gx__Color {
	int r;
	int g;
	int b;
};

struct gl__Shader {
	int program_id;
};

struct gg__Vec2 {
	int x;
	int y;
};

struct gg__Cfg {
	int width;
	int height;
	bool use_ortho;
	bool retina;
	bool resizable;
	int font_size;
	string font_path;
	bool create_window;
	void* window_user_ptr;
	string window_title;
	bool always_on_top;
	int scale;
};

struct glfw__TmpGlImportHack {
	gl__TmpGlImportHack hack;
};

struct glfw__Size {
	int width;
	int height;
};

struct glfw__Pos {
	int x;
	int y;
};

struct glfw__WinCfg {
	int width;
	int height;
	string title;
	void* ptr;
	bool borderless;
	int is_modal;
	bool is_browser;
	string url;
	bool always_on_top;
};

struct glfw__Window {
	void* data;
	string title;
	int mx;
	int my;
};

struct glm__Vec2 {
	f32 x;
	f32 y;
};

struct glm__Vec3 {
	f32 x;
	f32 y;
	f32 z;
};

struct gx__Image {
	void* obj;
	int id;
	int width;
	int height;
};

struct hashmapentry {
	string key;
	int val;
	hashmapentry* next;
};

struct mapnode {
	mapnode* left;
	mapnode* right;
	bool is_empty;
	string key;
	void* val;
};

struct rand__Pcg32 {
	u64 state;
	u64 inc;
};

struct os__File {
	void* cfile;
	bool opened;
};

struct os__FileInfo {
	string name;
	int size;
};

struct os__Result {
	int exit_code;
	string output;
};

struct rand__Splitmix64 {
	u64 state;
};

struct stbi__Image {
	int width;
	int height;
	int nr_channels;
	bool ok;
	void* data;
	string ext;
};

struct strings__Builder {
	array_byte buf;
	int len;
};

struct time__Time {
	int year;
	int month;
	int day;
	int hour;
	int minute;
	int second;
	int uni;
};

struct ustring {
	string s;
	array_int runes;
	int len;
};

struct varg_string {
	int len;
	string args[1];
};

struct freetype__Character {
	u32 texture_id;
	gg__Vec2 size;
	gg__Vec2 bearing;
	u32 advance;
};

struct freetype__FreeType {
	gl__Shader shader;
	int width;
	int height;
	u32 vao;
	u32 rect_vao;
	u32 rect_vbo;
	u32 line_vao;
	u32 line_vbo;
	u32 vbo;
	array_freetype__Character chars;
	FT_Face face;
	int scale;
	array_string utf_runes;
	array_freetype__Character utf_chars;
};

struct gg__GG {
	gl__Shader shader;
	u32 vao;
	u32 rect_vao;
	u32 rect_vbo;
	u32 line_vao;
	u32 line_vbo;
	u32 vbo;
	int scale;
	int width;
	int height;
	glfw__Window* window;
	void (*render_fn)(  /*FFF*/ );
};

struct gx__TextCfg {
	gx__Color color;
	int size;
	int align;
	int max_width;
	string family;
	bool bold;
	bool mono;
};



string _STR(const char*, ...);


string _STR_TMP(const char*, ...);

array new_array(int mylen, int cap, int elm_size);
array make(int len, int cap, int elm_size);
array new_array_from_c_array(int len, int cap, int elm_size, void* c_array);
array new_array_from_c_array_no_alloc(int len, int cap, int elm_size, void* c_array);
void array_ensure_cap(array* a, int required);
array array_repeat_old(void* val, int nr_repeats, int elm_size);
array array_repeat(array a, int nr_repeats);
void array_sort_with_compare(array* a, void* compare);
void array_insert(array* a, int i, void* val);
void array_prepend(array* a, void* val);
void v_array_delete(array* a, int i);
void* array_get(array a, int i);
void* array_first(array a);
void* array_last(array a);
array array_left(array a, int n);
array array_right(array a, int n);
array array_slice2(array a, int start, int _end, bool end_max);
array array_slice(array a, int start, int _end);
void array_set(array* a, int i, void* val);
void array_push(array* a, void* val);
void array_push_many(array* a, void* val, int size);
array array_reverse(array a);
array array_clone(array a);
void v_array_free(array a);
string array_string_str(array_string a);
string array_bool_str(array_bool a);
string array_byte_hex(array_byte b);
int copy(array_byte dst, array_byte src);
int compare_ints(int* a, int* b);
void array_int_sort(array_int* a);
int array_string_index(array_string a, string v);
int array_int_index(array_int a, int v);
int array_byte_index(array_byte a, byte v);
int array_char_index(array_char a, char v);
int array_int_reduce(array_int a, int (*iter)( int accum, int curr /*FFF*/ ), int accum_start);
bool array_int_eq(array_int a, array_int a2);
bool array_i64_eq(array_i64 a, array_i64 a2);
bool array_string_eq(array_string a, array_string a2);
bool array_byte_eq(array_byte a, array_byte a2);
bool array_f32_eq(array_f32 a, array_f32 a2);
void builtin__init();
void v_exit(int code);
bool isnil(void* v);
void on_panic(int (*f)( int  /*FFF*/ ));
void print_backtrace_skipping_top_frames(int skipframes);
void print_backtrace();
void panic_debug(int line_no, string file, string mod, string fn_name, string s);
void v_panic(string s);
void println(string s);
void eprintln(string s);
void print(string s);
byte* v_malloc(int n);
byte* v_calloc(int n);
void v_free(void* ptr);
void* memdup(void* src, int sz);
void v_ptr_free(void* ptr);
int is_atty(int fd);
bool print_backtrace_skipping_top_frames_msvc(int skipframes);
bool print_backtrace_skipping_top_frames_mingw(int skipframes);
bool print_backtrace_skipping_top_frames_nix(int xskipframes);
bool print_backtrace_skipping_top_frames_mac(int skipframes);
bool print_backtrace_skipping_top_frames_freebsd(int skipframes);
bool print_backtrace_skipping_top_frames_linux(int skipframes);
int backtrace(void* a, int b);
byteptr* backtrace_symbols(void* , int );
void backtrace_symbols_fd(void* , int , int );
int proc_pidpath(int , void* , int );
string f64_str(f64 d);
string f32_str(f32 d);
f32 f32_abs(f32 a);
f64 f64_abs(f64 a);
bool f64_eq(f64 a, f64 b);
bool f32_eq(f32 a, f32 b);
bool f64_eqbit(f64 a, f64 b);
bool f32_eqbit(f32 a, f32 b);
bool f64_ne(f64 a, f64 b);
bool f32_ne(f32 a, f32 b);
bool f64_nebit(f64 a, f64 b);
bool f32_nebit(f32 a, f32 b);
bool f64_lt(f64 a, f64 b);
bool f32_lt(f32 a, f32 b);
bool f64_ltbit(f64 a, f64 b);
bool f32_ltbit(f32 a, f32 b);
bool f64_le(f64 a, f64 b);
bool f32_le(f32 a, f32 b);
bool f64_lebit(f64 a, f64 b);
bool f32_lebit(f32 a, f32 b);
bool f64_gt(f64 a, f64 b);
bool f32_gt(f32 a, f32 b);
bool f64_gtbit(f64 a, f64 b);
bool f32_gtbit(f32 a, f32 b);
bool f64_ge(f64 a, f64 b);
bool f32_ge(f32 a, f32 b);
bool f64_gebit(f64 a, f64 b);
bool f32_gebit(f32 a, f32 b);
hashmap new_hashmap(int planned_nr_items);
void hashmap_set(hashmap* m, string key, int val);
int hashmap_get(hashmap* m, string key);
static inline f64 b_fabs(int v);
string ptr_str(void* ptr);
string int_str(int nn);
string u32_str(u32 nn);
string i64_str(i64 nn);
string u64_str(u64 nn);
string bool_str(bool b);
string int_hex(int n);
string i64_hex(i64 n);
bool array_byte_contains(array_byte a, byte val);
string rune_str(rune c);
string byte_str(byte c);
bool byte_is_capital(byte c);
array_byte array_byte_clone(array_byte b);
map new_map(int cap, int elm_size);
map new_map_init(int cap, int elm_size, string* keys, void* vals);
mapnode* new_node(string key, void* val, int element_size);
void map_insert(map* m, mapnode* n, string key, void* val);
bool mapnode_find(mapnode* n, string key, void* out, int element_size);
bool mapnode_find2(mapnode* n, string key, int element_size);
void map_set(map* m, string key, void* val);
int preorder_keys(mapnode* node, array_string* keys, int key_i);
array_string map_keys(map* m);
bool map_get(map m, string key, void* out);
void v_mapnode_delete(mapnode* n, string key, int element_size);
void v_map_delete(map* m, string key);
bool map_exists(map m, string key);
void map_print(map m);
void v_mapnode_free(mapnode* n);
void v_map_free(map* m);
string map_string_str(map_string m);
Option opt_ok(void* data, int size);
Option opt_none();
Option v_error(string s);
Option error_with_code(string s, int code);
int vstrlen(byte* s);
string tos(byte* s, int len);
string tos_clone(byte* s);
string tos2(byte* s);
string tos3(charptr s);
string string_clone(string a);
string cstring_to_vstring(byte* cstr);
string string_replace_once(string s, string rep, string with);
string string_replace(string s, string rep, string with);
int v_string_int(string s);
i64 string_i64(string s);
f32 string_f32(string s);
f64 string_f64(string s);
u32 string_u32(string s);
u64 string_u64(string s);
bool string_eq(string s, string a);
bool string_ne(string s, string a);
bool string_lt(string s, string a);
bool string_le(string s, string a);
bool string_gt(string s, string a);
bool string_ge(string s, string a);
string string_add(string s, string a);
array_string string_split(string s, string delim);
array_string string_split_nth(string s, string delim, int nth);
array_string string_split_into_lines(string s);
string string_left(string s, int n);
string string_right(string s, int n);
string string_substr2(string s, int start, int _end, bool end_max);
string string_substr(string s, int start, int end);
int string_index_old(string s, string p);
Option_int string_index(string s, string p);
int string_index_kmp(string s, string p);
int string_index_any(string s, string chars);
int string_last_index(string s, string p);
int string_index_after(string s, string p, int start);
int string_index_byte(string s, byte c);
int string_last_index_byte(string s, byte c);
int string_count(string s, string substr);
bool string_contains(string s, string p);
bool string_starts_with(string s, string p);
bool string_ends_with(string s, string p);
string string_to_lower(string s);
string string_to_upper(string s);
string string_capitalize(string s);
string string_title(string s);
string string_find_between(string s, string start, string end);
bool array_string_contains(array_string ar, string val);
bool array_int_contains(array_int ar, int val);
bool byte_is_space(byte c);
string string_trim_space(string s);
string string_trim(string s, string cutset);
string string_trim_left(string s, string cutset);
string string_trim_right(string s, string cutset);
int compare_strings(string* a, string* b);
int compare_strings_by_len(string* a, string* b);
int compare_lower_strings(string* a, string* b);
void array_string_sort(array_string* s);
void array_string_sort_ignore_case(array_string* s);
void array_string_sort_by_len(array_string* s);
ustring string_ustring(string s);
ustring string_ustring_tmp(string s);
bool ustring_eq(ustring u, ustring a);
bool ustring_ne(ustring u, ustring a);
bool ustring_lt(ustring u, ustring a);
bool ustring_le(ustring u, ustring a);
bool ustring_gt(ustring u, ustring a);
bool ustring_ge(ustring u, ustring a);
ustring ustring_add(ustring u, ustring a);
int ustring_index_after(ustring u, ustring p, int start);
int ustring_count(ustring u, ustring substr);
string ustring_substr(ustring u, int _start, int _end);
string ustring_left(ustring u, int pos);
string ustring_right(ustring u, int pos);
byte string_at(string s, int idx);
string ustring_at(ustring u, int idx);
void v_ustring_free(ustring u);
bool byte_is_digit(byte c);
bool byte_is_hex_digit(byte c);
bool byte_is_oct_digit(byte c);
bool byte_is_letter(byte c);
void v_string_free(string s);
string string_all_before(string s, string dot);
string string_all_before_last(string s, string dot);
string string_all_after(string s, string dot);
string array_string_join(array_string a, string del);
string array_string_join_lines(array_string s);
string string_reverse(string s);
string string_limit(string s, int max);
bool byte_is_white(byte c);
int string_hash(string s);
array_byte string_bytes(string s);
string string_repeat(string s, int count);
int utf8_char_len(byte b);
string utf32_to_str(u32 code);
string utf32_to_str_no_malloc(u32 code, void* buf);
int string_utf32_code(string _rune);
u16* string_to_wide(string _str);
string string_from_wide(u16* _wstr);
string string_from_wide2(u16* _wstr, int len);
int utf8_len(byte c);
int utf8_getchar();
bool gx__Image_is_empty(gx__Image img);
string gx__Color_str(gx__Color c);
bool gx__Color_eq(gx__Color a, gx__Color b);
gx__Color gx__rgb(int r, int g, int b);
gl__Shader gl__new_shader(string name);
void gl__Shader_use(gl__Shader s);
int gl__Shader_uni_location(gl__Shader s, string key);
void gl__Shader_set_mat4(gl__Shader s, string str, glm__Mat4 m);
void gl__Shader_set_int(gl__Shader s, string str, int n);
void gl__Shader_set_color(gl__Shader s, string str, gx__Color c);
void gl__init_glad();
void gl__viewport(int a, int b, int c, int d);
void gl__clear_color(int r, int g, int b, int a);
void gl__clear();
int gl__create_shader(int typ);
int gl__create_program();
void gl__shader_source(int shader, int a, string source, int b);
void gl__compile_shader(int shader);
int gl__shader_compile_status(int shader);
void gl__attach_shader(int program, int shader);
void gl__link_program(int program);
int gl__get_program_link_status(int program);
void gl__delete_shader(int shader);
string gl__shader_info_log(int shader);
string gl__get_program_info_log(int program);
void gl__bind_vao(u32 vao);
void gl__bind_buffer(int typ, u32 vbo);
u32 gl__gen_texture();
void gl__active_texture(int t);
void gl__bind_2d_texture(u32 texture);
void gl__delete_texture(u32 texture);
void gl__buffer_data(int typ, int size, void* arr, int draw_typ);
void gl__buffer_data_int(int typ, array_int vertices, int draw_typ);
void gl__buffer_data_f32(int typ, array_f32 vertices, int draw_typ);
void gl__set_vbo(u32 vbo, array_f32 vertices, int draw_typ);
void gl__set_ebo(u32 ebo, array_int indices, int draw_typ);
void gl__draw_arrays(int typ, int start, int len);
void gl__draw_elements(int mode, int count, int typ, int indices);
void gl__use_program(int program);
u32 gl__gen_vertex_array();
void gl__enable_vertex_attrib_array(int n);
u32 gl__gen_buffer();
void gl__vertex_attrib_pointer(int index, int size, int typ, bool normalized, int _stride, int _ptr);
void gl__tex_param(int key, int val);
void gl__enable(int val);
void gl__disable(int val);
void gl__scissor(int a, int b, int c, int d);
void gl__generate_mipmap(int typ);
strings__Builder strings__new_builder(int initial_size);
void strings__Builder_write(strings__Builder* b, string s);
void strings__Builder_writeln(strings__Builder* b, string s);
string strings__Builder_str(strings__Builder* b);
void strings__Builder_free(strings__Builder* b);
int strings__levenshtein_distance(string a, string b);
f32 strings__levenshtein_distance_percentage(string a, string b);
f32 strings__dice_coefficient(string s1, string s2);
string strings__repeat(byte c, int n);
byte strconv__byte_to_lower(byte c);
u64 strconv__common_parse_uint(string s, int _base, int _bit_size, bool error_on_non_digit, bool error_on_high_digit);
u64 strconv__parse_uint(string s, int _base, int _bit_size);
i64 strconv__common_parse_int(string _s, int base, int _bit_size, bool error_on_non_digit, bool error_on_high_digit);
i64 strconv__parse_int(string _s, int base, int _bit_size);
int strconv__atoi(string s);
bool strconv__underscore_ok(string s);
rand__Pcg32 rand__new_pcg32(u64 initstate, u64 initseq);
static inline u32 rand__Pcg32_next(rand__Pcg32* rng);
static inline u32 rand__Pcg32_bounded_next(rand__Pcg32* rng, u32 bound);
void rand__seed(int s);
int rand__next(int max);
int rand__rand_r(int* seed);
rand__Splitmix64 rand__new_splitmix64(u64 seed);
static inline u64 rand__Splitmix64_next(rand__Splitmix64* rng);
static inline u64 rand__Splitmix64_bounded_next(rand__Splitmix64* rng, u64 bound);
f64 math__inf(int sign);
f64 math__nan();
bool math__is_nan(f64 f);
bool math__is_inf(f64 f, int sign);
f64 math__abs(f64 a);
f64 math__acos(f64 a);
f64 math__asin(f64 a);
f64 math__atan(f64 a);
f64 math__atan2(f64 a, f64 b);
f64 math__cbrt(f64 a);
f64 math__ceil(f64 a);
f64 math__cos(f64 a);
f64 math__cosh(f64 a);
f64 math__degrees(f64 radians);
f64 math__exp(f64 a);
array_int math__digits(int _n, int base);
f64 math__erf(f64 a);
f64 math__erfc(f64 a);
f64 math__exp2(f64 a);
f64 math__floor(f64 a);
f64 math__fmod(f64 a, f64 b);
f64 math__gamma(f64 a);
i64 math__gcd(i64 a_, i64 b_);
f64 math__hypot(f64 a, f64 b);
i64 math__lcm(i64 a, i64 b);
f64 math__log(f64 a);
f64 math__log2(f64 a);
f64 math__log10(f64 a);
f64 math__log_gamma(f64 a);
f64 math__log_n(f64 a, f64 b);
f64 math__max(f64 a, f64 b);
f64 math__min(f64 a, f64 b);
f64 math__pow(f64 a, f64 b);
f64 math__radians(f64 degrees);
f64 math__round(f64 f);
f64 math__sin(f64 a);
f64 math__sinh(f64 a);
f64 math__sqrt(f64 a);
f64 math__tan(f64 a);
f64 math__tanh(f64 a);
f64 math__trunc(f64 a);
u32 math__f32_bits(f32 f);
f32 math__f32_from_bits(u32 b);
u64 math__f64_bits(f64 f);
f64 math__f64_from_bits(u64 b);
bool os__File_is_opened(os__File f);
array_byte os__File_read_bytes(os__File* f, int size);
array_byte os__File_read_bytes_at(os__File* f, int size, int pos);
Option_array_byte os__read_bytes(string path);
Option_string os__read_file(string path);
int os__file_size(string path);
void os__mv(string old, string new);
Option_bool os__cp(string old, string new);
Option_bool os__cp_r(string osource_path, string odest_path, bool overwrite);
Option_bool os__mv_by_cp(string source, string target);
FILE* os__vfopen(string path, string mode);
Option_array_string os__read_lines(string path);
Option_array_ustring os__read_ulines(string path);
Option_os__File os__open(string path);
Option_os__File os__create(string path);
Option_os__File os__open_append(string path);
void os__File_write(os__File* f, string s);
void os__File_write_bytes(os__File* f, void* data, int size);
void os__File_write_bytes_at(os__File* f, void* data, int size, int pos);
void os__File_writeln(os__File* f, string s);
void os__File_flush(os__File* f);
void os__File_close(os__File* f);
void* os__vpopen(string path);
_V_MulRet_int_V_bool os__posix_wait4_to_exit_status(int waitret);
int os__vpclose(void* f);
int os__system(string cmd);
string os__sigint_to_signal_name(int si);
string os__getenv(string key);
int os__setenv(string name, string value, bool overwrite);
int os__unsetenv(string name);
bool os__file_exists(string _path);
void os__rm(string path);
void os__rmdir(string path);
void os__print_c_errno();
string os__ext(string path);
string os__dir(string path);
string os__path_sans_ext(string path);
string os__basedir(string path);
string os__filename(string path);
string os__get_line();
string os__get_raw_line();
array_string os__get_lines();
string os__get_lines_joined();
string os__user_os();
string os__home_dir();
void os__write_file(string path, string text);
void os__clear();
void os__on_segfault(void* f);
string os__executable();
bool os__is_dir(string path);
void os__chdir(string path);
string os__getwd();
string os__realpath(string fpath);
array_string os__walk_ext(string path, string ext);
void os__walk(string path, void (*fnc)( string path /*FFF*/ ));
void os__signal(int signum, void* handler);
int os__fork();
int os__wait();
int os__file_last_mod_unix(string path);
void os__log(string s);
void os__flush_stdout();
void os__print_backtrace();
void os__mkdir_all(string path);
string os__join(string base, varg_string *dirs);
string os__tmpdir();
void os__chmod(string path, int mode);
array_string os__init_os_args(int argc, byteptr* argv);
string os__get_error_msg(int code);
Option_array_string os__ls(string path);
bool os__dir_exists(string path);
Option_bool os__mkdir(string path);
Option_os__Result os__exec(string cmd);
stbi__Image stbi__load(string path);
void stbi__Image_free(stbi__Image img);
void stbi__Image_tex_image_2d(stbi__Image img);
void stbi__set_flip_vertically_on_load(bool val);
string filepath__ext(string path);
bool filepath__is_abs(string path);
string filepath__join(string base, varg_string *dirs);
void glfw__init_glfw();
void glfw__Window_destroy(glfw__Window* w);
void glfw__terminate();
void glfw__mouse_move(void* w, f64 x, f64 y);
void glfw__window_hint(int key, int val);
glfw__Window* glfw__create_window(glfw__WinCfg c);
void glfw__Window_set_title(glfw__Window* w, string title);
void glfw__Window_make_context_current(glfw__Window* w);
void glfw__swap_interval(int interval);
void glfw__wait_events();
void glfw__poll_events();
void glfw__set_should_close(void* w, bool close);
bool glfw__Window_should_close(glfw__Window* w);
void glfw__Window_swap_buffers(glfw__Window* w);
void glfw__Window_onmousemove(glfw__Window* w, void* cb);
void glfw__Window_set_mouse_button_callback(glfw__Window* w, void* cb);
void glfw__Window_on_click(glfw__Window* w, void* cb);
void glfw__Window_set_scroll_callback(glfw__Window* w, void* cb);
void glfw__Window_on_scroll(glfw__Window* w, void* cb);
void glfw__post_empty_event();
void glfw__Window_onkeydown(glfw__Window* w, void* cb);
void glfw__Window_onchar(glfw__Window* w, void* cb);
f64 glfw__get_time();
bool glfw__key_pressed(void* wnd, int key);
string glfw__Window_get_clipboard_text(glfw__Window* w);
void glfw__Window_set_clipboard_text(glfw__Window* w, string s);
glfw__Pos glfw__Window_get_cursor_pos(glfw__Window* w);
void* glfw__Window_user_ptr(glfw__Window* w);
void glfw__Window_set_user_ptr(glfw__Window* w, void* ptr);
glfw__Size glfw__get_monitor_size();
string glfw__Size_str(glfw__Size size);
void* glfw__get_window_user_pointer(void* gwnd);
glm__Vec3 glm__vec3(f32 x, f32 y, f32 z);
glm__Mat4 glm__mat4(f32* f);
string glm__Vec3_str(glm__Vec3 v);
string glm__Vec2_str(glm__Vec2 v);
string glm__Mat4_str(glm__Mat4 m);
glm__Vec2 glm__vec2(int x, int y);
glm__Vec3 glm__Vec3_add(glm__Vec3 a, glm__Vec3 b);
glm__Vec3 glm__Vec3_sub(glm__Vec3 a, glm__Vec3 b);
glm__Vec3 glm__Vec3_mult_scalar(glm__Vec3 a, f32 b);
void glm__Vec3_print(glm__Vec3 a);
f32* glm__f32_calloc(int n);
glm__Mat4 glm__translate(glm__Mat4 m, glm__Vec3 v);
glm__Mat4 glm__ortho(f32 left, f32 right, f32 bottom, f32 top);
glm__Mat4 glm__scale(glm__Mat4 m, glm__Vec3 v);
glm__Mat4 glm__rotate_z(glm__Mat4 m, f32 rad);
glm__Mat4 glm__identity();
void glm__identity2(f32** res);
array_f32 glm__identity3();
f32* glm__ortho_js(f32 left, f32 right, f32 bottom, f32 top);
glm__Vec3 glm__cross(glm__Vec3 a, glm__Vec3 b);
void time__remove_me_when_c_bug_is_fixed();
time__Time time__now();
time__Time time__random();
time__Time time__unix(int abs);
time__Time time__convert_ctime(struct /*TM*/ tm t);
string time__Time_format_ss(time__Time t);
string time__Time_format(time__Time t);
string time__Time_smonth(time__Time t);
string time__Time_hhmm(time__Time t);
string time__Time_hhmm12(time__Time t);
string time__Time_hhmmss(time__Time t);
string time__Time_ymmdd(time__Time t);
string time__Time_ddmmy(time__Time t);
string time__Time_md(time__Time t);
string time__Time_clean(time__Time t);
string time__Time_clean12(time__Time t);
time__Time time__parse(string s);
time__Time time__new_time(time__Time t);
int time__Time_calc_unix(time__Time* t);
time__Time time__Time_add_seconds(time__Time t, int seconds);
time__Time time__Time_add_days(time__Time t, int days);
int time__since(time__Time t);
string time__Time_relative(time__Time t);
int time__day_of_week(int y, int m, int d);
int time__Time_day_of_week(time__Time t);
string time__Time_weekday_str(time__Time t);
i64 time__ticks();
void time__sleep(int seconds);
void time__usleep(int n);
void time__sleep_ms(int n);
bool time__is_leap_year(int year);
Option_int time__days_in_month(int month, int year);
string time__Time_get_fmt_time_str(time__Time t, time__FormatTime fmt_time);
string time__Time_get_fmt_date_str(time__Time t, time__FormatDelimiter fmt_dlmtr, time__FormatDate fmt_date);
string time__Time_get_fmt_str(time__Time t, time__FormatDelimiter fmt_dlmtr, time__FormatTime fmt_time, time__FormatDate fmt_date);
gg__Vec2 gg__vec2(int x, int y);
void gg__init_gg();
gg__GG* gg__new_context(gg__Cfg cfg);
void gg__clear(gx__Color color);
void gg__GG_render(gg__GG* gg);
void gg__GG_draw_triangle(gg__GG* ctx, f32 x1, f32 y1, f32 x2, f32 y2, f32 x3, f32 y3, gx__Color c);
void gg__GG_draw_triangle_tex(gg__GG* ctx, f32 x1, f32 y1, f32 x2, f32 y2, f32 x3, f32 y3, gx__Color c);
void gg__GG_draw_rect(gg__GG* ctx, f32 x, f32 y, f32 w, f32 h, gx__Color c);
void gg__GG_draw_rect2(gg__GG* ctx, f32 x, f32 y, f32 w, f32 h, gx__Color c);
void gg__todo_remove_me(gg__Cfg cfg, int scale);
void gg__update();
void gg__post_empty_event();
void gg__GG_circle(gg__GG c, int x, int y, int r);
void gg__GG_fill_color(gg__GG c, gx__Color color);
void gg__GG_fill(gg__GG c);
void gg__GG_move_to(gg__GG c, int x, int y);
void gg__GG_line_to(gg__GG c, int x, int y);
void gg__GG_stroke_width(gg__GG c, int size);
void gg__GG_stroke_color(gg__GG c, gx__Color color);
void gg__GG_stroke(gg__GG c);
void gg__GG_save(gg__GG c);
void gg__GG_restore(gg__GG c);
void gg__GG_intersect_scissor(gg__GG c, int x, int y, int w, int h);
void gg__GG_translate(gg__GG c, int x, int y);
int gg__GG_create_font(gg__GG c, string name, string file);
void gg__GG_text(gg__GG c, int x, int y, string text);
void gg__GG_text_box(gg__GG c, int x, int y, int max, string text);
void gg__GG_font_face(gg__GG c, string f);
void gg__GG_font_size(gg__GG c, int size);
void gg__GG_text_align(gg__GG c, int a);
u32 gg__create_image(string file);
void gg__GG_draw_line_c(gg__GG* ctx, f32 x, f32 y, f32 x2, f32 y2, gx__Color color);
void gg__GG_draw_line(gg__GG* c, f32 x, f32 y, f32 x2, f32 y2);
void gg__GG_draw_vertical(gg__GG* c, int x, int y, int height);
void gg__GG_draw_image(gg__GG* ctx, f32 x, f32 y, f32 w, f32 h, u32 tex_id);
void gg__GG_draw_empty_rect(gg__GG* c, int x, int y, int w, int h, gx__Color color);
freetype__Character freetype__ft_load_char(FT_Face face, i64 code);
freetype__FreeType* freetype__new_context(gg__Cfg cfg);
void freetype__FreeType_draw_text(freetype__FreeType* ctx, int _x, int _y, string text, gx__TextCfg cfg);
void freetype__FreeType_draw_text_fast(freetype__FreeType* ctx, int _x, int _y, ustring text, gx__TextCfg cfg);
void freetype__FreeType_private_draw_text(freetype__FreeType* ctx, int _x, int _y, ustring utext, gx__TextCfg cfg);
void freetype__FreeType_draw_text_def(freetype__FreeType* ctx, int x, int y, string text);
void main__main();
void Game_init_game(Game* g);
void Game_parse_tetros(Game* g);
void Game_run(Game* g);
void Game_move_tetro(Game* g);
bool Game_move_right(Game* g, int dx);
void Game_delete_completed_lines(Game* g);
void Game_delete_completed_line(Game* g, int y);
void Game_generate_tetro(Game* g);
void Game_get_tetro(Game* g);
void Game_drop_tetro(Game* g);
void Game_draw_tetro(Game* g);
void Game_draw_block(Game* g, int i, int j, int color_idx);
void Game_draw_field(Game* g);
void Game_draw_ui(Game* g);
void Game_draw_scene(Game* g);
array_Block main__parse_binary_tetro(int t_);
void main__key_down(void* wnd, int key, int code, int action, int mods);
bool array_eq_T_int(array_int a1, array_int a2);
bool array_eq_T_i64(array_i64 a1, array_i64 a2);
bool array_eq_T_string(array_string a1, array_string a2);
bool array_eq_T_byte(array_byte a1, array_byte a2);
bool array_eq_T_f32(array_f32 a1, array_f32 a2);

i64 total_m =  0; // global
int builtin__min_cap;
int builtin__max_cap;
array_int g_ustring_runes; // global
#define builtin__CP_UTF8  65001
gx__Color gx__Blue;
gx__Color gx__Red;
gx__Color gx__Green;
gx__Color gx__green;
gx__Color gx__Yellow;
gx__Color gx__Orange;
gx__Color gx__Purple;
gx__Color gx__Black;
gx__Color gx__Gray;
gx__Color gx__Indigo;
gx__Color gx__Pink;
gx__Color gx__Violet;
gx__Color gx__White;
gx__Color gx__white;
gx__Color gx__DarkBlue;
gx__Color gx__DarkGray;
gx__Color gx__DarkGreen;
gx__Color gx__DarkRed;
gx__Color gx__LightBlue;
gx__Color gx__LightGray;
gx__Color gx__LightGreen;
gx__Color gx__LightRed;
#define gx__ALIGN_LEFT  1
#define gx__ALIGN_RIGHT  4
string gl__TEXT_VERT;
string gl__TEXT_FRAG;
string gl__SIMPLE_VERT;
string gl__SIMPLE_FRAG;
#define strconv__int_size  32
u64 strconv__max_u64;
u64 math__uvnan;
u64 math__uvinf;
u64 math__uvneginf;
u64 math__uvone;
int math__mask;
int math__shift;
#define math__bias  1023
u64 math__sign_mask;
u64 math__frac_mask;
#define math__e  2.71828182845904523536028747135266249775724709369995957496696763
#define math__pi  3.14159265358979323846264338327950288419716939937510582097494459
#define math__phi  1.61803398874989484820458683436563811772030917980576286213544862
#define math__tau  6.28318530717958647692528676655900576839433879875021164194988918
#define math__sqrt2  1.41421356237309504880168872420969807856967187537694807317667974
#define math__sqrt_e  1.64872127070012814684865078781416357165377610071014801157507931
#define math__sqrt_pi  1.77245385090551602729816748334114518279754945612238712821380779
#define math__sqrt_tau  2.50662827463100050241576528481104525300698674060993831662992357
#define math__sqrt_phi  1.27201964951406896425242246173749149171560804184009624861664038
#define math__ln2  0.693147180559945309417232121458176568075500134360255254120680009
f32 math__log2_e;
#define math__ln10  2.30258509299404568401799145468436420760110148862877297603332790
f32 math__log10_e;
f32 math__max_f32;
f32 math__smallest_non_zero_f32;
f32 math__max_f64;
f32 math__smallest_non_zero_f64;
#define math__max_i8  127
int math__min_i8;
#define math__max_i16  32767
int math__min_i16;
#define math__max_i32  2147483647
int math__min_i32;
u64 math__min_i64;
#define math__max_i64  9223372036854775807
#define math__max_u8  255
#define math__max_u16  65535
#define math__max_u32  4294967295
#define math__max_u64  18446744073709551615
#define os__O_RDONLY  1
#define os__O_WRONLY  2
#define os__O_RDWR  3
#define os__O_APPEND  8
#define os__O_CREATE  16
#define os__O_EXCL  32
#define os__O_SYNC  64
#define os__O_TRUNC  128
int os__S_IFMT;
int os__S_IFDIR;
int os__STD_INPUT_HANDLE;
int os__STD_OUTPUT_HANDLE;
int os__STD_ERROR_HANDLE;
array_string os__args;
#define os__MAX_PATH  4096
#define os__PROT_READ  1
#define os__PROT_WRITE  2
int os__MAP_PRIVATE;
int os__MAP_ANONYMOUS;
string os__path_separator;
#define glfw__RESIZABLE  1
#define glfw__DECORATED  2
#define glfw__KEY_ESCAPE  256
#define glfw__key_space  32
#define glfw__KEY_LEFT_SUPER  343
#define glfw__KeyUp  265
#define glfw__KeyLeft  263
#define glfw__KeyRight  262
#define glfw__KeyDown  264
string time__days_string;
array_int time__month_days;
string time__months_string;
i64 time__absolute_zero_year;
#define time__seconds_per_minute  60
int time__seconds_per_hour;
int time__seconds_per_day;
int time__seconds_per_week;
int time__days_per_400_years;
int time__days_per_100_years;
int time__days_per_4_years;
array_int time__days_before;
#define time__time__FormatTime_hhmm12 0
#define time__time__FormatTime_hhmm24 1
#define time__time__FormatTime_hhmmss12 2
#define time__time__FormatTime_hhmmss24 3
#define time__time__FormatTime_no_time 4
#define time__time__FormatDate_ddmmyy 0
#define time__time__FormatDate_ddmmyyyy 1
#define time__time__FormatDate_mmddyy 2
#define time__time__FormatDate_mmddyyyy 3
#define time__time__FormatDate_mmmd 4
#define time__time__FormatDate_mmmdd 5
#define time__time__FormatDate_mmmddyyyy 6
#define time__time__FormatDate_no_date 7
#define time__time__FormatDate_yyyymmdd 8
#define time__time__FormatDelimiter_dot 0
#define time__time__FormatDelimiter_hyphen 1
#define time__time__FormatDelimiter_slash 2
#define time__time__FormatDelimiter_space 3
#define freetype__DEFAULT_FONT_SIZE  12
#define main__BlockSize  20
#define main__FieldHeight  20
#define main__FieldWidth  10
#define main__TetroSize  4
int main__WinWidth;
int main__WinHeight;
#define main__TimerPeriod  250
#define main__TextSize  12
#define main__LimitThickness  3
gx__TextCfg main__text_cfg;
gx__TextCfg main__over_cfg;
array_array_int main__BTetros;
array_gx__Color main__Colors;
gx__Color main__BackgroundColor;
gx__Color main__UIColor;
#define main__GameState_paused 0
#define main__GameState_running 1
#define main__GameState_gameover 2
typedef struct  thread_arg_Game_run   { Game* g ;} thread_arg_Game_run ;
void* Game_run_thread_wrapper(thread_arg_Game_run * arg) {Game_run( /*f*/arg -> g ); return 0; }

 array new_array(int mylen, int cap, int elm_size) {
array arr= (array) { .len =  mylen , .cap =  cap , .element_size =  elm_size , .data =  v_calloc ( cap * elm_size ) } ;
return  arr ;
 }
 array make(int len, int cap, int elm_size) {
return  new_array ( len , cap , elm_size ) ;
 }
 array new_array_from_c_array(int len, int cap, int elm_size, void* c_array) {
array arr= (array) { .len =  len , .cap =  cap , .element_size =  elm_size , .data =  v_calloc ( cap * elm_size ) } ;
 memcpy ( arr .data ,  c_array ,  len * elm_size ) ;
return  arr ;
 }
 array new_array_from_c_array_no_alloc(int len, int cap, int elm_size, void* c_array) {
array arr= (array) { .len =  len , .cap =  cap , .element_size =  elm_size , .data =  c_array } ;
return  arr ;
 }
 void array_ensure_cap(array* a, int required) {
 if ( required > a ->cap ) {
int cap= (( a ->cap == 0 ) ? ( 2 ) : ( a ->cap * 2 )) ;
 while ( required > cap ) {
 
 cap  *=  2 ;
 }
 ;
 if ( a ->cap == 0 ) {
 a ->data  =  v_calloc ( cap * a ->element_size ) ;
 }
  else { 
 a ->data  =  realloc ( a ->data ,  cap * a ->element_size ) ;
 }
 ;
 a ->cap  =  cap ;
 }
 ;
 }
 array array_repeat_old(void* val, int nr_repeats, int elm_size) {
 if ( nr_repeats < 0 ) {
 v_panic ( _STR("[0; len]: `len` is negative (len == %d)", nr_repeats) ) ;
 }
 ;
array arr= (array) { .len =  nr_repeats , .cap =  nr_repeats , .element_size =  elm_size , .data =  v_calloc ( nr_repeats * elm_size ) } ;
 for (
int i= 0  ;  i < nr_repeats  ;  i ++ ) { 
 
 memcpy ((byte*) arr .data + i * elm_size ,  val ,  elm_size ) ;
 }
 ;
return  arr ;
 }
 array array_repeat(array a, int nr_repeats) {
 if ( nr_repeats < 0 ) {
 v_panic ( _STR("array.repeat: count is negative (count == %d)", nr_repeats) ) ;
 }
 ;
array arr= (array) { .len =  nr_repeats * a .len , .cap =  nr_repeats * a .len , .element_size =  a .element_size , .data =  v_calloc ( nr_repeats * a .len * a .element_size ) } ;
 for (
int i= 0  ;  i < nr_repeats  ;  i ++ ) { 
 
 memcpy ((byte*) arr .data + i * a .len * a .element_size ,  a .data ,  a .len * a .element_size ) ;
 }
 ;
return  arr ;
 }
 void array_sort_with_compare(array* a, void* compare) {
 qsort ( a ->data ,  a ->len ,  a ->element_size ,  compare ) ;
 }
 void array_insert(array* a, int i, void* val) {
 if ( i < 0  ||  i > a ->len ) {
 v_panic ( _STR("array.insert: index out of range (i == %d, a.len == %d)", i, a ->len) ) ;
 }
 ;
 array_ensure_cap ( a ,  a ->len + 1 ) ;
int size= a ->element_size ;
 memmove ((byte*) a ->data + ( i + 1 ) * size , (byte*) a ->data + i * size ,  ( a ->len - i ) * size ) ;
 memcpy ((byte*) a ->data + i * size ,  val ,  size ) ;
 a ->len ++ ;
 }
 void array_prepend(array* a, void* val) {
 array_insert ( a ,  0 , val ) ;
 }
 void v_array_delete(array* a, int i) {
 if ( i < 0  ||  i >= a ->len ) {
 v_panic ( _STR("array.delete: index out of range (i == %d, a.len == %d)", i, a ->len) ) ;
 }
 ;
int size= a ->element_size ;
 memmove ((byte*) a ->data + i * size , (byte*) a ->data + ( i + 1 ) * size ,  ( a ->len - i ) * size ) ;
 a ->len -- ;
 }
 void* array_get(array a, int i) {
 if ( i < 0  ||  i >= a .len ) {
 v_panic ( _STR("array.get: index out of range (i == %d, a.len == %d)", i, a .len) ) ;
 }
 ;
return (byte*) a .data + i * a .element_size ;
 }
 void* array_first(array a) {
 if ( a .len == 0 ) {
 v_panic ( tos3("array.first: array is empty") ) ;
 }
 ;
return (byte*) a .data + 0 ;
 }
 void* array_last(array a) {
 if ( a .len == 0 ) {
 v_panic ( tos3("array.last: array is empty") ) ;
 }
 ;
return (byte*) a .data + ( a .len - 1 ) * a .element_size ;
 }
 array array_left(array a, int n) {
 if ( n < 0 ) {
 v_panic ( _STR("array.left: index is negative (n == %d)", n) ) ;
 }
 ;
 if ( n >= a .len ) {
return  array_slice ( a ,  0 , a .len ) ;
 }
 ;
return  array_slice ( a ,  0 , n ) ;
 }
 array array_right(array a, int n) {
 if ( n < 0 ) {
 v_panic ( _STR("array.right: index is negative (n == %d)", n) ) ;
 }
 ;
 if ( n >= a .len ) {
return  new_array ( 0 , 0 , a .element_size ) ;
 }
 ;
return  array_slice ( a ,  n , a .len ) ;
 }
 array array_slice2(array a, int start, int _end, bool end_max) {
int end= (( end_max ) ? ( a .len ) : ( _end )) ;
return  array_slice ( a ,  start , end ) ;
 }
 array array_slice(array a, int start, int _end) {
int end= _end ;
 if ( start > end ) {
 v_panic ( _STR("array.slice: invalid slice index (%d > %d)", start, end) ) ;
 }
 ;
 if ( end > a .len ) {
 v_panic ( _STR("array.slice: slice bounds out of range (%d >= %d)", end, a .len) ) ;
 }
 ;
 if ( start < 0 ) {
 v_panic ( _STR("array.slice: slice bounds out of range (%d < 0)", start) ) ;
 }
 ;
int l= end - start ;
array res= (array) { .element_size =  a .element_size , .data = (byte*) a .data + start * a .element_size , .len =  l , .cap =  l } ;
return  res ;
 }
 void array_set(array* a, int i, void* val) {
 if ( i < 0  ||  i >= a ->len ) {
 v_panic ( _STR("array.set: index out of range (i == %d, a.len == %d)", i, a ->len) ) ;
 }
 ;
 memcpy ((byte*) a ->data + a ->element_size * i ,  val ,  a ->element_size ) ;
 }
 void array_push(array* a, void* val) {
 array_ensure_cap ( a ,  a ->len + 1 ) ;
 memcpy ((byte*) a ->data + a ->element_size * a ->len ,  val ,  a ->element_size ) ;
 a ->len ++ ;
 }
 void array_push_many(array* a, void* val, int size) {
 array_ensure_cap ( a ,  a ->len + size ) ;
 memcpy ((byte*) a ->data + a ->element_size * a ->len ,  val ,  a ->element_size * size ) ;
 a ->len  +=  size ;
 }
 array array_reverse(array a) {
array arr= (array) { .len =  a .len , .cap =  a .cap , .element_size =  a .element_size , .data =  v_calloc ( a .cap * a .element_size ) } ;
 for (
int i= 0  ;  i < a .len  ;  i ++ ) { 
 
 memcpy ((byte*) arr .data + i * arr .element_size ,  & ( *(array*) array_get(  a , a .len - 1 - i) ) ,  arr .element_size ) ;
 }
 ;
return  arr ;
 }
 array array_clone(array a) {
array arr= (array) { .len =  a .len , .cap =  a .cap , .element_size =  a .element_size , .data =  v_calloc ( a .cap * a .element_size ) } ;
 memcpy ( arr .data ,  a .data ,  a .cap * a .element_size ) ;
return  arr ;
 }
 void v_array_free(array a) {
 free ( a .data ) ;
 }
 string array_string_str(array_string a) {
strings__Builder sb= strings__new_builder ( a .len * 3 ) ;
 strings__Builder_write (& /* ? */ sb ,  tos3("[") ) ;
 for (
int i= 0  ;  i < a .len  ;  i ++ ) { 
 
string val= ( *(string*) array_get(  a , i) ) ;
 strings__Builder_write (& /* ? */ sb ,  tos3("\"") ) ;
 strings__Builder_write (& /* ? */ sb ,  val ) ;
 strings__Builder_write (& /* ? */ sb ,  tos3("\"") ) ;
 if ( i < a .len - 1 ) {
 strings__Builder_write (& /* ? */ sb ,  tos3(", ") ) ;
 }
 ;
 }
 ;
 strings__Builder_write (& /* ? */ sb ,  tos3("]") ) ;
return  strings__Builder_str (& /* ? */ sb ) ;
 }
 string array_bool_str(array_bool a) {
strings__Builder sb= strings__new_builder ( a .len * 3 ) ;
 strings__Builder_write (& /* ? */ sb ,  tos3("[") ) ;
 for (
int i= 0  ;  i < a .len  ;  i ++ ) { 
 
bool val= ( *(bool*) array_get(  a , i) ) ;
 if ( val ) {
 strings__Builder_write (& /* ? */ sb ,  tos3("true") ) ;
 }
  else { 
 strings__Builder_write (& /* ? */ sb ,  tos3("false") ) ;
 }
 ;
 if ( i < a .len - 1 ) {
 strings__Builder_write (& /* ? */ sb ,  tos3(", ") ) ;
 }
 ;
 }
 ;
 strings__Builder_write (& /* ? */ sb ,  tos3("]") ) ;
return  strings__Builder_str (& /* ? */ sb ) ;
 }
 string array_byte_hex(array_byte b) {
byte* hex= v_malloc ( b .len * 2 + 1 ) ;
byte* ptr= & hex [/*ptr!*/ 0 ]/*rbyte 1*/ ;
 for (
int i= 0  ;  i < b .len  ;  i ++ ) { 
 
 ptr  +=  sprintf ( ((charptr)( ptr ) ) ,  "%02x" ,  ( *(byte*) array_get(  b , i) ) ) ;
 }
 ;
return  (tos2((byte *) hex ) ) ;
 }
 int copy(array_byte dst, array_byte src) {
 if ( dst .len > 0  &&  src .len > 0 ) {
int min= (( dst .len < src .len ) ? ( dst .len ) : ( src .len )) ;
 memcpy ( dst .data ,  array_left ( src ,  min ) .data ,  dst .element_size * min ) ;
return  min ;
 }
 ;
return  0 ;
 }
 int compare_ints(int* a, int* b) {
 if ( * a < * b ) {
return  - 1 ;
 }
 ;
 if ( * a > * b ) {
return  1 ;
 }
 ;
return  0 ;
 }
 void array_int_sort(array_int* a) {
 array_sort_with_compare ( a , & /*112 e="void*" g="fn (int*,int*) int" */ compare_ints ) ;
 }
 int array_string_index(array_string a, string v) {
 for (
int i= 0  ;  i < a .len  ;  i ++ ) { 
 
 if (string_eq( ( *(string*) array_get(  a , i) ) , v ) ) {
return  i ;
 }
 ;
 }
 ;
return  - 1 ;
 }
 int array_int_index(array_int a, int v) {
 for (
int i= 0  ;  i < a .len  ;  i ++ ) { 
 
 if ( ( *(int*) array_get(  a , i) ) == v ) {
return  i ;
 }
 ;
 }
 ;
return  - 1 ;
 }
 int array_byte_index(array_byte a, byte v) {
 for (
int i= 0  ;  i < a .len  ;  i ++ ) { 
 
 if ( ( *(byte*) array_get(  a , i) ) == v ) {
return  i ;
 }
 ;
 }
 ;
return  - 1 ;
 }
 int array_char_index(array_char a, char v) {
 for (
int i= 0  ;  i < a .len  ;  i ++ ) { 
 
 if ( ( *(char*) array_get(  a , i) ) == v ) {
return  i ;
 }
 ;
 }
 ;
return  - 1 ;
 }
 int array_int_reduce(array_int a, int (*iter)( int accum, int curr /*FFF*/ ), int accum_start) {
int _accum= 0 ;
 _accum  =  accum_start ;
 for (
int i= 0  ;  i < a .len  ;  i ++ ) { 
 
 _accum  =  iter ( _accum , ( *(int*) array_get(  a , i) ) ) ;
 }
 ;
return  _accum ;
 }
 bool array_int_eq(array_int a, array_int a2) {
return  array_eq_T_int ( a , a2 ) ;
 }
 bool array_i64_eq(array_i64 a, array_i64 a2) {
return  array_eq_T_i64 ( a , a2 ) ;
 }
 bool array_string_eq(array_string a, array_string a2) {
return  array_eq_T_string ( a , a2 ) ;
 }
 bool array_byte_eq(array_byte a, array_byte a2) {
return  array_eq_T_byte ( a , a2 ) ;
 }
 bool array_f32_eq(array_f32 a, array_f32 a2) {
return  array_eq_T_f32 ( a , a2 ) ;
 }
 void builtin__init() {
 #ifdef _WIN32
 #endif
 ;
 }
 void v_exit(int code) {
 exit ( code ) ;
 }
 bool isnil(void* v) {
return  v == 0 ;
 }
 void on_panic(int (*f)( int  /*FFF*/ )) {
 }
 void print_backtrace_skipping_top_frames(int skipframes) {
 #ifdef _WIN32
 #else
 if ( print_backtrace_skipping_top_frames_nix ( skipframes ) ) {
 
 return ;
 }
 ;
 #endif
 ;
 println ( tos3("print_backtrace_skipping_top_frames is not implemented on this platform for now...\n") ) ;
 }
 void print_backtrace() {
 print_backtrace_skipping_top_frames ( 2 ) ;
 }
 void panic_debug(int line_no, string file, string mod, string fn_name, string s) {
 println ( tos3("================ V panic ================") ) ;
printf( "   module: %.*s\n", mod.len, mod.str ) ;
printf( " function: %.*s()\n", fn_name.len, fn_name.str ) ;
printf( "     file: %.*s\n", file.len, file.str ) ;
 println (string_add( tos3("     line: ") , int_str ( line_no ) ) ) ;
printf( "  message: %.*s\n", s.len, s.str ) ;
 println ( tos3("=========================================") ) ;
 print_backtrace_skipping_top_frames ( 1 ) ;
 exit ( 1 ) ;
 }
 void v_panic(string s) {
printf( "V panic: %.*s\n", s.len, s.str ) ;
 print_backtrace ( ) ;
 exit ( 1 ) ;
 }
 void println(string s) {
 if ( isnil ( s .str ) ) {
 v_panic ( tos3("println(NIL)") ) ;
 }
 ;
 #ifdef _WIN32
 #else
 printf ( "%.*s\n" ,  s .len , (char*) s .str ) ;
 #endif
 ;
 }
 void eprintln(string s) {
 if ( isnil ( s .str ) ) {
 v_panic ( tos3("eprintln(NIL)") ) ;
 }
 ;
 #ifndef _WIN32
 fflush ( stdout ) ;
 fflush ( stderr ) ;
 fprintf ( stderr ,  "%.*s\n" ,  s .len , (char*) s .str ) ;
 fflush ( stderr ) ;
 
 return ;
 #endif
 ;
 println ( s ) ;
 }
 void print(string s) {
 #ifdef _WIN32
 #else
 printf ( "%.*s" ,  s .len , (char*) s .str ) ;
 #endif
 ;
 }
 byte* v_malloc(int n) {
 if ( n < 0 ) {
 v_panic ( tos3("malloc(<0)") ) ;
 }
 ;
 v_free ( malloc ( n ) ) ;
byte* ptr= malloc ( n ) ;
 if ( isnil ( ptr ) ) {
 v_panic ( _STR("malloc(%d) failed", n) ) ;
 }
 ;
return  ptr ;
 }
 byte* v_calloc(int n) {
 if ( n < 0 ) {
 v_panic ( tos3("calloc(<0)") ) ;
 }
 ;
return  calloc ( n ,  1 ) ;
 }
 void v_free(void* ptr) {
 free ( ptr ) ;
 }
 void* memdup(void* src, int sz) {
byte* mem= v_malloc ( sz ) ;
return  memcpy ((char*) mem ,  src ,  sz ) ;
 }
 void v_ptr_free(void* ptr) {
 free ( ptr ) ;
 }
 int is_atty(int fd) {
 #ifdef _WIN32
 #else
return  isatty ( fd ) ;
 #endif
 ;
 }
 bool print_backtrace_skipping_top_frames_msvc(int skipframes) {
 println ( tos3("not implemented, see builtin_windows.v") ) ;
return  0 ;
 }
 bool print_backtrace_skipping_top_frames_mingw(int skipframes) {
 println ( tos3("not implemented, see builtin_windows.v") ) ;
return  0 ;
 }
 bool print_backtrace_skipping_top_frames_nix(int xskipframes) {
int skipframes= xskipframes + 2 ;
 #ifdef __APPLE__
 #endif
 ;
 #ifdef __linux__
return  print_backtrace_skipping_top_frames_linux ( skipframes ) ;
 #endif
 ;
 #ifdef __FreeBSD__
 #endif
 ;
return  0 ;
 }
 bool print_backtrace_skipping_top_frames_mac(int skipframes) {
 #ifdef __APPLE__
 #endif
 ;
return  1 ;
 }
 bool print_backtrace_skipping_top_frames_freebsd(int skipframes) {
 #ifdef __FreeBSD__
 #endif
 ;
return  1 ;
 }
 bool print_backtrace_skipping_top_frames_linux(int skipframes) {
 #ifdef __TINYC__
printf( "TODO: print_backtrace_skipping_top_frames_linux %d with tcc fails tests with \"stack smashing detected\" .\n", skipframes ) ;
return  0 ;
 #endif
 ;
 #ifndef __BIONIC__
 #ifdef __GLIBC__
byte* buffer  [100 ] ;
int nr_ptrs= backtrace ( ((voidptr*)( buffer ) ) ,  100 ) ;
int nr_actual_frames= nr_ptrs - skipframes ;
array_string sframes=new_array_from_c_array(0, 0, sizeof(string), EMPTY_ARRAY_OF_ELEMS( string, 0 ) {   TCCSKIP(0) }) ;
byteptr* csymbols= ((byteptr*)( backtrace_symbols ( ((voidptr*)( & buffer [ skipframes ]/*rbyte* 0*/ ) ) ,  nr_actual_frames ) ) ) ;
 int tmp1 =  0;
 ;
for (int tmp2 = tmp1; tmp2 <  nr_actual_frames; tmp2++) {
 int i = tmp2;
 
_PUSH(& sframes , ( /*typ = array_string   tmp_typ=string*/ tos2 ( csymbols [/*ptr!*/ i ]/*rbyteptr 0*/ ) ), tmp3, string) ;
 }
 ;
 array_string tmp4 =  sframes;
 for (int tmp5 = 0; tmp5 < tmp4.len; tmp5++) {
 string sframe = ((string *) tmp4 . data)[tmp5];
 
string executable= string_all_before ( sframe ,  tos3("(") ) ;
string addr= string_all_before ( string_all_after ( sframe ,  tos3("[") ) ,  tos3("]") ) ;
string beforeaddr= string_all_before ( sframe ,  tos3("[") ) ;
string cmd= _STR("addr2line -e %.*s %.*s", executable.len, executable.str, addr.len, addr.str) ;
void* f= popen ((char*) cmd .str ,  "r" ) ;
 if ( isnil ( f ) ) {
 println ( sframe ) ;
 continue
 ;
 }
 ;
byte buf  [1000 ] = { 0 } ;
string output= tos3("") ;
 while ( fgets ( ((voidptr)( buf ) ) ,  1000 ,  f ) != 0 ) {
 
 output = string_add(output,  tos ( buf , vstrlen ( buf ) ) ) ;
 }
 ;
 output  = string_add( string_trim_space ( output ) , tos3(":") ) ;
 if ( 0 != ((int)( pclose ( f ) ) ) ) {
 println ( sframe ) ;
 continue
 ;
 }
 ;
 if ( (string_eq( output ,  tos3("??:0:") )  || string_eq( output,  tos3("??:?:") ) ) ) {
 output  =  tos3("") ;
 }
 ;
printf( "%-46s | %14s | %.*s\n", output.str, addr.str, beforeaddr.len, beforeaddr.str ) ;
 }
 ;
return  1 ;
 #else
 printf ( "backtrace_symbols_fd is missing, so printing backtraces is not available.\n" ) ;
 printf ( "Some libc implementations like musl simply do not provide it.\n" ) ;
 #endif
 ;
 #endif
 ;
return  0 ;
 }
 int backtrace(void* a, int b);
 byteptr* backtrace_symbols(void* , int );
 void backtrace_symbols_fd(void* , int , int );
 int proc_pidpath(int , void* , int );
 string f64_str(f64 d) {
byte* buf= v_malloc ( sizeof( double) * 5 + 1 ) ;
 sprintf ( ((charptr)( buf ) ) ,  "%f" ,  d ) ;
return  tos ( buf , vstrlen ( buf ) ) ;
 }
 string f32_str(f32 d) {
byte* buf= v_malloc ( sizeof( double) * 5 + 1 ) ;
 sprintf ( ((charptr)( buf ) ) ,  "%f" ,  d ) ;
return  tos ( buf , vstrlen ( buf ) ) ;
 }
 f32 f32_abs(f32 a) {
return  (( a < 0 ) ? ( - a ) : ( a )) ;
 }
 f64 f64_abs(f64 a) {
return  (( a < 0 ) ? ( - a ) : ( a )) ;
 }
 bool f64_eq(f64 a, f64 b) {
return  f64_abs ( a - b ) <= DBL_EPSILON ;
 }
 bool f32_eq(f32 a, f32 b) {
return  f32_abs ( a - b ) <= FLT_EPSILON ;
 }
 bool f64_eqbit(f64 a, f64 b) {
return  DEFAULT_EQUAL ( a ,  b ) ;
 }
 bool f32_eqbit(f32 a, f32 b) {
return  DEFAULT_EQUAL ( a ,  b ) ;
 }
 bool f64_ne(f64 a, f64 b) {
return  ! f64_eq ( a ,  b ) ;
 }
 bool f32_ne(f32 a, f32 b) {
return  ! f32_eq ( a ,  b ) ;
 }
 bool f64_nebit(f64 a, f64 b) {
return  DEFAULT_NOT_EQUAL ( a ,  b ) ;
 }
 bool f32_nebit(f32 a, f32 b) {
return  DEFAULT_NOT_EQUAL ( a ,  b ) ;
 }
 bool f64_lt(f64 a, f64 b) {
return  f64_ne ( a ,  b )  &&  f64_ltbit ( a ,  b ) ;
 }
 bool f32_lt(f32 a, f32 b) {
return  f32_ne ( a ,  b )  &&  f32_ltbit ( a ,  b ) ;
 }
 bool f64_ltbit(f64 a, f64 b) {
return  DEFAULT_LT ( a ,  b ) ;
 }
 bool f32_ltbit(f32 a, f32 b) {
return  DEFAULT_LT ( a ,  b ) ;
 }
 bool f64_le(f64 a, f64 b) {
return  ! f64_gt ( a ,  b ) ;
 }
 bool f32_le(f32 a, f32 b) {
return  ! f32_gt ( a ,  b ) ;
 }
 bool f64_lebit(f64 a, f64 b) {
return  DEFAULT_LE ( a ,  b ) ;
 }
 bool f32_lebit(f32 a, f32 b) {
return  DEFAULT_LE ( a ,  b ) ;
 }
 bool f64_gt(f64 a, f64 b) {
return  f64_ne ( a ,  b )  &&  f64_gtbit ( a ,  b ) ;
 }
 bool f32_gt(f32 a, f32 b) {
return  f32_ne ( a ,  b )  &&  f32_gtbit ( a ,  b ) ;
 }
 bool f64_gtbit(f64 a, f64 b) {
return  DEFAULT_GT ( a ,  b ) ;
 }
 bool f32_gtbit(f32 a, f32 b) {
return  DEFAULT_GT ( a ,  b ) ;
 }
 bool f64_ge(f64 a, f64 b) {
return  ! f64_lt ( a ,  b ) ;
 }
 bool f32_ge(f32 a, f32 b) {
return  ! f32_lt ( a ,  b ) ;
 }
 bool f64_gebit(f64 a, f64 b) {
return  DEFAULT_GE ( a ,  b ) ;
 }
 bool f32_gebit(f32 a, f32 b) {
return  DEFAULT_GE ( a ,  b ) ;
 }
 hashmap new_hashmap(int planned_nr_items) {
int cap= planned_nr_items * 5 ;
 if ( cap < builtin__min_cap ) {
 cap  =  builtin__min_cap ;
 }
 ;
 if ( cap > builtin__max_cap ) {
 cap  =  builtin__max_cap ;
 }
 ;
return  (hashmap) { .cap =  cap , .elm_size =  4 , .table =  make ( cap , cap , sizeof( hashmapentry) ) , .keys =  new_array(0, 1, sizeof( string )) , .nr_collisions =  0 } ;
 }
 void hashmap_set(hashmap* m, string key, int val) {
int hash= ((int)( b_fabs ( string_hash ( key ) ) ) ) ;
int idx= hash % m ->cap ;
 if ( ( *(hashmapentry*) array_get(  m ->table , idx) ) .key .len != 0 ) {
 m ->nr_collisions ++ ;
hashmapentry* e= & ( *(hashmapentry*) array_get(  m ->table , idx) ) ;
 while ( e ->next != 0 ) {
 
 e  =  e ->next ;
 }
 ;
 e ->next  =  (hashmapentry*)memdup(&(hashmapentry)  { key , val , 0 } , sizeof(hashmapentry)) ;
 }
  else { 
array_set(&/*q*/ m ->table , idx , & (hashmapentry []) {  (hashmapentry) { key , val , 0 } }) ;
 }
 ;
 }
 int hashmap_get(hashmap* m, string key) {
int hash= ((int)( b_fabs ( string_hash ( key ) ) ) ) ;
int idx= hash % m ->cap ;
hashmapentry* e= & ( *(hashmapentry*) array_get(  m ->table , idx) ) ;
 while ( e ->next != 0 ) {
 
 if (string_eq( e ->key , key ) ) {
return  e ->val ;
 }
 ;
 e  =  e ->next ;
 }
 ;
return  e ->val ;
 }
 static inline f64 b_fabs(int v) {
return  (( v < 0 ) ? ( - v ) : ( v )) ;
 }
 string ptr_str(void* ptr) {
byte* buf= v_malloc ( sizeof( double) * 5 + 1 ) ;
 sprintf ( ((charptr)( buf ) ) ,  "%p" ,  ptr ) ;
return  tos ( buf , vstrlen ( buf ) ) ;
 }
 string int_str(int nn) {
int n= nn ;
 if ( n == 0 ) {
return  tos3("0") ;
 }
 ;
int max= 16 ;
byte* buf= v_calloc ( max ) ;
int len= 0 ;
bool is_neg= 0 ;
 if ( n < 0 ) {
 n  =  - n ;
 is_neg  =  1 ;
 }
 ;
 while ( n > 0 ) {
 
int d= n % 10 ;
 buf [/*ptr!*/ max - len - 1 ]/*rbyte 1*/  =  d + ((int)( '0' ) ) ;
 len ++ ;
 n  =  n / 10 ;
 }
 ;
 if ( is_neg ) {
 buf [/*ptr!*/ max - len - 1 ]/*rbyte 1*/  =  '-' ;
 len ++ ;
 }
 ;
return  tos ((byte*)(byte*) buf + max - len , len ) ;
 }
 string u32_str(u32 nn) {
u32 n= nn ;
 if ( n == ((u32)( 0 ) ) ) {
return  tos3("0") ;
 }
 ;
int max= 16 ;
byte* buf= v_malloc ( max ) ;
int len= 0 ;
 while ( n > ((u32)( 0 ) ) ) {
 
u32 d= n % ((u32)( 10 ) ) ;
 buf [/*ptr!*/ max - len - 1 ]/*rbyte 1*/  =  d + ((u32)( '0' ) ) ;
 len ++ ;
 n  =  n / ((u32)( 10 ) ) ;
 }
 ;
return  tos ((byte*)(byte*) buf + max - len , len ) ;
 }
 string i64_str(i64 nn) {
i64 n= nn ;
 if ( n == ((i64)( 0 ) ) ) {
return  tos3("0") ;
 }
 ;
int max= 32 ;
byte* buf= v_malloc ( max ) ;
int len= 0 ;
bool is_neg= 0 ;
 if ( n < ((i64)( 0 ) ) ) {
 n  =  - n ;
 is_neg  =  1 ;
 }
 ;
 while ( n > ((i64)( 0 ) ) ) {
 
int d= ((int)( n % ((i64)( 10 ) ) ) ) ;
 buf [/*ptr!*/ max - len - 1 ]/*rbyte 1*/  =  d + ((int)( '0' ) ) ;
 len ++ ;
 n  =  n / ((i64)( 10 ) ) ;
 }
 ;
 if ( is_neg ) {
 buf [/*ptr!*/ max - len - 1 ]/*rbyte 1*/  =  '-' ;
 len ++ ;
 }
 ;
return  tos ((byte*)(byte*) buf + max - len , len ) ;
 }
 string u64_str(u64 nn) {
u64 n= nn ;
 if ( n == ((u64)( 0 ) ) ) {
return  tos3("0") ;
 }
 ;
int max= 32 ;
byte* buf= v_malloc ( max ) ;
int len= 0 ;
 while ( n > ((u64)( 0 ) ) ) {
 
u64 d= n % ((u64)( 10 ) ) ;
 buf [/*ptr!*/ max - len - 1 ]/*rbyte 1*/  =  d + ((u64)( '0' ) ) ;
 len ++ ;
 n  =  n / ((u64)( 10 ) ) ;
 }
 ;
return  tos ((byte*)(byte*) buf + max - len , len ) ;
 }
 string bool_str(bool b) {
 if ( b ) {
return  tos3("true") ;
 }
 ;
return  tos3("false") ;
 }
 string int_hex(int n) {
int len= (( n >= 0 ) ? ( int_str ( n ) .len + 3 ) : ( 11 )) ;
byte* hex= v_malloc ( len ) ;
int count= ((int)( sprintf ( ((charptr)( hex ) ) ,  "0x%x" ,  n ) ) ) ;
return  tos ( hex , count ) ;
 }
 string i64_hex(i64 n) {
int len= (( n >= ((i64)( 0 ) ) ) ? ( i64_str ( n ) .len + 3 ) : ( 19 )) ;
byte* hex= v_malloc ( len ) ;
int count= ((int)( sprintf ( ((charptr)( hex ) ) ,  "0x%" PRIx64 ,  n ) ) ) ;
return  tos ( hex , count ) ;
 }
 bool array_byte_contains(array_byte a, byte val) {
 array_byte tmp1 =  a;
 for (int tmp2 = 0; tmp2 < tmp1.len; tmp2++) {
 byte aa = ((byte *) tmp1 . data)[tmp2];
 
 if ( aa == val ) {
return  1 ;
 }
 ;
 }
 ;
return  0 ;
 }
 string rune_str(rune c) {
int fst_byte= ((int)( c ) )  >>  8 * 3 & 0xff ;
int len= utf8_char_len ( fst_byte ) ;
string str= (string) { .len =  len , .str =  v_malloc ( len + 1 ) } ;
 for (
int i= 0  ;  i < len  ;  i ++ ) { 
 
 str .str [/*ptr!*/ i ]/*rbyte 1*/  =  ((int)( c ) )  >>  8 * ( 3 - i ) & 0xff ;
 }
 ;
 str .str[ len ]/*rbyte 1*/  =  '\0' ;
return  str ;
 }
 string byte_str(byte c) {
string str= (string) { .len =  1 , .str =  v_malloc ( 2 ) } ;
 str .str [/*ptr!*/ 0 ]/*rbyte 1*/  =  c ;
 str .str [/*ptr!*/ 1 ]/*rbyte 1*/  =  '\0' ;
return  str ;
 }
 bool byte_is_capital(byte c) {
return  c >= 'A'  &&  c <= 'Z' ;
 }
 array_byte array_byte_clone(array_byte b) {
array_byte res= array_repeat (new_array_from_c_array(1, 1, sizeof(byte), EMPTY_ARRAY_OF_ELEMS( byte, 1 ) {  ((byte)( 0 ) )  }) ,  b .len ) ;
 for (
int i= 0  ;  i < b .len  ;  i ++ ) { 
 
array_set(&/*q*/ res , i , & (byte []) {  ( *(byte*) array_get(  b , i) ) }) ;
 }
 ;
return  res ;
 }
 map new_map(int cap, int elm_size) {
map res= (map) { .element_size =  elm_size , .root =  0 , .size =  0 } ;
return  res ;
 }
 map new_map_init(int cap, int elm_size, string* keys, void* vals) {
map res= (map) { .element_size =  elm_size , .root =  0 , .size =  0 } ;
 int tmp1 =  0;
 ;
for (int tmp2 = tmp1; tmp2 <  cap; tmp2++) {
 int i = tmp2;
 
 map_set (& /* ? */ res ,  keys [/*ptr!*/ i ]/*rstring 0*/ ,(byte*) vals + i * elm_size ) ;
 }
 ;
return  res ;
 }
 mapnode* new_node(string key, void* val, int element_size) {
mapnode* new_e= (mapnode*)memdup(&(mapnode)  { .key =  key , .val =  v_malloc ( element_size ) , .left =  0 , .right =  0 , .is_empty =  0 , } , sizeof(mapnode)) ;
 memcpy ( new_e ->val ,  val ,  element_size ) ;
return  new_e ;
 }
 void map_insert(map* m, mapnode* n, string key, void* val) {
 if (string_eq( n ->key , key ) ) {
 memcpy ( n ->val ,  val ,  m ->element_size ) ;
 
 return ;
 }
 ;
 if (string_gt( n ->key , key ) ) {
 if ( n ->left == 0 ) {
 n ->left  =  new_node ( key , val , m ->element_size ) ;
 m ->size ++ ;
 }
  else { 
 map_insert ( m ,  n ->left , key , val ) ;
 }
 ;
 
 return ;
 }
 ;
 if ( n ->right == 0 ) {
 n ->right  =  new_node ( key , val , m ->element_size ) ;
 m ->size ++ ;
 }
  else { 
 map_insert ( m ,  n ->right , key , val ) ;
 }
 ;
 }
 bool mapnode_find(mapnode* n, string key, void* out, int element_size) {
 if (string_eq( n ->key , key ) ) {
 memcpy ( out ,  n ->val ,  element_size ) ;
return  1 ;
 }
  else  if (string_gt( n ->key , key ) ) {
 if ( n ->left == 0 ) {
return  0 ;
 }
  else { 
return  mapnode_find (& /* ? */* n ->left ,  key , out , element_size ) ;
 }
 ;
 }
  else { 
 if ( n ->right == 0 ) {
return  0 ;
 }
  else { 
return  mapnode_find (& /* ? */* n ->right ,  key , out , element_size ) ;
 }
 ;
 }
 ;
 }
 bool mapnode_find2(mapnode* n, string key, int element_size) {
 if (string_eq( n ->key , key )  &&  ! n ->is_empty ) {
return  1 ;
 }
  else  if (string_gt( n ->key , key ) ) {
 if ( isnil ( n ->left ) ) {
return  0 ;
 }
  else { 
return  mapnode_find2 (& /* ? */* n ->left ,  key , element_size ) ;
 }
 ;
 }
  else { 
 if ( isnil ( n ->right ) ) {
return  0 ;
 }
  else { 
return  mapnode_find2 (& /* ? */* n ->right ,  key , element_size ) ;
 }
 ;
 }
 ;
 }
 void map_set(map* m, string key, void* val) {
 if ( isnil ( m ->root ) ) {
 m ->root  =  new_node ( key , val , m ->element_size ) ;
 m ->size ++ ;
 
 return ;
 }
 ;
 map_insert ( m ,  m ->root , key , val ) ;
 }
 int preorder_keys(mapnode* node, array_string* keys, int key_i) {
int i= key_i ;
 if ( ! node ->is_empty ) {
array_set( keys , i , & (string []) {  node ->key }) ;
 i ++ ;
 }
 ;
 if ( ! isnil ( node ->left ) ) {
 i  =  preorder_keys ( node ->left , keys , i ) ;
 }
 ;
 if ( ! isnil ( node ->right ) ) {
 i  =  preorder_keys ( node ->right , keys , i ) ;
 }
 ;
return  i ;
 }
 array_string map_keys(map* m) {
array_string keys= array_repeat (new_array_from_c_array(1, 1, sizeof(string), EMPTY_ARRAY_OF_ELEMS( string, 1 ) {  tos3("")  }) ,  m ->size ) ;
 if ( isnil ( m ->root ) ) {
return  keys ;
 }
 ;
 preorder_keys ( m ->root ,& /*111*/ (array[]){ keys }[0]  , 0 ) ;
return  keys ;
 }
 bool map_get(map m, string key, void* out) {
 if ( m .root == 0 ) {
return  0 ;
 }
 ;
return  mapnode_find (& /* ? */* m .root ,  key , out , m .element_size ) ;
 }
 void v_mapnode_delete(mapnode* n, string key, int element_size) {
 if (string_eq( n ->key , key ) ) {
 memset ( n ->val ,  0 ,  element_size ) ;
 n ->is_empty  =  1 ;
 
 return ;
 }
  else  if (string_gt( n ->key , key ) ) {
 if ( isnil ( n ->left ) ) {
 
 return ;
 }
  else { 
 v_mapnode_delete ( n ->left ,  key , element_size ) ;
 }
 ;
 }
  else { 
 if ( isnil ( n ->right ) ) {
 
 return ;
 }
  else { 
 v_mapnode_delete ( n ->right ,  key , element_size ) ;
 }
 ;
 }
 ;
 }
 void v_map_delete(map* m, string key) {
 if ( map_exists (* m ,  key ) ) {
 v_mapnode_delete ( m ->root ,  key , m ->element_size ) ;
 m ->size -- ;
 }
 ;
 }
 bool map_exists(map m, string key) {
return  ! isnil ( m .root )  &&  mapnode_find2 (& /* ? */* m .root ,  key , m .element_size ) ;
 }
 void map_print(map m) {
 println ( tos3("<<<<<<<<") ) ;
 println ( tos3(">>>>>>>>>>") ) ;
 }
 void v_mapnode_free(mapnode* n) {
 if ( n ->val != 0 ) {
 v_free ( n ->val ) ;
 }
 ;
 if ( n ->left != 0 ) {
 v_mapnode_free ( n ->left ) ;
 }
 ;
 if ( n ->right != 0 ) {
 v_mapnode_free ( n ->right ) ;
 }
 ;
 v_free ( n ) ;
 }
 void v_map_free(map* m) {
 if ( m ->root == 0 ) {
 
 return ;
 }
 ;
 v_mapnode_free ( m ->root ) ;
 }
 string map_string_str(map_string m) {
 if ( m .size == 0 ) {
return  tos3("{}") ;
 }
 ;
strings__Builder sb= strings__new_builder ( 50 ) ;
 strings__Builder_writeln (& /* ? */ sb ,  tos3("{") ) ;
 map_string tmp3 =  m;
 array_string keys_tmp3 = map_keys(& tmp3 ); 
 for (int l = 0; l < keys_tmp3 .len; l++) {
 string key = ((string*)keys_tmp3 .data)[l];
 string val = tos3(""); map_get(tmp3, key, & val);
 
 strings__Builder_writeln (& /* ? */ sb ,  _STR("  \"%.*s\" => \"%.*s\"", key.len, key.str, val.len, val.str) ) ;
 }
 ;
 strings__Builder_writeln (& /* ? */ sb ,  tos3("}") ) ;
return  strings__Builder_str (& /* ? */ sb ) ;
 }
 Option opt_ok(void* data, int size) {
 if ( size >= 300 ) {
 v_panic ( _STR("option size too big: %d (max is 300), this is a temporary limit", size) ) ;
 }
 ;
Option res= (Option) { .ok =  1 , .error =  tos3("") , .ecode =  0 , .is_none =  0 } ;
 memcpy ( res .data ,  data ,  size ) ;
return  res ;
 }
 Option opt_none() {
return  (Option) { .is_none =  1 , .error =  tos3("") , .ecode =  0 , .ok =  0 , } ;
 }
 Option v_error(string s) {
return  (Option) { .error =  s , .ecode =  0 , .ok =  0 , .is_none =  0 } ;
 }
 Option error_with_code(string s, int code) {
return  (Option) { .error =  s , .ecode =  code , .ok =  0 , .is_none =  0 } ;
 }
 int vstrlen(byte* s) {
return  strlen ( ((charptr)( s ) ) ) ;
 }
 string tos(byte* s, int len) {
 if ( s == 0 ) {
 v_panic ( tos3("tos(): nil string") ) ;
 }
 ;
return  (string) { .str =  s , .len =  len } ;
 }
 string tos_clone(byte* s) {
 if ( s == 0 ) {
 v_panic ( tos3("tos: nil string") ) ;
 }
 ;
return  string_clone ( tos2 ( s ) ) ;
 }
 string tos2(byte* s) {
 if ( s == 0 ) {
 v_panic ( tos3("tos2: nil string") ) ;
 }
 ;
return  (string) { .str =  s , .len =  vstrlen ( s ) } ;
 }
 string tos3(charptr s) {
 if ( s == 0 ) {
 v_panic ( tos3("tos3: nil string") ) ;
 }
 ;
return  (string) { .str =  ((byteptr)( s ) ) , .len =  strlen ( s ) } ;
 }
 string string_clone(string a) {
string b= (string) { .len =  a .len , .str =  v_malloc ( a .len + 1 ) } ;
 for (
int i= 0  ;  i < a .len  ;  i ++ ) { 
 
 b .str[ i ]/*rbyte 1*/  =  a .str[ i ]/*rbyte 0*/ ;
 }
 ;
 b .str[ a .len ]/*rbyte 1*/  =  '\0' ;
return  b ;
 }
 string cstring_to_vstring(byte* cstr) {
int slen= strlen ((char*) cstr ) ;
byteptr s= ((byteptr)( memdup ( cstr , slen + 1 ) ) ) ;
 s [/*ptr!*/ slen ]/*rbyteptr 1*/  =  '\0' ;
return  tos ( s , slen ) ;
 }
 string string_replace_once(string s, string rep, string with) {
Option_int tmp1 =  string_index ( s ,  rep ) ;
 int index;
 if (!tmp1 .ok) {
 string err = tmp1 . error;
 int errcode = tmp1 . ecode;
return  s ;
 }
 index = *(int*)tmp1.data;
 ;
return string_add(string_add( string_substr ( s ,  0 , index ) , with ) , string_substr ( s ,  index + rep .len , s .len ) ) ;
 }
 string string_replace(string s, string rep, string with) {
 if ( s .len == 0  ||  rep .len == 0 ) {
return  s ;
 }
 ;
array_int idxs=new_array_from_c_array(0, 0, sizeof(int), EMPTY_ARRAY_OF_ELEMS( int, 0 ) {   TCCSKIP(0) }) ;
string rem= s ;
int rstart= 0 ;
 while (1) { 
Option_int tmp2 =  string_index ( rem ,  rep ) ;
 int i;
 if (!tmp2 .ok) {
 string err = tmp2 . error;
 int errcode = tmp2 . ecode;
 break
 ;
 }
 i = *(int*)tmp2.data;
 ;
_PUSH(& idxs , ( /*typ = array_int   tmp_typ=int*/ rstart + i ), tmp3, int) ;
 i  +=  rep .len ;
 rstart  +=  i ;
 rem  =  string_substr ( rem ,  i , rem .len ) ;
 }
 ;
 if ( idxs .len == 0 ) {
return  s ;
 }
 ;
int new_len= s .len + idxs .len * ( with .len - rep .len ) ;
byte* b= v_malloc ( new_len + 1 ) ;
int idx_pos= 0 ;
int cur_idx= ( *(int*) array_get(  idxs , idx_pos) ) ;
int b_i= 0 ;
 for (
int i= 0  ;  i < s .len  ;  i ++ ) { 
 
 if ( i == cur_idx ) {
 for (
int j= 0  ;  j < with .len  ;  j ++ ) { 
 
 b [/*ptr!*/ b_i ]/*rbyte 1*/  =  with .str[ j ]/*rbyte 0*/ ;
 b_i ++ ;
 }
 ;
 i  +=  rep .len - 1 ;
 idx_pos ++ ;
 if ( idx_pos < idxs .len ) {
 cur_idx  =  ( *(int*) array_get(  idxs , idx_pos) ) ;
 }
 ;
 }
  else { 
 b [/*ptr!*/ b_i ]/*rbyte 1*/  =  s .str[ i ]/*rbyte 0*/ ;
 b_i ++ ;
 }
 ;
 }
 ;
 b [/*ptr!*/ new_len ]/*rbyte 1*/  =  '\0' ;
return  tos ( b , new_len ) ;
 }
 int v_string_int(string s) {
return  ((int)( strconv__common_parse_int ( s , 0 , 32 , 0 , 0 ) ) ) ;
 }
 i64 string_i64(string s) {
return  strconv__common_parse_int ( s , 0 , 64 , 0 , 0 ) ;
 }
 f32 string_f32(string s) {
return  atof ( ((charptr)( s .str ) ) ) ;
 }
 f64 string_f64(string s) {
return  atof ( ((charptr)( s .str ) ) ) ;
 }
 u32 string_u32(string s) {
return  ((u32)( strconv__common_parse_uint ( s , 0 , 32 , 0 , 0 ) ) ) ;
 }
 u64 string_u64(string s) {
return  strconv__common_parse_uint ( s , 0 , 64 , 0 , 0 ) ;
 }
 bool string_eq(string s, string a) {
 if ( isnil ( s .str ) ) {
 v_panic ( tos3("string.eq(): nil string") ) ;
 }
 ;
 if ( s .len != a .len ) {
return  0 ;
 }
 ;
 for (
int i= 0  ;  i < s .len  ;  i ++ ) { 
 
 if ( s .str[ i ]/*rbyte 0*/ != a .str[ i ]/*rbyte 0*/ ) {
return  0 ;
 }
 ;
 }
 ;
return  1 ;
 }
 bool string_ne(string s, string a) {
return  ! string_eq ( s ,  a ) ;
 }
 bool string_lt(string s, string a) {
 for (
int i= 0  ;  i < s .len  ;  i ++ ) { 
 
 if ( i >= a .len  ||  s .str[ i ]/*rbyte 0*/ > a .str[ i ]/*rbyte 0*/ ) {
return  0 ;
 }
  else  if ( s .str[ i ]/*rbyte 0*/ < a .str[ i ]/*rbyte 0*/ ) {
return  1 ;
 }
 ;
 }
 ;
 if ( s .len < a .len ) {
return  1 ;
 }
 ;
return  0 ;
 }
 bool string_le(string s, string a) {
return  string_lt ( s ,  a )  ||  string_eq ( s ,  a ) ;
 }
 bool string_gt(string s, string a) {
return  ! string_le ( s ,  a ) ;
 }
 bool string_ge(string s, string a) {
return  ! string_lt ( s ,  a ) ;
 }
 string string_add(string s, string a) {
int new_len= a .len + s .len ;
string res= (string) { .len =  new_len , .str =  v_malloc ( new_len + 1 ) } ;
 for (
int j= 0  ;  j < s .len  ;  j ++ ) { 
 
 res .str[ j ]/*rbyte 1*/  =  s .str[ j ]/*rbyte 0*/ ;
 }
 ;
 for (
int j= 0  ;  j < a .len  ;  j ++ ) { 
 
 res .str[ s .len + j ]/*rbyte 1*/  =  a .str[ j ]/*rbyte 0*/ ;
 }
 ;
 res .str[ new_len ]/*rbyte 1*/  =  '\0' ;
return  res ;
 }
 array_string string_split(string s, string delim) {
return  string_split_nth ( s ,  delim , 0 ) ;
 }
 array_string string_split_nth(string s, string delim, int nth) {
array_string res=new_array_from_c_array(0, 0, sizeof(string), EMPTY_ARRAY_OF_ELEMS( string, 0 ) {   TCCSKIP(0) }) ;
int i= 0 ;
 if ( delim .len == 0 ) {
 i  =  1 ;
 string tmp8 =  s;
 ;
for (int tmp9 = 0; tmp9 < tmp8 .len; tmp9 ++) {
 byte ch = tmp8.str[tmp9];
 
 if ( nth > 0  &&  i >= nth ) {
_PUSH(& res , ( /*typ = array_string   tmp_typ=string*/ string_substr ( s ,  i , s .len ) ), tmp10, string) ;
 break
 ;
 }
 ;
_PUSH(& res , ( /*typ = array_string   tmp_typ=string*/ byte_str ( ch ) ), tmp11, string) ;
 i ++ ;
 }
 ;
return  res ;
 }
 ;
int start= 0 ;
 while ( i <= s .len ) {
 
bool is_delim= s .str[ i ]/*rbyte 0*/ == delim .str[ 0 ]/*rbyte 0*/ ;
int j= 0 ;
 while ( is_delim  &&  j < delim .len ) {
 
 is_delim  =  is_delim  &&  s .str[ i + j ]/*rbyte 0*/ == delim .str[ j ]/*rbyte 0*/ ;
 j ++ ;
 }
 ;
bool was_last= nth > 0  &&  res .len == nth ;
 if ( was_last ) {
 break
 ;
 }
 ;
bool last= i == s .len - 1 ;
 if ( is_delim  ||  last ) {
 if ( ! is_delim  &&  last ) {
 i ++ ;
 }
 ;
string val= string_substr ( s ,  start , i ) ;
 if ( string_starts_with ( val ,  delim ) ) {
 val  =  string_right ( val ,  delim .len ) ;
 }
 ;
_PUSH(& res , ( /*typ = array_string   tmp_typ=string*/ val ), tmp12, string) ;
 start  =  i + delim .len ;
 }
 ;
 i ++ ;
 }
 ;
 if ( string_ends_with ( s ,  delim )  &&  ( nth < 1  ||  res .len < nth ) ) {
_PUSH(& res , ( /*typ = array_string   tmp_typ=string*/ tos3("") ), tmp13, string) ;
 }
 ;
return  res ;
 }
 array_string string_split_into_lines(string s) {
array_string res=new_array_from_c_array(0, 0, sizeof(string), EMPTY_ARRAY_OF_ELEMS( string, 0 ) {   TCCSKIP(0) }) ;
 if ( s .len == 0 ) {
return  res ;
 }
 ;
int start= 0 ;
 for (
int i= 0  ;  i < s .len  ;  i ++ ) { 
 
bool last= i == s .len - 1 ;
 if ( ((int)( s .str[ i ]/*rbyte 0*/ ) ) == 10  ||  last ) {
 if ( last ) {
 i ++ ;
 }
 ;
string line= string_substr ( s ,  start , i ) ;
_PUSH(& res , ( /*typ = array_string   tmp_typ=string*/ line ), tmp14, string) ;
 start  =  i + 1 ;
 }
 ;
 }
 ;
return  res ;
 }
 string string_left(string s, int n) {
 if ( n >= s .len ) {
return  s ;
 }
 ;
return  string_substr ( s ,  0 , n ) ;
 }
 string string_right(string s, int n) {
 if ( n >= s .len ) {
return  tos3("") ;
 }
 ;
return  string_substr ( s ,  n , s .len ) ;
 }
 string string_substr2(string s, int start, int _end, bool end_max) {
int end= (( end_max ) ? ( s .len ) : ( _end )) ;
return  string_substr ( s ,  start , end ) ;
 }
 string string_substr(string s, int start, int end) {
 if ( start > end  ||  start > s .len  ||  end > s .len  ||  start < 0  ||  end < 0 ) {
 v_panic ( _STR("substr(%d, %d) out of bounds (len=%d)", start, end, s .len) ) ;
 }
 ;
int len= end - start ;
string res= (string) { .len =  len , .str =  v_malloc ( len + 1 ) } ;
 for (
int i= 0  ;  i < len  ;  i ++ ) { 
 
 res .str [/*ptr!*/ i ]/*rbyte 1*/  =  s .str [/*ptr!*/ start + i ]/*rbyte 0*/ ;
 }
 ;
 res .str [/*ptr!*/ len ]/*rbyte 1*/  =  '\0' ;
return  res ;
 }
 int string_index_old(string s, string p) {
 if ( p .len > s .len ) {
return  - 1 ;
 }
 ;
int i= 0 ;
 while ( i < s .len ) {
 
int j= 0 ;
 while ( j < p .len  &&  s .str[ i + j ]/*rbyte 0*/ == p .str[ j ]/*rbyte 0*/ ) {
 
 j ++ ;
 }
 ;
 if ( j == p .len ) {
return  i ;
 }
 ;
 i ++ ;
 }
 ;
return  - 1 ;
 }
 Option_int string_index(string s, string p) {
 if ( p .len > s .len ) {
return  opt_none() ;
 }
 ;
int i= 0 ;
 while ( i < s .len ) {
 
int j= 0 ;
 while ( j < p .len  &&  s .str[ i + j ]/*rbyte 0*/ == p .str[ j ]/*rbyte 0*/ ) {
 
 j ++ ;
 }
 ;
 if ( j == p .len ) {
int tmp15 = OPTION_CAST(int)( i); 
 return opt_ok(&tmp15, sizeof(int)) ;
 }
 ;
 i ++ ;
 }
 ;
return  opt_none() ;
 }
 int string_index_kmp(string s, string p) {
 if ( p .len > s .len ) {
return  - 1 ;
 }
 ;
array_int prefix= array_repeat (new_array_from_c_array(1, 1, sizeof(int), EMPTY_ARRAY_OF_ELEMS( int, 1 ) {  0  }) ,  p .len ) ;
int j= 0 ;
 for (
int i= 1  ;  i < p .len  ;  i ++ ) { 
 
 while ( p .str[ j ]/*rbyte 0*/ != p .str[ i ]/*rbyte 0*/  &&  j > 0 ) {
 
 j  =  ( *(int*) array_get(  prefix , j - 1) ) ;
 }
 ;
 if ( p .str[ j ]/*rbyte 0*/ == p .str[ i ]/*rbyte 0*/ ) {
 j ++ ;
 }
 ;
array_set(&/*q*/ prefix , i , & (int []) {  j }) ;
 }
 ;
 j  =  0 ;
 for (
int i= 0  ;  i < s .len  ;  i ++ ) { 
 
 while ( p .str[ j ]/*rbyte 0*/ != s .str[ i ]/*rbyte 0*/  &&  j > 0 ) {
 
 j  =  ( *(int*) array_get(  prefix , j - 1) ) ;
 }
 ;
 if ( p .str[ j ]/*rbyte 0*/ == s .str[ i ]/*rbyte 0*/ ) {
 j ++ ;
 }
 ;
 if ( j == p .len ) {
return  i - p .len + 1 ;
 }
 ;
 }
 ;
return  - 1 ;
 }
 int string_index_any(string s, string chars) {
 string tmp20 =  chars;
 ;
for (int tmp21 = 0; tmp21 < tmp20 .len; tmp21 ++) {
 byte c = tmp20.str[tmp21];
 
Option_int tmp22 =  string_index ( s ,  byte_str ( c ) ) ;
 int index;
 if (!tmp22 .ok) {
 string err = tmp22 . error;
 int errcode = tmp22 . ecode;
 continue
 ;
 }
 index = *(int*)tmp22.data;
 ;
return  index ;
 }
 ;
return  - 1 ;
 }
 int string_last_index(string s, string p) {
 if ( p .len > s .len ) {
return  - 1 ;
 }
 ;
int i= s .len - p .len ;
 while ( i >= 0 ) {
 
int j= 0 ;
 while ( j < p .len  &&  s .str[ i + j ]/*rbyte 0*/ == p .str[ j ]/*rbyte 0*/ ) {
 
 j ++ ;
 }
 ;
 if ( j == p .len ) {
return  i ;
 }
 ;
 i -- ;
 }
 ;
return  - 1 ;
 }
 int string_index_after(string s, string p, int start) {
 if ( p .len > s .len ) {
return  - 1 ;
 }
 ;
int strt= start ;
 if ( start < 0 ) {
 strt  =  0 ;
 }
 ;
 if ( start >= s .len ) {
return  - 1 ;
 }
 ;
int i= strt ;
 while ( i < s .len ) {
 
int j= 0 ;
int ii= i ;
 while ( j < p .len  &&  s .str[ ii ]/*rbyte 0*/ == p .str[ j ]/*rbyte 0*/ ) {
 
 j ++ ;
 ii ++ ;
 }
 ;
 if ( j == p .len ) {
return  i ;
 }
 ;
 i ++ ;
 }
 ;
return  - 1 ;
 }
 int string_index_byte(string s, byte c) {
 for (
int i= 0  ;  i < s .len  ;  i ++ ) { 
 
 if ( s .str[ i ]/*rbyte 0*/ == c ) {
return  i ;
 }
 ;
 }
 ;
return  - 1 ;
 }
 int string_last_index_byte(string s, byte c) {
 for (
int i= s .len - 1  ;  i >= 0  ;  i -- ) { 
 
 if ( s .str[ i ]/*rbyte 0*/ == c ) {
return  i ;
 }
 ;
 }
 ;
return  - 1 ;
 }
 int string_count(string s, string substr) {
 if ( s .len == 0  ||  substr .len == 0 ) {
return  0 ;
 }
 ;
 if ( substr .len > s .len ) {
return  0 ;
 }
 ;
int n= 0 ;
int i= 0 ;
 while (1) { 
 i  =  string_index_after ( s ,  substr , i ) ;
 if ( i == - 1 ) {
return  n ;
 }
 ;
 i  +=  substr .len ;
 n ++ ;
 }
 ;
return  0 ;
 }
 bool string_contains(string s, string p) {
Option_int tmp23 =  string_index ( s ,  p ) ;
 if (!tmp23 .ok) {
 string err = tmp23 . error;
 int errcode = tmp23 . ecode;
return  0 ;
 }
 ;
return  1 ;
 }
 bool string_starts_with(string s, string p) {
Option_int tmp24 =  string_index ( s ,  p ) ;
 int idx;
 if (!tmp24 .ok) {
 string err = tmp24 . error;
 int errcode = tmp24 . ecode;
return  0 ;
 }
 idx = *(int*)tmp24.data;
 ;
return  idx == 0 ;
 }
 bool string_ends_with(string s, string p) {
 if ( p .len > s .len ) {
return  0 ;
 }
 ;
bool res= string_last_index ( s ,  p ) == s .len - p .len ;
return  res ;
 }
 string string_to_lower(string s) {
byte* b= v_malloc ( s .len + 1 ) ;
 for (
int i= 0  ;  i < s .len  ;  i ++ ) { 
 
 b [/*ptr!*/ i ]/*rbyte 1*/  =  tolower ( s .str [/*ptr!*/ i ]/*rbyte 0*/ ) ;
 }
 ;
return  tos ( b , s .len ) ;
 }
 string string_to_upper(string s) {
byte* b= v_malloc ( s .len + 1 ) ;
 for (
int i= 0  ;  i < s .len  ;  i ++ ) { 
 
 b [/*ptr!*/ i ]/*rbyte 1*/  =  toupper ( s .str [/*ptr!*/ i ]/*rbyte 0*/ ) ;
 }
 ;
return  tos ( b , s .len ) ;
 }
 string string_capitalize(string s) {
string sl= string_to_lower ( s ) ;
string cap=string_add( string_to_upper ( byte_str ( sl .str[ 0 ]/*rbyte 0*/ ) ) , string_right ( sl ,  1 ) ) ;
return  cap ;
 }
 string string_title(string s) {
array_string words= string_split ( s ,  tos3(" ") ) ;
array_string tit=new_array_from_c_array(0, 0, sizeof(string), EMPTY_ARRAY_OF_ELEMS( string, 0 ) {   TCCSKIP(0) }) ;
 array_string tmp25 =  words;
 for (int tmp26 = 0; tmp26 < tmp25.len; tmp26++) {
 string word = ((string *) tmp25 . data)[tmp26];
 
_PUSH(& tit , ( /*typ = array_string   tmp_typ=string*/ string_capitalize ( word ) ), tmp27, string) ;
 }
 ;
string title= array_string_join ( tit ,  tos3(" ") ) ;
return  title ;
 }
 string string_find_between(string s, string start, string end) {
Option_int tmp28 =  string_index ( s ,  start ) ;
 int start_pos;
 if (!tmp28 .ok) {
 string err = tmp28 . error;
 int errcode = tmp28 . ecode;
return  tos3("") ;
 }
 start_pos = *(int*)tmp28.data;
 ;
string val= string_right ( s ,  start_pos + start .len ) ;
Option_int tmp29 =  string_index ( val ,  end ) ;
 int end_pos;
 if (!tmp29 .ok) {
 string err = tmp29 . error;
 int errcode = tmp29 . ecode;
return  val ;
 }
 end_pos = *(int*)tmp29.data;
 ;
return  string_left ( val ,  end_pos ) ;
 }
 bool array_string_contains(array_string ar, string val) {
 array_string tmp30 =  ar;
 for (int tmp31 = 0; tmp31 < tmp30.len; tmp31++) {
 string s = ((string *) tmp30 . data)[tmp31];
 
 if (string_eq( s , val ) ) {
return  1 ;
 }
 ;
 }
 ;
return  0 ;
 }
 bool array_int_contains(array_int ar, int val) {
 array_int tmp32 =  ar;
 for (int i = 0; i < tmp32.len; i++) {
 int s = ((int *) tmp32 . data)[i];
 
 if ( s == val ) {
return  1 ;
 }
 ;
 }
 ;
return  0 ;
 }
 bool byte_is_space(byte c) {
return  ( c  == ' '  ||  c ==  '\n'  ||  c ==  '\t'  ||  c ==  '\v'  ||  c ==  '\f'  ||  c ==  '\r' ) ;
 }
 string string_trim_space(string s) {
return  string_trim ( s ,  tos3(" \n\t\v\f\r") ) ;
 }
 string string_trim(string s, string cutset) {
 if ( s .len < 1  ||  cutset .len < 1 ) {
return  s ;
 }
 ;
array_byte cs_arr= string_bytes ( cutset ) ;
int pos_left= 0 ;
int pos_right= s .len - 1 ;
bool cs_match= 1 ;
 while ( pos_left <= s .len  &&  pos_right >= - 1  &&  cs_match ) {
 
 cs_match  =  0 ;
 if ((_IN(byte, ( s .str[ pos_left ]/*rbyte 0*/ ),  cs_arr )) ) {
 pos_left ++ ;
 cs_match  =  1 ;
 }
 ;
 if ((_IN(byte, ( s .str[ pos_right ]/*rbyte 0*/ ),  cs_arr )) ) {
 pos_right -- ;
 cs_match  =  1 ;
 }
 ;
 if ( pos_left > pos_right ) {
return  tos3("") ;
 }
 ;
 }
 ;
return  string_substr ( s ,  pos_left , pos_right + 1 ) ;
 }
 string string_trim_left(string s, string cutset) {
 if ( s .len < 1  ||  cutset .len < 1 ) {
return  s ;
 }
 ;
array_byte cs_arr= string_bytes ( cutset ) ;
int pos= 0 ;
 while ( pos <= s .len  && (_IN(byte, ( s .str[ pos ]/*rbyte 0*/ ),  cs_arr )) ) {
 
 pos ++ ;
 }
 ;
return  string_right ( s ,  pos ) ;
 }
 string string_trim_right(string s, string cutset) {
 if ( s .len < 1  ||  cutset .len < 1 ) {
return  s ;
 }
 ;
array_byte cs_arr= string_bytes ( cutset ) ;
int pos= s .len - 1 ;
 while ( pos >= - 1  && (_IN(byte, ( s .str[ pos ]/*rbyte 0*/ ),  cs_arr )) ) {
 
 pos -- ;
 }
 ;
return  string_left ( s ,  pos + 1 ) ;
 }
 int compare_strings(string* a, string* b) {
 if ( string_lt (* a , * b ) ) {
return  - 1 ;
 }
 ;
 if ( string_gt (* a , * b ) ) {
return  1 ;
 }
 ;
return  0 ;
 }
 int compare_strings_by_len(string* a, string* b) {
 if ( a ->len < b ->len ) {
return  - 1 ;
 }
 ;
 if ( a ->len > b ->len ) {
return  1 ;
 }
 ;
return  0 ;
 }
 int compare_lower_strings(string* a, string* b) {
string aa= string_to_lower (* a ) ;
string bb= string_to_lower (* b ) ;
return  compare_strings (& /*114*/ aa ,& /*114*/ bb ) ;
 }
 void array_string_sort(array_string* s) {
 array_sort_with_compare ( s , & /*112 e="void*" g="fn (string*,string*) int" */ compare_strings ) ;
 }
 void array_string_sort_ignore_case(array_string* s) {
 array_sort_with_compare ( s , & /*112 e="void*" g="fn (string*,string*) int" */ compare_lower_strings ) ;
 }
 void array_string_sort_by_len(array_string* s) {
 array_sort_with_compare ( s , & /*112 e="void*" g="fn (string*,string*) int" */ compare_strings_by_len ) ;
 }
 ustring string_ustring(string s) {
ustring res= (ustring) { .s =  s , .runes =  new_array ( 0 , s .len , sizeof( int) ) , .len =  0 } ;
 for (
int i= 0  ;  i < s .len  ;  i ++ ) { 
 
int char_len= utf8_char_len ( s .str [/*ptr!*/ i ]/*rbyte 0*/ ) ;
_PUSH(& res .runes , ( /*typ = array_int   tmp_typ=int*/ i ), tmp33, int) ;
 i  +=  char_len - 1 ;
 res .len ++ ;
 }
 ;
return  res ;
 }
 ustring string_ustring_tmp(string s) {
 if ( g_ustring_runes .len == 0 ) {
 g_ustring_runes  =  new_array ( 0 , 128 , sizeof( int) ) ;
 }
 ;
ustring res= (ustring) { .s =  s , .runes =  new_array(0, 1, sizeof( int )) , .len =  0 } ;
 res .runes  =  g_ustring_runes ;
 res .runes .len  =  s .len ;
int j= 0 ;
 for (
int i= 0  ;  i < s .len  ;  i ++ ) { 
 
int char_len= utf8_char_len ( s .str [/*ptr!*/ i ]/*rbyte 0*/ ) ;
array_set(&/*q*/ res .runes , j , & (int []) {  i }) ;
 j ++ ;
 i  +=  char_len - 1 ;
 res .len ++ ;
 }
 ;
return  res ;
 }
 bool ustring_eq(ustring u, ustring a) {
 if ( u .len != a .len  || string_ne( u .s , a .s ) ) {
return  0 ;
 }
 ;
return  1 ;
 }
 bool ustring_ne(ustring u, ustring a) {
return  ! ustring_eq ( u ,  a ) ;
 }
 bool ustring_lt(ustring u, ustring a) {
return string_lt( u .s , a .s ) ;
 }
 bool ustring_le(ustring u, ustring a) {
return  ustring_lt ( u ,  a )  ||  ustring_eq ( u ,  a ) ;
 }
 bool ustring_gt(ustring u, ustring a) {
return  ! ustring_le ( u ,  a ) ;
 }
 bool ustring_ge(ustring u, ustring a) {
return  ! ustring_lt ( u ,  a ) ;
 }
 ustring ustring_add(ustring u, ustring a) {
ustring res= (ustring) { .s = string_add( u .s , a .s ) , .runes =  new_array ( 0 , u .s .len + a .s .len , sizeof( int) ) , .len =  0 } ;
int j= 0 ;
 for (
int i= 0  ;  i < u .s .len  ;  i ++ ) { 
 
int char_len= utf8_char_len ( u .s .str [/*ptr!*/ i ]/*rbyte 0*/ ) ;
_PUSH(& res .runes , ( /*typ = array_int   tmp_typ=int*/ j ), tmp34, int) ;
 i  +=  char_len - 1 ;
 j  +=  char_len ;
 res .len ++ ;
 }
 ;
 for (
int i= 0  ;  i < a .s .len  ;  i ++ ) { 
 
int char_len= utf8_char_len ( a .s .str [/*ptr!*/ i ]/*rbyte 0*/ ) ;
_PUSH(& res .runes , ( /*typ = array_int   tmp_typ=int*/ j ), tmp35, int) ;
 i  +=  char_len - 1 ;
 j  +=  char_len ;
 res .len ++ ;
 }
 ;
return  res ;
 }
 int ustring_index_after(ustring u, ustring p, int start) {
 if ( p .len > u .len ) {
return  - 1 ;
 }
 ;
int strt= start ;
 if ( start < 0 ) {
 strt  =  0 ;
 }
 ;
 if ( start > u .len ) {
return  - 1 ;
 }
 ;
int i= strt ;
 while ( i < u .len ) {
 
int j= 0 ;
int ii= i ;
 while ( j < p .len  && string_eq( ustring_at ( u ,  ii ) , ustring_at ( p ,  j ) ) ) {
 
 j ++ ;
 ii ++ ;
 }
 ;
 if ( j == p .len ) {
return  i ;
 }
 ;
 i ++ ;
 }
 ;
return  - 1 ;
 }
 int ustring_count(ustring u, ustring substr) {
 if ( u .len == 0  ||  substr .len == 0 ) {
return  0 ;
 }
 ;
 if ( substr .len > u .len ) {
return  0 ;
 }
 ;
int n= 0 ;
int i= 0 ;
 while (1) { 
 i  =  ustring_index_after ( u ,  substr , i ) ;
 if ( i == - 1 ) {
return  n ;
 }
 ;
 i  +=  substr .len ;
 n ++ ;
 }
 ;
return  0 ;
 }
 string ustring_substr(ustring u, int _start, int _end) {
 if ( _start > _end  ||  _start > u .len  ||  _end > u .len  ||  _start < 0  ||  _end < 0 ) {
 v_panic ( _STR("substr(%d, %d) out of bounds (len=%d)", _start, _end, u .len) ) ;
 }
 ;
int end= (( _end >= u .len ) ? ( u .s .len ) : ( ( *(int*) array_get(  u .runes , _end) ) )) ;
return  string_substr ( u .s ,  ( *(int*) array_get(  u .runes , _start) ) , end ) ;
 }
 string ustring_left(ustring u, int pos) {
 if ( pos >= u .len ) {
return  u .s ;
 }
 ;
return  ustring_substr ( u ,  0 , pos ) ;
 }
 string ustring_right(ustring u, int pos) {
 if ( pos >= u .len ) {
return  tos3("") ;
 }
 ;
return  ustring_substr ( u ,  pos , u .len ) ;
 }
 byte string_at(string s, int idx) {
 if ( idx < 0  ||  idx >= s .len ) {
 v_panic ( _STR("string index out of range: %d / %d", idx, s .len) ) ;
 }
 ;
return  s .str [/*ptr!*/ idx ]/*rbyte 0*/ ;
 }
 string ustring_at(ustring u, int idx) {
 if ( idx < 0  ||  idx >= u .len ) {
 v_panic ( _STR("string index out of range: %d / %d", idx, u .runes .len) ) ;
 }
 ;
return  ustring_substr ( u ,  idx , idx + 1 ) ;
 }
 void v_ustring_free(ustring u) {
 v_array_free ( u .runes ) ;
 }
 bool byte_is_digit(byte c) {
return  c >= '0'  &&  c <= '9' ;
 }
 bool byte_is_hex_digit(byte c) {
return  byte_is_digit ( c )  ||  ( c >= 'a'  &&  c <= 'f' )  ||  ( c >= 'A'  &&  c <= 'F' ) ;
 }
 bool byte_is_oct_digit(byte c) {
return  c >= '0'  &&  c <= '7' ;
 }
 bool byte_is_letter(byte c) {
return  ( c >= 'a'  &&  c <= 'z' )  ||  ( c >= 'A'  &&  c <= 'Z' ) ;
 }
 void v_string_free(string s) {
 v_free ( s .str ) ;
 }
 string string_all_before(string s, string dot) {
Option_int tmp40 =  string_index ( s ,  dot ) ;
 int pos;
 if (!tmp40 .ok) {
 string err = tmp40 . error;
 int errcode = tmp40 . ecode;
return  s ;
 }
 pos = *(int*)tmp40.data;
 ;
return  string_left ( s ,  pos ) ;
 }
 string string_all_before_last(string s, string dot) {
int pos= string_last_index ( s ,  dot ) ;
 if ( pos == - 1 ) {
return  s ;
 }
 ;
return  string_left ( s ,  pos ) ;
 }
 string string_all_after(string s, string dot) {
int pos= string_last_index ( s ,  dot ) ;
 if ( pos == - 1 ) {
return  s ;
 }
 ;
return  string_right ( s ,  pos + dot .len ) ;
 }
 string array_string_join(array_string a, string del) {
 if ( a .len == 0 ) {
return  tos3("") ;
 }
 ;
int len= 0 ;
 array_string tmp41 =  a;
 for (int i = 0; i < tmp41.len; i++) {
 string val = ((string *) tmp41 . data)[i];
 
 len  +=  val .len + del .len ;
 }
 ;
 len  -=  del .len ;
string res= tos3("") ;
 res .len  =  len ;
 res .str  =  v_malloc ( res .len + 1 ) ;
int idx= 0 ;
 array_string tmp42 =  a;
 for (int i = 0; i < tmp42.len; i++) {
 string val = ((string *) tmp42 . data)[i];
 
 for (
int j= 0  ;  j < val .len  ;  j ++ ) { 
 
byte c= val .str[ j ]/*rbyte 0*/ ;
 res .str [/*ptr!*/ idx ]/*rbyte 1*/  =  val .str [/*ptr!*/ j ]/*rbyte 0*/ ;
 idx ++ ;
 }
 ;
 if ( i != a .len - 1 ) {
 for (
int k= 0  ;  k < del .len  ;  k ++ ) { 
 
 res .str [/*ptr!*/ idx ]/*rbyte 1*/  =  del .str [/*ptr!*/ k ]/*rbyte 0*/ ;
 idx ++ ;
 }
 ;
 }
 ;
 }
 ;
 res .str [/*ptr!*/ res .len ]/*rbyte 1*/  =  '\0' ;
return  res ;
 }
 string array_string_join_lines(array_string s) {
return  array_string_join ( s ,  tos3("\n") ) ;
 }
 string string_reverse(string s) {
string res= (string) { .len =  s .len , .str =  v_malloc ( s .len ) } ;
 for (
int i= s .len - 1  ;  i >= 0  ;  i -- ) { 
 
 res .str[ s .len - i - 1 ]/*rbyte 1*/  =  s .str[ i ]/*rbyte 0*/ ;
 }
 ;
return  res ;
 }
 string string_limit(string s, int max) {
ustring u= string_ustring ( s ) ;
 if ( u .len <= max ) {
return  s ;
 }
 ;
return  ustring_substr ( u ,  0 , max ) ;
 }
 bool byte_is_white(byte c) {
int i= ((int)( c ) ) ;
return  i == 10  ||  i == 32  ||  i == 9  ||  i == 13  ||  c == '\r' ;
 }
 int string_hash(string s) {
int h= 0 ;
 if ( h == 0  &&  s .len > 0 ) {
 string tmp43 =  s;
 ;
for (int tmp44 = 0; tmp44 < tmp43 .len; tmp44 ++) {
 byte c = tmp43.str[tmp44];
 
 h  =  h * 31 + ((int)( c ) ) ;
 }
 ;
 }
 ;
return  h ;
 }
 array_byte string_bytes(string s) {
 if ( s .len == 0 ) {
return new_array_from_c_array(0, 0, sizeof(byte), EMPTY_ARRAY_OF_ELEMS( byte, 0 ) {   TCCSKIP(0) }) ;
 }
 ;
array_byte buf= array_repeat (new_array_from_c_array(1, 1, sizeof(byte), EMPTY_ARRAY_OF_ELEMS( byte, 1 ) {  ((byte)( 0 ) )  }) ,  s .len ) ;
 memcpy ( buf .data , (char*) s .str ,  s .len ) ;
return  buf ;
 }
 string string_repeat(string s, int count) {
 if ( count <= 1 ) {
return  s ;
 }
 ;
byte* ret= v_malloc ( s .len * count + 1 ) ;
 int tmp45 =  0;
 ;
for (int tmp46 = tmp45; tmp46 <  count; tmp46++) {
 int i = tmp46;
 
 int tmp47 =  0;
 ;
for (int tmp48 = tmp47; tmp48 <  s .len; tmp48++) {
 int j = tmp48;
 
 ret [/*ptr!*/ i * s .len + j ]/*rbyte 1*/  =  s .str[ j ]/*rbyte 0*/ ;
 }
 ;
 }
 ;
 ret [/*ptr!*/ s .len * count ]/*rbyte 1*/  =  0 ;
return  (tos2((byte *) ret ) ) ;
 }
 int utf8_char_len(byte b) {
return  ( ( 0xe5000000  >>  ( ( b  >>  3 ) & 0x1e ) ) & 3 ) + 1 ;
 }
 string utf32_to_str(u32 code) {
int icode= ((int)( code ) ) ;
byte* buffer= v_malloc ( 5 ) ;
 if ( icode <= 127 ) {
 buffer [/*ptr!*/ 0 ]/*rbyte 1*/  =  icode ;
return  tos ( buffer , 1 ) ;
 }
 ;
 if ( ( icode <= 2047 ) ) {
 buffer [/*ptr!*/ 0 ]/*rbyte 1*/  =  192 | ( icode  >>  6 ) ;
 buffer [/*ptr!*/ 1 ]/*rbyte 1*/  =  128 | ( icode & 63 ) ;
return  tos ( buffer , 2 ) ;
 }
 ;
 if ( ( icode <= 65535 ) ) {
 buffer [/*ptr!*/ 0 ]/*rbyte 1*/  =  224 | ( icode  >>  12 ) ;
 buffer [/*ptr!*/ 1 ]/*rbyte 1*/  =  128 | ( ( icode  >>  6 ) & 63 ) ;
 buffer [/*ptr!*/ 2 ]/*rbyte 1*/  =  128 | ( icode & 63 ) ;
return  tos ( buffer , 3 ) ;
 }
 ;
 if ( ( icode <= 1114111 ) ) {
 buffer [/*ptr!*/ 0 ]/*rbyte 1*/  =  240 | ( icode  >>  18 ) ;
 buffer [/*ptr!*/ 1 ]/*rbyte 1*/  =  128 | ( ( icode  >>  12 ) & 63 ) ;
 buffer [/*ptr!*/ 2 ]/*rbyte 1*/  =  128 | ( ( icode  >>  6 ) & 63 ) ;
 buffer [/*ptr!*/ 3 ]/*rbyte 1*/  =  128 | ( icode & 63 ) ;
return  tos ( buffer , 4 ) ;
 }
 ;
return  tos3("") ;
 }
 string utf32_to_str_no_malloc(u32 code, void* buf) {
int icode= ((int)( code ) ) ;
byteptr buffer= ((byteptr)( buf ) ) ;
 if ( icode <= 127 ) {
 buffer [/*ptr!*/ 0 ]/*rbyteptr 1*/  =  icode ;
return  tos ( buffer , 1 ) ;
 }
 ;
 if ( ( icode <= 2047 ) ) {
 buffer [/*ptr!*/ 0 ]/*rbyteptr 1*/  =  192 | ( icode  >>  6 ) ;
 buffer [/*ptr!*/ 1 ]/*rbyteptr 1*/  =  128 | ( icode & 63 ) ;
return  tos ( buffer , 2 ) ;
 }
 ;
 if ( ( icode <= 65535 ) ) {
 buffer [/*ptr!*/ 0 ]/*rbyteptr 1*/  =  224 | ( icode  >>  12 ) ;
 buffer [/*ptr!*/ 1 ]/*rbyteptr 1*/  =  128 | ( ( icode  >>  6 ) & 63 ) ;
 buffer [/*ptr!*/ 2 ]/*rbyteptr 1*/  =  128 | ( icode & 63 ) ;
return  tos ( buffer , 3 ) ;
 }
 ;
 if ( ( icode <= 1114111 ) ) {
 buffer [/*ptr!*/ 0 ]/*rbyteptr 1*/  =  240 | ( icode  >>  18 ) ;
 buffer [/*ptr!*/ 1 ]/*rbyteptr 1*/  =  128 | ( ( icode  >>  12 ) & 63 ) ;
 buffer [/*ptr!*/ 2 ]/*rbyteptr 1*/  =  128 | ( ( icode  >>  6 ) & 63 ) ;
 buffer [/*ptr!*/ 3 ]/*rbyteptr 1*/  =  128 | ( icode & 63 ) ;
return  tos ( buffer , 4 ) ;
 }
 ;
return  tos3("") ;
 }
 int string_utf32_code(string _rune) {
 if ( _rune .len == 0 ) {
return  0 ;
 }
 ;
 if ( _rune .len == 1 ) {
return  ((int)( _rune .str[ 0 ]/*rbyte 0*/ ) ) ;
 }
 ;
byte b= ((byte)( ((int)( _rune .str[ 0 ]/*rbyte 0*/ ) ) ) ) ;
 b  =  b  <<  _rune .len ;
int res= ((int)( b ) ) ;
int shift= 6 - _rune .len ;
 for (
int i= 1  ;  i < _rune .len  ;  i ++ ) { 
 
int c= ((int)( _rune .str[ i ]/*rbyte 0*/ ) ) ;
 res  =  res  <<  shift ;
 res  |=  c & 63 ;
 shift  =  6 ;
 }
 ;
return  res ;
 }
 u16* string_to_wide(string _str) {
 #ifdef _WIN32
 #else
return  0 ;
 #endif
 ;
 }
 string string_from_wide(u16* _wstr) {
 #ifdef _WIN32
 #else
return  tos3("") ;
 #endif
 ;
 }
 string string_from_wide2(u16* _wstr, int len) {
 #ifdef _WIN32
 #else
return  tos3("") ;
 #endif
 ;
 }
 int utf8_len(byte c) {
int b= 0 ;
byte x= c ;
 if ( ( ( x & 240 ) != 0 ) ) {
 x  >>=  4 ;
 }
  else { 
 b  +=  4 ;
 }
 ;
 if ( ( ( x & 12 ) != 0 ) ) {
 x  >>=  2 ;
 }
  else { 
 b  +=  2 ;
 }
 ;
 if ( ( ( x & 2 ) == 0 ) ) {
 b ++ ;
 }
 ;
return  b ;
 }
 int utf8_getchar() {
int c= ((int)( getchar ( ) ) ) ;
int len= utf8_len ( ~ c ) ;
 if ( c < 0 ) {
return  0 ;
 }
  else  if ( len == 0 ) {
return  c ;
 }
  else  if ( len == 1 ) {
return  - 1 ;
 }
  else { 
int uc= ((int)( c & ( ( 1  <<  ( 7 - len ) ) - 1 ) ) ) ;
 for (
int i= 0  ;  i + 1 < len  ;  i ++ ) { 
 
int c2= ((int)( getchar ( ) ) ) ;
 if ( c2 != - 1  &&  ( c2  >>  6 ) == 2 ) {
 uc  <<=  6 ;
 uc  |=  ((int)( ( c2 & 63 ) ) ) ;
 }
  else  if ( c2 == - 1 ) {
return  0 ;
 }
  else { 
return  - 1 ;
 }
 ;
 }
 ;
return  uc ;
 }
 ;
 }
 bool gx__Image_is_empty(gx__Image img) {
return  isnil ( img .obj ) ;
 }
 string gx__Color_str(gx__Color c) {
return  _STR("{%d, %d, %d}", c .r, c .g, c .b) ;
 }
 bool gx__Color_eq(gx__Color a, gx__Color b) {
return  a .r == b .r  &&  a .g == b .g  &&  a .b == b .b ;
 }
 gx__Color gx__rgb(int r, int g, int b) {
gx__Color res= (gx__Color) { .r =  r , .g =  g , .b =  b } ;
return  res ;
 }
 gl__Shader gl__new_shader(string name) {
string dir= tos3("") ;
 if ( string_starts_with ( name ,  tos3("/") ) ) {
 dir  =  tos3("") ;
 }
 ;
string vertex_path= _STR("%.*s%.*s.vert", dir.len, dir.str, name.len, name.str) ;
string fragment_path= _STR("%.*s%.*s.frag", dir.len, dir.str, name.len, name.str) ;
string vertex_src= tos3("") ;
string fragment_src= tos3("") ;
 if (string_eq( name , tos3("text") ) ) {
 vertex_src  =  gl__TEXT_VERT ;
 fragment_src  =  gl__TEXT_FRAG ;
 }
  else  if (string_eq( name , tos3("simple") ) ) {
 vertex_src  =  gl__SIMPLE_VERT ;
 fragment_src  =  gl__SIMPLE_FRAG ;
 }
 ;
int vertex_shader= gl__create_shader ( GL_VERTEX_SHADER ) ;
 gl__shader_source ( vertex_shader , 1 , vertex_src , 0 ) ;
 gl__compile_shader ( vertex_shader ) ;
 if ( gl__shader_compile_status ( vertex_shader ) == 0 ) {
string log= gl__shader_info_log ( vertex_shader ) ;
printf( "shader %d compilation failed\n", vertex_shader ) ;
printf( "shader source = %.*s\n", vertex_src.len, vertex_src.str ) ;
 println ( tos3("shader failed to compile") ) ;
 v_exit ( 1 ) ;
 }
 ;
int fragment_shader= gl__create_shader ( GL_FRAGMENT_SHADER ) ;
 gl__shader_source ( fragment_shader , 1 , fragment_src , 0 ) ;
 gl__compile_shader ( fragment_shader ) ;
 if ( gl__shader_compile_status ( fragment_shader ) == 0 ) {
printf( "fragment %d shader compilation failed\n", fragment_shader ) ;
 println ( tos3("shader failed to compile") ) ;
 v_exit ( 1 ) ;
 }
 ;
int shader_program= gl__create_program ( ) ;
 gl__attach_shader ( shader_program , vertex_shader ) ;
 gl__attach_shader ( shader_program , fragment_shader ) ;
 gl__link_program ( shader_program ) ;
int success= gl__get_program_link_status ( shader_program ) ;
 if ( success == 0 ) {
 println ( tos3("shader compilation failed") ) ;
printf( "vertex source = %.*s\n", vertex_src.len, vertex_src.str ) ;
printf( "fragment source = %.*s\n", fragment_src.len, fragment_src.str ) ;
 println ( tos3("shader failed to compile") ) ;
 v_exit ( 1 ) ;
 }
 ;
gl__Shader shader= (gl__Shader) { .program_id =  shader_program } ;
return  shader ;
 }
 void gl__Shader_use(gl__Shader s) {
 gl__use_program ( s .program_id ) ;
 }
 int gl__Shader_uni_location(gl__Shader s, string key) {
return  glGetUniformLocation ( s .program_id , (char*) key .str ) ;
 }
 void gl__Shader_set_mat4(gl__Shader s, string str, glm__Mat4 m) {
 glUniformMatrix4fv ( gl__Shader_uni_location ( s ,  str ) ,  1 ,  0 ,  m .data ) ;
 }
 void gl__Shader_set_int(gl__Shader s, string str, int n) {
 glUniform1i ( gl__Shader_uni_location ( s ,  str ) ,  n ) ;
 }
 void gl__Shader_set_color(gl__Shader s, string str, gx__Color c) {
 glUniform3f ( gl__Shader_uni_location ( s ,  str ) ,  ((f32)( c .r ) ) / 255.0 ,  ((f32)( c .g ) ) / 255.0 ,  ((f32)( c .b ) ) / 255.0 ) ;
 }
 void gl__init_glad() {
void* ok= gladLoadGL ( ) ;
 if ( isnil ( ok ) ) {
 println ( tos3("Failed to initialize glad OpenGL context") ) ;
 v_exit ( 1 ) ;
 }
 ;
 }
 void gl__viewport(int a, int b, int c, int d) {
 glViewport ( a ,  b ,  c ,  d ) ;
 }
 void gl__clear_color(int r, int g, int b, int a) {
 glClearColor ( ((f32)( r ) ) / 255.0 ,  ((f32)( g ) ) / 255.0 ,  ((f32)( b ) ) / 255.0 ,  ((f32)( a ) ) / 255.0 ) ;
 }
 void gl__clear() {
 glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT ) ;
 }
 int gl__create_shader(int typ) {
return  glCreateShader ( typ ) ;
 }
 int gl__create_program() {
return  glCreateProgram ( ) ;
 }
 void gl__shader_source(int shader, int a, string source, int b) {
 glShaderSource ( shader ,  a ,  & source .str ,  b ) ;
 }
 void gl__compile_shader(int shader) {
 glCompileShader ( shader ) ;
 }
 int gl__shader_compile_status(int shader) {
int success= 0 ;
 glGetShaderiv ( shader ,  GL_COMPILE_STATUS ,  & success ) ;
return  success ;
 }
 void gl__attach_shader(int program, int shader) {
 glAttachShader ( program ,  shader ) ;
 }
 void gl__link_program(int program) {
 glLinkProgram ( program ) ;
 }
 int gl__get_program_link_status(int program) {
int success= 0 ;
 glGetProgramiv ( program ,  GL_LINK_STATUS ,  & success ) ;
return  success ;
 }
 void gl__delete_shader(int shader) {
 glDeleteShader ( shader ) ;
 }
 string gl__shader_info_log(int shader) {
byte info_log  [512 ] = { 0 } ;
 glGetShaderInfoLog ( shader ,  512 ,  0 ,  info_log ) ;
return  tos_clone ( info_log ) ;
 }
 string gl__get_program_info_log(int program) {
byte info_log  [1024 ] = { 0 } ;
 glGetProgramInfoLog ( program ,  1024 ,  0 ,  info_log ) ;
return  tos_clone ( info_log ) ;
 }
 void gl__bind_vao(u32 vao) {
 glBindVertexArray ( vao ) ;
 }
 void gl__bind_buffer(int typ, u32 vbo) {
 glBindBuffer ( typ ,  vbo ) ;
 }
 u32 gl__gen_texture() {
u32 res= ((u32)( 0 ) ) ;
 glGenTextures ( 1 ,  & res ) ;
return  res ;
 }
 void gl__active_texture(int t) {
 glActiveTexture ( t ) ;
 }
 void gl__bind_2d_texture(u32 texture) {
 glBindTexture ( GL_TEXTURE_2D ,  texture ) ;
 }
 void gl__delete_texture(u32 texture) {
 glDeleteTextures ( 1 ,  & texture ) ;
 }
 void gl__buffer_data(int typ, int size, void* arr, int draw_typ) {
 glBufferData ( typ ,  size ,  arr ,  draw_typ ) ;
 }
 void gl__buffer_data_int(int typ, array_int vertices, int draw_typ) {
int size= sizeof( int) * vertices .len ;
 glBufferData ( typ ,  size ,  vertices .data ,  draw_typ ) ;
 }
 void gl__buffer_data_f32(int typ, array_f32 vertices, int draw_typ) {
int size= sizeof( f32) * vertices .len ;
 glBufferData ( typ ,  size ,  vertices .data ,  draw_typ ) ;
 }
 void gl__set_vbo(u32 vbo, array_f32 vertices, int draw_typ) {
 gl__bind_buffer ( GL_ARRAY_BUFFER , vbo ) ;
 gl__buffer_data_f32 ( GL_ARRAY_BUFFER , vertices , draw_typ ) ;
 }
 void gl__set_ebo(u32 ebo, array_int indices, int draw_typ) {
 gl__bind_buffer ( GL_ELEMENT_ARRAY_BUFFER , ebo ) ;
 gl__buffer_data_int ( GL_ELEMENT_ARRAY_BUFFER , indices , draw_typ ) ;
 }
 void gl__draw_arrays(int typ, int start, int len) {
 glDrawArrays ( typ ,  start ,  len ) ;
 }
 void gl__draw_elements(int mode, int count, int typ, int indices) {
 glDrawElements ( mode ,  count ,  typ ,  indices ) ;
 }
 void gl__use_program(int program) {
 glUseProgram ( program ) ;
 }
 u32 gl__gen_vertex_array() {
u32 vao= ((u32)( 0 ) ) ;
 glGenVertexArrays ( 1 ,  & vao ) ;
return  vao ;
 }
 void gl__enable_vertex_attrib_array(int n) {
 glEnableVertexAttribArray ( n ) ;
 }
 u32 gl__gen_buffer() {
u32 vbo= ((u32)( 0 ) ) ;
 glGenBuffers ( 1 ,  & vbo ) ;
return  vbo ;
 }
 void gl__vertex_attrib_pointer(int index, int size, int typ, bool normalized, int _stride, int _ptr) {
int stride= _stride ;
int ptr= _ptr ;
 if ( typ == GL_FLOAT ) {
 stride  *=  sizeof( f32) ;
 ptr  *=  sizeof( f32) ;
 }
 ;
 glVertexAttribPointer ( index ,  size ,  typ ,  normalized ,  stride ,  ptr ) ;
 }
 void gl__tex_param(int key, int val) {
 glTexParameteri ( GL_TEXTURE_2D ,  key ,  val ) ;
 }
 void gl__enable(int val) {
 glEnable ( val ) ;
 }
 void gl__disable(int val) {
 glDisable ( val ) ;
 }
 void gl__scissor(int a, int b, int c, int d) {
 glScissor ( a ,  b ,  c ,  d ) ;
 }
 void gl__generate_mipmap(int typ) {
 glGenerateMipmap ( typ ) ;
 }
 strings__Builder strings__new_builder(int initial_size) {
return  (strings__Builder) { .buf =  make ( 0 , initial_size , 1 ) , .len =  0 } ;
 }
 void strings__Builder_write(strings__Builder* b, string s) {
 array_push_many (& /* ? */ b ->buf ,  s .str , s .len ) ;
 b ->len  +=  s .len ;
 }
 void strings__Builder_writeln(strings__Builder* b, string s) {
 array_push_many (& /* ? */ b ->buf ,  s .str , s .len ) ;
_PUSH(& b ->buf , ( /*typ = array_byte   tmp_typ=byte*/ '\n' ), tmp1, byte) ;
 b ->len  +=  s .len + 1 ;
 }
 string strings__Builder_str(strings__Builder* b) {
_PUSH(& b ->buf , ( /*typ = array_byte   tmp_typ=byte*/ '\0' ), tmp2, byte) ;
return  (tos((byte *) b ->buf .data ,  b ->len ) ) ;
 }
 void strings__Builder_free(strings__Builder* b) {
 {
 v_free ( b ->buf .data ) ;
 }
 ;
 b ->buf  =  make ( 0 , 1 , 1 ) ;
 b ->len  =  0 ;
 }
 int strings__levenshtein_distance(string a, string b) {
array_int f= array_repeat (new_array_from_c_array(1, 1, sizeof(int), EMPTY_ARRAY_OF_ELEMS( int, 1 ) {  0  }) ,  b .len + 1 ) ;
 string tmp1 =  a;
 ;
for (int tmp2 = 0; tmp2 < tmp1 .len; tmp2 ++) {
 byte ca = tmp1.str[tmp2];
 
int j= 1 ;
int fj1= ( *(int*) array_get(  f , 0) ) ;
 ( *(int*) array_get(  f , 0) ) ++ ;
 string tmp7 =  b;
 ;
for (int tmp8 = 0; tmp8 < tmp7 .len; tmp8 ++) {
 byte cb = tmp7.str[tmp8];
 
int mn= (( ( *(int*) array_get(  f , j) ) + 1 <= ( *(int*) array_get(  f , j - 1) ) + 1 ) ? ( ( *(int*) array_get(  f , j) ) + 1 ) : ( ( *(int*) array_get(  f , j - 1) ) + 1 )) ;
 if ( cb != ca ) {
 mn  =  (( mn <= fj1 + 1 ) ? ( mn ) : ( fj1 + 1 )) ;
 }
  else { 
 mn  =  (( mn <= fj1 ) ? ( mn ) : ( fj1 )) ;
 }
 ;
 fj1  =  ( *(int*) array_get(  f , j) ) ;
array_set(&/*q*/ f , j , & (int []) {  mn }) ;
 j ++ ;
 }
 ;
 }
 ;
return  ( *(int*) array_get(  f , f .len - 1) ) ;
 }
 f32 strings__levenshtein_distance_percentage(string a, string b) {
int d= strings__levenshtein_distance ( a , b ) ;
int l= (( a .len >= b .len ) ? ( a .len ) : ( b .len )) ;
return  ( 1.00 - ((f32)( d ) ) / ((f32)( l ) ) ) * 100.00 ;
 }
 f32 strings__dice_coefficient(string s1, string s2) {
 if ( s1 .len == 0  ||  s2 .len == 0 ) {
return  0.0 ;
 }
 ;
 if (string_eq( s1 , s2 ) ) {
return  1.0 ;
 }
 ;
 if ( s1 .len < 2  ||  s2 .len < 2 ) {
return  0.0 ;
 }
 ;
string a= (( s1 .len > s2 .len ) ? ( s1 ) : ( s2 )) ;
string b= ((string_eq( a , s1 ) ) ? ( s2 ) : ( s1 )) ;
map_int first_bigrams= new_map(1, sizeof(int)) ;
 for (
int i= 0  ;  i < a .len - 1  ;  i ++ ) { 
 
string bigram= string_substr2( a ,  i , i + 2 , false) ; 
 int tmp23 = 0; bool tmp24 = map_get(/*similarity.v : 47*/ first_bigrams , bigram, & tmp23); 

int q= (((_IN_MAP( ( bigram ),  first_bigrams )) ) ? ( tmp23 + 1 ) : ( 1 )) ;
map_set(& first_bigrams , bigram , & (int []) {  q }) ;
 }
 ;
int intersection_size= 0 ;
 for (
int i= 0  ;  i < b .len - 1  ;  i ++ ) { 
 
string bigram= string_substr2( b ,  i , i + 2 , false) ; 
 int tmp27 = 0; bool tmp28 = map_get(/*similarity.v : 53*/ first_bigrams , bigram, & tmp27); 

int count= (((_IN_MAP( ( bigram ),  first_bigrams )) ) ? ( tmp27 ) : ( 0 )) ;
 if ( count > 0 ) {
map_set(& first_bigrams , bigram , & (int []) {  count - 1 }) ;
 intersection_size ++ ;
 }
 ;
 }
 ;
return  ( 2.0 * intersection_size ) / ( ((f32)( a .len ) ) + ((f32)( b .len ) ) - 2 ) ;
 }
 string strings__repeat(byte c, int n) {
 if ( n <= 0 ) {
return  tos3("") ;
 }
 ;
array_byte arr= array_repeat (new_array_from_c_array(1, 1, sizeof(byte), EMPTY_ARRAY_OF_ELEMS( byte, 1 ) {  c  }) ,  n + 1 ) ;
array_set(&/*q*/ arr , n , & (byte []) {  '\0' }) ;
return  (tos((byte *) arr .data ,  n ) ) ;
 }
 byte strconv__byte_to_lower(byte c) {
return  c | ( 'x' - 'X' ) ;
 }
 u64 strconv__common_parse_uint(string s, int _base, int _bit_size, bool error_on_non_digit, bool error_on_high_digit) {
int bit_size= _bit_size ;
int base= _base ;
 if ( s .len < 1  ||  ! strconv__underscore_ok ( s ) ) {
return  ((u64)( 0 ) ) ;
 }
 ;
bool base0= base == 0 ;
int start_index= 0 ;
 if ( 2 <= base  &&  base <= 36 ) {
 }
  else  if ( base == 0 ) {
 base  =  10 ;
 if ( string_at( s ,  0) == '0' ) {
 if ( s .len >= 3  &&  strconv__byte_to_lower ( string_at( s ,  1) ) == 'b' ) {
 base  =  2 ;
 start_index  +=  2 ;
 }
  else  if ( s .len >= 3  &&  strconv__byte_to_lower ( string_at( s ,  1) ) == 'o' ) {
 base  =  8 ;
 start_index  +=  2 ;
 }
  else  if ( s .len >= 3  &&  strconv__byte_to_lower ( string_at( s ,  1) ) == 'x' ) {
 base  =  16 ;
 start_index  +=  2 ;
 }
  else { 
 base  =  8 ;
 start_index ++ ;
 }
 ;
 }
 ;
 }
  else { 
return  ((u64)( 0 ) ) ;
 }
 ;
 if ( bit_size == 0 ) {
 bit_size  =  ((int)( strconv__int_size ) ) ;
 }
  else  if ( bit_size < 0  ||  bit_size > 64 ) {
return  ((u64)( 0 ) ) ;
 }
 ;
u64 cutoff= ((u64)( strconv__max_u64 / ((u64)( base ) ) ) ) + ((u64)( 1 ) ) ;
u64 max_val= (( bit_size == 64 ) ? ( strconv__max_u64 ) : ( ((u64)( ((u64)( 1 ) )  <<  ((u64)( bit_size ) ) ) ) - ((u64)( 1 ) ) )) ;
bool underscores= 0 ;
u64 n= ((u64)( 0 ) ) ;
 int tmp9 =  start_index;
 ;
for (int tmp10 = tmp9; tmp10 <  s .len; tmp10++) {
 int i = tmp10;
 
byte c= string_at( s ,  i) ;
byte cl= strconv__byte_to_lower ( c ) ;
byte d= ((byte)( 0 ) ) ;
 if ( c == '_'  &&  base0 ) {
 underscores  =  1 ;
 continue
 ;
 }
  else  if ( '0' <= c  &&  c <= '9' ) {
 d  =  c - '0' ;
 }
  else  if ( 'a' <= cl  &&  cl <= 'z' ) {
 d  =  cl - 'a' + 10 ;
 }
  else { 
 if ( error_on_non_digit ) {
return  ((u64)( 0 ) ) ;
 }
  else { 
 break
 ;
 }
 ;
 }
 ;
 if ( d >= ((byte)( base ) ) ) {
 if ( error_on_high_digit ) {
return  ((u64)( 0 ) ) ;
 }
  else { 
 break
 ;
 }
 ;
 }
 ;
 if ( n >= cutoff ) {
return  max_val ;
 }
 ;
 n  *=  ((u64)( base ) ) ;
u64 n1= n + ((u64)( d ) ) ;
 if ( n1 < n  ||  n1 > max_val ) {
return  max_val ;
 }
 ;
 n  =  n1 ;
 }
 ;
 if ( underscores  &&  ! strconv__underscore_ok ( s ) ) {
return  ((u64)( 0 ) ) ;
 }
 ;
return  n ;
 }
 u64 strconv__parse_uint(string s, int _base, int _bit_size) {
return  strconv__common_parse_uint ( s , _base , _bit_size , 1 , 1 ) ;
 }
 i64 strconv__common_parse_int(string _s, int base, int _bit_size, bool error_on_non_digit, bool error_on_high_digit) {
string s= _s ;
int bit_size= _bit_size ;
 if ( s .len < 1 ) {
return  ((i64)( 0 ) ) ;
 }
 ;
bool neg= 0 ;
 if ( string_at( s ,  0) == '+' ) {
 s  =  string_substr2( s ,  1 , -1, true) ;
 }
  else  if ( string_at( s ,  0) == '-' ) {
 neg  =  1 ;
 s  =  string_substr2( s ,  1 , -1, true) ;
 }
 ;
u64 un= strconv__common_parse_uint ( s , base , bit_size , error_on_non_digit , error_on_high_digit ) ;
 if ( un == 0 ) {
return  ((i64)( 0 ) ) ;
 }
 ;
 if ( bit_size == 0 ) {
 bit_size  =  ((int)( strconv__int_size ) ) ;
 }
 ;
u64 cutoff= ((u64)( ((u64)( 1 ) )  <<  ((u64)( bit_size - 1 ) ) ) ) ;
 if ( ! neg  &&  un >= cutoff ) {
return  ((i64)( cutoff - ((u64)( 1 ) ) ) ) ;
 }
 ;
 if ( neg  &&  un > cutoff ) {
return  - ((i64)( cutoff ) ) ;
 }
 ;
return  (( neg ) ? ( - ((i64)( un ) ) ) : ( ((i64)( un ) ) )) ;
 }
 i64 strconv__parse_int(string _s, int base, int _bit_size) {
return  strconv__common_parse_int ( _s , base , _bit_size , 1 , 1 ) ;
 }
 int strconv__atoi(string s) {
 if ( ( strconv__int_size == 32  &&  ( 0 < s .len  &&  s .len < 10 ) )  ||  ( strconv__int_size == 64  &&  ( 0 < s .len  &&  s .len < 19 ) ) ) {
int start_idx= 0 ;
 if ( string_at( s ,  0) == '-'  ||  string_at( s ,  0) == '+' ) {
 start_idx ++ ;
 if ( s .len - start_idx < 1 ) {
return  0 ;
 }
 ;
 }
 ;
int n= 0 ;
 int tmp25 =  start_idx;
 ;
for (int tmp26 = tmp25; tmp26 <  s .len; tmp26++) {
 int i = tmp26;
 
byte ch= string_at( s ,  i) - '0' ;
 if ( ch > 9 ) {
return  0 ;
 }
 ;
 n  =  n * 10 + ((int)( ch ) ) ;
 }
 ;
return  (( string_at( s ,  0) == '-' ) ? ( - n ) : ( n )) ;
 }
 ;
i64 int64= strconv__parse_int ( s , 10 , 0 ) ;
return  ((int)( int64 ) ) ;
 }
 bool strconv__underscore_ok(string s) {
byte saw= '^' ;
int i= 0 ;
 if ( s .len >= 1  &&  ( string_at( s ,  0) == '-'  ||  string_at( s ,  0) == '+' ) ) {
 i ++ ;
 }
 ;
bool hex= 0 ;
 if ( s .len - i >= 2  &&  string_at( s ,  i) == '0'  &&  ( strconv__byte_to_lower ( string_at( s ,  i + 1) ) == 'b'  ||  strconv__byte_to_lower ( string_at( s ,  i + 1) ) == 'o'  ||  strconv__byte_to_lower ( string_at( s ,  i + 1) ) == 'x' ) ) {
 saw  =  '0' ;
 hex  =  strconv__byte_to_lower ( string_at( s ,  i + 1) ) == 'x' ;
 i  +=  2 ;
 }
 ;
 for (
  ;  i < s .len  ;  i ++ ) { 
 
 if ( ( '0' <= string_at( s ,  i)  &&  string_at( s ,  i) <= '9' )  ||  ( hex  &&  'a' <= strconv__byte_to_lower ( string_at( s ,  i) )  &&  strconv__byte_to_lower ( string_at( s ,  i) ) <= 'f' ) ) {
 saw  =  '0' ;
 continue
 ;
 }
 ;
 if ( string_at( s ,  i) == '_' ) {
 if ( saw != '0' ) {
return  0 ;
 }
 ;
 saw  =  '_' ;
 continue
 ;
 }
 ;
 if ( saw == '_' ) {
return  0 ;
 }
 ;
 saw  =  '!' ;
 }
 ;
return  saw != '_' ;
 }
 rand__Pcg32 rand__new_pcg32(u64 initstate, u64 initseq) {
rand__Pcg32 rng= (rand__Pcg32) { .state =  0 , .inc =  0 } ;
 rng .state  =  ((u64)( 0 ) ) ;
 rng .inc  =  ((u64)( ((u64)( initseq  <<  ((u64)( 1 ) ) ) ) | ((u64)( 1 ) ) ) ) ;
 rand__Pcg32_next (& /* ? */ rng ) ;
 rng .state  +=  initstate ;
 rand__Pcg32_next (& /* ? */ rng ) ;
return  rng ;
 }
 static inline u32 rand__Pcg32_next(rand__Pcg32* rng) {
u64 oldstate= rng ->state ;
 rng ->state  =  oldstate * ((u64)( 6364136223846793005 ) ) + rng ->inc ;
u32 xorshifted= ((u32)( ((u64)( ((u64)( oldstate  >>  ((u64)( 18 ) ) ) ) ^ oldstate ) )  >>  ((u64)( 27 ) ) ) ) ;
u32 rot= ((u32)( oldstate  >>  ((u64)( 59 ) ) ) ) ;
return  ((u32)( ( xorshifted  >>  rot ) | ( xorshifted  <<  ( ( - rot ) & ((u32)( 31 ) ) ) ) ) ) ;
 }
 static inline u32 rand__Pcg32_bounded_next(rand__Pcg32* rng, u32 bound) {
u32 threshold= ((u32)( - bound % bound ) ) ;
 while (1) { 
u32 r= rand__Pcg32_next ( rng ) ;
 if ( r >= threshold ) {
return  ((u32)( r % bound ) ) ;
 }
 ;
 }
 ;
return  ((u32)( 0 ) ) ;
 }
 void rand__seed(int s) {
 srand ( s ) ;
 }
 int rand__next(int max) {
return  rand ( ) % max ;
 }
 int rand__rand_r(int* seed) {
 {
int* rs= seed ;
int ns= ( * rs * 1103515245 + 12345 ) ;
 * rs  =  ns ; 
return  ns & 0x7fffffff ;
 }
 ;
 }
 rand__Splitmix64 rand__new_splitmix64(u64 seed) {
return  (rand__Splitmix64) { seed } ;
 }
 static inline u64 rand__Splitmix64_next(rand__Splitmix64* rng) {
 rng ->state  +=  ((u64)( 0x9e3779b97f4a7c15 ) ) ;
u64 z= rng ->state ;
 z  =  ( z ^ ((u64)( ( z  >>  ((u64)( 30 ) ) ) ) ) ) * ((u64)( 0xbf58476d1ce4e5b9 ) ) ;
 z  =  ( z ^ ((u64)( ( z  >>  ((u64)( 27 ) ) ) ) ) ) * ((u64)( 0x94d049bb133111eb ) ) ;
return  z ^ ((u64)( z  >>  ((u64)( 31 ) ) ) ) ;
 }
 static inline u64 rand__Splitmix64_bounded_next(rand__Splitmix64* rng, u64 bound) {
u64 threshold= ((u64)( - bound % bound ) ) ;
 while (1) { 
u64 r= rand__Splitmix64_next ( rng ) ;
 if ( r >= threshold ) {
return  ((u64)( r % bound ) ) ;
 }
 ;
 }
 ;
return  ((u64)( 0 ) ) ;
 }
 f64 math__inf(int sign) {
u64 v= (( sign >= 0 ) ? ( math__uvinf ) : ( math__uvneginf )) ;
return  math__f64_from_bits ( v ) ;
 }
 f64 math__nan() {
return  math__f64_from_bits ( math__uvnan ) ;
 }
 bool math__is_nan(f64 f) {
return f64_ne( f , f ) ;
 }
 bool math__is_inf(f64 f, int sign) {
return  ( sign >= 0  && f64_gt( f , math__max_f64 ) )  ||  ( sign <= 0  && f64_lt( f , - math__max_f64 ) ) ;
 }
 f64 math__abs(f64 a) {
 if (f64_lt( a , 0 ) ) {
return  - a ;
 }
 ;
return  a ;
 }
 f64 math__acos(f64 a) {
return  acos ( a ) ;
 }
 f64 math__asin(f64 a) {
return  asin ( a ) ;
 }
 f64 math__atan(f64 a) {
return  atan ( a ) ;
 }
 f64 math__atan2(f64 a, f64 b) {
return  atan2 ( a ,  b ) ;
 }
 f64 math__cbrt(f64 a) {
return  cbrt ( a ) ;
 }
 f64 math__ceil(f64 a) {
return  ceil ( a ) ;
 }
 f64 math__cos(f64 a) {
return  cos ( a ) ;
 }
 f64 math__cosh(f64 a) {
return  cosh ( a ) ;
 }
 f64 math__degrees(f64 radians) {
return  radians * ( 180.0 / math__pi ) ;
 }
 f64 math__exp(f64 a) {
return  exp ( a ) ;
 }
 array_int math__digits(int _n, int base) {
 if ( base < 2 ) {
 v_panic ( _STR("digits: Cannot find digits of n with base %d", base) ) ;
 }
 ;
int n= _n ;
int sign= 1 ;
 if ( n < 0 ) {
 sign  =  - 1 ;
 n  =  - n ;
 }
 ;
array_int res=new_array_from_c_array(0, 0, sizeof(int), EMPTY_ARRAY_OF_ELEMS( int, 0 ) {   TCCSKIP(0) }) ;
 while ( n != 0 ) {
 
_PUSH(& res , ( /*typ = array_int   tmp_typ=int*/ ( n % base ) * sign ), tmp1, int) ;
 n  /=  base ;
 }
 ;
return  res ;
 }
 f64 math__erf(f64 a) {
return  erf ( a ) ;
 }
 f64 math__erfc(f64 a) {
return  erfc ( a ) ;
 }
 f64 math__exp2(f64 a) {
return  exp2 ( a ) ;
 }
 f64 math__floor(f64 a) {
return  floor ( a ) ;
 }
 f64 math__fmod(f64 a, f64 b) {
return  fmod ( a ,  b ) ;
 }
 f64 math__gamma(f64 a) {
return  tgamma ( a ) ;
 }
 i64 math__gcd(i64 a_, i64 b_) {
i64 a= a_ ;
i64 b= b_ ;
 if ( a < 0 ) {
 a  =  - a ;
 }
 ;
 if ( b < 0 ) {
 b  =  - b ;
 }
 ;
 while ( b != 0 ) {
 
 a  %=  b ;
 if ( a == 0 ) {
return  b ;
 }
 ;
 b  %=  a ;
 }
 ;
return  a ;
 }
 f64 math__hypot(f64 a, f64 b) {
return  hypot ( a ,  b ) ;
 }
 i64 math__lcm(i64 a, i64 b) {
 if ( a == 0 ) {
return  a ;
 }
 ;
i64 res= a * ( b / math__gcd ( b , a ) ) ;
 if ( res < 0 ) {
return  - res ;
 }
 ;
return  res ;
 }
 f64 math__log(f64 a) {
return  log ( a ) ;
 }
 f64 math__log2(f64 a) {
return  log2 ( a ) ;
 }
 f64 math__log10(f64 a) {
return  log10 ( a ) ;
 }
 f64 math__log_gamma(f64 a) {
return  lgamma ( a ) ;
 }
 f64 math__log_n(f64 a, f64 b) {
return  log ( a ) / log ( b ) ;
 }
 f64 math__max(f64 a, f64 b) {
 if (f64_gt( a , b ) ) {
return  a ;
 }
 ;
return  b ;
 }
 f64 math__min(f64 a, f64 b) {
 if (f64_lt( a , b ) ) {
return  a ;
 }
 ;
return  b ;
 }
 f64 math__pow(f64 a, f64 b) {
return  pow ( a ,  b ) ;
 }
 f64 math__radians(f64 degrees) {
return  degrees * ( math__pi / 180.0 ) ;
 }
 f64 math__round(f64 f) {
return  round ( f ) ;
 }
 f64 math__sin(f64 a) {
return  sin ( a ) ;
 }
 f64 math__sinh(f64 a) {
return  sinh ( a ) ;
 }
 f64 math__sqrt(f64 a) {
return  sqrt ( a ) ;
 }
 f64 math__tan(f64 a) {
return  tan ( a ) ;
 }
 f64 math__tanh(f64 a) {
return  tanh ( a ) ;
 }
 f64 math__trunc(f64 a) {
return  trunc ( a ) ;
 }
 u32 math__f32_bits(f32 f) {
u32* p= ((u32*)( & f ) ) ;
return  * p ;
 }
 f32 math__f32_from_bits(u32 b) {
f32* p= ((f32*)( & b ) ) ;
return  * p ;
 }
 u64 math__f64_bits(f64 f) {
u64* p= ((u64*)( & f ) ) ;
return  * p ;
 }
 f64 math__f64_from_bits(u64 b) {
f64* p= ((f64*)( & b ) ) ;
return  * p ;
 }
 bool os__File_is_opened(os__File f) {
return  f .opened ;
 }
 array_byte os__File_read_bytes(os__File* f, int size) {
return  os__File_read_bytes_at ( f ,  size , 0 ) ;
 }
 array_byte os__File_read_bytes_at(os__File* f, int size, int pos) {
array_byte arr= array_repeat (new_array_from_c_array(1, 1, sizeof(byte), EMPTY_ARRAY_OF_ELEMS( byte, 1 ) {  '0'  }) ,  size ) ;
 fseek ( f ->cfile ,  pos ,  SEEK_SET ) ;
int nreadbytes= fread ( arr .data ,  1 ,  size ,  f ->cfile ) ;
 fseek ( f ->cfile ,  0 ,  SEEK_SET ) ;
return   array_slice2(  arr , 0 , nreadbytes , false)  ;
 }
 Option_array_byte os__read_bytes(string path) {
FILE* fp= os__vfopen ( path , tos3("rb") ) ;
 if ( isnil ( fp ) ) {
return  v_error ( _STR("failed to open file \"%.*s\"", path.len, path.str) ) ;
 }
 ;
 fseek ( fp ,  0 ,  SEEK_END ) ;
int fsize= ftell ( fp ) ;
 rewind ( fp ) ;
printf( "fsize=%d\n", fsize ) ;
array_byte res= array_repeat (new_array_from_c_array(1, 1, sizeof(byte), EMPTY_ARRAY_OF_ELEMS( byte, 1 ) {  '0'  }) ,  fsize ) ;
int nreadbytes= fread ( res .data ,  fsize ,  1 ,  fp ) ;
 fclose ( fp ) ;
array_byte tmp5 = OPTION_CAST(array_byte)(  array_slice2(  res , 0 , nreadbytes , false) ); 
 return opt_ok(&tmp5, sizeof(array_byte)) ;
 }
 Option_string os__read_file(string path) {
string mode= tos3("rb") ;
FILE* fp= os__vfopen ( path , mode ) ;
 if ( isnil ( fp ) ) {
return  v_error ( _STR("failed to open file \"%.*s\"", path.len, path.str) ) ;
 }
 ;
 fseek ( fp ,  0 ,  SEEK_END ) ;
int fsize= ftell ( fp ) ;
 rewind ( fp ) ;
byte* str= v_malloc ( fsize + 1 ) ;
 fread ((char*) str ,  fsize ,  1 ,  fp ) ;
 fclose ( fp ) ;
 str [/*ptr!*/ fsize ]/*rbyte 1*/  =  0 ;
string tmp6 = OPTION_CAST(string)( (tos((byte *) str ,  fsize ) )); 
 return opt_ok(&tmp6, sizeof(string)) ;
 }
 int os__file_size(string path) { 
 struct /*c struct init*/ 

stat s ;
 #ifdef _WIN32
 #else
 stat ( ((charptr)( path .str ) ) ,  & s ) ;
 #endif
 ;
return  s .st_size ;
 }
 void os__mv(string old, string new) {
 #ifdef _WIN32
 #else
 rename ( ((charptr)( old .str ) ) ,  ((charptr)( new .str ) ) ) ;
 #endif
 ;
 }
 Option_bool os__cp(string old, string new) {
 #ifdef _WIN32
 #else
 os__system ( _STR("cp %.*s %.*s", old.len, old.str, new.len, new.str) ) ;
bool tmp7 = OPTION_CAST(bool)( 1); 
 return opt_ok(&tmp7, sizeof(bool)) ;
 #endif
 ;
 }
 Option_bool os__cp_r(string osource_path, string odest_path, bool overwrite) {
string source_path= os__realpath ( osource_path ) ;
string dest_path= os__realpath ( odest_path ) ;
 if ( ! os__file_exists ( source_path ) ) {
return  v_error ( tos3("Source path doesn\'t exist") ) ;
 }
 ;
 if ( ! os__is_dir ( source_path ) ) {
string adjasted_path= (( os__is_dir ( dest_path ) ) ? ( filepath__join ( dest_path , &(varg_string){.len=1,.args={ os__filename ( source_path )}} ) ) : ( dest_path )) ;
 if ( os__file_exists ( adjasted_path ) ) {
 if ( overwrite ) {
 os__rm ( adjasted_path ) ;
 }
  else { 
return  v_error ( tos3("Destination file path already exist") ) ;
 }
 ;
 }
 ;
Option_bool tmp8 =  os__cp ( source_path , adjasted_path ) ;
 if (!tmp8 .ok) {
 string err = tmp8 . error;
 int errcode = tmp8 . ecode;
return  v_error ( err ) ;
 }
 ;
bool tmp9 = OPTION_CAST(bool)( 1); 
 return opt_ok(&tmp9, sizeof(bool)) ;
 }
 ;
 if ( ! os__is_dir ( dest_path ) ) {
return  v_error ( tos3("Destination path is not a valid directory") ) ;
 }
 ;
Option_array_string tmp10 =  os__ls ( source_path ) ;
 array_string files;
 if (!tmp10 .ok) {
 string err = tmp10 . error;
 int errcode = tmp10 . ecode;
return  v_error ( err ) ;
 }
 files = *(array_string*)tmp10.data;
 ;
 array_string tmp11 =  files;
 for (int tmp12 = 0; tmp12 < tmp11.len; tmp12++) {
 string file = ((string *) tmp11 . data)[tmp12];
 
string sp= filepath__join ( source_path , &(varg_string){.len=1,.args={ file}} ) ;
string dp= filepath__join ( dest_path , &(varg_string){.len=1,.args={ file}} ) ;
 if ( os__is_dir ( sp ) ) {
Option_bool tmp13 =  os__mkdir ( dp ) ;
 if (!tmp13 .ok) {
 string err = tmp13 . error;
 int errcode = tmp13 . ecode;
 v_panic ( err ) ;
 }
 ;
 }
 ;
Option_bool tmp14 =  os__cp_r ( sp , dp , overwrite ) ;
 if (!tmp14 .ok) {
 string err = tmp14 . error;
 int errcode = tmp14 . ecode;
 os__rmdir ( dp ) ;
 v_panic ( err ) ;
 }
 ;
 }
 ;
bool tmp15 = OPTION_CAST(bool)( 1); 
 return opt_ok(&tmp15, sizeof(bool)) ;
 }
 Option_bool os__mv_by_cp(string source, string target) {
Option_bool tmp16 =  os__cp ( source , target ) ;
 if (!tmp16 .ok) {
 string err = tmp16 . error;
 int errcode = tmp16 . ecode;
return  v_error ( err ) ;
 }
 ;
 os__rm ( source ) ;
bool tmp17 = OPTION_CAST(bool)( 1); 
 return opt_ok(&tmp17, sizeof(bool)) ;
 }
 FILE* os__vfopen(string path, string mode) {
 #ifdef _WIN32
 #else
return  fopen ( ((charptr)( path .str ) ) ,  ((charptr)( mode .str ) ) ) ;
 #endif
 ;
 }
 Option_array_string os__read_lines(string path) {
array_string res=new_array_from_c_array(0, 0, sizeof(string), EMPTY_ARRAY_OF_ELEMS( string, 0 ) {   TCCSKIP(0) }) ;
int buf_len= 1024 ;
byte* buf= v_malloc ( buf_len ) ;
string mode= tos3("rb") ;
FILE* fp= os__vfopen ( path , mode ) ;
 if ( isnil ( fp ) ) {
return  v_error ( _STR("read_lines() failed to open file \"%.*s\"", path.len, path.str) ) ;
 }
 ;
int buf_index= 0 ;
 while ( fgets ((char*)(byte*) buf + buf_index ,  buf_len - buf_index ,  fp ) != 0 ) {
 
int len= vstrlen ( buf ) ;
 if ( len == buf_len - 1  &&  buf [/*ptr!*/ len - 1 ]/*rbyte 1*/ != 10 ) {
 buf_len  *=  2 ;
 buf  =  realloc ((char*) buf ,  buf_len ) ;
 if ( isnil ( buf ) ) {
return  v_error ( tos3("could not reallocate the read buffer") ) ;
 }
 ;
 buf_index  =  len ;
 continue
 ;
 }
 ;
 if ( buf [/*ptr!*/ len - 1 ]/*rbyte 1*/ == 10  ||  buf [/*ptr!*/ len - 1 ]/*rbyte 1*/ == 13 ) {
 buf [/*ptr!*/ len - 1 ]/*rbyte 1*/  =  '\0' ;
 }
 ;
 if ( len > 1  &&  buf [/*ptr!*/ len - 2 ]/*rbyte 1*/ == 13 ) {
 buf [/*ptr!*/ len - 2 ]/*rbyte 1*/  =  '\0' ;
 }
 ;
_PUSH(& res , ( /*typ = array_string   tmp_typ=string*/ tos_clone ( buf ) ), tmp18, string) ;
 buf_index  =  0 ;
 }
 ;
 fclose ( fp ) ;
array_string tmp19 = OPTION_CAST(array_string)( res); 
 return opt_ok(&tmp19, sizeof(array_string)) ;
 }
 Option_array_ustring os__read_ulines(string path) {
Option_array_string tmp20 =  os__read_lines ( path ) ;
 array_string lines;
 if (!tmp20 .ok) {
 string err = tmp20 . error;
 int errcode = tmp20 . ecode;
string tmp21 = OPTION_CAST(string)( err); 
 return opt_ok(&tmp21, sizeof(string)) ;
 }
 lines = *(array_string*)tmp20.data;
 ;
array_ustring ulines=new_array_from_c_array(0, 0, sizeof(ustring), EMPTY_ARRAY_OF_ELEMS( ustring, 0 ) {   TCCSKIP(0) }) ;
 array_string tmp22 =  lines;
 for (int tmp23 = 0; tmp23 < tmp22.len; tmp23++) {
 string myline = ((string *) tmp22 . data)[tmp23];
 
_PUSH(& ulines , ( /*typ = array_ustring   tmp_typ=ustring*/ string_ustring ( myline ) ), tmp24, ustring) ;
 }
 ;
array_ustring tmp25 = OPTION_CAST(array_ustring)( ulines); 
 return opt_ok(&tmp25, sizeof(array_ustring)) ;
 }
 Option_os__File os__open(string path) {
os__File file= (os__File) { .cfile =  0 , .opened =  0 } ;
 #ifdef _WIN32
 #else
byte* cpath= path .str ;
 file  =  (os__File) { .cfile =  fopen ( ((charptr)( cpath ) ) ,  "rb" ) , .opened =  0 } ;
 #endif
 ;
 if ( isnil ( file .cfile ) ) {
return  v_error ( _STR("failed to open file \"%.*s\"", path.len, path.str) ) ;
 }
 ;
 file .opened  =  1 ;
os__File tmp26 = OPTION_CAST(os__File)( file); 
 return opt_ok(&tmp26, sizeof(os__File)) ;
 }
 Option_os__File os__create(string path) {
os__File file= (os__File) { .cfile =  0 , .opened =  0 } ;
 #ifdef _WIN32
 #else
byte* cpath= path .str ;
 file  =  (os__File) { .cfile =  fopen ( ((charptr)( cpath ) ) ,  "wb" ) , .opened =  0 } ;
 #endif
 ;
 if ( isnil ( file .cfile ) ) {
return  v_error ( _STR("failed to create file \"%.*s\"", path.len, path.str) ) ;
 }
 ;
 file .opened  =  1 ;
os__File tmp27 = OPTION_CAST(os__File)( file); 
 return opt_ok(&tmp27, sizeof(os__File)) ;
 }
 Option_os__File os__open_append(string path) {
os__File file= (os__File) { .cfile =  0 , .opened =  0 } ;
 #ifdef _WIN32
 #else
byte* cpath= path .str ;
 file  =  (os__File) { .cfile =  fopen ( ((charptr)( cpath ) ) ,  "ab" ) , .opened =  0 } ;
 #endif
 ;
 if ( isnil ( file .cfile ) ) {
return  v_error ( _STR("failed to create(append) file \"%.*s\"", path.len, path.str) ) ;
 }
 ;
 file .opened  =  1 ;
os__File tmp28 = OPTION_CAST(os__File)( file); 
 return opt_ok(&tmp28, sizeof(os__File)) ;
 }
 void os__File_write(os__File* f, string s) {
 fputs ((char*) s .str ,  f ->cfile ) ;
 }
 void os__File_write_bytes(os__File* f, void* data, int size) {
 fwrite ( data ,  1 ,  size ,  f ->cfile ) ;
 }
 void os__File_write_bytes_at(os__File* f, void* data, int size, int pos) {
 fseek ( f ->cfile ,  pos ,  SEEK_SET ) ;
 fwrite ( data ,  1 ,  size ,  f ->cfile ) ;
 fseek ( f ->cfile ,  0 ,  SEEK_END ) ;
 }
 void os__File_writeln(os__File* f, string s) {
 if ( ! f ->opened ) {
 
 return ;
 }
 ;
 fputs ((char*) s .str ,  f ->cfile ) ;
 fputs ( "\n" ,  f ->cfile ) ;
 }
 void os__File_flush(os__File* f) {
 if ( ! f ->opened ) {
 
 return ;
 }
 ;
 fflush ( f ->cfile ) ;
 }
 void os__File_close(os__File* f) {
 if ( ! f ->opened ) {
 
 return ;
 }
 ;
 f ->opened  =  0 ;
 fflush ( f ->cfile ) ;
 fclose ( f ->cfile ) ;
 }
 void* os__vpopen(string path) {
 #ifdef _WIN32
 #else
byte* cpath= path .str ;
return  popen ((char*) cpath ,  "r" ) ;
 #endif
 ;
 }
 _V_MulRet_int_V_bool os__posix_wait4_to_exit_status(int waitret) {
 #ifdef _WIN32
 #else
int ret= 0 ;
bool is_signaled= 1 ;
 if ( WIFEXITED ( waitret ) ) {
 ret  =  WEXITSTATUS ( waitret ) ;
 is_signaled  =  0 ;
 }
  else  if ( WIFSIGNALED ( waitret ) ) {
 ret  =  WTERMSIG ( waitret ) ;
 is_signaled  =  1 ;
 }
 ;
return (_V_MulRet_int_V_bool){.var_0=ret,.var_1=is_signaled} ;
 #endif
 ;
 }
 int os__vpclose(void* f) {
 #ifdef _WIN32
 #else
_V_MulRet_int_V_bool _V_mret_1993_ret__= os__posix_wait4_to_exit_status ( ((int)( pclose ( f ) ) ) ) ;
 int ret = _V_mret_1993_ret__.var_0 ;
return  ret ;
 #endif
 ;
 }
 int os__system(string cmd) {
int ret= ((int)( 0 ) ) ;
 #ifdef _WIN32
 #else
 ret  =  system ((char*) cmd .str ) ;
 #endif
 ;
 if ( ret == - 1 ) {
 os__print_c_errno ( ) ;
 }
 ;
 #ifndef _WIN32
_V_MulRet_int_V_bool _V_mret_2118_pret_is_signaled= os__posix_wait4_to_exit_status ( ret ) ;
 int pret = _V_mret_2118_pret_is_signaled.var_0 ;
 bool is_signaled = _V_mret_2118_pret_is_signaled.var_1 ;
 if ( is_signaled ) {
 println (string_add(string_add( _STR("Terminated by signal %2d (", ret) , os__sigint_to_signal_name ( pret ) ) , tos3(")") ) ) ;
 }
 ;
 ret  =  pret ;
 #endif
 ;
return  ret ;
 }
 string os__sigint_to_signal_name(int si) { 
 int tmp29 =  si; 

 if ( tmp29 ==  1 ) { 
return  tos3("SIGHUP") ;
 }
 else  if ( tmp29 ==  2 ) { 
return  tos3("SIGINT") ;
 }
 else  if ( tmp29 ==  3 ) { 
return  tos3("SIGQUIT") ;
 }
 else  if ( tmp29 ==  4 ) { 
return  tos3("SIGILL") ;
 }
 else  if ( tmp29 ==  6 ) { 
return  tos3("SIGABRT") ;
 }
 else  if ( tmp29 ==  8 ) { 
return  tos3("SIGFPE") ;
 }
 else  if ( tmp29 ==  9 ) { 
return  tos3("SIGKILL") ;
 }
 else  if ( tmp29 ==  11 ) { 
return  tos3("SIGSEGV") ;
 }
 else  if ( tmp29 ==  13 ) { 
return  tos3("SIGPIPE") ;
 }
 else  if ( tmp29 ==  14 ) { 
return  tos3("SIGALRM") ;
 }
 else  if ( tmp29 ==  15 ) { 
return  tos3("SIGTERM") ;
 }
 ;
 #ifdef __linux__ 
 int tmp30 =  si; 

 if (( tmp30 ==  30 ) || ( tmp30 ==  10 ) || ( tmp30 ==  16 )  ) { 
return  tos3("SIGUSR1") ;
 }
 else  if (( tmp30 ==  31 ) || ( tmp30 ==  12 ) || ( tmp30 ==  17 )  ) { 
return  tos3("SIGUSR2") ;
 }
 else  if (( tmp30 ==  20 ) || ( tmp30 ==  17 ) || ( tmp30 ==  18 )  ) { 
return  tos3("SIGCHLD") ;
 }
 else  if (( tmp30 ==  19 ) || ( tmp30 ==  18 ) || ( tmp30 ==  25 )  ) { 
return  tos3("SIGCONT") ;
 }
 else  if (( tmp30 ==  17 ) || ( tmp30 ==  19 ) || ( tmp30 ==  23 )  ) { 
return  tos3("SIGSTOP") ;
 }
 else  if (( tmp30 ==  18 ) || ( tmp30 ==  20 ) || ( tmp30 ==  24 )  ) { 
return  tos3("SIGTSTP") ;
 }
 else  if (( tmp30 ==  21 ) || ( tmp30 ==  21 ) || ( tmp30 ==  26 )  ) { 
return  tos3("SIGTTIN") ;
 }
 else  if (( tmp30 ==  22 ) || ( tmp30 ==  22 ) || ( tmp30 ==  27 )  ) { 
return  tos3("SIGTTOU") ;
 }
 else  if ( tmp30 ==  5 ) { 
return  tos3("SIGTRAP") ;
 }
 else  if ( tmp30 ==  7 ) { 
return  tos3("SIGBUS") ;
 }
 ;
 #endif
 ;
return  tos3("unknown") ;
 }
 string os__getenv(string key) {
 #ifdef _WIN32
 #else
char* s= getenv ((char*) key .str ) ;
 if ( s == 0 ) {
return  tos3("") ;
 }
 ;
return  cstring_to_vstring ( ((byteptr)( s ) ) ) ;
 #endif
 ;
 }
 int os__setenv(string name, string value, bool overwrite) {
 #ifdef _WIN32
 #else
return  setenv ((char*) name .str , (char*) value .str ,  overwrite ) ;
 #endif
 ;
 }
 int os__unsetenv(string name) {
 #ifdef _WIN32
 #else
return  unsetenv ((char*) name .str ) ;
 #endif
 ;
 }
 bool os__file_exists(string _path) {
 #ifdef _WIN32
 #else
return  access ((char*) _path .str ,  0 ) != - 1 ;
 #endif
 ;
 }
 void os__rm(string path) {
 #ifdef _WIN32
 #else
 remove ((char*) path .str ) ;
 #endif
 ;
 }
 void os__rmdir(string path) {
 #ifndef _WIN32
 rmdir ((char*) path .str ) ;
 #else
 RemoveDirectory ( string_to_wide ( path ) ) ;
 #endif
 ;
 }
 void os__print_c_errno() {
 }
 string os__ext(string path) {
int pos= string_last_index ( path ,  tos3(".") ) ;
 if ( pos == - 1 ) {
return  tos3("") ;
 }
 ;
return  string_substr2( path ,  pos , -1, true) ;
 }
 string os__dir(string path) {
 if (string_eq( path , tos3(".") ) ) {
return  os__getwd ( ) ;
 }
 ;
int pos= string_last_index ( path ,  os__path_separator ) ;
 if ( pos == - 1 ) {
return  tos3(".") ;
 }
 ;
return  string_substr2( path ,  0 , pos , false) ;
 }
 string os__path_sans_ext(string path) {
int pos= string_last_index ( path ,  tos3(".") ) ;
 if ( pos == - 1 ) {
return  path ;
 }
 ;
return  string_substr2( path ,  0 , pos , false) ;
 }
 string os__basedir(string path) {
int pos= string_last_index ( path ,  os__path_separator ) ;
 if ( pos == - 1 ) {
return  path ;
 }
 ;
return  string_substr2( path ,  0 , pos , false) ;
 }
 string os__filename(string path) {
return  string_all_after ( path ,  os__path_separator ) ;
 }
 string os__get_line() {
string str= os__get_raw_line ( ) ;
 #ifdef _WIN32
 #else
return  string_trim_right ( str ,  tos3("\n") ) ;
 #endif
 ;
 }
 string os__get_raw_line() {
 #ifdef _WIN32
 #else
size_t max= ((size_t)( 256 ) ) ;
charptr buf= ((charptr)( v_malloc ( ((int)( max ) ) ) ) ) ;
int nr_chars= getline ( & buf ,  & max ,  stdin ) ;
 if ( nr_chars == 0 ) {
return  tos3("") ;
 }
 ;
return  (tos((byte *) ((byteptr)( buf ) ) ,  nr_chars ) ) ;
 #endif
 ;
 }
 array_string os__get_lines() {
string line= tos3("") ;
array_string inputstr=new_array_from_c_array(0, 0, sizeof(string), EMPTY_ARRAY_OF_ELEMS( string, 0 ) {   TCCSKIP(0) }) ;
 while (1) { 
 line  =  os__get_line ( ) ;
 if ( ( line .len <= 0 ) ) {
 break
 ;
 }
 ;
 line  =  string_trim_space ( line ) ;
_PUSH(& inputstr , ( /*typ = array_string   tmp_typ=string*/ line ), tmp39, string) ;
 }
 ;
return  inputstr ;
 }
 string os__get_lines_joined() {
string line= tos3("") ;
string inputstr= tos3("") ;
 while (1) { 
 line  =  os__get_line ( ) ;
 if ( line .len <= 0 ) {
 break
 ;
 }
 ;
 line  =  string_trim_space ( line ) ;
 inputstr = string_add(inputstr,  line ) ;
 }
 ;
return  inputstr ;
 }
 string os__user_os() {
 #ifdef __linux__
return  tos3("linux") ;
 #endif
 ;
 #ifdef __APPLE__
 #endif
 ;
 #ifdef _WIN32
 #endif
 ;
 #ifdef __FreeBSD__
 #endif
 ;
 #ifdef __OpenBSD__
 #endif
 ;
 #ifdef __NetBSD__
 #endif
 ;
 #ifdef __DragonFly__
 #endif
 ;
 #ifdef __BIONIC__
 #endif
 ;
 #ifdef __sun
 #endif
 ;
return  tos3("unknown") ;
 }
 string os__home_dir() {
string home= os__getenv ( tos3("HOME") ) ;
 #ifdef _WIN32
 #endif
 ;
 home = string_add(home,  os__path_separator ) ;
return  home ;
 }
 void os__write_file(string path, string text) {
Option_os__File tmp40 =  os__create ( path ) ;
 os__File f;
 if (!tmp40 .ok) {
 string err = tmp40 . error;
 int errcode = tmp40 . ecode;
 
 return ;
 }
 f = *(os__File*)tmp40.data;
 ;
 os__File_write (& /* ? */ f ,  text ) ;
 os__File_close (& /* ? */ f ) ;
 }
 void os__clear() {
 #ifndef _WIN32
 printf ( "\x1b[2J" ) ;
 printf ( "\x1b[H" ) ;
 #endif
 ;
 }
 void os__on_segfault(void* f) {
 #ifdef _WIN32
 #endif
 ;
 #ifdef __APPLE__
 #endif
 ;
 }
 string os__executable() {
 #ifdef __linux__
byte* result= v_calloc ( os__MAX_PATH ) ;
int count= readlink ( "/proc/self/exe" , (char*) result ,  os__MAX_PATH ) ;
 if ( count < 0 ) {
 eprintln ( tos3("os.executable() failed at reading /proc/self/exe to get exe path") ) ;
return  ( *(string*) array_get(  os__args , 0) ) ;
 }
 ;
return  (tos2((byte *) result ) ) ;
 #endif
 ;
 #ifdef _WIN32
 #endif
 ;
 #ifdef __APPLE__
 #endif
 ;
 #ifdef __FreeBSD__
 #endif
 ;
 #ifdef __OpenBSD__
 #endif
 ;
 #ifdef __sun
 #endif
 ;
 #ifdef __NetBSD__
 #endif
 ;
 #ifdef __DragonFly__
 #endif
 ;
return  ( *(string*) array_get(  os__args , 0) ) ;
 }
 bool os__is_dir(string path) {
 #ifdef _WIN32
 #else 
 struct /*c struct init*/ 

stat statbuf ;
byte* cstr= path .str ;
 if ( stat ((char*) cstr ,  & statbuf ) != 0 ) {
return  0 ;
 }
 ;
return  ( ((int)( statbuf .st_mode ) ) & os__S_IFMT ) == os__S_IFDIR ;
 #endif
 ;
 }
 void os__chdir(string path) {
 #ifdef _WIN32
 #else
 chdir ((char*) path .str ) ;
 #endif
 ;
 }
 string os__getwd() {
 #ifdef _WIN32
 #else
byte* buf= v_calloc ( 512 ) ;
 if ( getcwd ((char*) buf ,  512 ) == 0 ) {
return  tos3("") ;
 }
 ;
return  (tos2((byte *) buf ) ) ;
 #endif
 ;
 }
 string os__realpath(string fpath) {
byte* fullpath= v_calloc ( os__MAX_PATH ) ;
charptr ret= ((charptr)( 0 ) ) ;
 #ifdef _WIN32
 #else
 ret  =  realpath ((char*) fpath .str , (char*) fullpath ) ;
 if ( ret == 0 ) {
return  fpath ;
 }
 ;
 #endif
 ;
return  (tos2((byte *) fullpath ) ) ;
 }
 array_string os__walk_ext(string path, string ext) {
 if ( ! os__is_dir ( path ) ) {
return new_array_from_c_array(0, 0, sizeof(string), EMPTY_ARRAY_OF_ELEMS( string, 0 ) {   TCCSKIP(0) }) ;
 }
 ;
Option_array_string tmp45 =  os__ls ( path ) ;
 array_string files;
 if (!tmp45 .ok) {
 string err = tmp45 . error;
 int errcode = tmp45 . ecode;
 v_panic ( err ) ;
 }
 files = *(array_string*)tmp45.data;
 ;
array_string res=new_array_from_c_array(0, 0, sizeof(string), EMPTY_ARRAY_OF_ELEMS( string, 0 ) {   TCCSKIP(0) }) ;
string separator= (( string_ends_with ( path ,  os__path_separator ) ) ? ( tos3("") ) : ( os__path_separator )) ;
 array_string tmp46 =  files;
 for (int i = 0; i < tmp46.len; i++) {
 string file = ((string *) tmp46 . data)[i];
 
 if ( string_starts_with ( file ,  tos3(".") ) ) {
 continue
 ;
 }
 ;
string p=string_add(string_add( path , separator ) , file ) ;
 if ( os__is_dir ( p ) ) {
_PUSH_MANY(& res , ( /*typ = array_string   tmp_typ=string*/ os__walk_ext ( p , ext ) ), tmp47, array_string) ;
 }
  else  if ( string_ends_with ( file ,  ext ) ) {
_PUSH(& res , ( /*typ = array_string   tmp_typ=string*/ p ), tmp48, string) ;
 }
 ;
 }
 ;
return  res ;
 }
 void os__walk(string path, void (*fnc)( string path /*FFF*/ )) {
 if ( ! os__is_dir ( path ) ) {
 
 return ;
 }
 ;
Option_array_string tmp49 =  os__ls ( path ) ;
 array_string files;
 if (!tmp49 .ok) {
 string err = tmp49 . error;
 int errcode = tmp49 . ecode;
 v_panic ( err ) ;
 }
 files = *(array_string*)tmp49.data;
 ;
 array_string tmp50 =  files;
 for (int tmp51 = 0; tmp51 < tmp50.len; tmp51++) {
 string file = ((string *) tmp50 . data)[tmp51];
 
string p=string_add(string_add( path , os__path_separator ) , file ) ;
 if ( os__is_dir ( p ) ) {
 os__walk ( p , fnc ) ;
 }
  else  if ( os__file_exists ( p ) ) {
 fnc ( p ) ;
 }
 ;
 }
 ;
 
 return ;
 }
 void os__signal(int signum, void* handler) {
 signal ( signum ,  handler ) ;
 }
 int os__fork() {
int pid= - 1 ;
 #ifndef _WIN32
 pid  =  fork ( ) ;
 #endif
 ;
 #ifdef _WIN32
 #endif
 ;
return  pid ;
 }
 int os__wait() {
int pid= - 1 ;
 #ifndef _WIN32
 pid  =  wait ( 0 ) ;
 #endif
 ;
 #ifndef _WIN32
 v_panic ( tos3("os.wait not supported in windows") ) ;
 #endif
 ;
return  pid ;
 }
 int os__file_last_mod_unix(string path) { 
 struct /*c struct init*/ 

stat attr ;
 stat ((char*) path .str ,  & attr ) ;
return  attr .st_mtime ;
 }
 void os__log(string s) {
 println (string_add( tos3("os.log: ") , s ) ) ;
 }
 void os__flush_stdout() {
 fflush ( stdout ) ;
 }
 void os__print_backtrace() {
 }
 void os__mkdir_all(string path) {
string p= (( string_starts_with ( path ,  os__path_separator ) ) ? ( os__path_separator ) : ( tos3("") )) ;
 array_string tmp52 =  string_split ( path ,  os__path_separator );
 for (int tmp53 = 0; tmp53 < tmp52.len; tmp53++) {
 string subdir = ((string *) tmp52 . data)[tmp53];
 
 p = string_add(p, string_add( subdir , os__path_separator ) ) ;
 if ( ! os__dir_exists ( p ) ) {
Option_bool tmp54 =  os__mkdir ( p ) ;
 if (!tmp54 .ok) {
 string err = tmp54 . error;
 int errcode = tmp54 . ecode;
 v_panic ( err ) ;
 }
 ;
 }
 ;
 }
 ;
 }
 string os__join(string base, varg_string *dirs) {
 println ( tos3("use filepath.join") ) ;
return  filepath__join ( base ,  dirs ) ;
 }
 string os__tmpdir() {
string path= os__getenv ( tos3("TMPDIR") ) ;
 #ifdef __linux__
 if (string_eq( path , tos3("") ) ) {
 path  =  tos3("/tmp") ;
 }
 ;
 #endif
 ;
 #ifdef __APPLE__
 #endif
 ;
 #ifdef _WIN32
 #endif
 ;
return  path ;
 }
 void os__chmod(string path, int mode) {
 chmod ((char*) path .str ,  mode ) ;
 }
 array_string os__init_os_args(int argc, byteptr* argv) {
array_string args=new_array_from_c_array(0, 0, sizeof(string), EMPTY_ARRAY_OF_ELEMS( string, 0 ) {   TCCSKIP(0) }) ;
 int tmp1 =  0;
 ;
for (int tmp2 = tmp1; tmp2 <  argc; tmp2++) {
 int i = tmp2;
 
_PUSH(& args , ( /*typ = array_string   tmp_typ=string*/ (tos2((byte *) argv [/*ptr!*/ i ]/*rbyteptr 0*/ ) ) ), tmp3, string) ;
 }
 ;
return  args ;
 }
 string os__get_error_msg(int code) {
charptr ptr_text= strerror ( code ) ;
 if ( ptr_text == 0 ) {
return  tos3("") ;
 }
 ;
return  tos3 ( ptr_text ) ;
 }
 Option_array_string os__ls(string path) {
array_string res=new_array_from_c_array(0, 0, sizeof(string), EMPTY_ARRAY_OF_ELEMS( string, 0 ) {   TCCSKIP(0) }) ;
void* dir= opendir ((char*) path .str ) ;
 if ( isnil ( dir ) ) {
return  v_error ( _STR("ls() couldnt open dir \"%.*s\"", path.len, path.str) ) ;
 }
 ; 
 struct /*c struct init*/ 

dirent* ent= 0 ;
 while (1) { 
 ent  =  readdir ( dir ) ;
 if ( isnil ( ent ) ) {
 break
 ;
 }
 ;
string name= tos_clone ( ((byteptr)( ent ->d_name ) ) ) ;
 if (string_ne( name , tos3(".") )  && string_ne( name , tos3("..") )  && string_ne( name , tos3("") ) ) {
_PUSH(& res , ( /*typ = array_string   tmp_typ=string*/ name ), tmp4, string) ;
 }
 ;
 }
 ;
 closedir ( dir ) ;
array_string tmp5 = OPTION_CAST(array_string)( res); 
 return opt_ok(&tmp5, sizeof(array_string)) ;
 }
 bool os__dir_exists(string path) {
void* dir= opendir ((char*) path .str ) ;
bool res= ! isnil ( dir ) ;
 if ( res ) {
 closedir ( dir ) ;
 }
 ;
return  res ;
 }
 Option_bool os__mkdir(string path) {
 if (string_eq( path , tos3(".") ) ) {
bool tmp6 = OPTION_CAST(bool)( 1); 
 return opt_ok(&tmp6, sizeof(bool)) ;
 }
 ;
string apath= os__realpath ( path ) ;
int r= ((int)( mkdir ((char*) apath .str ,  511 ) ) ) ;
 if ( r == - 1 ) {
return  v_error ( os__get_error_msg ( errno ) ) ;
 }
 ;
bool tmp7 = OPTION_CAST(bool)( 1); 
 return opt_ok(&tmp7, sizeof(bool)) ;
 }
 Option_os__Result os__exec(string cmd) {
string pcmd= _STR("%.*s 2>&1", cmd.len, cmd.str) ;
void* f= os__vpopen ( pcmd ) ;
 if ( isnil ( f ) ) {
return  v_error ( _STR("exec(\"%.*s\") failed", cmd.len, cmd.str) ) ;
 }
 ;
byte buf  [1000 ] = { 0 } ;
string res= tos3("") ;
 while ( fgets ( ((charptr)( buf ) ) ,  1000 ,  f ) != 0 ) {
 
 res = string_add(res,  tos ( buf , vstrlen ( buf ) ) ) ;
 }
 ;
 res  =  string_trim_space ( res ) ;
int exit_code= os__vpclose ( f ) ;
os__Result tmp8 = OPTION_CAST(os__Result)( (os__Result) { .output =  res , .exit_code =  exit_code }); 
 return opt_ok(&tmp8, sizeof(os__Result)) ;
 }
 stbi__Image stbi__load(string path) {
string ext= string_all_after ( path ,  tos3(".") ) ;
stbi__Image res= (stbi__Image) { .ok =  1 , .ext =  ext , .data =  0 , .width =  0 , .height =  0 , .nr_channels =  0 , } ;
int flag= ((string_eq( ext , tos3("png") ) ) ? ( STBI_rgb_alpha ) : ( 0 )) ;
 res .data  =  stbi_load ((char*) path .str ,  & res .width ,  & res .height ,  & res .nr_channels ,  flag ) ;
 if ( isnil ( res .data ) ) {
 println ( tos3("stbi image failed to load") ) ;
 v_exit ( 1 ) ;
 }
 ;
return  res ;
 }
 void stbi__Image_free(stbi__Image img) {
 stbi_image_free ( img .data ) ;
 }
 void stbi__Image_tex_image_2d(stbi__Image img) {
int rgb_flag= GL_RGB ;
 if (string_eq( img .ext , tos3("png") ) ) {
 rgb_flag  =  GL_RGBA ;
 }
 ;
 glTexImage2D ( GL_TEXTURE_2D ,  0 ,  rgb_flag ,  img .width ,  img .height ,  0 ,  rgb_flag ,  GL_UNSIGNED_BYTE ,  img .data ) ;
 }
 void stbi__set_flip_vertically_on_load(bool val) {
 stbi_set_flip_vertically_on_load ( val ) ;
 }
 string filepath__ext(string path) {
int pos= string_last_index_byte ( path ,  '.' ) ;
 if ( pos != - 1 ) {
return  string_substr2( path ,  pos , -1, true) ;
 }
 ;
return  tos3("") ;
 }
 bool filepath__is_abs(string path) {
 #ifdef _WIN32
 #endif
 ;
return  string_at( path ,  0) == '/' ;
 }
 string filepath__join(string base, varg_string *dirs) {
array_string result=new_array_from_c_array(0, 0, sizeof(string), EMPTY_ARRAY_OF_ELEMS( string, 0 ) {   TCCSKIP(0) }) ;
_PUSH(& result , ( /*typ = array_string   tmp_typ=string*/ string_trim_right ( base ,  tos3("\\/") ) ), tmp5, string) ;
 for (int tmp7 = 0; tmp7 <  dirs->len; tmp7++) {
 string d = ((string *)  dirs->args)[tmp7];
 
_PUSH(& result , ( /*typ = array_string   tmp_typ=string*/ d ), tmp8, string) ;
 }
 ;
return  array_string_join ( result ,  os__path_separator ) ;
 }
 void glfw__init_glfw() {
 glfwInit ( ) ;
 glfwWindowHint ( GLFW_CONTEXT_VERSION_MAJOR ,  3 ) ;
 glfwWindowHint ( GLFW_CONTEXT_VERSION_MINOR ,  3 ) ;
 glfwWindowHint ( GLFW_OPENGL_FORWARD_COMPAT ,  GL_TRUE ) ;
 glfwWindowHint ( GLFW_OPENGL_PROFILE ,  GLFW_OPENGL_CORE_PROFILE ) ;
 }
 void glfw__Window_destroy(glfw__Window* w) {
 glfwDestroyWindow ( w ->data ) ;
 }
 void glfw__terminate() {
 glfwTerminate ( ) ;
 }
 void glfw__mouse_move(void* w, f64 x, f64 y) {
 }
 void glfw__window_hint(int key, int val) {
 glfwWindowHint ( key ,  val ) ;
 }
 glfw__Window* glfw__create_window(glfw__WinCfg c) {
 if ( c .borderless ) {
 glfw__window_hint ( GLFW_RESIZABLE , 0 ) ;
 glfw__window_hint ( GLFW_DECORATED , 0 ) ;
 }
 ;
 if ( c .always_on_top ) {
 glfw__window_hint ( GLFW_FLOATING , 1 ) ;
 }
 ;
void* cwindow= glfwCreateWindow ( c .width ,  c .height , (char*) c .title .str ,  0 ,  0 ) ;
 if ( isnil ( cwindow ) ) {
 println ( tos3("failed to create a glfw window, make sure you have a GPU driver installed") ) ;
 glfwTerminate ( ) ;
 }
 ;
printf( "create window wnd=%p ptr==%p\n", cwindow, c .ptr ) ;
 glfwSetWindowUserPointer ( cwindow ,  c .ptr ) ;
glfw__Window* window= (glfw__Window*)memdup(&(glfw__Window)  { .data =  cwindow , .title =  c .title , .mx =  0 , .my =  0 } , sizeof(glfw__Window)) ;
return  window ;
 }
 void glfw__Window_set_title(glfw__Window* w, string title) {
 glfwSetWindowTitle ( w ->data , (char*) title .str ) ;
 }
 void glfw__Window_make_context_current(glfw__Window* w) {
 glfwMakeContextCurrent ( w ->data ) ;
 }
 void glfw__swap_interval(int interval) {
 glfwSwapInterval ( interval ) ;
 }
 void glfw__wait_events() {
 glfwWaitEvents ( ) ;
 }
 void glfw__poll_events() {
 glfwPollEvents ( ) ;
 }
 void glfw__set_should_close(void* w, bool close) {
 glfwSetWindowShouldClose ( w ,  close ) ;
 }
 bool glfw__Window_should_close(glfw__Window* w) {
return  glfwWindowShouldClose ( w ->data ) ;
 }
 void glfw__Window_swap_buffers(glfw__Window* w) {
 glfwSwapBuffers ( w ->data ) ;
 }
 void glfw__Window_onmousemove(glfw__Window* w, void* cb) {
 glfwSetCursorPosCallback ( w ->data ,  cb ) ;
 }
 void glfw__Window_set_mouse_button_callback(glfw__Window* w, void* cb) {
 glfwSetMouseButtonCallback ( w ->data ,  cb ) ;
 }
 void glfw__Window_on_click(glfw__Window* w, void* cb) {
 glfwSetMouseButtonCallback ( w ->data ,  cb ) ;
 }
 void glfw__Window_set_scroll_callback(glfw__Window* w, void* cb) {
 glfwSetScrollCallback ( w ->data ,  cb ) ;
 }
 void glfw__Window_on_scroll(glfw__Window* w, void* cb) {
 glfwSetScrollCallback ( w ->data ,  cb ) ;
 }
 void glfw__post_empty_event() {
 glfwPostEmptyEvent ( ) ;
 }
 void glfw__Window_onkeydown(glfw__Window* w, void* cb) {
 glfwSetKeyCallback ( w ->data ,  cb ) ;
 }
 void glfw__Window_onchar(glfw__Window* w, void* cb) {
 glfwSetCharModsCallback ( w ->data ,  cb ) ;
 }
 f64 glfw__get_time() {
return  glfwGetTime ( ) ;
 }
 bool glfw__key_pressed(void* wnd, int key) {
return  ((int)( glfwGetKey ( wnd ,  key ) ) ) == GLFW_PRESS ;
 }
 string glfw__Window_get_clipboard_text(glfw__Window* w) {
return  (tos2((byte *) ((byteptr)( glfwGetClipboardString ( w ->data ) ) ) ) ) ;
 }
 void glfw__Window_set_clipboard_text(glfw__Window* w, string s) {
 glfwSetClipboardString ( w ->data , (char*) s .str ) ;
 }
 glfw__Pos glfw__Window_get_cursor_pos(glfw__Window* w) {
f64 x= ((f64)( 0 ) ) ;
f64 y= ((f64)( 0 ) ) ;
 glfwGetCursorPos ( w ->data ,  & x ,  & y ) ;
return  (glfw__Pos) { .x =  ((int)( x ) ) , .y =  ((int)( y ) ) } ;
 }
 void* glfw__Window_user_ptr(glfw__Window* w) {
return  glfwGetWindowUserPointer ( w ->data ) ;
 }
 void glfw__Window_set_user_ptr(glfw__Window* w, void* ptr) {
 glfwSetWindowUserPointer ( w ->data ,  ptr ) ;
 }
 glfw__Size glfw__get_monitor_size() {
GLFWvidmode* mode= glfwGetVideoMode ( glfwGetPrimaryMonitor ( ) ) ;
return  (glfw__Size) { mode ->width , mode ->height } ;
 }
 string glfw__Size_str(glfw__Size size) {
return  _STR("{%d, %d}", size .width, size .height) ;
 }
 void* glfw__get_window_user_pointer(void* gwnd) {
return  glfwGetWindowUserPointer ( gwnd ) ;
 }
 glm__Vec3 glm__vec3(f32 x, f32 y, f32 z) {
glm__Vec3 res= (glm__Vec3) { .x =  x , .y =  y , .z =  z } ;
return  res ;
 }
 glm__Mat4 glm__mat4(f32* f) {
glm__Mat4 res= (glm__Mat4) { .data =  f } ;
return  res ;
 }
 string glm__Vec3_str(glm__Vec3 v) {
return  _STR("Vec3{ %f, %f, %f }", v .x, v .y, v .z) ;
 }
 string glm__Vec2_str(glm__Vec2 v) {
return  _STR("Vec3{ %f, %f }", v .x, v .y) ;
 }
 string glm__Mat4_str(glm__Mat4 m) {
string s= tos3("[ ") ;
 for (
int i= 0  ;  i < 4  ;  i ++ ) { 
 
 if ( i != 0 ) {
 s = string_add(s,  tos3("  ") ) ;
 }
 ;
 for (
int j= 0  ;  j < 4  ;  j ++ ) { 
 
f32 val= m .data [/*ptr!*/ i * 4 + j ]/*rf32 0*/ ;
 s = string_add(s,  _STR("%.2f ", val) ) ;
 }
 ;
 if ( i != 3 ) {
 s = string_add(s,  tos3("\n") ) ;
 }
 ;
 }
 ;
 s = string_add(s,  tos3("]") ) ;
return  s ;
 }
 glm__Vec2 glm__vec2(int x, int y) {
glm__Vec2 res= (glm__Vec2) { .x =  x , .y =  y } ;
return  res ;
 }
 glm__Vec3 glm__Vec3_add(glm__Vec3 a, glm__Vec3 b) {
glm__Vec3 res= (glm__Vec3) { .x =  a .x + b .x , .y =  a .y + b .y , .z =  a .z + b .z } ;
return  res ;
 }
 glm__Vec3 glm__Vec3_sub(glm__Vec3 a, glm__Vec3 b) {
glm__Vec3 res= (glm__Vec3) { .x =  a .x - b .x , .y =  a .y - b .y , .z =  a .z - b .z } ;
return  res ;
 }
 glm__Vec3 glm__Vec3_mult_scalar(glm__Vec3 a, f32 b) {
glm__Vec3 res= (glm__Vec3) { .x =  a .x * b , .y =  a .y * b , .z =  a .z * b } ;
return  res ;
 }
 void glm__Vec3_print(glm__Vec3 a) {
f32 x= a .x ;
f32 y= a .y ;
f32 z= a .z ;
 printf ( "vec3{%f,%f,%f}\n" ,  x ,  y ,  z ) ;
 }
 f32* glm__f32_calloc(int n) {
return  ((f32*)( v_calloc ( n * sizeof( f32) ) ) ) ;
 }
 glm__Mat4 glm__translate(glm__Mat4 m, glm__Vec3 v) {
f32* a= m .data ;
f32* out= glm__f32_calloc ( 16 ) ;
f32 x= v .x ;
f32 y= v .y ;
f32 z= v .z ;
f32 a00= a [/*ptr!*/ 0 ]/*rf32 0*/ ;
f32 a01= a [/*ptr!*/ 1 ]/*rf32 0*/ ;
f32 a02= a [/*ptr!*/ 2 ]/*rf32 0*/ ;
f32 a03= a [/*ptr!*/ 3 ]/*rf32 0*/ ;
f32 a10= a [/*ptr!*/ 4 ]/*rf32 0*/ ;
f32 a11= a [/*ptr!*/ 5 ]/*rf32 0*/ ;
f32 a12= a [/*ptr!*/ 6 ]/*rf32 0*/ ;
f32 a13= a [/*ptr!*/ 7 ]/*rf32 0*/ ;
f32 a20= a [/*ptr!*/ 8 ]/*rf32 0*/ ;
f32 a21= a [/*ptr!*/ 9 ]/*rf32 0*/ ;
f32 a22= a [/*ptr!*/ 10 ]/*rf32 0*/ ;
f32 a23= a [/*ptr!*/ 11 ]/*rf32 0*/ ;
 out [/*ptr!*/ 0 ]/*rf32 1*/  =  a00 ;
 out [/*ptr!*/ 1 ]/*rf32 1*/  =  a01 ;
 out [/*ptr!*/ 2 ]/*rf32 1*/  =  a02 ;
 out [/*ptr!*/ 3 ]/*rf32 1*/  =  a03 ;
 out [/*ptr!*/ 4 ]/*rf32 1*/  =  a10 ;
 out [/*ptr!*/ 5 ]/*rf32 1*/  =  a11 ;
 out [/*ptr!*/ 6 ]/*rf32 1*/  =  a12 ;
 out [/*ptr!*/ 7 ]/*rf32 1*/  =  a13 ;
 out [/*ptr!*/ 8 ]/*rf32 1*/  =  a20 ;
 out [/*ptr!*/ 9 ]/*rf32 1*/  =  a21 ;
 out [/*ptr!*/ 10 ]/*rf32 1*/  =  a22 ;
 out [/*ptr!*/ 11 ]/*rf32 1*/  =  a23 ;
 out [/*ptr!*/ 12 ]/*rf32 1*/  =  a00 * x + a10 * y + a20 * z + a [/*ptr!*/ 12 ]/*rf32 0*/ ;
 out [/*ptr!*/ 13 ]/*rf32 1*/  =  a01 * x + a11 * y + a21 * z + a [/*ptr!*/ 13 ]/*rf32 0*/ ;
 out [/*ptr!*/ 14 ]/*rf32 1*/  =  a02 * x + a12 * y + a22 * z + a [/*ptr!*/ 14 ]/*rf32 0*/ ;
 out [/*ptr!*/ 15 ]/*rf32 1*/  =  a03 * x + a13 * y + a23 * z + a [/*ptr!*/ 15 ]/*rf32 0*/ ;
return  glm__mat4 ( out ) ;
 }
 glm__Mat4 glm__ortho(f32 left, f32 right, f32 bottom, f32 top) {
int n= 16 ;
f32* res= glm__f32_calloc ( n ) ;
 res [/*ptr!*/ 0 ]/*rf32 1*/  =  2.0 / ((f32)( right - left ) ) ;
 res [/*ptr!*/ 5 ]/*rf32 1*/  =  2.0 / ((f32)( top - bottom ) ) ;
 res [/*ptr!*/ 10 ]/*rf32 1*/  =  1.0 ;
 res [/*ptr!*/ 12 ]/*rf32 1*/  =  - ( right + left ) / ( right - left ) ;
 res [/*ptr!*/ 13 ]/*rf32 1*/  =  - ( top + bottom ) / ( top - bottom ) ;
 res [/*ptr!*/ 15 ]/*rf32 1*/  =  1.0 ;
return  glm__mat4 ( res ) ;
 }
 glm__Mat4 glm__scale(glm__Mat4 m, glm__Vec3 v) {
f32* a= m .data ;
f32* out= glm__f32_calloc ( 16 ) ;
f32 x= v .x ;
f32 y= v .y ;
f32 z= v .z ;
 out [/*ptr!*/ 0 ]/*rf32 1*/  =  a [/*ptr!*/ 0 ]/*rf32 0*/ * v .x ;
 out [/*ptr!*/ 1 ]/*rf32 1*/  =  a [/*ptr!*/ 1 ]/*rf32 0*/ * x ;
 out [/*ptr!*/ 2 ]/*rf32 1*/  =  a [/*ptr!*/ 2 ]/*rf32 0*/ * x ;
 out [/*ptr!*/ 3 ]/*rf32 1*/  =  a [/*ptr!*/ 3 ]/*rf32 0*/ * x ;
 out [/*ptr!*/ 4 ]/*rf32 1*/  =  a [/*ptr!*/ 4 ]/*rf32 0*/ * y ;
 out [/*ptr!*/ 5 ]/*rf32 1*/  =  a [/*ptr!*/ 5 ]/*rf32 0*/ * y ;
 out [/*ptr!*/ 6 ]/*rf32 1*/  =  a [/*ptr!*/ 6 ]/*rf32 0*/ * y ;
 out [/*ptr!*/ 7 ]/*rf32 1*/  =  a [/*ptr!*/ 7 ]/*rf32 0*/ * y ;
 out [/*ptr!*/ 8 ]/*rf32 1*/  =  a [/*ptr!*/ 8 ]/*rf32 0*/ * z ;
 out [/*ptr!*/ 9 ]/*rf32 1*/  =  a [/*ptr!*/ 9 ]/*rf32 0*/ * z ;
 out [/*ptr!*/ 10 ]/*rf32 1*/  =  a [/*ptr!*/ 10 ]/*rf32 0*/ * z ;
 out [/*ptr!*/ 11 ]/*rf32 1*/  =  a [/*ptr!*/ 11 ]/*rf32 0*/ * z ;
 out [/*ptr!*/ 12 ]/*rf32 1*/  =  a [/*ptr!*/ 12 ]/*rf32 0*/ ;
 out [/*ptr!*/ 13 ]/*rf32 1*/  =  a [/*ptr!*/ 13 ]/*rf32 0*/ ;
 out [/*ptr!*/ 14 ]/*rf32 1*/  =  a [/*ptr!*/ 14 ]/*rf32 0*/ ;
 out [/*ptr!*/ 15 ]/*rf32 1*/  =  a [/*ptr!*/ 15 ]/*rf32 0*/ ;
return  glm__mat4 ( out ) ;
 }
 glm__Mat4 glm__rotate_z(glm__Mat4 m, f32 rad) {
f32* a= m .data ;
f32* out= glm__f32_calloc ( 16 ) ;
f64 s= math__sin ( rad ) ;
f64 c= math__cos ( rad ) ;
f32 a00= a [/*ptr!*/ 0 ]/*rf32 0*/ ;
f32 a01= a [/*ptr!*/ 1 ]/*rf32 0*/ ;
f32 a02= a [/*ptr!*/ 2 ]/*rf32 0*/ ;
f32 a03= a [/*ptr!*/ 3 ]/*rf32 0*/ ;
f32 a10= a [/*ptr!*/ 4 ]/*rf32 0*/ ;
f32 a11= a [/*ptr!*/ 5 ]/*rf32 0*/ ;
f32 a12= a [/*ptr!*/ 6 ]/*rf32 0*/ ;
f32 a13= a [/*ptr!*/ 7 ]/*rf32 0*/ ;
 out [/*ptr!*/ 8 ]/*rf32 1*/  =  a [/*ptr!*/ 8 ]/*rf32 0*/ ;
 out [/*ptr!*/ 9 ]/*rf32 1*/  =  a [/*ptr!*/ 9 ]/*rf32 0*/ ;
 out [/*ptr!*/ 10 ]/*rf32 1*/  =  a [/*ptr!*/ 10 ]/*rf32 0*/ ;
 out [/*ptr!*/ 11 ]/*rf32 1*/  =  a [/*ptr!*/ 11 ]/*rf32 0*/ ;
 out [/*ptr!*/ 12 ]/*rf32 1*/  =  a [/*ptr!*/ 12 ]/*rf32 0*/ ;
 out [/*ptr!*/ 13 ]/*rf32 1*/  =  a [/*ptr!*/ 13 ]/*rf32 0*/ ;
 out [/*ptr!*/ 14 ]/*rf32 1*/  =  a [/*ptr!*/ 14 ]/*rf32 0*/ ;
 out [/*ptr!*/ 15 ]/*rf32 1*/  =  a [/*ptr!*/ 15 ]/*rf32 0*/ ;
 out [/*ptr!*/ 0 ]/*rf32 1*/  =  a00 * c + a10 * s ;
 out [/*ptr!*/ 1 ]/*rf32 1*/  =  a01 * c + a11 * s ;
 out [/*ptr!*/ 2 ]/*rf32 1*/  =  a02 * c + a12 * s ;
 out [/*ptr!*/ 3 ]/*rf32 1*/  =  a03 * c + a13 * s ;
 out [/*ptr!*/ 4 ]/*rf32 1*/  =  a10 * c - a00 * s ;
 out [/*ptr!*/ 5 ]/*rf32 1*/  =  a11 * c - a01 * s ;
 out [/*ptr!*/ 6 ]/*rf32 1*/  =  a12 * c - a02 * s ;
 out [/*ptr!*/ 7 ]/*rf32 1*/  =  a13 * c - a03 * s ;
return  glm__mat4 ( out ) ;
 }
 glm__Mat4 glm__identity() {
int n= 16 ;
f32* res= glm__f32_calloc ( sizeof( f32) * n ) ;
 res [/*ptr!*/ 0 ]/*rf32 1*/  =  1 ;
 res [/*ptr!*/ 5 ]/*rf32 1*/  =  1 ;
 res [/*ptr!*/ 10 ]/*rf32 1*/  =  1 ;
 res [/*ptr!*/ 15 ]/*rf32 1*/  =  1 ;
return  glm__mat4 ( res ) ;
 }
 void glm__identity2(f32** res) {
 res [/*ptr!*/ 0 ]/*rf32 1*/  =  1 ;
 res [/*ptr!*/ 5 ]/*rf32 1*/  =  1 ;
 res [/*ptr!*/ 10 ]/*rf32 1*/  =  1 ;
 res [/*ptr!*/ 15 ]/*rf32 1*/  =  1 ;
 }
 array_f32 glm__identity3() {
array_f32 res=new_array_from_c_array_no_alloc(16, 16, sizeof(f32), EMPTY_ARRAY_OF_ELEMS( f32, 16 ) {  1.0 ,  0 ,  0 ,  0 ,  0 ,  1 ,  0 ,  0 ,  0 ,  0 ,  1 ,  0 ,  0 ,  0 ,  0 ,  1 ,   }) ;
return  res ;
 }
 f32* glm__ortho_js(f32 left, f32 right, f32 bottom, f32 top) {
int mynear= 1 ;
int myfar= 1 ;
f32 lr= 1.0 / ( left - right ) ;
f32 bt= 1.0 / ( bottom - top ) ;
f32 nf= 1.0 / 1.0 ;
f32* out= ( ((f32*)( v_malloc ( sizeof( f32) * 16 ) ) ) ) ;
 out [/*ptr!*/ 0 ]/*rf32 1*/  =  - 2.0 * lr ;
 out [/*ptr!*/ 1 ]/*rf32 1*/  =  0 ;
 out [/*ptr!*/ 2 ]/*rf32 1*/  =  0 ;
 out [/*ptr!*/ 3 ]/*rf32 1*/  =  0 ;
 out [/*ptr!*/ 4 ]/*rf32 1*/  =  0 ;
 out [/*ptr!*/ 5 ]/*rf32 1*/  =  - 2.0 * bt ;
 out [/*ptr!*/ 6 ]/*rf32 1*/  =  0 ;
 out [/*ptr!*/ 7 ]/*rf32 1*/  =  0 ;
 out [/*ptr!*/ 8 ]/*rf32 1*/  =  0 ;
 out [/*ptr!*/ 9 ]/*rf32 1*/  =  0 ;
 out [/*ptr!*/ 10 ]/*rf32 1*/  =  2.0 * nf ;
 out [/*ptr!*/ 11 ]/*rf32 1*/  =  0 ;
 out [/*ptr!*/ 12 ]/*rf32 1*/  =  ( left + right ) * lr ;
 out [/*ptr!*/ 13 ]/*rf32 1*/  =  ( top + bottom ) * bt ;
 out [/*ptr!*/ 14 ]/*rf32 1*/  =  1.0 * nf ;
 out [/*ptr!*/ 15 ]/*rf32 1*/  =  1 ;
return  out ;
 }
 glm__Vec3 glm__cross(glm__Vec3 a, glm__Vec3 b) {
return  (glm__Vec3) { .x =  0.0 , .y =  0.0 , .z =  0.0 } ;
 }
 void time__remove_me_when_c_bug_is_fixed() {
 }
 time__Time time__now() {
time_t t= time ( 0 ) ; 
 struct /*c struct init*/ 

tm* now= 0 ;
 now  =  localtime ( & t ) ;
return  time__convert_ctime (* now ) ;
 }
 time__Time time__random() {
int now_unix= time__now ( ) .uni ;
int rand_unix= rand__next ( now_unix ) ;
return  time__unix ( rand_unix ) ;
 }
 time__Time time__unix(int abs) {
int d= abs / time__seconds_per_day ;
int n= d / time__days_per_400_years ;
int y= 400 * n ;
 d  -=  time__days_per_400_years * n ;
 n  =  d / time__days_per_100_years ;
 n  -=  n  >>  2 ;
 y  +=  100 * n ;
 d  -=  time__days_per_100_years * n ;
 n  =  d / time__days_per_4_years ;
 y  +=  4 * n ;
 d  -=  time__days_per_4_years * n ;
 n  =  d / 365 ;
 n  -=  n  >>  2 ;
 y  +=  n ;
 d  -=  365 * n ;
int yday= ((int)( d ) ) ;
int day= yday ;
int year= abs / ((int)( 3.154e+7 ) ) + 1970 ;
int hour= ((int)( abs % time__seconds_per_day ) ) / time__seconds_per_hour ;
int minute= ((int)( abs % time__seconds_per_hour ) ) / time__seconds_per_minute ;
int second= ((int)( abs % time__seconds_per_minute ) ) ;
 if ( time__is_leap_year ( year ) ) {
 if ( day > 31 + 29 - 1 ) {
 day -- ;
 }
  else  if ( day == 31 + 29 - 1 ) {
 day  =  29 ;
return  (time__Time) { .year =  year , .month =  2 , .day =  day , .hour =  hour , .minute =  minute , .second =  second , .uni =  0 } ;
 }
 ;
 }
 ;
int month= day / 31 ;
int begin= 0 ;
int end= ((int)( ( *(int*) array_get(  time__days_before , month + 1) ) ) ) ;
 if ( day >= end ) {
 month ++ ;
 begin  =  end ;
 }
  else { 
 begin  =  ((int)( ( *(int*) array_get(  time__days_before , month) ) ) ) ;
 }
 ;
 month ++ ;
 day  =  day - begin + 1 ;
return  (time__Time) { .year =  year , .month =  month , .day =  day , .hour =  hour , .minute =  minute , .second =  second , .uni =  abs } ;
 }
 time__Time time__convert_ctime(struct /*TM*/ tm t) {
return  (time__Time) { .year =  t .tm_year + 1900 , .month =  t .tm_mon + 1 , .day =  t .tm_mday , .hour =  t .tm_hour , .minute =  t .tm_min , .second =  t .tm_sec , .uni =  mktime ( & t ) } ;
 }
 string time__Time_format_ss(time__Time t) {
return  time__Time_get_fmt_str ( t ,  time__time__FormatDelimiter_hyphen , time__time__FormatTime_hhmmss24 , time__time__FormatDate_yyyymmdd ) ;
 }
 string time__Time_format(time__Time t) {
return  time__Time_get_fmt_str ( t ,  time__time__FormatDelimiter_hyphen , time__time__FormatTime_hhmm24 , time__time__FormatDate_yyyymmdd ) ;
 }
 string time__Time_smonth(time__Time t) {
int i= t .month - 1 ;
return  string_substr2( time__months_string ,  i * 3 , ( i + 1 ) * 3 , false) ;
 }
 string time__Time_hhmm(time__Time t) {
return  time__Time_get_fmt_time_str ( t ,  time__time__FormatTime_hhmm24 ) ;
 }
 string time__Time_hhmm12(time__Time t) {
return  time__Time_get_fmt_time_str ( t ,  time__time__FormatTime_hhmm12 ) ;
 }
 string time__Time_hhmmss(time__Time t) {
return  time__Time_get_fmt_time_str ( t ,  time__time__FormatTime_hhmmss24 ) ;
 }
 string time__Time_ymmdd(time__Time t) {
return  time__Time_get_fmt_date_str ( t ,  time__time__FormatDelimiter_hyphen , time__time__FormatDate_yyyymmdd ) ;
 }
 string time__Time_ddmmy(time__Time t) {
return  time__Time_get_fmt_date_str ( t ,  time__time__FormatDelimiter_dot , time__time__FormatDate_ddmmyyyy ) ;
 }
 string time__Time_md(time__Time t) {
return  time__Time_get_fmt_date_str ( t ,  time__time__FormatDelimiter_space , time__time__FormatDate_mmmd ) ;
 }
 string time__Time_clean(time__Time t) {
time__Time nowe= time__now ( ) ;
 if ( t .month == nowe .month  &&  t .year == nowe .year  &&  t .day == nowe .day ) {
return  time__Time_get_fmt_time_str ( t ,  time__time__FormatTime_hhmm24 ) ;
 }
 ;
 if ( t .year == nowe .year ) {
return  time__Time_get_fmt_str ( t ,  time__time__FormatDelimiter_space , time__time__FormatTime_hhmm24 , time__time__FormatDate_mmmd ) ;
 }
 ;
return  time__Time_format ( t ) ;
 }
 string time__Time_clean12(time__Time t) {
time__Time nowe= time__now ( ) ;
 if ( t .month == nowe .month  &&  t .year == nowe .year  &&  t .day == nowe .day ) {
return  time__Time_get_fmt_time_str ( t ,  time__time__FormatTime_hhmm12 ) ;
 }
 ;
 if ( t .year == nowe .year ) {
return  time__Time_get_fmt_str ( t ,  time__time__FormatDelimiter_space , time__time__FormatTime_hhmm12 , time__time__FormatDate_mmmd ) ;
 }
 ;
return  time__Time_format ( t ) ;
 }
 time__Time time__parse(string s) {
Option_int tmp7 =  string_index ( s ,  tos3(" ") ) ;
 int pos;
 if (!tmp7 .ok) {
 string err = tmp7 . error;
 int errcode = tmp7 . ecode;
 println ( tos3("bad time format") ) ;
return  time__now ( ) ;
 }
 pos = *(int*)tmp7.data;
 ;
string symd= string_substr2( s ,  0 , pos , false) ;
array_string ymd= string_split ( symd ,  tos3("-") ) ;
 if ( ymd .len != 3 ) {
 println ( tos3("bad time format") ) ;
return  time__now ( ) ;
 }
 ;
string shms= string_substr2( s ,  pos , -1, true) ;
array_string hms= string_split ( shms ,  tos3(":") ) ;
string hour= ( *(string*) array_get(  hms , 0) ) ;
string minute= ( *(string*) array_get(  hms , 1) ) ;
string second= ( *(string*) array_get(  hms , 2) ) ;
return  time__new_time ( (time__Time) { .year =  v_string_int ( ( *(string*) array_get(  ymd , 0) ) ) , .month =  v_string_int ( ( *(string*) array_get(  ymd , 1) ) ) , .day =  v_string_int ( ( *(string*) array_get(  ymd , 2) ) ) , .hour =  v_string_int ( hour ) , .minute =  v_string_int ( minute ) , .second =  v_string_int ( second ) , .uni =  0 } ) ;
 }
 time__Time time__new_time(time__Time t) {
return  (time__Time){ .uni =  time__Time_calc_unix (& /* ? */ t ) , .year = t.year, .month = t.month, .day = t.day, .hour = t.hour, .minute = t.minute, .second = t.second, } ;
 }
 int time__Time_calc_unix(time__Time* t) {
 if ( t ->uni != 0 ) {
return  t ->uni ;
 }

struct /*TM*/ tm tt= (struct tm) { .tm_sec =  t ->second , .tm_min =  t ->minute , .tm_hour =  t ->hour , .tm_mday =  t ->day , .tm_mon =  t ->month - 1 , .tm_year =  t ->year - 1900 } ;
return  mktime ( & tt ) ;
 }
 time__Time time__Time_add_seconds(time__Time t, int seconds) {
return  time__unix ( t .uni + seconds ) ;
 }
 time__Time time__Time_add_days(time__Time t, int days) {
return  time__unix ( t .uni + days * 3600 * 24 ) ;
 }
 int time__since(time__Time t) {
return  0 ;
 }
 string time__Time_relative(time__Time t) {
time__Time now= time__now ( ) ;
int secs= now .uni - t .uni ;
 if ( secs <= 30 ) {
return  tos3("now") ;
 }
 ;
 if ( secs < 60 ) {
return  tos3("1m") ;
 }
 ;
 if ( secs < 3600 ) {
return  _STR("%dm", secs / 60) ;
 }
 ;
 if ( secs < 3600 * 24 ) {
return  _STR("%dh", secs / 3600) ;
 }
 ;
 if ( secs < 3600 * 24 * 5 ) {
return  _STR("%dd", secs / 3600 / 24) ;
 }
 ;
 if ( secs > 3600 * 24 * 10000 ) {
return  tos3("") ;
 }
 ;
return  time__Time_md ( t ) ;
 }
 int time__day_of_week(int y, int m, int d) {
array_int t=new_array_from_c_array(12, 12, sizeof(int), EMPTY_ARRAY_OF_ELEMS( int, 12 ) {  0 ,  3 ,  2 ,  5 ,  0 ,  3 ,  5 ,  1 ,  4 ,  6 ,  2 ,  4  }) ;
int sy= y ;
 if ( ( m < 3 ) ) {
 sy  =  sy - 1 ;
 }
 ;
return  ( sy + sy / 4 - sy / 100 + sy / 400 + ( *(int*) array_get(  t , m - 1) ) + d - 1 ) % 7 + 1 ;
 }
 int time__Time_day_of_week(time__Time t) {
return  time__day_of_week ( t .year , t .month , t .day ) ;
 }
 string time__Time_weekday_str(time__Time t) {
int i= time__Time_day_of_week ( t ) - 1 ;
return  string_substr2( time__days_string ,  i * 3 , ( i + 1 ) * 3 , false) ;
 }
 i64 time__ticks() {
 #ifdef _WIN32
 #else 
 struct /*c struct init*/ 

timeval ts ;
 gettimeofday ( & ts ,  0 ) ;
return  ((i64)( ts .tv_sec * ((u64)( 1000 ) ) + ( ts .tv_usec / ((u64)( 1000 ) ) ) ) ) ;
 #endif
 ;
 }
 void time__sleep(int seconds) {
 #ifdef _WIN32
 #else
 sleep ( seconds ) ;
 #endif
 ;
 }
 void time__usleep(int n) {
 #ifdef _WIN32
 #else
 usleep ( n ) ;
 #endif
 ;
 }
 void time__sleep_ms(int n) {
 #ifdef _WIN32
 #else
 usleep ( n * 1000 ) ;
 #endif
 ;
 }
 bool time__is_leap_year(int year) {
return  ( year % 4 == 0 )  &&  ( year % 100 != 0  ||  year % 400 == 0 ) ;
 }
 Option_int time__days_in_month(int month, int year) {
 if ( month > 12  ||  month < 1 ) {
return  v_error ( _STR("Invalid month: %d", month) ) ;
 }
 ;
int extra= (( month == 2  &&  time__is_leap_year ( year ) ) ? ( 1 ) : ( 0 )) ;
int res= ( *(int*) array_get(  time__month_days , month - 1) ) + extra ;
int tmp30 = OPTION_CAST(int)( res); 
 return opt_ok(&tmp30, sizeof(int)) ;
 }
 string time__Time_get_fmt_time_str(time__Time t, time__FormatTime fmt_time) {
 if ( fmt_time == time__time__FormatTime_no_time ) {
return  tos3("") ;
 }
 ;
string tp= (( t .hour > 11 ) ? ( tos3("p.m.") ) : ( tos3("a.m.") )) ;
int hour= (( t .hour > 12 ) ? ( t .hour - 12 ) : ( (( t .hour == 0 ) ? ( 12 ) : ( t .hour )))) ; 
 time__FormatTime tmp31 =  fmt_time; 

return  ( ( tmp31 ==  time__time__FormatTime_hhmm12 ) ? ( _STR("%d:%02d %.*s", hour, t .minute, tp.len, tp.str) ) : ( ( tmp31 ==  time__time__FormatTime_hhmm24 ) ? ( _STR("%02d:%02d", t .hour, t .minute) ) : ( ( tmp31 ==  time__time__FormatTime_hhmmss12 ) ? ( _STR("%d:%02d:%02d %.*s", hour, t .minute, t .second, tp.len, tp.str) ) : ( ( tmp31 ==  time__time__FormatTime_hhmmss24 ) ? ( _STR("%02d:%02d:%02d", t .hour, t .minute, t .second) ) :( _STR("unknown enumeration %d", fmt_time) ))))) ;
 }
 string time__Time_get_fmt_date_str(time__Time t, time__FormatDelimiter fmt_dlmtr, time__FormatDate fmt_date) {
 if ( fmt_date == time__time__FormatDate_no_date ) {
return  tos3("") ;
 }
 ;
string month= _STR("%.*s", time__Time_smonth ( t ).len, time__Time_smonth ( t ).str) ;
string year= string_substr2( int_str ( t .year ) ,  2 , -1, true) ; 
 time__FormatDate tmp34 =  fmt_date; 
 
 time__FormatDelimiter tmp35 =  fmt_dlmtr; 

return  string_replace ( ( ( tmp34 ==  time__time__FormatDate_ddmmyy ) ? ( _STR("%02d|%02d|%.*s", t .day, t .month, year.len, year.str) ) : ( ( tmp34 ==  time__time__FormatDate_ddmmyyyy ) ? ( _STR("%02d|%02d|%d", t .day, t .month, t .year) ) : ( ( tmp34 ==  time__time__FormatDate_mmddyy ) ? ( _STR("%02d|%02d|%.*s", t .month, t .day, year.len, year.str) ) : ( ( tmp34 ==  time__time__FormatDate_mmddyyyy ) ? ( _STR("%02d|%02d|%d", t .month, t .day, t .year) ) : ( ( tmp34 ==  time__time__FormatDate_mmmd ) ? ( _STR("%.*s|%d", month.len, month.str, t .day) ) : ( ( tmp34 ==  time__time__FormatDate_mmmdd ) ? ( _STR("%.*s|%02d", month.len, month.str, t .day) ) : ( ( tmp34 ==  time__time__FormatDate_mmmddyyyy ) ? ( _STR("%.*s|%02d|%d", month.len, month.str, t .day, t .year) ) : ( ( tmp34 ==  time__time__FormatDate_yyyymmdd ) ? ( _STR("%d|%02d|%02d", t .year, t .month, t .day) ) :( _STR("unknown enumeration %d", fmt_date) ))))))))) ,  tos3("|") , ( ( tmp35 ==  time__time__FormatDelimiter_dot ) ? ( tos3(".") ) : ( ( tmp35 ==  time__time__FormatDelimiter_hyphen ) ? ( tos3("-") ) : ( ( tmp35 ==  time__time__FormatDelimiter_slash ) ? ( tos3("/") ) : ( ( tmp35 ==  time__time__FormatDelimiter_space ) ? ( tos3(" ") ) :( _STR("unknown enumeration %d", fmt_dlmtr) ))))) ) ;
 }
 string time__Time_get_fmt_str(time__Time t, time__FormatDelimiter fmt_dlmtr, time__FormatTime fmt_time, time__FormatDate fmt_date) {
 if ( fmt_date == time__time__FormatDate_no_date ) {
 if ( fmt_time == time__time__FormatTime_no_time ) {
return  tos3("") ;
 }
  else { 
return  time__Time_get_fmt_time_str ( t ,  fmt_time ) ;
 }
 ;
 }
  else { 
 if ( fmt_time != time__time__FormatTime_no_time ) {
return string_add(string_add( time__Time_get_fmt_date_str ( t ,  fmt_dlmtr , fmt_date ) , tos3(" ") ) , time__Time_get_fmt_time_str ( t ,  fmt_time ) ) ;
 }
  else { 
return  time__Time_get_fmt_date_str ( t ,  fmt_dlmtr , fmt_date ) ;
 }
 ;
 }
 ;
 }
 gg__Vec2 gg__vec2(int x, int y) {
gg__Vec2 res= (gg__Vec2) { .x =  x , .y =  y } ;
return  res ;
 }
 void gg__init_gg() {
 glfw__init_glfw ( ) ;
 println ( gl__TEXT_VERT ) ;
 gl__init_glad ( ) ;
 }
 gg__GG* gg__new_context(gg__Cfg cfg) {
glfw__Window* window= 0 ;
 if ( cfg .create_window ) {
 if ( cfg .resizable ) {
 glfw__window_hint ( GLFW_RESIZABLE , 1 ) ;
 }
  else { 
 glfw__window_hint ( GLFW_RESIZABLE , 0 ) ;
 }
 ;
 window  =  glfw__create_window ( (glfw__WinCfg) { .title =  cfg .window_title , .width =  cfg .width , .height =  cfg .height , .ptr =  cfg .window_user_ptr , .always_on_top =  cfg .always_on_top , .borderless =  0 , .is_modal =  0 , .is_browser =  0 , .url =  tos3("") , } ) ;
 glfw__Window_make_context_current (& /* ? */* window ) ;
 gg__init_gg ( ) ;
 }
 ;
gl__Shader shader= gl__new_shader ( tos3("simple") ) ;
 gl__Shader_use ( shader ) ;
 if ( cfg .use_ortho ) {
glm__Mat4 projection= glm__ortho ( 0 , cfg .width , cfg .height , 0 ) ;
 gl__Shader_set_mat4 ( shader ,  tos3("projection") , projection ) ;
 }
  else { 
 gl__Shader_set_mat4 ( shader ,  tos3("projection") , glm__identity ( ) ) ;
 }
 ;
u32 vao= gl__gen_vertex_array ( ) ;
u32 vbo= gl__gen_buffer ( ) ;
int scale= 1 ;
 if ( cfg .retina ) {
 scale  =  2 ;
 }
 ;
 gg__todo_remove_me ( cfg , scale ) ;
return  (gg__GG*)memdup(&(gg__GG)  { .shader =  shader , .width =  cfg .width , .height =  cfg .height , .vao =  vao , .vbo =  vbo , .window =  window , .scale =  scale , .rect_vao =  0 , .rect_vbo =  0 , .line_vao =  0 , .line_vbo =  0 , } , sizeof(gg__GG)) ;
 }
 void gg__clear(gx__Color color) {
 gl__clear_color ( color .r , color .g , color .b , 255 ) ;
 gl__clear ( ) ;
 }
 void gg__GG_render(gg__GG* gg) {
 glfw__Window_swap_buffers (& /* ? */* gg ->window ) ;
 glfw__wait_events ( ) ;
 }
 void gg__GG_draw_triangle(gg__GG* ctx, f32 x1, f32 y1, f32 x2, f32 y2, f32 x3, f32 y3, gx__Color c) {
 gl__Shader_use ( ctx ->shader ) ;
 gl__Shader_set_color ( ctx ->shader ,  tos3("color") , c ) ;
array_f32 vertices=new_array_from_c_array_no_alloc(9, 9, sizeof(f32), EMPTY_ARRAY_OF_ELEMS( f32, 9 ) {  x1 ,  y1 ,  0 ,  x2 ,  y2 ,  0 ,  x3 ,  y3 ,  0 ,   }) ;
 gl__bind_vao ( ctx ->vao ) ;
 gl__set_vbo ( ctx ->vbo , vertices , GL_STATIC_DRAW ) ;
 gl__vertex_attrib_pointer ( 0 , 3 , GL_FLOAT , 0 , 3 , 0 ) ;
 gl__enable_vertex_attrib_array ( 0 ) ;
 gl__draw_arrays ( GL_TRIANGLES , 0 , 3 ) ;
 }
 void gg__GG_draw_triangle_tex(gg__GG* ctx, f32 x1, f32 y1, f32 x2, f32 y2, f32 x3, f32 y3, gx__Color c) {
 gl__Shader_use ( ctx ->shader ) ;
 gl__Shader_set_color ( ctx ->shader ,  tos3("color") , c ) ;
 gl__Shader_set_int ( ctx ->shader ,  tos3("has_texture") , 1 ) ;
array_f32 vertices=new_array_from_c_array_no_alloc(24, 24, sizeof(f32), EMPTY_ARRAY_OF_ELEMS( f32, 24 ) {  x1 ,  y1 ,  0 ,  0 ,  0 ,  0 ,  1 ,  1 ,  x2 ,  y2 ,  0 ,  0 ,  0 ,  0 ,  1 ,  0 ,  x3 ,  y3 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,   }) ;
 gl__bind_vao ( ctx ->vao ) ;
 gl__set_vbo ( ctx ->vbo , vertices , GL_STATIC_DRAW ) ;
 gl__vertex_attrib_pointer ( 0 , 3 , GL_FLOAT , 0 , 3 , 0 ) ;
 gl__enable_vertex_attrib_array ( 0 ) ;
 gl__vertex_attrib_pointer ( 1 , 3 , GL_FLOAT , 0 , 8 , 3 ) ;
 gl__enable_vertex_attrib_array ( 1 ) ;
 gl__vertex_attrib_pointer ( 2 , 2 , GL_FLOAT , 0 , 8 , 6 ) ;
 gl__enable_vertex_attrib_array ( 2 ) ;
 gl__draw_elements ( GL_TRIANGLES , 6 , GL_UNSIGNED_INT , 0 ) ;
 }
 void gg__GG_draw_rect(gg__GG* ctx, f32 x, f32 y, f32 w, f32 h, gx__Color c) {
 gg__GG_draw_rect2 (& /* ? */* ctx ,  x , y , w , h , c ) ;
 }
 void gg__GG_draw_rect2(gg__GG* ctx, f32 x, f32 y, f32 w, f32 h, gx__Color c) {
 glDeleteBuffers ( 1 ,  & ctx ->vao ) ;
 glDeleteBuffers ( 1 ,  & ctx ->vbo ) ;
 gl__Shader_use ( ctx ->shader ) ;
 gl__Shader_set_color ( ctx ->shader ,  tos3("color") , c ) ;
 gl__Shader_set_int ( ctx ->shader ,  tos3("has_texture") , 0 ) ;
 #ifdef __linux__
 #endif
 ;
array_f32 vertices=new_array_from_c_array_no_alloc(12, 12, sizeof(f32), EMPTY_ARRAY_OF_ELEMS( f32, 12 ) {  x + w ,  y ,  0 ,  x + w ,  y + h ,  0 ,  x ,  y + h ,  0 ,  x ,  y ,  0 ,   }) ;
array_int indices=new_array_from_c_array_no_alloc(6, 6, sizeof(int), EMPTY_ARRAY_OF_ELEMS( int, 6 ) {  0 ,  1 ,  3 ,  1 ,  2 ,  3  }) ;
 gl__bind_vao ( ctx ->vao ) ;
 gl__set_vbo ( ctx ->vbo , vertices , GL_STATIC_DRAW ) ;
u32 ebo= gl__gen_buffer ( ) ;
 gl__set_ebo ( ebo , indices , GL_STATIC_DRAW ) ;
 gl__vertex_attrib_pointer ( 0 , 3 , GL_FLOAT , 0 , 3 , 0 ) ;
 gl__enable_vertex_attrib_array ( 0 ) ;
 gl__bind_vao ( ctx ->vao ) ;
 gl__draw_elements ( GL_TRIANGLES , 6 , GL_UNSIGNED_INT , 0 ) ;
 glDeleteBuffers ( 1 ,  & ebo ) ;
 }
 void gg__todo_remove_me(gg__Cfg cfg, int scale) {
 if ( ! cfg .use_ortho ) {
 
 return ;
 }
 ;
int width= cfg .width * scale ;
int height= cfg .height * scale ;
int font_size= cfg .font_size * scale ;
 gl__enable ( GL_BLEND ) ;
gl__Shader shader= gl__new_shader ( tos3("text") ) ;
 gl__Shader_use ( shader ) ;
glm__Mat4 projection= glm__ortho ( 0 , width , 0 , height ) ;
 gl__Shader_set_mat4 ( shader ,  tos3("projection") , projection ) ;
u32 vao= gl__gen_vertex_array ( ) ;
u32 vbo= gl__gen_buffer ( ) ;
 gl__bind_vao ( vao ) ;
 gl__bind_buffer ( GL_ARRAY_BUFFER , vbo ) ;
 gl__enable_vertex_attrib_array ( 0 ) ;
 gl__vertex_attrib_pointer ( 0 , 4 , GL_FLOAT , 0 , 4 , 0 ) ;
 }
 void gg__update() {
 }
 void gg__post_empty_event() {
 glfw__post_empty_event ( ) ;
 }
 void gg__GG_circle(gg__GG c, int x, int y, int r) {
 }
 void gg__GG_fill_color(gg__GG c, gx__Color color) {
 }
 void gg__GG_fill(gg__GG c) {
 }
 void gg__GG_move_to(gg__GG c, int x, int y) {
 }
 void gg__GG_line_to(gg__GG c, int x, int y) {
 }
 void gg__GG_stroke_width(gg__GG c, int size) {
 }
 void gg__GG_stroke_color(gg__GG c, gx__Color color) {
 }
 void gg__GG_stroke(gg__GG c) {
 }
 void gg__GG_save(gg__GG c) {
 }
 void gg__GG_restore(gg__GG c) {
 }
 void gg__GG_intersect_scissor(gg__GG c, int x, int y, int w, int h) {
 }
 void gg__GG_translate(gg__GG c, int x, int y) {
 }
 int gg__GG_create_font(gg__GG c, string name, string file) {
return  0 ;
 }
 void gg__GG_text(gg__GG c, int x, int y, string text) {
 }
 void gg__GG_text_box(gg__GG c, int x, int y, int max, string text) {
 }
 void gg__GG_font_face(gg__GG c, string f) {
 }
 void gg__GG_font_size(gg__GG c, int size) {
 }
 void gg__GG_text_align(gg__GG c, int a) {
 }
 u32 gg__create_image(string file) {
printf( "gg create image \"%.*s\"\n", file.len, file.str ) ;
 if ( string_contains ( file ,  tos3("twitch") ) ) {
return  ((u32)( 0 ) ) ;
 }
 ;
 if ( ! os__file_exists ( file ) ) {
printf( "gg create image no such file \"%.*s\"\n", file.len, file.str ) ;
return  ((u32)( 0 ) ) ;
 }
 ;
u32 texture= gl__gen_texture ( ) ;
stbi__Image img= stbi__load ( file ) ;
 gl__bind_2d_texture ( texture ) ;
 stbi__Image_tex_image_2d ( img ) ;
 gl__generate_mipmap ( GL_TEXTURE_2D ) ;
 stbi__Image_free ( img ) ;
return  texture ;
 }
 void gg__GG_draw_line_c(gg__GG* ctx, f32 x, f32 y, f32 x2, f32 y2, gx__Color color) {
 glDeleteBuffers ( 1 ,  & ctx ->vao ) ;
 glDeleteBuffers ( 1 ,  & ctx ->vbo ) ;
 gl__Shader_use ( ctx ->shader ) ;
 gl__Shader_set_color ( ctx ->shader ,  tos3("color") , color ) ;
array_f32 vertices=new_array_from_c_array_no_alloc(4, 4, sizeof(f32), EMPTY_ARRAY_OF_ELEMS( f32, 4 ) {  ((f32)( x ) ) ,  ((f32)( y ) ) ,  ((f32)( x2 ) ) ,  ((f32)( y2 ) )  }) ;
 gl__bind_vao ( ctx ->vao ) ;
 gl__set_vbo ( ctx ->vbo , vertices , GL_STATIC_DRAW ) ;
 gl__vertex_attrib_pointer ( 0 , 2 , GL_FLOAT , 0 , 2 , 0 ) ;
 gl__enable_vertex_attrib_array ( 0 ) ;
 gl__bind_vao ( ctx ->vao ) ;
 gl__draw_arrays ( GL_LINES , 0 , 2 ) ;
 }
 void gg__GG_draw_line(gg__GG* c, f32 x, f32 y, f32 x2, f32 y2) {
 gg__GG_draw_line_c (& /* ? */* c ,  x , y , x2 , y2 , gx__Gray ) ;
 }
 void gg__GG_draw_vertical(gg__GG* c, int x, int y, int height) {
 gg__GG_draw_line (& /* ? */* c ,  x , y , x , y + height ) ;
 }
 void gg__GG_draw_image(gg__GG* ctx, f32 x, f32 y, f32 w, f32 h, u32 tex_id) {
 gl__Shader_use ( ctx ->shader ) ;
 gl__Shader_set_int ( ctx ->shader ,  tos3("has_texture") , 1 ) ;
array_f32 vertices=new_array_from_c_array_no_alloc(32, 32, sizeof(f32), EMPTY_ARRAY_OF_ELEMS( f32, 32 ) {  x + w ,  y ,  0 ,  1 ,  0 ,  0 ,  1 ,  1 ,  x + w ,  y + h ,  0 ,  0 ,  1 ,  0 ,  1 ,  0 ,  x ,  y + h ,  0 ,  0 ,  0 ,  1 ,  0 ,  0 ,  x ,  y ,  0 ,  1 ,  1 ,  0 ,  0 ,  1 ,   }) ;
array_int indices=new_array_from_c_array_no_alloc(6, 6, sizeof(int), EMPTY_ARRAY_OF_ELEMS( int, 6 ) {  0 ,  1 ,  3 ,  1 ,  2 ,  3  }) ;
 gl__bind_vao ( ctx ->vao ) ;
 gl__set_vbo ( ctx ->vbo , vertices , GL_STATIC_DRAW ) ;
u32 ebo= gl__gen_buffer ( ) ;
 gl__set_ebo ( ebo , indices , GL_STATIC_DRAW ) ;
 gl__vertex_attrib_pointer ( 0 , 3 , GL_FLOAT , 0 , 8 , 0 ) ;
 gl__enable_vertex_attrib_array ( 0 ) ;
 gl__vertex_attrib_pointer ( 1 , 3 , GL_FLOAT , 0 , 8 , 3 ) ;
 gl__enable_vertex_attrib_array ( 1 ) ;
 gl__vertex_attrib_pointer ( 2 , 2 , GL_FLOAT , 0 , 8 , 6 ) ;
 gl__enable_vertex_attrib_array ( 2 ) ;
 gl__bind_2d_texture ( ((u32)( tex_id ) ) ) ;
 gl__bind_vao ( ctx ->vao ) ;
 gl__draw_elements ( GL_TRIANGLES , 6 , GL_UNSIGNED_INT , 0 ) ;
 }
 void gg__GG_draw_empty_rect(gg__GG* c, int x, int y, int w, int h, gx__Color color) {
 gg__GG_draw_line_c (& /* ? */* c ,  x , y , x + w , y , color ) ;
 gg__GG_draw_line_c (& /* ? */* c ,  x , y , x , y + h , color ) ;
 gg__GG_draw_line_c (& /* ? */* c ,  x , y + h , x + w , y + h , color ) ;
 gg__GG_draw_line_c (& /* ? */* c ,  x + w , y , x + w , y + h , color ) ;
 }
 freetype__Character freetype__ft_load_char(FT_Face face, i64 code) {
int ret= FT_Load_Char ( face ,  code ,  FT_LOAD_RENDER ) ;
 if ( ret != 0 ) {
 println (string_add( _STR("freetype: failed to load glyph (utf32 code=%lld, ", code) , _STR("error code=%d)", ret) ) ) ;
return  (freetype__Character) { .texture_id =  0 , .advance =  0 } ;
 }
 ;
int texture= 0 ;
 glGenTextures ( 1 ,  & texture ) ;
 glBindTexture ( GL_TEXTURE_2D ,  texture ) ;
int fgwidth= face ->glyph ->bitmap .width ;
int fgrows= face ->glyph ->bitmap .rows ;
 glTexImage2D ( GL_TEXTURE_2D ,  0 ,  GL_RED ,  fgwidth ,  fgrows ,  0 ,  GL_RED ,  GL_UNSIGNED_BYTE ,  face ->glyph ->bitmap .buffer ) ;
 glTexParameteri ( GL_TEXTURE_2D ,  GL_TEXTURE_WRAP_S ,  GL_CLAMP_TO_EDGE ) ;
 glTexParameteri ( GL_TEXTURE_2D ,  GL_TEXTURE_WRAP_T ,  GL_CLAMP_TO_EDGE ) ;
 glTexParameteri ( GL_TEXTURE_2D ,  GL_TEXTURE_MIN_FILTER ,  GL_LINEAR ) ;
 glTexParameteri ( GL_TEXTURE_2D ,  GL_TEXTURE_MAG_FILTER ,  GL_LINEAR ) ;
int fgleft= face ->glyph ->bitmap_left ;
int fgtop= face ->glyph ->bitmap_top ;
return  (freetype__Character) { .texture_id =  ((u32)( texture ) ) , .size =  gg__vec2 ( ((int)( ((u32)( fgwidth ) ) ) ) , ((int)( ((u32)( fgrows ) ) ) ) ) , .bearing =  gg__vec2 ( ((int)( ((u32)( fgleft ) ) ) ) , ((int)( ((u32)( fgtop ) ) ) ) ) , .advance =  ( ((u32)( face ->glyph ->advance .x ) ) ) } ;
 }
 freetype__FreeType* freetype__new_context(gg__Cfg cfg) {
int scale= cfg .scale ;
 if ( ! cfg .use_ortho ) {
return  (freetype__FreeType*)memdup(&(freetype__FreeType)  { .width =  0 , .height =  0 , .vao =  0 , .rect_vao =  0 , .rect_vbo =  0 , .line_vao =  0 , .line_vbo =  0 , .vbo =  0 , .chars =  new_array(0, 1, sizeof( freetype__Character )) , .scale =  0 , .utf_runes =  new_array(0, 1, sizeof( string )) , .utf_chars =  new_array(0, 1, sizeof( freetype__Character )) } , sizeof(freetype__FreeType)) ;
 }
 ;
int width= cfg .width * scale ;
int height= cfg .height * scale ;
int font_size= cfg .font_size * scale ;
 gl__enable ( GL_BLEND ) ;
 glBlendFunc ( GL_SRC_ALPHA ,  GL_ONE_MINUS_SRC_ALPHA ) ;
gl__Shader shader= gl__new_shader ( tos3("text") ) ;
 gl__Shader_use ( shader ) ;
glm__Mat4 projection= glm__ortho ( 0 , width , 0 , height ) ;
 gl__Shader_set_mat4 ( shader ,  tos3("projection") , projection ) ;
FT_Library ft= (FT_Library) { 0 } ;
void* ret= FT_Init_FreeType ( & ft ) ;
 if ( ret != 0 ) {
 v_panic ( tos3("freetype: Could not init FreeType Library") ) ;
 }
 ;
string font_path= cfg .font_path ;
 if (string_eq( font_path , tos3("") ) ) {
 font_path  =  tos3("RobotoMono-Regular.ttf") ;
 }
 ;
 if ( ! os__file_exists ( font_path ) ) {
string exe_path= os__executable ( ) ;
string exe_dir= os__basedir ( exe_path ) ;
 font_path  =  _STR("%.*s/%.*s", exe_dir.len, exe_dir.str, font_path.len, font_path.str) ;
 }
 ;
 if ( ! os__file_exists ( font_path ) ) {
printf( "failed to load %.*s\n", font_path.len, font_path.str ) ;
return  0 ;
 }
 ;
printf( "Trying to load font from %.*s\n", font_path.len, font_path.str ) ;
FT_Face face ;
 ret  =  ((int)( FT_New_Face ( ft , (char*) font_path .str ,  0 ,  & face ) ) ) ;
 if ( ret != 0 ) {
printf( "freetype: failed to load the font (error=%p)\n", ret ) ;
 v_exit ( 1 ) ;
 }
 ;
 FT_Set_Pixel_Sizes ( face ,  0 ,  font_size ) ;
 glPixelStorei ( GL_UNPACK_ALIGNMENT ,  1 ) ;
array_freetype__Character chars=new_array_from_c_array(0, 0, sizeof(freetype__Character), EMPTY_ARRAY_OF_ELEMS( freetype__Character, 0 ) {   TCCSKIP(0) }) ;
 for (
int c= 0  ;  c < 128  ;  c ++ ) { 
 
freetype__Character ch= freetype__ft_load_char ( face , ((i64)( c ) ) ) ;
_PUSH(& chars , ( /*typ = array_freetype__Character   tmp_typ=freetype__Character*/ ch ), tmp1, freetype__Character) ;
 }
 ;
freetype__Character ch= (freetype__Character) { .texture_id =  0 , .advance =  0 } ;
u32 vao= gl__gen_vertex_array ( ) ;
printf( "new gg text context vao=%u\n", vao ) ;
u32 vbo= gl__gen_buffer ( ) ;
 gl__bind_vao ( vao ) ;
 gl__bind_buffer ( GL_ARRAY_BUFFER , vbo ) ;
 gl__enable_vertex_attrib_array ( 0 ) ;
 gl__vertex_attrib_pointer ( 0 , 4 , GL_FLOAT , 0 , 4 , 0 ) ;
freetype__FreeType* ctx= (freetype__FreeType*)memdup(&(freetype__FreeType)  { .shader =  shader , .width =  width , .height =  height , .scale =  scale , .vao =  vao , .vbo =  vbo , .chars =  chars , .face =  face , .rect_vao =  0 , .rect_vbo =  0 , .line_vao =  0 , .line_vbo =  0 , .utf_runes =  new_array(0, 1, sizeof( string )) , .utf_chars =  new_array(0, 1, sizeof( freetype__Character )) } , sizeof(freetype__FreeType)) ;
return  ctx ;
 }
 void freetype__FreeType_draw_text(freetype__FreeType* ctx, int _x, int _y, string text, gx__TextCfg cfg) {
ustring utext= string_ustring ( text ) ;
 freetype__FreeType_private_draw_text ( ctx ,  _x , _y , utext , cfg ) ;
 }
 void freetype__FreeType_draw_text_fast(freetype__FreeType* ctx, int _x, int _y, ustring text, gx__TextCfg cfg) {
 freetype__FreeType_private_draw_text ( ctx ,  _x , _y , text , cfg ) ;
 }
 void freetype__FreeType_private_draw_text(freetype__FreeType* ctx, int _x, int _y, ustring utext, gx__TextCfg cfg) {
f32 x= ((f32)( _x ) ) ;
f32 y= ((f32)( _y ) ) ;
 if ( cfg .align == gx__ALIGN_RIGHT ) {
int width= utext .len * 7 ;
 x  -=  width + 10 ;
 }
 ;
 x  *=  ctx ->scale ;
 y  =  y * ((int)( ctx ->scale ) ) + ( ( cfg .size * ctx ->scale ) / 2 ) + 5 * ctx ->scale ;
 y  =  ((f32)( ctx ->height ) ) - y ;
gx__Color color= cfg .color ;
 gl__Shader_use ( ctx ->shader ) ;
 gl__Shader_set_color ( ctx ->shader ,  tos3("textColor") , color ) ;
 glActiveTexture ( GL_TEXTURE0 ) ;
 gl__bind_vao ( ctx ->vao ) ;
 for (
int i= 0  ;  i < utext .len  ;  i ++ ) { 
 
string _rune= ustring_at ( utext ,  i ) ;
freetype__Character ch= (freetype__Character) { .texture_id =  0 , .advance =  0 } ;
bool found= 0 ;
 if ( _rune .len == 1 ) {
byte idx= string_at( _rune ,  0) ;
 if ( idx < 0  ||  idx >= ctx ->chars .len ) {
printf( "BADE RUNE %.*s\n", _rune.len, _rune.str ) ;
 continue
 ;
 }
 ;
 found  =  1 ;
 ch  =  ( *(freetype__Character*) array_get(  ctx ->chars , string_at( _rune ,  0)) ) ;
 }
  else  if ( _rune .len > 1 ) {
 for (
int j= 0  ;  j < ctx ->utf_runes .len  ;  j ++ ) { 
 
string rune_j= ( *(string*) array_get(  ctx ->utf_runes , j) ) ;
 if (string_eq( rune_j , _rune ) ) {
 ch  =  ( *(freetype__Character*) array_get(  ctx ->utf_chars , j) ) ;
 found  =  1 ;
 break
 ;
 }
 ;
 }
 ;
 }
 ;
 if ( ! found  &&  _rune .len > 0  &&  string_at( _rune ,  0) > 32 ) {
byte c= string_at( _rune ,  0) ;
 ch  =  freetype__ft_load_char ( ctx ->face , string_utf32_code ( _rune ) ) ;
_PUSH(& ctx ->utf_runes , ( /*typ = array_string   tmp_typ=string*/ _rune ), tmp16, string) ;
_PUSH(& ctx ->utf_chars , ( /*typ = array_freetype__Character   tmp_typ=freetype__Character*/ ch ), tmp17, freetype__Character) ;
 }
 ;
f32 xpos= x + ((f32)( ch .bearing .x ) ) * 1 ;
f32 ypos= y - ((f32)( ch .size .y - ch .bearing .y ) ) * 1 ;
f32 w= ((f32)( ch .size .x ) ) * 1 ;
f32 h= ((f32)( ch .size .y ) ) * 1 ;
array_f32 vertices=new_array_from_c_array(24, 24, sizeof(f32), EMPTY_ARRAY_OF_ELEMS( f32, 24 ) {  xpos ,  ypos + h ,  0.0 ,  0.0 ,  xpos ,  ypos ,  0.0 ,  1.0 ,  xpos + w ,  ypos ,  1.0 ,  1.0 ,  xpos ,  ypos + h ,  0.0 ,  0.0 ,  xpos + w ,  ypos ,  1.0 ,  1.0 ,  xpos + w ,  ypos + h ,  1.0 ,  0.0  }) ;
 glBindTexture ( GL_TEXTURE_2D ,  ch .texture_id ) ;
 gl__bind_buffer ( GL_ARRAY_BUFFER , ctx ->vbo ) ;
 glBufferData ( GL_ARRAY_BUFFER ,  96 ,  vertices .data ,  GL_DYNAMIC_DRAW ) ;
 gl__draw_arrays ( GL_TRIANGLES , 0 , 6 ) ;
 x  +=  ch .advance  >>  ((u32)( 6 ) ) ;
 }
 ;
 gl__bind_vao ( ((u32)( 0 ) ) ) ;
 glBindTexture ( GL_TEXTURE_2D ,  0 ) ;
 }
 void freetype__FreeType_draw_text_def(freetype__FreeType* ctx, int x, int y, string text) {
gx__TextCfg cfg= (gx__TextCfg) { .color =  gx__Black , .size =  freetype__DEFAULT_FONT_SIZE , .align =  gx__ALIGN_LEFT , .max_width =  0 , .family =  tos3("") , .bold =  0 , .mono =  0 } ;
 freetype__FreeType_draw_text ( ctx ,  x , y , text , cfg ) ;
 }
 void main__main() {
 glfw__init_glfw ( ) ;
Game* game= (Game*)memdup(&(Game)  { .gg =  gg__new_context ( (gg__Cfg) { .width =  main__WinWidth , .height =  main__WinHeight , .use_ortho =  1 , .create_window =  1 , .window_title =  tos3("V Tetris") , .window_user_ptr =  game , .retina =  0 , .resizable =  0 , .font_size =  0 , .font_path =  tos3("") , .always_on_top =  0 , .scale =  0 } ) , .ft =  0 , .score =  0 , .pos_x =  0 , .pos_y =  0 , .field =  new_array(0, 1, sizeof( array_int )) , .tetro =  new_array(0, 1, sizeof( Block )) , .tetros_cache =  new_array(0, 1, sizeof( Block )) , .tetro_idx =  0 , .rotation_idx =  0 , .font_loaded =  0 } , sizeof(Game)) ;
 glfw__Window_set_user_ptr (& /* ? */* game ->gg ->window ,  game ) ;
 Game_init_game ( game ) ;
 glfw__Window_onkeydown ( game ->gg ->window , & /*112 e="void*" g="fn (void*,int,int,int,int)" */ main__key_down ) ;
 thread_arg_Game_run * tmp1 = malloc(sizeof(thread_arg_Game_run));
 tmp1 -> g =  game ;
 pthread_t _thread2;
 int tmp3 = pthread_create(& _thread2, NULL, (void *)Game_run_thread_wrapper,  tmp1);
 ;
 gg__clear ( main__BackgroundColor ) ;
 game ->ft  =  freetype__new_context ( (gg__Cfg) { .width =  main__WinWidth , .height =  main__WinHeight , .use_ortho =  1 , .font_size =  18 , .scale =  2 , .retina =  0 , .resizable =  0 , .font_path =  tos3("") , .create_window =  0 , .window_user_ptr =  0 , .window_title =  tos3("") , .always_on_top =  0 , } ) ;
 game ->font_loaded  =  ( game ->ft != 0 ) ;
 while (1) { 
 gg__clear ( main__BackgroundColor ) ;
 Game_draw_scene ( game ) ;
 gg__GG_render (& /* ? */* game ->gg ) ;
 if ( glfw__Window_should_close (& /* ? */* game ->gg ->window ) ) {
 glfw__Window_destroy (& /* ? */* game ->gg ->window ) ;
 
 return ;
 }
 ;
 }
 ;
 }
 void Game_init_game(Game* g) {
 Game_parse_tetros ( g ) ;
 rand__seed ( time__now ( ) .uni ) ;
 Game_generate_tetro ( g ) ;
 g ->field  = new_array_from_c_array(0, 0, sizeof(array_int), EMPTY_ARRAY_OF_ELEMS( array_int, 0 ) {   TCCSKIP(0) }) ;
 for (
int i= 0  ;  i < main__FieldHeight + 2  ;  i ++ ) { 
 
array_int row= array_repeat (new_array_from_c_array(1, 1, sizeof(int), EMPTY_ARRAY_OF_ELEMS( int, 1 ) {  0  }) ,  main__FieldWidth + 2 ) ;
array_set(&/*q*/ row , 0 , & (int []) {  - 1 }) ;
array_set(&/*q*/ row , main__FieldWidth + 1 , & (int []) {  - 1 }) ;
_PUSH(& g ->field , ( /*typ = array_array_int   tmp_typ=array_int*/ row ), tmp4, array_int) ;
 }
 ;
array_int first_row= ( *(array_int*) array_get(  g ->field , 0) ) ;
array_int last_row= ( *(array_int*) array_get(  g ->field , main__FieldHeight + 1) ) ;
 for (
int j= 0  ;  j < main__FieldWidth + 2  ;  j ++ ) { 
 
array_set(&/*q*/ first_row , j , & (int []) {  - 1 }) ;
array_set(&/*q*/ last_row , j , & (int []) {  - 1 }) ;
 }
 ;
 g ->score  =  0 ;
 g ->state  =  main__GameState_running ;
 }
 void Game_parse_tetros(Game* g) {
 array_array_int tmp9 =  main__BTetros;
 for (int tmp10 = 0; tmp10 < tmp9.len; tmp10++) {
 array_int b_tetros = ((array_int *) tmp9 . data)[tmp10];
 
 array_int tmp11 =  b_tetros;
 for (int tmp12 = 0; tmp12 < tmp11.len; tmp12++) {
 int b_tetro = ((int *) tmp11 . data)[tmp12];
 
 array_Block tmp13 =  main__parse_binary_tetro ( b_tetro );
 for (int tmp14 = 0; tmp14 < tmp13.len; tmp14++) {
 Block t = ((Block *) tmp13 . data)[tmp14];
 
_PUSH(& g ->tetros_cache , ( /*typ = array_Block   tmp_typ=Block*/ t ), tmp15, Block) ;
 }
 ;
 }
 ;
 }
 ;
 }
 void Game_run(Game* g) {
 while (1) { 
 if ( g ->state == main__GameState_running ) {
 Game_move_tetro ( g ) ;
 Game_delete_completed_lines ( g ) ;
 }
 ;
 glfw__post_empty_event ( ) ;
 time__sleep_ms ( main__TimerPeriod ) ;
 }
 ;
 }
 void Game_move_tetro(Game* g) {
 array_Block tmp16 =  g ->tetro;
 for (int tmp17 = 0; tmp17 < tmp16.len; tmp17++) {
 Block block = ((Block *) tmp16 . data)[tmp17];
 
int y= block .y + g ->pos_y + 1 ;
int x= block .x + g ->pos_x ;
array_int row= ( *(array_int*) array_get(  g ->field , y) ) ;
 if ( ( *(int*) array_get(  row , x) ) != 0 ) {
 if ( g ->pos_y < 2 ) {
 g ->state  =  main__GameState_gameover ;
 
 return ;
 }
 ;
 Game_drop_tetro (& /* ? */* g ) ;
 Game_generate_tetro ( g ) ;
 
 return ;
 }
 ;
 }
 ;
 g ->pos_y ++ ;
 }
 bool Game_move_right(Game* g, int dx) {
 for (
int i= 0  ;  i < main__TetroSize  ;  i ++ ) { 
 
Block tetro= ( *(Block*) array_get(  g ->tetro , i) ) ;
int y= tetro .y + g ->pos_y ;
int x= tetro .x + g ->pos_x + dx ;
array_int row= ( *(array_int*) array_get(  g ->field , y) ) ;
 if ( ( *(int*) array_get(  row , x) ) != 0 ) {
return  0 ;
 }
 ;
 }
 ;
 g ->pos_x  +=  dx ;
return  1 ;
 }
 void Game_delete_completed_lines(Game* g) {
 for (
int y= main__FieldHeight  ;  y >= 1  ;  y -- ) { 
 
 Game_delete_completed_line ( g ,  y ) ;
 }
 ;
 }
 void Game_delete_completed_line(Game* g, int y) {
 for (
int x= 1  ;  x <= main__FieldWidth  ;  x ++ ) { 
 
array_int f= ( *(array_int*) array_get(  g ->field , y) ) ;
 if ( ( *(int*) array_get(  f , x) ) == 0 ) {
 
 return ;
 }
 ;
 }
 ;
 g ->score  +=  10 ;
 for (
int yy= y - 1  ;  yy >= 1  ;  yy -- ) { 
 
 for (
int x= 1  ;  x <= main__FieldWidth  ;  x ++ ) { 
 
array_int a= ( *(array_int*) array_get(  g ->field , yy + 1) ) ;
array_int b= ( *(array_int*) array_get(  g ->field , yy) ) ;
array_set(&/*q*/ a , x , & (int []) {  ( *(int*) array_get(  b , x) ) }) ;
 }
 ;
 }
 ;
 }
 void Game_generate_tetro(Game* g) {
 g ->pos_y  =  0 ;
 g ->pos_x  =  main__FieldWidth / 2 - main__TetroSize / 2 ;
 g ->tetro_idx  =  rand__next ( main__BTetros .len ) ;
 g ->rotation_idx  =  0 ;
 Game_get_tetro ( g ) ;
 }
 void Game_get_tetro(Game* g) {
int idx= g ->tetro_idx * main__TetroSize * main__TetroSize + g ->rotation_idx * main__TetroSize ;
 g ->tetro  =   array_slice2(  g ->tetros_cache , idx , idx + main__TetroSize , false)  ;
 }
 void Game_drop_tetro(Game* g) {
 for (
int i= 0  ;  i < main__TetroSize  ;  i ++ ) { 
 
Block tetro= ( *(Block*) array_get(  g ->tetro , i) ) ;
int x= tetro .x + g ->pos_x ;
int y= tetro .y + g ->pos_y ;
array_int row= ( *(array_int*) array_get(  g ->field , y) ) ;
array_set(&/*q*/ row , x , & (int []) {  g ->tetro_idx + 1 }) ;
 }
 ;
 }
 void Game_draw_tetro(Game* g) {
 for (
int i= 0  ;  i < main__TetroSize  ;  i ++ ) { 
 
Block tetro= ( *(Block*) array_get(  g ->tetro , i) ) ;
 Game_draw_block (& /* ? */* g ,  g ->pos_y + tetro .y , g ->pos_x + tetro .x , g ->tetro_idx + 1 ) ;
 }
 ;
 }
 void Game_draw_block(Game* g, int i, int j, int color_idx) {
gx__Color color= (( g ->state == main__GameState_gameover ) ? ( gx__Gray ) : ( ( *(gx__Color*) array_get(  main__Colors , color_idx) ) )) ;
 gg__GG_draw_rect (& /* ? */* g ->gg ,  ( j - 1 ) * main__BlockSize , ( i - 1 ) * main__BlockSize , main__BlockSize - 1 , main__BlockSize - 1 , color ) ;
 }
 void Game_draw_field(Game* g) {
 for (
int i= 1  ;  i < main__FieldHeight + 1  ;  i ++ ) { 
 
 for (
int j= 1  ;  j < main__FieldWidth + 1  ;  j ++ ) { 
 
array_int f= ( *(array_int*) array_get(  g ->field , i) ) ;
 if ( ( *(int*) array_get(  f , j) ) > 0 ) {
 Game_draw_block (& /* ? */* g ,  i , j , ( *(int*) array_get(  f , j) ) ) ;
 }
 ;
 }
 ;
 }
 ;
 }
 void Game_draw_ui(Game* g) {
 if ( g ->font_loaded ) {
 freetype__FreeType_draw_text ( g ->ft ,  1 , 3 , int_str ( g ->score ) , main__text_cfg ) ;
 if ( g ->state == main__GameState_gameover ) {
 gg__GG_draw_rect (& /* ? */* g ->gg ,  0 , main__WinHeight / 2 - main__TextSize , main__WinWidth , 5 * main__TextSize , main__UIColor ) ;
 freetype__FreeType_draw_text ( g ->ft ,  1 , main__WinHeight / 2 + 0 * main__TextSize , tos3("Game Over") , main__over_cfg ) ;
 freetype__FreeType_draw_text ( g ->ft ,  1 , main__WinHeight / 2 + 2 * main__TextSize , tos3("Space to restart") , main__over_cfg ) ;
 }
  else  if ( g ->state == main__GameState_paused ) {
 gg__GG_draw_rect (& /* ? */* g ->gg ,  0 , main__WinHeight / 2 - main__TextSize , main__WinWidth , 5 * main__TextSize , main__UIColor ) ;
 freetype__FreeType_draw_text ( g ->ft ,  1 , main__WinHeight / 2 + 0 * main__TextSize , tos3("Game Paused") , main__text_cfg ) ;
 freetype__FreeType_draw_text ( g ->ft ,  1 , main__WinHeight / 2 + 2 * main__TextSize , tos3("SPACE to resume") , main__text_cfg ) ;
 }
 ;
 }
 ;
 }
 void Game_draw_scene(Game* g) {
 Game_draw_tetro (& /* ? */* g ) ;
 Game_draw_field (& /* ? */* g ) ;
 Game_draw_ui ( g ) ;
 }
 array_Block main__parse_binary_tetro(int t_) {
int t= t_ ;
array_Block res= array_repeat (new_array_from_c_array(1, 1, sizeof(Block), EMPTY_ARRAY_OF_ELEMS( Block, 1 ) {  (Block) { .x =  0 , .y =  0 }  }) ,  4 ) ;
int cnt= 0 ;
bool horizontal= t == 9 ;
 for (
int i= 0  ;  i <= 3  ;  i ++ ) { 
 
int p= ((int)( math__pow ( 10 , 3 - i ) ) ) ;
int digit= ((int)( t / p ) ) ;
 t  %=  p ;
 for (
int j= 3  ;  j >= 0  ;  j -- ) { 
 
int bin= digit % 2 ;
 digit  /=  2 ;
 if ( bin == 1  ||  ( horizontal  &&  i == main__TetroSize - 1 ) ) {
Block* point= & ( *(Block*) array_get(  res , cnt) ) ;
 point ->x  =  j ;
 point ->y  =  i ;
 cnt ++ ;
 }
 ;
 }
 ;
 }
 ;
return  res ;
 }
 void main__key_down(void* wnd, int key, int code, int action, int mods) {
 if ( action != 2  &&  action != 1 ) {
 
 return ;
 }
 ;
Game* game= ((Game*)( glfw__get_window_user_pointer ( wnd ) ) ) ; 
 int tmp56 =  key; 

 if ( tmp56 ==  glfw__KEY_ESCAPE ) { 
 glfw__set_should_close ( wnd , 1 ) ;
 }
 else  if ( tmp56 ==  glfw__key_space ) { 
 if ( game ->state == main__GameState_running ) {
 game ->state  =  main__GameState_paused ;
 }
  else  if ( game ->state == main__GameState_paused ) {
 game ->state  =  main__GameState_running ;
 }
  else  if ( game ->state == main__GameState_gameover ) {
 Game_init_game ( game ) ;
 game ->state  =  main__GameState_running ;
 }
 ;
 }
 ;
 if ( game ->state != main__GameState_running ) {
 
 return ;
 }
 ; 
 int tmp57 =  key; 

 if ( tmp57 ==  glfw__KeyUp ) { 
int old_rotation_idx= game ->rotation_idx ;
 game ->rotation_idx ++ ;
 if ( game ->rotation_idx == main__TetroSize ) {
 game ->rotation_idx  =  0 ;
 }
 ;
 Game_get_tetro ( game ) ;
 if ( ! Game_move_right ( game ,  0 ) ) {
 game ->rotation_idx  =  old_rotation_idx ;
 Game_get_tetro ( game ) ;
 }
 ;
 if ( game ->pos_x < 0 ) {
 }
 ;
 }
 else  if ( tmp57 ==  glfw__KeyLeft ) { 
 Game_move_right ( game ,  - 1 ) ;
 }
 else  if ( tmp57 ==  glfw__KeyRight ) { 
 Game_move_right ( game ,  1 ) ;
 }
 else  if ( tmp57 ==  glfw__KeyDown ) { 
 Game_move_tetro ( game ) ;
 }
 ;
 }
 void init() {
g_str_buf=malloc(1000);

builtin__min_cap =  2  <<  10;
builtin__max_cap =  2  <<  20;
gx__Blue =  (gx__Color) { .r =  0 , .g =  0 , .b =  255 };
gx__Red =  (gx__Color) { .r =  255 , .g =  0 , .b =  0 };
gx__Green =  (gx__Color) { .r =  0 , .g =  255 , .b =  0 };
gx__green =  (gx__Color) { .r =  0 , .g =  255 , .b =  0 };
gx__Yellow =  (gx__Color) { .r =  255 , .g =  255 , .b =  0 };
gx__Orange =  (gx__Color) { .r =  255 , .g =  165 , .b =  0 };
gx__Purple =  (gx__Color) { .r =  128 , .g =  0 , .b =  128 };
gx__Black =  (gx__Color) { .r =  0 , .g =  0 , .b =  0 };
gx__Gray =  (gx__Color) { .r =  128 , .g =  128 , .b =  128 };
gx__Indigo =  (gx__Color) { .r =  75 , .g =  0 , .b =  130 };
gx__Pink =  (gx__Color) { .r =  255 , .g =  192 , .b =  203 };
gx__Violet =  (gx__Color) { .r =  238 , .g =  130 , .b =  238 };
gx__White =  (gx__Color) { .r =  255 , .g =  255 , .b =  255 };
gx__white =  (gx__Color) { .r =  255 , .g =  255 , .b =  255 };
gx__DarkBlue =  (gx__Color) { .r =  0 , .g =  0 , .b =  139 };
gx__DarkGray =  (gx__Color) { .r =  169 , .g =  169 , .b =  169 };
gx__DarkGreen =  (gx__Color) { .r =  0 , .g =  100 , .b =  0 };
gx__DarkRed =  (gx__Color) { .r =  139 , .g =  0 , .b =  0 };
gx__LightBlue =  (gx__Color) { .r =  173 , .g =  216 , .b =  230 };
gx__LightGray =  (gx__Color) { .r =  211 , .g =  211 , .b =  211 };
gx__LightGreen =  (gx__Color) { .r =  144 , .g =  238 , .b =  144 };
gx__LightRed =  (gx__Color) { .r =  255 , .g =  204 , .b =  203 };
gl__TEXT_VERT =  tos3("#version 330 core\nlayout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>\nout vec2 TexCoords;\n\nuniform mat4 projection;\n\nvoid main()\n{\n    gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);\n    TexCoords = vertex.zw;\n}  ");
gl__TEXT_FRAG =  tos3("#version 330 core\nin vec2 TexCoords;\nout vec4 color;\n\nuniform sampler2D text;\nuniform vec3 textColor;\n\nvoid main()\n{\n    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);\n    color = vec4(textColor, 1.0) * sampled;\n}  ");
gl__SIMPLE_VERT =  tos3(" #version 330 core\n\nlayout (location = 0) in vec3 aPos;\nlayout (location = 1) in vec3 aColor;\nlayout (location = 2) in vec2 aTexCoord;\n\n\nout vec3 ourColor;\nout vec2 TexCoord;\n\nuniform mat4 projection;\n\nvoid main() {\n    gl_Position = projection *  vec4(aPos, 1.0);\n//    gl_Position = vec4(aPos, 1.0);\n\n ourColor = aColor;\n//TexCoord = vec2(aTexCoord.x, aTexCoord.y);\n    TexCoord = aTexCoord;\n}\n");
gl__SIMPLE_FRAG =  tos3("#version 330 core\n\nout vec4 FragColor;\nuniform vec3 color;\n\nuniform bool has_texture;\n\nin vec3 ourColor;\nin vec2 TexCoord;\n\nuniform sampler2D ourTexture;\n\n\nvoid main()     {\n//    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n//    FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);\nif (has_texture) {\n    FragColor = texture(ourTexture, TexCoord);\n\n}  else {\n    FragColor = vec4(color, 1.0f);\n}\n}\n");
strconv__max_u64 =  ((u64)( UINT64_MAX ) );
math__uvnan =  0x7FF8000000000001;
math__uvinf =  0x7FF0000000000000;
math__uvneginf =  0xFFF0000000000000;
math__uvone =  0x3FF0000000000000;
math__mask =  0x7FF;
math__shift =  64 - 11 - 1;
math__sign_mask =  ((u64)( ((u64)( 1 ) )  <<  63 ) );
math__frac_mask =  ((u64)( ((u64)( ((u64)( 1 ) )  <<  ((u64)( math__shift ) ) ) ) - ((u64)( 1 ) ) ) );
math__log2_e =  1.0 / math__ln2;
math__log10_e =  1.0 / math__ln10;
math__max_f32 =  3.40282346638528859811704183484516925440e+38;
math__smallest_non_zero_f32 =  1.401298464324817070923729583289916131280e-45;
math__max_f64 =  1.797693134862315708145274237317043567981e+308;
math__smallest_non_zero_f64 =  4.940656458412465441765687928682213723651e-324;
math__min_i8 =  - 128;
math__min_i16 =  - 32768;
math__min_i32 =  - 2147483648;
math__min_i64 =  - 9223372036854775807 - 1;
os__S_IFMT =  0xF000;
os__S_IFDIR =  0x4000;
os__STD_INPUT_HANDLE =  - 10;
os__STD_OUTPUT_HANDLE =  - 11;
os__STD_ERROR_HANDLE =  - 12;
os__args = new_array_from_c_array(0, 0, sizeof(string), EMPTY_ARRAY_OF_ELEMS( string, 0 ) {   TCCSKIP(0) });
os__MAP_PRIVATE =  0x02;
os__MAP_ANONYMOUS =  0x20;
os__path_separator =  tos3("/");
time__days_string =  tos3("MonTueWedThuFriSatSun");
time__month_days = new_array_from_c_array(12, 12, sizeof(int), EMPTY_ARRAY_OF_ELEMS( int, 12 ) {  31 ,  28 ,  31 ,  30 ,  31 ,  30 ,  31 ,  31 ,  30 ,  31 ,  30 ,  31  });
time__months_string =  tos3("JanFebMarAprMayJunJulAugSepOctNovDec");
time__absolute_zero_year =  ((i64)( - 292277022399 ) );
time__seconds_per_hour =  60 * time__seconds_per_minute;
time__seconds_per_day =  24 * time__seconds_per_hour;
time__seconds_per_week =  7 * time__seconds_per_day;
time__days_per_400_years =  365 * 400 + 97;
time__days_per_100_years =  365 * 100 + 24;
time__days_per_4_years =  365 * 4 + 1;
time__days_before = new_array_from_c_array(13, 13, sizeof(int), EMPTY_ARRAY_OF_ELEMS( int, 13 ) {  0 ,  31 ,  31 + 28 ,  31 + 28 + 31 ,  31 + 28 + 31 + 30 ,  31 + 28 + 31 + 30 + 31 ,  31 + 28 + 31 + 30 + 31 + 30 ,  31 + 28 + 31 + 30 + 31 + 30 + 31 ,  31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 ,  31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 ,  31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 ,  31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30 ,  31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30 + 31 ,   });
main__WinWidth =  main__BlockSize * main__FieldWidth;
main__WinHeight =  main__BlockSize * main__FieldHeight;
main__text_cfg =  (gx__TextCfg) { .align =  gx__ALIGN_LEFT , .size =  main__TextSize , .color =  gx__rgb ( 0 , 0 , 0 ) , .max_width =  0 , .family =  tos3("") , .bold =  0 , .mono =  0 };
main__over_cfg =  (gx__TextCfg) { .align =  gx__ALIGN_LEFT , .size =  main__TextSize , .color =  gx__White , .max_width =  0 , .family =  tos3("") , .bold =  0 , .mono =  0 };
main__BTetros = new_array_from_c_array(7, 7, sizeof(array_int), EMPTY_ARRAY_OF_ELEMS( array_int, 7 ) { new_array_from_c_array(4, 4, sizeof(int), EMPTY_ARRAY_OF_ELEMS( int, 4 ) {  66 ,  66 ,  66 ,  66  }) , new_array_from_c_array(4, 4, sizeof(int), EMPTY_ARRAY_OF_ELEMS( int, 4 ) {  27 ,  131 ,  72 ,  232  }) , new_array_from_c_array(4, 4, sizeof(int), EMPTY_ARRAY_OF_ELEMS( int, 4 ) {  36 ,  231 ,  36 ,  231  }) , new_array_from_c_array(4, 4, sizeof(int), EMPTY_ARRAY_OF_ELEMS( int, 4 ) {  63 ,  132 ,  63 ,  132  }) , new_array_from_c_array(4, 4, sizeof(int), EMPTY_ARRAY_OF_ELEMS( int, 4 ) {  311 ,  17 ,  223 ,  74  }) , new_array_from_c_array(4, 4, sizeof(int), EMPTY_ARRAY_OF_ELEMS( int, 4 ) {  322 ,  71 ,  113 ,  47  }) , new_array_from_c_array(4, 4, sizeof(int), EMPTY_ARRAY_OF_ELEMS( int, 4 ) {  1111 ,  9 ,  1111 ,  9  }) ,   });
main__Colors = new_array_from_c_array(9, 9, sizeof(gx__Color), EMPTY_ARRAY_OF_ELEMS( gx__Color, 9 ) {  gx__rgb ( 0 , 0 , 0 ) ,  gx__rgb ( 253 , 32 , 47 ) ,  gx__rgb ( 0 , 110 , 194 ) ,  gx__rgb ( 170 , 170 , 0 ) ,  gx__rgb ( 170 , 0 , 170 ) ,  gx__rgb ( 50 , 90 , 110 ) ,  gx__rgb ( 0 , 170 , 0 ) ,  gx__rgb ( 170 , 85 , 0 ) ,  gx__rgb ( 0 , 170 , 170 ) ,   });
main__BackgroundColor =  gx__White;
main__UIColor =  gx__Red;
builtin__init();

}
 
string _STR(const char *fmt, ...) {
	va_list argptr;
	va_start(argptr, fmt);
	size_t len = vsnprintf(0, 0, fmt, argptr) + 1;
	va_end(argptr);
	byte* buf = malloc(len);
	va_start(argptr, fmt);
	vsprintf((char *)buf, fmt, argptr);
	va_end(argptr);
#ifdef DEBUG_ALLOC
	puts("_STR:");
	puts(buf);
#endif
	return tos2(buf);
}

string _STR_TMP(const char *fmt, ...) {
	va_list argptr;
	va_start(argptr, fmt);
	//size_t len = vsnprintf(0, 0, fmt, argptr) + 1;
	va_end(argptr);
	va_start(argptr, fmt);
	vsprintf((char *)g_str_buf, fmt, argptr);
	va_end(argptr);
#ifdef DEBUG_ALLOC
	//puts("_STR_TMP:");
	//puts(g_str_buf);
#endif
	return tos2(g_str_buf);
}


 int main(int argc, char** argv) { 
   init();
   os__args = os__init_os_args(argc, (byteptr*)argv);
 
   main__main();
 free(g_str_buf);
 
   return 0;
 }
 bool array_eq_T_int(array_int a1, array_int a2) {
 if ( a1 .len != a2 .len ) {
return  0 ;
 }
 ;
 for (
int i= 0  ;  i < a1 .len  ;  i ++ ) { 
 
 if ( ( *(int*) array_get(  a1 , i) ) != ( *(int*) array_get(  a2 , i) ) ) {
return  0 ;
 }
 ;
 }
 ;
return  1 ;
 }
 bool array_eq_T_i64(array_i64 a1, array_i64 a2) {
 if ( a1 .len != a2 .len ) {
return  0 ;
 }
 ;
 for (
int i= 0  ;  i < a1 .len  ;  i ++ ) { 
 
 if ( ( *(i64*) array_get(  a1 , i) ) != ( *(i64*) array_get(  a2 , i) ) ) {
return  0 ;
 }
 ;
 }
 ;
return  1 ;
 }
 bool array_eq_T_string(array_string a1, array_string a2) {
 if ( a1 .len != a2 .len ) {
return  0 ;
 }
 ;
 for (
int i= 0  ;  i < a1 .len  ;  i ++ ) { 
 
 if (string_ne( ( *(string*) array_get(  a1 , i) ) , ( *(string*) array_get(  a2 , i) ) ) ) {
return  0 ;
 }
 ;
 }
 ;
return  1 ;
 }
 bool array_eq_T_byte(array_byte a1, array_byte a2) {
 if ( a1 .len != a2 .len ) {
return  0 ;
 }
 ;
 for (
int i= 0  ;  i < a1 .len  ;  i ++ ) { 
 
 if ( ( *(byte*) array_get(  a1 , i) ) != ( *(byte*) array_get(  a2 , i) ) ) {
return  0 ;
 }
 ;
 }
 ;
return  1 ;
 }
 bool array_eq_T_f32(array_f32 a1, array_f32 a2) {
 if ( a1 .len != a2 .len ) {
return  0 ;
 }
 ;
 for (
int i= 0  ;  i < a1 .len  ;  i ++ ) { 
 
 if (f32_ne( ( *(f32*) array_get(  a1 , i) ) , ( *(f32*) array_get(  a2 , i) ) ) ) {
return  0 ;
 }
 ;
 }
 ;
return  1 ;
 }

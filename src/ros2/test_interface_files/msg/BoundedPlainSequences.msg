# Bounded sequences of different POD types
bool[<=3] bool_values
byte[<=3] byte_values
char[<=3] char_values
float32[<=3] float32_values
float64[<=3] float64_values
int8[<=3] int8_values
uint8[<=3] uint8_values
int16[<=3] int16_values
uint16[<=3] uint16_values
int32[<=3] int32_values
uint32[<=3] uint32_values
int64[<=3] int64_values
uint64[<=3] uint64_values
BasicTypes[<=3] basic_types_values
Constants[<=3] constants_values
Defaults[<=3] defaults_values
bool[<=3] bool_values_default [false, true, false]
byte[<=3] byte_values_default [0, 1, 255]
char[<=3] char_values_default [0, 1, 127]
float32[<=3] float32_values_default [1.125, 0.0, -1.125]
float64[<=3] float64_values_default [3.1415, 0.0, -3.1415]
int8[<=3] int8_values_default [0, 127, -128]
uint8[<=3] uint8_values_default [0, 1, 255]
int16[<=3] int16_values_default [0, 32767, -32768]
uint16[<=3] uint16_values_default [0, 1, 65535]
int32[<=3] int32_values_default [0, 2147483647, -2147483648]
uint32[<=3] uint32_values_default [0, 1, 4294967295]
int64[<=3] int64_values_default [0, 9223372036854775807, -9223372036854775808]
uint64[<=3] uint64_values_default [0, 1, 18446744073709551615]
# Regression test: check alignment of basic field after a sequence field is correct
int32 alignment_check

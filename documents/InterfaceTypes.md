# IDL Mapping to basic data

|OMG IDL             |  C++            | Default value|
|:-------------------|:----------------|:--------|
|short               |  int16_t        |  0      |
|long                |  int32_t        |  0      |
|long long           |  int64_t        |  0      |
|unsigned short      |  uint16_t       |  0      |
|unsigned long       |  uint32_t       |  0      |
|unsigned long long  |  uint64_t       |  0      |
|float               |  float          |  0.0    |
|double              |  double         |  0.0    |
|long double         |  long double    |  0.0    |
|char                |  char           |  0      |
|wchar               |  wchar_t        |  0      |
|boolean             |  bool           |  FALSE  |
|octet               |  uint8_t        |  0      |

## Python types

|ctypes         |C type              |Python type              |info|
|:--------------|:-------------------|:------------------------|:-|
|c_bool         |Bool                |bool (1)                 ||
|c_char         |char                |1-character bytes object ||
|c_wchar        |wchar_t             |1-character string       ||
|c_byte         |char                |int                      ||
|c_ubyte        |unsigned char       |int                      ||
|c_short        |short               |int                      ||
|c_ushort       |unsigned short      |int                      ||
|c_int          |int                 |int                      ||
|c_uint         |unsigned int        |int                      ||
|c_long         |long                |int                      ||
|c_ulong        |unsigned long       |int                      ||
|c_longlong     |__int64             |int                      ||
|c_longlong     |long long           |int                      ||
|c_ulonglong    |unsigned __int64    |int                      ||
|c_ulonglong    |unsigned long long  |int                      ||
|c_size_t       |size_t              |int                      ||
|c_ssize_t      |ssize_t             |int                      ||
|c_ssize_t      |Py_ssize_t          |int                      ||
|c_time_t       |time_t              |int                      ||
|c_float        |float               |float                    ||
|c_double       |double              |float                    ||
|c_longdouble   |long double         |float                    ||
|c_char_p       |char*               |bytes object or None     |(NULL terminated)|
|c_wchar_p      |wchar_t*            |string or None           |(NULL terminated)|
|c_void_p       |void*               |int or None              ||


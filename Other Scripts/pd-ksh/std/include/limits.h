/* Implementation-defined limits */

#if __STDC__
#define	Signed	signed
#else
#define	Signed	
#endif

#define	CHAR_BIT	8

#define	_S_MIN(type)	(-(Signed type)((unsigned type) ~0 >> 1) - 1)
#define	_S_MAX(type)	((Signed type)((unsigned type) ~0 >> 1))

#define	UCHAR_MIN	((unsigned char) 0)
#define	UCHAR_MAX	((unsigned char) ~0)
#define	SCHAR_MIN	_S_MIN(char)
#define	SCHAR_MAX	_S_MAX(char)

#define	_U_CHAR		((char) ~0 == (unsigned char) ~0)
#define	CHAR_MIN	(_U_CHAR ? UCHAR_MIN : SCHAR_MIN)
#define	CHAR_MAX	(_U_CHAR ? UCHAR_MAX : SCHAR_MAX)

#define	USHRT_MAX	((unsigned short) ~0)
#define	SHRT_MIN	_S_MIN(short)
#define	SHRT_MAX	_S_MAX(short)

#define	UINT_MAX	((unsigned int) ~0)
#define	INT_MIN		_S_MIN(int)
#define	INT_MAX		_S_MAX(int)

#define	ULONG_MAX	((unsigned long) ~0)
#define	LONG_MIN	_S_MIN(long)
#define	LONG_MAX	_S_MAX(long)


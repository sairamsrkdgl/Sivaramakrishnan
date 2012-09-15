#if ! _STDARGS_H
#define	_STDARGS_H 1

typedef void *va_list;
#define va_start(va, anchor) va = (void *) &(&anchor)[1]
#define va_end(va)
#define va_arg(va, mode) ((mode *)(va += sizeof(mode)))[-1]

#endif

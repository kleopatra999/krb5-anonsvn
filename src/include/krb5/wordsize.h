/*
 * $Source$
 * $Author$
 * $Id$
 *
 * Copyright 1989 by the Massachusetts Institute of Technology.
 *
 * For copying and distribution information, please see the file
 * <krb5/mit-copyright.h>.
 *
 * Word-size related definition.
 */

#include <krb5/copyright.h>

#ifndef __KRB5_WORDSIZE__
#define __KRB5_WORDSIZE__

#ifdef BITS16
#define __OK
typedef	int	krb5_int16;
typedef	long	krb5_int32;
typedef	unsigned char	krb5_octet;
#endif

#ifdef BITS32
#define __OK
typedef	short	krb5_int16;
typedef	int	krb5_int32;
typedef	unsigned char	krb5_octet;
#endif

#ifndef __OK
 Error:  must define word size!
#endif /* __OK */

#undef __OK

#define KRB5_INT32_MAX	2147483647
/* this strange form is necessary since - is a unary operator, not a sign
   indicator */
#define KRB5_INT32_MIN	(-KRB5_INT32_MAX-1)

#endif /* __KRB5_WORDSIZE__ */

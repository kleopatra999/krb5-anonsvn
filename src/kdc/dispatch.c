/*
 * $Source$
 * $Author$
 *
 * Copyright 1990 by the Massachusetts Institute of Technology.
 *
 * Export of this software from the United States of America is assumed
 *   to require a specific license from the United States Government.
 *   It is the responsibility of any person or organization contemplating
 *   export to obtain such a license before exporting.
 * 
 * WITHIN THAT CONSTRAINT, permission to use, copy, modify, and
 * distribute this software and its documentation for any purpose and
 * without fee is hereby granted, provided that the above copyright
 * notice appear in all copies and that both that copyright notice and
 * this permission notice appear in supporting documentation, and that
 * the name of M.I.T. not be used in advertising or publicity pertaining
 * to distribution of the software without specific, written prior
 * permission.  M.I.T. makes no representations about the suitability of
 * this software for any purpose.  It is provided "as is" without express
 * or implied warranty.
 * 
 *
 * Dispatch an incoming packet.
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_dispatch_c[] =
"$Id$";
#endif	/* !lint & !SABER */

#include <krb5/copyright.h>
#include <krb5/krb5.h>
#include <krb5/asn1.h>
#include <krb5/kdb.h>
#include <syslog.h>
#include "kdc_util.h"

krb5_error_code
dispatch(pkt, from, is_secondary, response)
    krb5_data *pkt;
    const krb5_fulladdr *from;
    int		is_secondary;
    krb5_data **response;
{

    krb5_error_code retval;
    krb5_kdc_req *as_req;

    /* decode incoming packet, and dispatch */

    /* try the replay lookaside buffer */
    if (kdc_check_lookaside(pkt, response)) {
	/* a hit! */
	syslog(LOG_INFO, "DISPATCH: replay found and re-transmitted");
	return 0;
    }
    /* try TGS_REQ first; they are more common! */

    if (krb5_is_tgs_req(pkt)) {
	retval = process_tgs_req(pkt, from, is_secondary, response);
    } else if (krb5_is_as_req(pkt)) {
	if (!(retval = decode_krb5_as_req(pkt, &as_req))) {
	    retval = process_as_req(as_req, from, is_secondary, response);
	    krb5_free_kdc_req(as_req);
	}
    }
#ifdef KRB4
    else if (pkt->data[0] == 4)		/* old version */
	retval = process_v4(pkt, from, is_secondary, response);
#endif
    else
	retval = KRB5KRB_AP_ERR_MSG_TYPE;
    /* put the response into the lookaside buffer */
    if (!retval)
	kdc_insert_lookaside(pkt, *response);

    return retval;
}

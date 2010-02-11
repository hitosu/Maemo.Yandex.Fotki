/* 
* This file is part of libsharing
*
* Copyright © 2008-2009 Nokia Corporation and/or its subsidiary(-ies). All rights reserved.
*
* Contact: Alexander Bokovoy <alexander.bokovoy@nokia.com>
*
* This software, including documentation, is protected by copyright controlled by
* Nokia Corporation. All rights are reserved. Copying, including reproducing, storing,
* adapting or translating, any or all of this material requires the prior written consent
* of Nokia Corporation. This material also contains confidential information which may
* not be disclosed to others without the prior written consent of Nokia. 
*/

/**
 * SECTION:sharing-service-option
 * @short_description: Object is simple id name pair used with #SharingAccount.
 *
 * #SharingServiceOptionValue is abstraction of sharing account specific option values.
 */
 
#ifndef _SHARING_SERVICE_OPTION_H_
#define _SHARING_SERVICE_OPTION_H_

#include <glib.h>

G_BEGIN_DECLS

typedef struct SharingServiceOptionValue SharingServiceOptionValue;

SharingServiceOptionValue * sharing_service_option_value_new (const gchar * id,
    const gchar * name, const gchar * description);
void sharing_service_option_value_free (SharingServiceOptionValue * value);
void sharing_service_option_values_free (GSList * values);
    
G_END_DECLS

#endif /* #ifndef _SHARING_SERVICE_OPTION_H_ */

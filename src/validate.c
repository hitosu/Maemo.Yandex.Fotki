/*
 * This file is part of sharing-plugin-template
 *
 * Copyright (C) 2008-2009 Nokia Corporation. All rights reserved.
 *
 * This maemo code example is licensed under a MIT-style license,
 * that can be found in the file called "COPYING" in the root
 * directory.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include <sharing-account.h>
#include <osso-log.h>
#include <conicconnection.h>
#include "validate.h"
#include "common.h"

/**
 * test:
 * @account: #SharingAccount to be tested
 * @con: Connection used
 * @dead_mans_switch: Turn to %FALSE at least every 30 seconds.
 *
 * Test if #SharingAccount is valid.
 *
 * Returns: #SharingPluginInterfaceTestAccountResult
 */
SharingPluginInterfaceAccountValidateResult validate (SharingAccount* account,
    ConIcConnection* con, gboolean *cont, gboolean* dead_mans_switch)
{
    SharingPluginInterfaceAccountValidateResult ret = SHARING_ACCOUNT_VALIDATE_FAILED;

    char* sessionKey = NULL;
    char* sessionRequestId = NULL;
    char* token = NULL;

    if (yandexGetSessionKey(&sessionKey, &sessionRequestId) == YANDEX_GET_SESSION_KEY_SUCCESS) {
    	yandexGetAuthTokenResult res =
    		yandexGetAuthToken(sessionRequestId, sessionKey,
							   sharing_account_get_username(account), sharing_account_get_password(account),
							   &token);
    	switch (res) {
    	case YANDEX_GET_AUTH_TOKEN_SUCCESS:
    		ret = SHARING_ACCOUNT_VALIDATE_SUCCESS;
    		break;
    	case YANDEX_GET_AUTH_TOKEN_FAILED:
    		ret = SHARING_ACCOUNT_VALIDATE_ERROR_UNKNOWN;
    		break;
    	case YANDEX_GET_AUTH_TOKEN_INVALID_USER:
    		ret = SHARING_ACCOUNT_VALIDATE_FAILED;
    		break;
    	}
    }

    if (token) free(token);
    if (sessionKey) free(sessionKey);
    if (sessionRequestId) free(sessionRequestId);

    return ret;
}


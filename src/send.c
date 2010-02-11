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
#include <string.h>
#include <stdlib.h>
#include <glib.h>
#include <osso-log.h>
#include "send.h"
#include "common.h"

/**
 * send:
 * @account: #SharingTransfer to be send
 * @con: Connection used
 * @dead_mans_switch: Turn to %FALSE at least every 30 seconds.
 *
 * Sends #SharingTransfer to service.
 *
 * Returns: #SharingPluginInterfaceSendResult
 */
SharingPluginInterfaceSendResult share_item (SharingTransfer* transfer,
    ConIcConnection* con, gboolean* dead_mans_switch)
{
    SharingPluginInterfaceSendResult ret = SHARING_SEND_ERROR_UNKNOWN;
    SharingEntry *entry = sharing_transfer_get_entry( transfer );

    yandexPhotoAccessType access = YANDEX_PHOTO_ACCESS_PUBLIC;
    const gchar* privacyOption = sharing_entry_get_option(entry,"privacy");
    if (privacyOption) {
    	if (strcmp(privacyOption,"friends")==0) access = YANDEX_PHOTO_ACCESS_FRIENDS;
    	else if (strcmp(privacyOption,"private")==0) access = YANDEX_PHOTO_ACCESS_PRIVATE;
    }
    yandexPhotoPublishSettings publish = YANDEX_PHOTO_DONT_PUBLISH;
    const gchar* publishOption = sharing_entry_get_option(entry,"publish");
    if (publishOption) {
    	if (strcmp(publishOption,"yes")==0) publish = YANDEX_PHOTO_PUBLISH;
    }
    const gchar* album = sharing_entry_get_option(entry,"album");
    if (strcmp(album,"0")==0) album = NULL;

    SharingAccount* account = sharing_entry_get_account(entry);
    char* sessionKey = NULL;
    char* sessionRequestId = NULL;
    char* token = NULL;

    yandexGetAuthTokenResult token_res = YANDEX_GET_AUTH_TOKEN_FAILED;
    *dead_mans_switch = FALSE;
    if (yandexGetSessionKey(&sessionKey, &sessionRequestId) == YANDEX_GET_SESSION_KEY_SUCCESS) {
    	*dead_mans_switch = FALSE;
    	token_res = yandexGetAuthToken(sessionRequestId, sessionKey,
							   sharing_account_get_username(account), sharing_account_get_password(account),
							   &token);
    }
    *dead_mans_switch = FALSE;

    if (YANDEX_GET_AUTH_TOKEN_SUCCESS == token_res && token != NULL) {
		GSList* p;
		ret = SHARING_SEND_SUCCESS;
		for (p = sharing_entry_get_media (entry); p != NULL; p = g_slist_next(p)) {
			*dead_mans_switch = FALSE;
			if (!sharing_transfer_continue(transfer)) break;
			SharingEntryMedia* media = p->data;
			if (!sharing_entry_media_get_sent (media)) {
				yandexPhotoOptions options;
				options.album = album;
				options.access = access;
				yandexSendPhotoResult send_res = yandexSendPhoto(token,media,transfer,options);
				if (YANDEX_SEND_PHOTO_SUCCESS == send_res) sharing_entry_media_set_sent(media,TRUE);
				else ret = SHARING_SEND_ERROR_AUTH;
			}
		}
		if (!sharing_transfer_continue(transfer)) ret = SHARING_SEND_CANCELLED;
    } else if (YANDEX_GET_AUTH_TOKEN_INVALID_USER == token_res) {
    	ret = SHARING_SEND_ERROR_AUTH;
    }

    if (token) free(token);
    if (sessionKey) free(sessionKey);
    if (sessionRequestId) free(sessionRequestId);

    return ret;
}


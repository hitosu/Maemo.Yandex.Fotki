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

#include <gtk/gtk.h>
#include <glib.h>

// Somehow garage builder doesn't have normal libsharing headers
#include "libsharing/sharing-plugin-interface.h"
#include "libsharing/sharing-transfer.h"
#include "libsharing/sharing-service-option.h"

#include <conicconnection.h>
#include <osso-log.h>
#include <stdlib.h>

#include "send.h"
#include "validate.h"
#include "common.h"

guint sharing_plugin_interface_init (gboolean* dead_mans_switch);
guint sharing_plugin_interface_uninit (gboolean* dead_mans_switch);
SharingPluginInterfaceSendResult sharing_plugin_interface_send(SharingTransfer* transfer, ConIcConnection* con, gboolean* dead_mans_switch);
SharingPluginInterfaceAccountSetupResult sharing_plugin_interface_account_setup(GtkWindow* parent, SharingService* service, SharingAccount** worked_on,osso_context_t* osso);
SharingPluginInterfaceAccountValidateResult sharing_plugin_interface_account_validate (SharingAccount* account, ConIcConnection* con, gboolean *cont, gboolean* dead_mans_switch);
SharingPluginInterfaceEditAccountResult sharing_plugin_interface_edit_account (GtkWindow* parent, SharingAccount* account, ConIcConnection* con, gboolean* dead_mans_switch);
gboolean sharing_plugin_interface_update_options(SharingAccount* account, ConIcConnection* con, gboolean* cont, gboolean* dead_mans_switch, UpdateOptionsCallback cb_func, gpointer cb_data);

/**
 * sharing_plugin_interface_init:
 * @dead_mans_switch: What?
 *
 * Initialize interface
 *
 * Returns: 0
 */
guint sharing_plugin_interface_init (gboolean* dead_mans_switch)
{
    ULOG_DEBUG_L("sharing_manager_plugin_interface_init");
    return 0;
}

/**
 * sharing_plugin_interface_uninit:
 * @dead_mans_switch: What?
 *
 * Uninitialize interface
 *
 * Returns: 0
 */
guint sharing_plugin_interface_uninit (gboolean* dead_mans_switch)
{
    ULOG_DEBUG_L("sharing_manager_plugin_interface_uninit");
    return 0;
}

/**
 * sharing_plugin_interface_send:
 * @transfer: Transfer to be send
 * @con: Connection used
 * @dead_mans_switch:
 *
 * Send interface.
 *
 * Returns: Result of send
 */
SharingPluginInterfaceSendResult sharing_plugin_interface_send
    (SharingTransfer* transfer, ConIcConnection* con,
    gboolean* dead_mans_switch)
{
    ULOG_DEBUG_L ("sharing_plugin_interface_send");
    SharingPluginInterfaceSendResult ret_val = SHARING_SEND_ERROR_UNKNOWN;
    ret_val = share_item (transfer, con, dead_mans_switch);
    return ret_val;
}

/**
 * sharing_plugin_interface_account_setup:
 * @transfer: #SharingTransfer send
 * @service: #SharingService
 * @worked_on: Connection used
 * @osso_context_t: Osso context
 *
 * Send interface
 *
 * Returns: Result of account setup
 */
SharingPluginInterfaceAccountSetupResult sharing_plugin_interface_account_setup
    (GtkWindow* parent, SharingService* service, SharingAccount** worked_on,
    osso_context_t* osso)
{
    ULOG_DEBUG_L ("sharing_plugin_interface_account_setup");
    SharingPluginInterfaceAccountSetupResult ret = 0;
    return ret;
}

/**
 * sharing_manager_plugin_interface_account_validate:
 * @account: Account tested
 * @con: Connection used to test account
 * @dead_mans_switch:
 *
 * Validates account information.
 *
 * Returns: Result of account validation
 */
SharingPluginInterfaceAccountValidateResult
sharing_plugin_interface_account_validate (SharingAccount* account,
    ConIcConnection* con, gboolean *cont, gboolean* dead_mans_switch)
{
    ULOG_DEBUG_L ("sharing_plugin_interface_account_validate");
    SharingPluginInterfaceAccountValidateResult ret_val = 0;
    ret_val = validate (account, con, cont, dead_mans_switch);
    return ret_val;
}

/**
 * sharing_plugin_interface_edit_account:
 * @account: Account tested
 * @con: Connection used to test account
 * @dead_mans_switch:
 *
 * Edit account plugin implementation.
 *
 * Returns: Result of account edit
 */
SharingPluginInterfaceEditAccountResult
    sharing_plugin_interface_edit_account (GtkWindow* parent,
    SharingAccount* account, ConIcConnection* con, gboolean* dead_mans_switch)
{
    SharingPluginInterfaceEditAccountResult ret = 0;
    ULOG_DEBUG_L ("sharing_plugin_interface_edit_account");
    return ret;
}

gboolean sharing_plugin_interface_update_options(
        SharingAccount* account, ConIcConnection* con, gboolean* cont,
        gboolean* dead_mans_switch, UpdateOptionsCallback cb_func,
        gpointer cb_data) {
	gboolean ret = TRUE;
	SharingPluginInterfaceUpdateOptionsResult updateResult = SHARING_UPDATE_OPTIONS_ERROR_UNKNOWN;

    char* sessionKey = NULL;
    char* sessionRequestId = NULL;
    char* token = NULL;

    *dead_mans_switch = FALSE;
    if (yandexGetSessionKey(&sessionKey, &sessionRequestId) == YANDEX_GET_SESSION_KEY_SUCCESS) {
    	*dead_mans_switch = FALSE;
    	if (YANDEX_GET_AUTH_TOKEN_SUCCESS == yandexGetAuthToken(sessionRequestId, sessionKey,
							   sharing_account_get_username(account), sharing_account_get_password(account),
							   &token)) {
    		GSList* albumsList = NULL;
    		*dead_mans_switch = FALSE;
    		if (YANDEX_GET_ALBUM_LIST_SUCCESS == yandexGetAlbumsList(token,sharing_account_get_username(account),&albumsList) && albumsList) {
    			sharing_account_set_option_values(account,"album",albumsList);
    			updateResult = SHARING_UPDATE_OPTIONS_SUCCESS;
    		}
    		*dead_mans_switch = FALSE;
    		if (albumsList) sharing_service_option_values_free(albumsList);
    	}
    	*dead_mans_switch = FALSE;
    }
    *dead_mans_switch = FALSE;

    if (token) free(token);
    if (sessionKey) free(sessionKey);
    if (sessionRequestId) free(sessionRequestId);

	if (cb_func != NULL) {
		void (*fp) (SharingPluginInterfaceUpdateOptionsResult, gpointer);
		fp = cb_func;
		fp(updateResult, cb_data);
	}

	return ret;
}

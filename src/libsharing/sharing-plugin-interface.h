/*
 * This file is part of Sharing Application Library
 *
 * Copyright (C) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
 *
 * Contact: Alexander Bokovoy <alexander.bokovoy@nokia.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * version 2.1 as published by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 *
 */

/**
 * SECTION:sharing-plugin-interface
 * @short_description: Sharing plugin interface defines the plugin gmodule
 * interface.
 *
 * #SharingPluginInterface defines the interface that Sharing framework uses
 * when calling the plugin.
 */

#ifndef _SHARING_PLUGIN_INTERFACE_H_
#define _SHARING_PLUGIN_INTERFACE_H_

#include <gtk/gtk.h>
#include <libosso.h>
#include <conicconnection.h>
#include "sharing-account.h"
#include "sharing-transfer.h"

G_BEGIN_DECLS

/**
 * SharingTransferStatus:
 * @SHARING_SEND_SUCCESS: Send succesfully done.
 * @SHARING_SEND_ERROR_UNKNOWN: Cannot proceed.
 * @SHARING_SEND_ERROR_AUTH: Authentication failed. Account has to be edited.
 * @SHARING_SEND_ERROR_CONNECTION: Connection (wifi/bt/etc.) lacking.
 * @SHARING_SEND_CANCELLED: Task cancel noticed and returned.
 * @SHARING_SEND_ERROR_FILE_FORMAT: File format not supported by service.
 * @SHARING_SEND_ERROR_UPLOAD_LIMIT: Service upload limit exceeded.
 * @SHARING_SEND_ERROR_ACCOUNT_NOT_FOUND: Account not found.
 * @SHARING_SEND_ERROR_TIMEOUT: HTTP timeouts etc.
 */
typedef enum {
    SHARING_SEND_SUCCESS = 0,
    SHARING_SEND_ERROR_UNKNOWN = 1,
    SHARING_SEND_ERROR_AUTH = 2,
    SHARING_SEND_ERROR_CONNECTION = 3,
    SHARING_SEND_CANCELLED = 4,
    SHARING_SEND_ERROR_FILE_FORMAT = 5,
    SHARING_SEND_ERROR_UPLOAD_LIMIT = 6,
    SHARING_SEND_ERROR_ACCOUNT_NOT_FOUND = 7,
    SHARING_SEND_ERROR_TIMEOUT = 8
} SharingPluginInterfaceSendResult;

/**
 * SharingPluginInterfaceAccountSetupResult:
 * @SHARING_ACCOUNT_SETUP_SUCCESS: Setup flow succesfully done.
 * @SHARING_ACCOUNT_SETUP_ERROR_UNKNOWN: General failure.
 * @SHARING_ACCOUNT_SETUP_ERROR_CONNECTION: Connection & connection timeout
 * error.
 */
typedef enum {
    SHARING_ACCOUNT_SETUP_SUCCESS = 0,
    SHARING_ACCOUNT_SETUP_ERROR_UNKNOWN = 1,
    SHARING_ACCOUNT_SETUP_ERROR_CONNECTION = 2
} SharingPluginInterfaceAccountSetupResult;

/**
 * SharingPluginInterfaceAccountValidateResult:
 * @SHARING_ACCOUNT_VALIDATE_SUCCESS: Setup flow succesfully done.
 * @SHARING_ACCOUNT_VALIDATE_ACCOUNT_NOT_FOUND: Account data could not be
 * loaded.
 * @SHARING_ACCOUNT_VALIDATE_FAILED: Account information invalid.
 * @SHARING_ACCOUNT_VALIDATE_ERROR_UNKNOWN: General error.
 * @SHARING_ACCOUNT_VALIDATE_ERROR_CONNECTION: Connection & connection timeout
 * @SHARING_ACCOUNT_VALIDATE_NOT_STARTED: Validation process not started.
 * @SHARING_ACCOUNT_VALIDATE_CANCELLED: Returned when dialog is validating
 * dialog is closed by user.
 */
typedef enum {
    SHARING_ACCOUNT_VALIDATE_SUCCESS = 0,
    SHARING_ACCOUNT_VALIDATE_ACCOUNT_NOT_FOUND = 1,
    SHARING_ACCOUNT_VALIDATE_FAILED = 2,
    SHARING_ACCOUNT_VALIDATE_ERROR_UNKNOWN = 3,
    SHARING_ACCOUNT_VALIDATE_ERROR_CONNECTION = 4,
    SHARING_ACCOUNT_VALIDATE_NOT_STARTED = 5,
    SHARING_ACCOUNT_VALIDATE_CANCELLED = 6
} SharingPluginInterfaceAccountValidateResult;

/**
 * SharingPluginInterfaceEditAccountResult:
 * @SHARING_EDIT_ACCOUNT_SUCCESS: Editing succesfully done.
 * @SHARING_EDIT_ACCOUNT_ERROR_UNKNOWN: General error.
 * @SHARING_EDIT_ACCOUNT_ERROR_CONNECTION: Connection & connection timeout.
 * @SHARING_EDIT_ACCOUNT_NOT_STARTED: Editing process not started.
 * @SHARING_EDIT_ACCOUNT_DELETE: Account is wanted to be deleted.
 * @SHARING_EDIT_ACCOUNT_CANCELLED: Editing cancelled.
 */
typedef enum {
    SHARING_EDIT_ACCOUNT_SUCCESS = 0,
    SHARING_EDIT_ACCOUNT_ERROR_UNKNOWN = 1,
    SHARING_EDIT_ACCOUNT_ERROR_CONNECTION = 2,
    SHARING_EDIT_ACCOUNT_NOT_STARTED = 3,
    SHARING_EDIT_ACCOUNT_DELETE = 4,
    SHARING_EDIT_ACCOUNT_CANCELLED = 5
} SharingPluginInterfaceEditAccountResult;

/**
 * SharingPluginInterfaceUpdateOptionsResult:
 * @SHARING_UPDATE_OPTIONS_SUCCESS: Updating succesfully done.
 * @SHARING_UPDATE_OPTIONS_ERROR_UNKNOWN: General error.
 * @SHARING_UPDATE_OPTIONS_CANCELLED: Updating cancelled.
 * @SHARING_UPDATE_OPTIONS_NO_UPDATE_FUNCTIONALITY: Does not support update.
 * @SHARING_UPDATE_OPTIONS_ERROR_PARAMETERS: Parameters invalid.
 */
typedef enum {
    SHARING_UPDATE_OPTIONS_SUCCESS = 0,
    SHARING_UPDATE_OPTIONS_ERROR_UNKNOWN = 1,
    SHARING_UPDATE_OPTIONS_CANCELLED = 2,
    SHARING_UPDATE_OPTIONS_NO_UPDATE_FUNCTIONALITY = 3,
    SHARING_UPDATE_OPTIONS_ERROR_PARAMETERS = 4
} SharingPluginInterfaceUpdateOptionsResult;

/**
 * Plugin interface function binary API.
 */
#define SHARING_PLUGIN_INTERFACE_INIT \
    "sharing_plugin_interface_init"
#define SHARING_PLUGIN_INTERFACE_UNINIT \
    "sharing_plugin_interface_uninit"
#define SHARING_PLUGIN_INTERFACE_SEND \
    "sharing_plugin_interface_send"
#define SHARING_PLUGIN_INTERFACE_ACCOUNT_SETUP \
    "sharing_plugin_interface_account_setup"
#define SHARING_PLUGIN_INTERFACE_ACCOUNT_VALIDATE \
    "sharing_plugin_interface_account_validate"
#define SHARING_PLUGIN_INTERFACE_EDIT_ACCOUNT \
    "sharing_plugin_interface_edit_account"
#define SHARING_PLUGIN_INTERFACE_UPDATE_OPTIONS \
    "sharing_plugin_interface_update_options"

/**
 * SharingPluginInterfaceInit:
 * @dead_mans_switch: While in function this switch should be turned to %FALSE
 * at least every 30 seconds.
 *
 * Sharing Service Plugin Interface function for plugin init. This
 * function must be implemented in Sharing Plugins. Function
 * is called straight after plugin is loaded.
 *
 * Returns: Plugin should return 0 if initialization was successful else 1.
 */
typedef guint (*SharingPluginInterfaceInit)(
    gboolean* dead_mans_switch);

/**
 * SharingPluginInterfaceUninit:
 * @dead_mans_switch: While in function this switch should be turned to %FALSE
 * at least every 30 seconds.
 *
 * Sharing Service Plugin Interface function for SharingEntry sending. This
 * function must be implemented in Sharing Plugins. Function
 * is called before plugin is unloaded.
 *
 * Returns: Plugin should return 0 if uninitialization was successful else 1.
 */
typedef guint (*SharingPluginInterfaceUninit)(
    gboolean* dead_mans_switch);

/**
 * SharingPluginInterfaceSend:
 * @SharingTransfer: Transfer to be send.
 * @con: ConIc connection.
 * @dead_mans_switch: While in function this switch should be turned to %FALSE
 * as many times as possible. Do NOT use any GTK idle / timeout function
 * or any other separate to do this!
 *
 * Sharing Service Plugin Interface function for #SharingTransfer sending. This
 * function must be implemented in Sharing Plugins.
 *
 * Returns: Plugin should return most fitting
 *     #SharingPluginInterfaceSendResult value.
 */
typedef SharingPluginInterfaceSendResult (*SharingPluginInterfaceSend)(
    SharingTransfer* transfer, ConIcConnection* con, 
    gboolean* dead_mans_switch);

/**
 * SharingPluginInterfaceAccountSetup:
 * @parent: Parent window.
 * @service: Service information.
 * @worked_on: If account is already created in account manager flow
 * 	   it is given as parameter here. Otherwise %NULL value given.
 * @osso: osso_context_t to be used for the Dialog.
 *
 * Sharing Service Plugin Interface function for #SharingAccount
 * setup.
 * 
 * Returns: Plugin should return most fitting
 *     #SharingPluginInterfaceAccountSetupResult value.
 */
typedef SharingPluginInterfaceAccountSetupResult (*SharingPluginInterfaceAccountSetup)(
    GtkWindow* parent, SharingService* service, 
    SharingAccount** worked_on, osso_context_t* osso);

/**
 * SharingPluginInterfaceAccountValidate:
 * @account: Account to be validated.
 * @con: ConIc connection.
 * @cont: %TRUE in common. %FALSE if validation dialog is cancelled, end your
 *     validation flow in this case.
 * @dead_mans_switch: While in function this switch should be turned to %FALSE
 * at least every 30 seconds.
 * 
 * Sharing Service Plugin Interface function for validating #SharingAccount.
 * This function must be implemented in Sharing Plugins.
 *
 * Returns: Plugin should return most fitting 
 *     #SharingPluginInterfaceAccountValidateResult value.
 */
typedef SharingPluginInterfaceAccountValidateResult (*SharingPluginInterfaceAccountValidate)(
    SharingAccount* account, ConIcConnection* con, 
    gboolean *cont, gboolean* dead_mans_switch);
    
/**
 * SharingPluginInterfaceEditAccount:
 * @parent: Parent window.
 * @account: Account to be edited.
 * @con: ConIc connection.
 * @dead_mans_switch: While in function this switch should be turned to %FALSE
 * at least every 30 seconds.
 * 
 * Sharing Service Plugin Interface function to edit #SharingAccount. This
 * function must be implemented in Sharing Plugins.
 *
 * Returns: Plugin should return most fitting
 *     #SharingPluginInterfaceEditAccountResult value.
 */
typedef SharingPluginInterfaceEditAccountResult (*SharingPluginInterfaceEditAccount)(
    GtkWindow* parent, SharingAccount* account,  
    ConIcConnection* con, gboolean* dead_mans_switch);

/**
 * UpdateOptionsCallback:
 * @result: Result of plug-in's flow.
 * @cb_data: Callback function data got before.
 *
 * Update options flow done callback at SharingPluginInterfaceUpdateOptions
 * as parameter.
 */
typedef void (*UpdateOptionsCallback) (
    SharingPluginInterfaceUpdateOptionsResult result, gpointer cb_data);

/**
 * SharingPluginInterfaceUpdateOptions:
 * @account: #SharingAccount that will have the updated options.
 * @con: ConIcConnection* con.
 * @dead_mans_switch: While in function this switch should be turned to %FALSE
 * at least every 30 seconds.
 * @cb_func: Callback function called when update flow ends.
 * @cb_data: Callback function cb_data.
 *
 * Updates options (albums) set to account.
 *
 * Returns: #TRUE if start was OK, #FALSE otherwise. Callback is be called
 * in #TRUE case only!
 */
typedef gboolean (*SharingPluginInterfaceUpdateOptions)(
        SharingAccount* account, ConIcConnection* con, gboolean* cont,
        gboolean* dead_mans_switch, UpdateOptionsCallback cb_func,
        gpointer cb_data);

G_END_DECLS

#endif /* _SHARING_PLUGIN_INTERFACE_H_ */

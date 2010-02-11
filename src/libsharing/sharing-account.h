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

#ifndef _SHARING_ACCOUNT_H_
#define _SHARING_ACCOUNT_H_

#include <glib.h>
#include "sharing-service.h"

G_BEGIN_DECLS

typedef struct SharingAccount SharingAccount;

void sharing_account_free (SharingAccount* self);
gboolean sharing_account_set_param (const SharingAccount* self,
    const gchar* key, const gchar* value);
gchar* sharing_account_get_param (const SharingAccount* self, const gchar* key);
gboolean sharing_account_set_option_values (const SharingAccount* self,
    const gchar* key, const GSList * values);

/**
 * sharing_account_set_username:
 * @self: Used #SharingAccount.
 * @value: Username to set.
 *
 * Macro for setting the username to account.
 */
#define sharing_account_set_username(self,value) \
    sharing_account_set_param(self, "username", value)
/**
 * sharing_account_get_username:
 * @self: Used #SharingAccount.
 *
 * Macro for getting the username from account.
 *
 * Returns: Username as gchar*.
 */
#define sharing_account_get_username(self) \
    sharing_account_get_param(self, "username")
/**
 * sharing_account_set_password:
 * @self: Used #SharingAccount.
 * @value: Password to set.
 *
 * Macro for setting the password to account.
 */
#define sharing_account_set_password(self,value) \
    sharing_account_set_param(self, "password", value)
/**
 * sharing_account_get_password:
 * self: Used #SharingAccount.
 *
 * Macro for getting the username from account.
 *
 * Returns: Password as gchar*
 */
#define sharing_account_get_password(self) \
    sharing_account_get_param(self, "password") 


G_END_DECLS

#endif /* #ifndef _SHARING_ACCOUNT_H_ */

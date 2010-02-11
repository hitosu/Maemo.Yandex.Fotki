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

#ifndef _SHARING_ENTRY_H_
#define _SHARING_ENTRY_H_

#include <glib.h>
#include "sharing-entry-media.h"
#include "sharing-account.h"

G_BEGIN_DECLS

typedef struct SharingEntry SharingEntry;

SharingAccount * sharing_entry_get_account (const SharingEntry * self);
const gchar * sharing_entry_get_option (SharingEntry * self, const gchar * id);
GSList * sharing_entry_get_media (SharingEntry * self);
guint64 sharing_entry_get_size (const SharingEntry * self);

G_END_DECLS

#endif /* #ifndef _SHARING_ENTRY_H_ */

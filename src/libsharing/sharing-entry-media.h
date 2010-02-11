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

#ifndef _SHARING_ENTRY_MEDIA_H_
#define _SHARING_ENTRY_MEDIA_H_

#include <glib.h>

G_BEGIN_DECLS

typedef struct SharingEntryMedia SharingEntryMedia;


gchar* sharing_entry_media_get_name (const SharingEntryMedia * self);
gchar * sharing_entry_media_get_filename (const SharingEntryMedia * self);
gchar * sharing_entry_media_get_filename_without_extension
    (const SharingEntryMedia * self);
gchar* sharing_entry_media_get_title (const SharingEntryMedia * self);
const gchar* sharing_entry_media_get_desc (const SharingEntryMedia * self);
const GSList * sharing_entry_media_get_tags (const SharingEntryMedia * self);
const gchar* sharing_entry_media_get_localpath (const SharingEntryMedia * self);
gchar* sharing_entry_media_get_mime (const SharingEntryMedia * self);
guint64 sharing_entry_media_get_size (const SharingEntryMedia * self);
void sharing_entry_media_set_sent (SharingEntryMedia * self, gboolean sent);
gboolean sharing_entry_media_get_sent (SharingEntryMedia * self);
void sharing_entry_media_set_metadata_editable (SharingEntryMedia * self,
    gboolean editable);

G_END_DECLS

#endif /* #ifndef _SHARING_ENTRY_MEDIA_H_ */


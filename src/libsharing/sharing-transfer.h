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

#ifndef _SHARING_TRANSFER_H_
#define _SHARING_TRANSFER_H_

#include <glib.h>
#include "sharing-entry.h"

G_BEGIN_DECLS

typedef struct SharingTransfer SharingTransfer;
SharingEntry* sharing_transfer_get_entry (SharingTransfer* self);
SharingAccount* sharing_transfer_get_account (SharingTransfer* self);
void sharing_transfer_set_progress (SharingTransfer* self, gdouble value);
gboolean sharing_transfer_continue (SharingTransfer* self);

G_END_DECLS

#endif /* #ifndef _SHARING_TRANSFER_H_ */

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
 * SECTION:sharing-service
 * @short_description: Object used to get information on sharing services
 * defined on device.
 *
 * #SharingService is abstraction of sharing services defined on device.
 * It provides easy API to get all needed information out of
 * definition files.
 */

#ifndef _SHARING_SERVICE_H_
#define _SHARING_SERVICE_H_

#include <glib.h>

G_BEGIN_DECLS

typedef struct SharingService SharingService;

gchar * sharing_service_get_name (SharingService * self);

G_END_DECLS

#endif /* #ifndef _SHARING_SERVICE_H_ */

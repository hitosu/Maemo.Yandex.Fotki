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
 * SECTION:sharing-tag
 * @short_description: Tag element of SharingEntryMedia.
 *
 * #SharingTag can be used to check the information and type of tags specified inside #SharingEntryMedia.
 */


#ifndef _SHARING_TAG_H_
#define _SHARING_TAG_H_

#include <glib.h>

G_BEGIN_DECLS

/**
 * SharingEntryMediaMetadata:
 * @SHARING_TAG_SHARE: Share Tag seen at Sharing Dialog "Tags" -area
 * @SHARING_TAG_LOCATION_COUNTRY: Tag seen at Sharing Dialog Location tags
 * @SHARING_TAG_LOCATION_CITY: Tag seen at Sharing Dialog Location tags
 * @SHARING_TAG_LOCATION_SUBURB: Tag seen at Sharing Dialog Location tags
 * @SHARING_TAG_FAVOURITE: Tag seen in Sharing Dialog as a star icon, 
 *  describing a favourite.
 * @SHARING_TAG_UNKNOWN: Unknown type, use only when type is not known like
 * in initialization phase.
 *
 * Tag types from tags that are seen at GUI (except SHARING_TAG_UNKNOWN).
 */
typedef enum {
    SHARING_TAG_SHARE = 0,
    SHARING_TAG_GEO_COUNTRY = 1 << 0,
    SHARING_TAG_GEO_CITY = 1 << 1,
    SHARING_TAG_GEO_SUBURB = 1 << 2,
    SHARING_TAG_FAVOURITE = 1 << 3,
    SHARING_TAG_GPS_LATITUDE = 1 << 4,
    SHARING_TAG_GPS_LONGITUDE = 1 << 5,
    SHARING_TAG_GPS_ALTITUDE = 1 << 6,
    SHARING_TAG_CREATOR = 1 << 7,
    SHARING_TAG_UNKNOWN = ~0,
} SharingTagType;

typedef struct SharingTag SharingTag;

SharingTagType sharing_tag_get_type (const SharingTag * self);
const gchar* sharing_tag_get_word (const SharingTag * self);

G_END_DECLS

#endif /* #ifndef _SHARING_TAG_H_ */


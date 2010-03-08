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

#ifndef _COMMON_H_
#define _COMMON_H_

#include "libsharing/sharing-entry.h"
#include "libsharing/sharing-transfer.h"
#include <glib.h>

#define PLUGIN_USER_AGENT "Maemo.5 Yandex.Fotki sharing plugin/0.2.1 (Hitosu<hub@hito.su>)"
#define PUB_CHANNEL "Maemo Sharing Plugin by Hito.su"
#define APP_PLATFORM "Maemo"
#define APP_VERSION "0.2.1"

G_BEGIN_DECLS

	typedef enum {
		YANDEX_GET_SESSION_KEY_SUCCESS = 0,
		YANDEX_GET_SESSION_KEY_FAILED = 1,
	} yandexGetSessionKeyResult;
	yandexGetSessionKeyResult yandexGetSessionKey(char** key, char** request_id);

	typedef enum {
		YANDEX_GET_AUTH_TOKEN_SUCCESS = 0,
		YANDEX_GET_AUTH_TOKEN_FAILED = 1,
		YANDEX_GET_AUTH_TOKEN_INVALID_USER = 2
	} yandexGetAuthTokenResult;
	yandexGetAuthTokenResult yandexGetAuthToken(const char* request_id, const char* key, const char* username, const char* password, char** token);

	typedef enum {
		YANDEX_SEND_PHOTO_SUCCESS = 0,
		YANDEX_SEND_PHOTO_FAILED = 1,
		YANDEX_SEND_PHOTO_FILE_NOT_FOUND = 2,
		YANDEX_SEND_PHOTO_FILE_INVALID_TOKEN = 3
	} yandexSendPhotoResult;
	typedef enum {
		YANDEX_PHOTO_ACCESS_PUBLIC = 0,
		YANDEX_PHOTO_ACCESS_FRIENDS = 1,
		YANDEX_PHOTO_ACCESS_PRIVATE = 2
	} yandexPhotoAccessType;
	typedef enum {
		YANDEX_PHOTO_PUBLISH = 0,
		YANDEX_PHOTO_DONT_PUBLISH = 1
	} yandexPhotoPublishSettings;
	typedef struct {
		const char* album;
		yandexPhotoAccessType access;
		yandexPhotoPublishSettings publish;
	} yandexPhotoOptions;
	yandexSendPhotoResult yandexSendPhoto(const char* token, const SharingEntryMedia* photo, SharingTransfer* transfer, yandexPhotoOptions options);

	typedef enum {
		YANDEX_GET_ALBUM_LIST_SUCCESS = 0,
		YANDEX_GET_ALBUM_LIST_FAILED = 1,
		YANDEX_GET_ALBUM_LIST_INVALID_TOKEN = 2
	} yandexGetAlbumsListResult;
	yandexGetAlbumsListResult yandexGetAlbumsList(const char* token, const char* username, GSList** albumOptionValues);

G_END_DECLS

#endif // _COMMON_H_


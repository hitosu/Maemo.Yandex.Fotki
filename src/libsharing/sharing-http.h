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

#ifndef _SHARING_HTTP_H_
#define _SHARING_HTTP_H_

#include <glib.h>
#include <conicconnection.h>

G_BEGIN_DECLS

typedef struct SharingHTTP SharingHTTP;

/**
 * SharingHTTPProgressCallback:
 * @http: Pointer to #SharingHTTP object calling
 * @bytes_sent: How many bytes is sent
 * @user_data: User data pointer
 *
 * Progress callback function prototype for #SharingHTTP
 *
 * Returns: %TRUE to continue, %FALSE to cancel transfer
 */
typedef gboolean (*SharingHTTPProgressCallback) (SharingHTTP * http,
    guint64 bytes_sent, gpointer user_data);


/**
 * SharingHTTPRunResponse:
 * @SHARING_HTTP_RUNRES_SUCCESS: Run was a success.
 * @SHARING_HTTP_RUNRES_UNKNOWN_FAILURE: Unknown failure.
 * @SHARING_HTTP_RUNRES_INVALID_PARAMETERS: Invalid parameters given.
 * @SHARING_HTTP_RUNRES_CONNECTION_PROBLEM: Couldn't get connection to URL.
 * @SHARING_HTTP_RUNRES_CANCELLED: Run was cancelled by callback.
 * @SHARING_HTTP_RUNRES_ALREADY_RUNNING: There is already active process
 * ongoing.
 *
 * Response codes for sharing_http_run()
 */  
typedef enum {
    SHARING_HTTP_RUNRES_SUCCESS,
    SHARING_HTTP_RUNRES_UNKNOWN_FAILURE, 
    SHARING_HTTP_RUNRES_INVALID_PARAMETERS,
    SHARING_HTTP_RUNRES_CONNECTION_PROBLEM,
    SHARING_HTTP_RUNRES_CANCELLED,
    SHARING_HTTP_RUNRES_ALREADY_RUNNING
} SharingHTTPRunResponse;

SharingHTTP * sharing_http_new();

gint sharing_http_ref (SharingHTTP * self);
gint sharing_http_unref (SharingHTTP * self);

void sharing_http_set_connection (SharingHTTP * self,
    ConIcConnection * connection);
ConIcConnection * sharing_http_get_connection (SharingHTTP * self);

void sharing_http_set_timeouts (SharingHTTP * self, glong connecting_timeout,
    glong connection_timeout);

void sharing_http_set_progress_callback (SharingHTTP * self,
    SharingHTTPProgressCallback callback, gpointer user_data);

SharingHTTPRunResponse sharing_http_run (SharingHTTP * self, const gchar * url);

void sharing_http_cancel (SharingHTTP * self);

gboolean sharing_http_add_req_header (SharingHTTP * self, const gchar * name,
    const gchar * value);
gboolean sharing_http_add_req_header_line (SharingHTTP * self,
    const gchar * line);
void sharing_http_remove_req_headers (SharingHTTP * self);

gboolean sharing_http_add_req_multipart_file (SharingHTTP * self,
    const gchar * name, const gchar * filepath, const gchar * type);
    
gboolean sharing_http_add_req_multipart_file_with_filename (SharingHTTP * self,
    const gchar * name, const gchar * filepath, const gchar * type,
    const gchar * filename);    

gboolean sharing_http_add_req_multipart_data (SharingHTTP * self,
    const gchar * name, const gchar * data, gint data_len, const gchar * type);
    
void sharing_http_clear_multiparts (SharingHTTP * self);

void sharing_http_set_res_buffer_size_limit (SharingHTTP * self, gsize limit);
gsize sharing_http_get_res_buffer_size_limit (SharingHTTP * self);

gint sharing_http_get_res_code (SharingHTTP * self);
const gchar * sharing_http_get_res_content (SharingHTTP * self, gsize * len);
const gchar * sharing_http_get_res_body (SharingHTTP * self, gsize * len);

void sharing_http_set_user_agent_name (SharingHTTP * self,
    const gchar * name);

G_END_DECLS

#endif /* #ifndef _SHARING_HTTP_H_ */


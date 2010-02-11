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

#include <stdio.h>
#include <glib.h>
#include <osso-log.h>
#include "common.h"
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <curl/curl.h>
#include <string>
#include <sys/stat.h>
#include "libsharing/sharing-tag.h"
#include "libsharing/sharing-service-option.h"
#include "base64.h"
#include "CP_RSA.h"

static size_t getUrlContentWriteFunction(void *ptr, size_t size, size_t nmemb, void *data);
std::string   getUrlContent(const char* url,const char* token);
std::string getXmlElementValueByXPath(xmlDocPtr xmlDoc, const char* xmlXPath);
static gchar* createTagsStr (const GSList* tags);

yandexGetSessionKeyResult yandexGetSessionKey(char** key, char** request_id) {
	yandexGetSessionKeyResult ret = YANDEX_GET_SESSION_KEY_FAILED;
	std::string getSessionKeyRequestBody = getUrlContent("http://auth.mobile.yandex.ru/yamrsa/key/",NULL);
	if (!getSessionKeyRequestBody.empty()) {
    	xmlDocPtr xmlDoc = NULL;
    	xmlDoc = xmlReadMemory(getSessionKeyRequestBody.c_str(), (int) getSessionKeyRequestBody.length(), "yandexGetSessionKey.xml", NULL, 0);
    	if (xmlDoc != NULL) {
    		std::string str_key = getXmlElementValueByXPath(xmlDoc, "/response/key");
    		std::string str_request_id = getXmlElementValueByXPath(xmlDoc, "/response/request_id");
    		if (!str_key.empty() && !str_request_id.empty()) {
    			ret = YANDEX_GET_SESSION_KEY_SUCCESS;
    			*key = (char*) malloc(str_key.length()+1);
    			if (*key) strcpy(*key, str_key.c_str());
    			*request_id = (char*) malloc(str_request_id.length()+1);
    			if (*request_id) strcpy(*request_id, str_request_id.c_str());
    		}
    		xmlFreeDoc(xmlDoc);
    	}
	}
	return ret;
}

yandexGetAuthTokenResult yandexGetAuthToken(const char* request_id, const char* key, const char* username, const char* password, char** token) {
	yandexGetAuthTokenResult ret = YANDEX_GET_AUTH_TOKEN_FAILED;

	/* Build crypted base64 encoded credentials string */
	CCryptoProviderRSA encrypter;
	encrypter.ImportPublicKey(key);
	std::string credentials("<credentials login=\"");
	credentials += username;
	credentials += "\" password=\"";
	credentials += password;
	credentials += "\"/>";
	char crypted_credentials[MAX_CRYPT_BITS / sizeof(char)] = "\0";
	size_t crypted_credentials_length = 0;
	encrypter.Encrypt(credentials.c_str(), credentials.size(), crypted_credentials, crypted_credentials_length);
	std::string b64_crypted_credentials = base64_encode((unsigned char *)crypted_credentials, crypted_credentials_length);

	/* Send request */
	CURL *curl;
	CURLcode res;
	std::string body;
	struct curl_httppost *formpost = NULL;
	struct curl_httppost *lastptr = NULL;
	struct curl_slist *headerlist=NULL;
	static const char expectHeader[] = "Expect:";
	curl_formadd(&formpost,
	             &lastptr,
	             CURLFORM_COPYNAME, "request_id",
				 CURLFORM_COPYCONTENTS, request_id,
	             CURLFORM_END);
	curl_formadd(&formpost,
	             &lastptr,
	             CURLFORM_COPYNAME, "credentials",
				 CURLFORM_COPYCONTENTS, b64_crypted_credentials.c_str(),
	             CURLFORM_END);
	curl = curl_easy_init();
	if (curl) {
		headerlist = curl_slist_append(headerlist, expectHeader);
		curl_easy_setopt(curl, CURLOPT_URL, "http://auth.mobile.yandex.ru/yamrsa/token/");
		curl_easy_setopt(curl, CURLOPT_USERAGENT, PLUGIN_USER_AGENT);
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
		curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, getUrlContentWriteFunction);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&body);
		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
		curl_formfree(formpost);
		curl_slist_free_all (headerlist);
		if (CURLE_OK == res && !body.empty()) {
			std::string str_token;
			xmlDocPtr xmlDoc = NULL;
			xmlDoc = xmlReadMemory(body.c_str(), body.length(), "yandexGetAuthToken.xml", NULL, 0);
			if (xmlDoc != NULL) {
				str_token = getXmlElementValueByXPath(xmlDoc,"/response/token");
				if (str_token.empty()) ret = YANDEX_GET_AUTH_TOKEN_INVALID_USER;
				else {
					ret = YANDEX_GET_AUTH_TOKEN_SUCCESS;
					*token = (char*) malloc(str_token.length()+1);
					if (*token) strcpy(*token, str_token.c_str());
				}
				xmlFreeDoc(xmlDoc);
			}
		}
	}
	return ret;
}

int curlProgressCallback(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow) {
	SharingTransfer* transfer = (SharingTransfer*) clientp;
	gdouble progress = 0;
	if (ultotal > 0) progress = ulnow / ultotal;
	if (transfer) sharing_transfer_set_progress(transfer,progress);
	return 0;
}

yandexSendPhotoResult yandexSendPhoto(const char* token, const SharingEntryMedia* photo, SharingTransfer* transfer, yandexPhotoOptions options) {
	yandexSendPhotoResult ret = YANDEX_SEND_PHOTO_FAILED;

	const gchar* filepath = sharing_entry_media_get_localpath(photo);
	gchar* filename = sharing_entry_media_get_filename(photo);
	gchar* title = sharing_entry_media_get_title(photo);
	gchar* contentType = sharing_entry_media_get_mime(photo);
	gchar* tags = createTagsStr(sharing_entry_media_get_tags (photo));
	std::string access_type = "public";
	if (options.access == YANDEX_PHOTO_ACCESS_FRIENDS) access_type = "friends";
	else if (options.access == YANDEX_PHOTO_ACCESS_PRIVATE) access_type = "private";
	std::string publish = "0";
	if (options.publish == YANDEX_PHOTO_PUBLISH) publish = "1";

	struct stat fileStat;
	if (stat(filepath,&fileStat) == 0) {
		CURL *curl;
		CURLcode res;
		std::string body;
		struct curl_httppost *formpost = NULL;
		struct curl_httppost *lastptr = NULL;
		struct curl_slist *headerlist=NULL;
		static const char expectHeader[] = "Expect:";
		curl_formadd(&formpost,
					 &lastptr,
					 CURLFORM_COPYNAME, "image",
					 CURLFORM_FILE, filepath,
					 CURLFORM_FILENAME, filename,
					 CURLFORM_CONTENTTYPE, contentType,
					 CURLFORM_END);
		if (options.album) {
			curl_formadd(&formpost,
						 &lastptr,
						 CURLFORM_COPYNAME, "album",
						 CURLFORM_COPYCONTENTS, options.album,
						 CURLFORM_END);
		}
		curl_formadd(&formpost,
		             &lastptr,
		             CURLFORM_COPYNAME, "access_type",
					 CURLFORM_COPYCONTENTS, access_type.c_str(),
		             CURLFORM_END);
		if (title)
		curl_formadd(&formpost,
		             &lastptr,
		             CURLFORM_COPYNAME, "title",
					 CURLFORM_COPYCONTENTS, title,
		             CURLFORM_END);
		if (tags)
		curl_formadd(&formpost,
		             &lastptr,
		             CURLFORM_COPYNAME, "tags",
					 CURLFORM_COPYCONTENTS, tags,
		             CURLFORM_END);
		curl_formadd(&formpost,
		             &lastptr,
		             CURLFORM_COPYNAME, "pub_channel",
					 CURLFORM_COPYCONTENTS, PUB_CHANNEL,
		             CURLFORM_END);
		curl_formadd(&formpost,
		             &lastptr,
		             CURLFORM_COPYNAME, "app_platform",
					 CURLFORM_COPYCONTENTS, APP_PLATFORM,
		             CURLFORM_END);
		curl_formadd(&formpost,
		             &lastptr,
		             CURLFORM_COPYNAME, "app_version",
					 CURLFORM_COPYCONTENTS, APP_VERSION,
		             CURLFORM_END);
		curl_formadd(&formpost,
		             &lastptr,
		             CURLFORM_COPYNAME, "yaru",
					 CURLFORM_COPYCONTENTS, publish.c_str(),
		             CURLFORM_END);
		curl = curl_easy_init();
		if (curl) {
			headerlist = curl_slist_append(headerlist, expectHeader);
			char authHeader[2048];
			sprintf(authHeader,"Authorization: FimpToken realm=\"fotki.yandex.ru\", token=\"%s\"",token);
			headerlist = curl_slist_append(headerlist, authHeader);
			curl_easy_setopt(curl, CURLOPT_URL, "http://api-fotki.yandex.ru/post/");
			curl_easy_setopt(curl, CURLOPT_USERAGENT, PLUGIN_USER_AGENT);
			curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
			curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, getUrlContentWriteFunction);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&body);
			curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0);
			curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, curlProgressCallback);
			curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, transfer);
			res = curl_easy_perform(curl);
			curl_easy_cleanup(curl);
			if (CURLE_OK == res) ret = YANDEX_SEND_PHOTO_SUCCESS;
			else ret = YANDEX_SEND_PHOTO_FILE_INVALID_TOKEN; // TODO: check other errors too
		}
		curl_formfree(formpost);
		curl_slist_free_all (headerlist);
	} else ret = YANDEX_SEND_PHOTO_FILE_NOT_FOUND;

	if (tags) g_free(tags);
	if (contentType) g_free(contentType);
	if (title) g_free(title);
	if (filename) g_free(filename);

	return ret;
}

yandexGetAlbumsListResult yandexGetAlbumsList(const char* token, const char* username, GSList** albumOptionValues) {
	yandexGetAlbumsListResult ret = YANDEX_GET_ALBUM_LIST_FAILED;

	std::string albumsListURL("http://api-fotki.yandex.ru/api/users/");
	albumsListURL += username; albumsListURL += "/albums/";
	std::string body = getUrlContent(albumsListURL.c_str(),token);
	if (!body.empty()) {
		xmlDocPtr xmlDoc = NULL;
		xmlDoc = xmlReadMemory(body.c_str(), body.length(), "albums.xml", NULL, 0);
		if (xmlDoc != NULL) {
			xmlNodePtr feed = xmlDoc->children;
			if (feed && (xmlStrEqual(feed->name, (const xmlChar*)"feed"))) {
				xmlNodePtr entry = feed->children;
				while (entry) {
					if (xmlStrEqual(entry->name,(const xmlChar*)"entry")) {
						xmlChar* path = xmlGetNodePath(entry);
						if (path) free(path);
						xmlNodePtr id = NULL;
						xmlNodePtr title = NULL;
						xmlNodePtr summary = NULL;
						xmlNodePtr isProtected = NULL;
						xmlNodePtr entryChildren = entry->children;
						while (entryChildren) {
							if 		(xmlStrEqual(entryChildren->name,(const xmlChar*)"id")) id = entryChildren;
							else if (xmlStrEqual(entryChildren->name,(const xmlChar*)"title")) title = entryChildren;
							else if (xmlStrEqual(entryChildren->name,(const xmlChar*)"summary")) summary = entryChildren;
							else if (xmlStrEqual(entryChildren->name,(const xmlChar*)"protected")) isProtected = entryChildren;
							entryChildren = entryChildren->next;
						}
						if (id) {
							xmlChar* idStr = xmlNodeGetContent(id);
							if (idStr) {
								const xmlChar* idStrNum = NULL;
								int s;
								for (s=xmlStrlen(idStr);s>0;s--)
									if (idStr[s] == ':') {
										idStrNum = idStr+s+1;
										break;
									}
								xmlChar* titleStr = title ? xmlNodeGetContent(title) : NULL;
								xmlChar* summaryStr = summary ? xmlNodeGetContent(summary) : NULL;
								xmlChar* isProtectedStr = isProtected ? xmlGetProp(isProtected,(const xmlChar*)"value") : NULL;

								SharingServiceOptionValue* albumValue = sharing_service_option_value_new((const gchar*)idStrNum,titleStr?(const gchar*)titleStr:"Unnamed",summaryStr?(const gchar*)summaryStr:"");
								*albumOptionValues = g_slist_append(*albumOptionValues,albumValue);
								ret = YANDEX_GET_ALBUM_LIST_SUCCESS;

								if (idStr) free(idStr);
								if (titleStr) free(titleStr);
								if (summaryStr) free(summaryStr);
								if (isProtectedStr) free(isProtectedStr);
							}
						}
					}
					entry = entry->next;
				}
			}
			xmlFreeDoc(xmlDoc);
		}
	}

	return ret;
}

static size_t getUrlContentWriteFunction(void *ptr, size_t size, size_t nmemb, void *data) {
	std::string* body = (std::string*) data;
	size_t realsize = size * nmemb;
	if (body) body->append((const char*)ptr,realsize);
	return realsize;
}
std::string getUrlContent(const char* url, const char* token) {
	CURL *curl;
	CURLcode res;
	std::string body;
	struct curl_slist *headerlist = NULL;
	curl = curl_easy_init();
	if(curl) {
		if (token) {
			std::string authHeader("Authorization: FimpToken realm=\"fotki.yandex.ru\", token=\"");
			authHeader += token; authHeader += "\"";
			headerlist = curl_slist_append(headerlist, authHeader.c_str());
		}
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_USERAGENT, PLUGIN_USER_AGENT);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, getUrlContentWriteFunction);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&body);
		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
		if (headerlist) curl_slist_free_all (headerlist);
		if (CURLE_OK != res) body.clear();
	}
	return body;
}

std::string getXmlElementValueByXPath(xmlDocPtr xmlDoc, const char* xmlXPath) {
	std::string content;
	if (xmlDoc != NULL) {
		xmlXPathContextPtr context = xmlXPathNewContext(xmlDoc);
		if (context != NULL) {
			xmlXPathObjectPtr result = xmlXPathEvalExpression((const xmlChar*)xmlXPath, context);
			if (result != NULL) {
				if(!xmlXPathNodeSetIsEmpty(result->nodesetval)) {
					content = (char*) xmlNodeGetContent(result->nodesetval->nodeTab[0]);
				}
				xmlXPathFreeObject(result);
			}
			xmlXPathFreeContext(context);
		}
	}
	return content;
}

static gchar* createTagsStr (const GSList* tags) {
   gchar* ret = NULL;
   for (const GSList* p = tags; p != NULL; p = g_slist_next (p)) {
       SharingTag* tag = (SharingTag*)(p->data);
       SharingTagType type = sharing_tag_get_type(tag);
       if (SHARING_TAG_SHARE != type) continue;
       const gchar* tmp = sharing_tag_get_word (tag);
       if (tmp != NULL) {
           gchar* new_ret = NULL;
           if (ret != NULL) {
               new_ret = g_strdup_printf ("%s, %s", ret, tmp);
               g_free (ret); /* old return is freed */
           } else {
               new_ret = g_strdup (tmp);
           }
           ret = new_ret;
       }
   }
   return ret;
}

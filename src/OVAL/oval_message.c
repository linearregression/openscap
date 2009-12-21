/**
 * @file oval_message.c
 * \brief Open Vulnerability and Assessment Language
 *
 * See more details at http://oval.mitre.org/
 */

/*
 * Copyright 2008 Red Hat Inc., Durham, North Carolina.
 * All Rights Reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Authors:
 *      "David Niemoller" <David.Niemoller@g2-inc.com>
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "oval_agent_api_impl.h"
#include "oval_system_characteristics_impl.h"
#include "oval_collection_impl.h"
#include "../common/public/debug.h"

typedef struct oval_message {
	void *model;
	char*                   text;
	oval_message_level_t level;
} oval_message_t;

struct oval_message *oval_message_new()
{
	oval_message_t *message = (oval_message_t*) oscap_alloc(sizeof(oval_message_t));
        if (message == NULL)
                return NULL;

	message->text              = NULL;
	message->level             = OVAL_MESSAGE_LEVEL_NONE;
	return message;
}

bool oval_message_is_valid(struct oval_message *message)
{
	return true;//TODO
}
bool oval_message_is_locked(struct oval_message *message)
{
	return false;//TODO
}

struct oval_message *oval_message_clone(struct oval_message *old_message)
{
	struct oval_message *new_message = oval_message_new();
	oval_message_level_t level = oval_message_get_level(old_message);
	oval_message_set_level(new_message, level);
	char *text = oval_message_get_text(old_message);
	oval_message_set_text(new_message, oscap_strdup(text));
	return new_message;
}

void oval_message_free(struct oval_message *message)
{
        __attribute__nonnull__(message);

	if(message->text != NULL)
                oscap_free(message->text);

	message->text = NULL;
	oscap_free(message);
}

bool oval_message_iterator_has_more(struct oval_message_iterator *oc_message)
{
	return oval_collection_iterator_has_more((struct oval_iterator *)
						 oc_message);
}

struct oval_message *oval_message_iterator_next(struct oval_message_iterator
						*oc_message)
{
	return (struct oval_message *)
	    oval_collection_iterator_next((struct oval_iterator *)oc_message);
}

void oval_message_iterator_free(struct oval_message_iterator
						*oc_message)
{
    oval_collection_iterator_free((struct oval_iterator *)oc_message);
}

char *oval_message_get_text(struct oval_message *message) {

        __attribute__nonnull__(message);

        return message->text;
}

oval_message_level_t oval_message_get_level(struct oval_message *message){

        __attribute__nonnull__(message);

        return message->level;
}

void oval_message_set_text(struct oval_message *message, char *text)
{
	if(message && !oval_message_is_locked(message)){
		if(message->text!=NULL)
                        oscap_free(message->text);
		message->text = (text == NULL) ? NULL : oscap_strdup(text);
	} else 
                oscap_dprintf("WARNING: attempt to update locked content (%s:%d)", __FILE__, __LINE__);
}
void oval_message_set_level(struct oval_message *message, oval_message_level_t level)
{
	if(message && !oval_message_is_locked(message)){
		message->level = level;
	} else 
                oscap_dprintf("WARNING: attempt to update locked content (%s:%d)", __FILE__, __LINE__);
}

static void oval_message_parse_tag_consumer(char* text, void* message){
	oval_message_set_text(message, text);
}

int oval_message_parse_tag(xmlTextReaderPtr reader,
			       struct oval_parser_context *context, oscap_consumer_func consumer, void* client)
{
	int return_code = 1;
	struct oval_message *message = oval_message_new();
	{/*message->lever*/
		oval_message_set_level(message, oval_message_level_parse(reader,"level",OVAL_MESSAGE_LEVEL_INFO));
	}
	{/*message->text*/
		return_code = oval_parser_text_value(reader, context, &oval_message_parse_tag_consumer, message);
	}
	if(return_code != 1){
		oscap_dprintf("WARNING: oval_warning_parse_tag:: return code is not 1::(%d)", return_code);
	}else{
		oscap_dprintf("DEBUG: oval_message_parse_tag::"
		"\n    message->level    = %d    message->text     = %s",
                oval_message_get_level   (message), oval_message_get_text    (message));

		(*consumer)(message, client);
	}
	return return_code;
}

void oval_message_to_print(struct oval_message *message, char *indent,
			      int idx)
{
	char nxtindent[100];

	if (strlen(indent) > 80)
		indent = "....";

	if (idx == 0)
		snprintf(nxtindent, sizeof(nxtindent), "%sMESSAGE.", indent);
	else
		snprintf(nxtindent, sizeof(nxtindent), "%sMESSAGE[%d].", indent, idx);

	/*
	char*                   text;
	oval_message_level_enum level;
	 */
	oscap_dprintf("%sLEVEL = %d\n", nxtindent, oval_message_get_level(message));
	oscap_dprintf("%sTEXT  = %s\n", nxtindent, oval_message_get_text (message));
}

void oval_message_to_dom  (struct oval_message *message, xmlDoc *doc, xmlNode *tag_parent){

	if(message){
	    xmlNs *ns_syschar = xmlSearchNsByHref(doc, tag_parent, OVAL_SYSCHAR_NAMESPACE);
	    xmlNode *tag_message = xmlNewChild
			(tag_parent, ns_syschar, BAD_CAST "message", BAD_CAST oval_message_get_text(message));
	    xmlNewProp(tag_message, BAD_CAST "level", BAD_CAST oval_message_level_text(oval_message_get_level(message)));
	}
}


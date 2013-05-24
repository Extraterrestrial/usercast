/*
 * Copyright (c) 2013 Pavlo Lavrenenko
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#define PURPLE_PLUGINS

#define USERCAST_PLUGIN_ID      "core-usercast"
#define USERCAST_PLUGIN_NAME    "usercast"
#define USERCAST_PLUGIN_VERSION "0.0.1"

#include <conversation.h>
#include <signals.h>
#include <gtkconv.h>
#include <version.h>
#include <debug.h>

static gboolean
conversation_nick_clicked(PurpleConversation* conv, gchar* nick, guint button)
{
    gchar* user_cast = NULL;
    GtkTextIter it;
    gint pos;
    /* char c; */

    if (purple_conversation_get_type(conv) != PURPLE_CONV_TYPE_CHAT)
    {
        purple_debug_warning(USERCAST_PLUGIN_NAME, "Conversation `%s' is not a chat\n", conv->name);
        return FALSE;
    }

    if (!PIDGIN_IS_PIDGIN_CONVERSATION(conv))
    {
        purple_debug_warning(USERCAST_PLUGIN_NAME, "Conversation `%s' is not a pidgin conversation\n",
                             conv->name);
        return FALSE;
    }

    if (button != 1)
        return FALSE;
        
				gtk_text_buffer_get_iter_at_mark(PIDGIN_CONVERSATION(conv)->entry_buffer, &it, gtk_text_buffer_get_insert(PIDGIN_CONVERSATION(conv)->entry_buffer));
				pos = gtk_text_iter_get_offset(&it);
				if(pos == 0) {
				  user_cast = g_strdup_printf("%s: ", nick);
          gtk_text_buffer_insert_at_cursor(PIDGIN_CONVERSATION(conv)->entry_buffer, user_cast, -1);
          g_free(user_cast);
        }
				else {
					gtk_text_buffer_insert_at_cursor(PIDGIN_CONVERSATION(conv)->entry_buffer, nick, -1);
				}
        purple_debug_info(USERCAST_PLUGIN_NAME, "Casted user `%s' to `%s'\n", nick, conv->name);
        return TRUE;
}

static gboolean
plugin_load(PurplePlugin *plugin)
{
    purple_signal_connect(pidgin_conversations_get_handle(), "chat-nick-clicked",
                          plugin, PURPLE_CALLBACK(conversation_nick_clicked), NULL);
    return TRUE;
}

static gboolean
plugin_unload(PurplePlugin *plugin)
{
    purple_signals_disconnect_by_handle(plugin);
    return TRUE;
}

static PurplePluginInfo info =
{
    PURPLE_PLUGIN_MAGIC,
    PURPLE_MAJOR_VERSION,
    PURPLE_MINOR_VERSION,
    PURPLE_PLUGIN_STANDARD,
    NULL,
    0,
    NULL,
    PURPLE_PRIORITY_DEFAULT,
    USERCAST_PLUGIN_ID,
    USERCAST_PLUGIN_NAME,
    USERCAST_PLUGIN_VERSION,
    "Paste username into chat conversation",
    "Double-click username to paste it into chat input area",
    "Pavlo Lavrenenko <santa.ssh@gmail.com>",
    "https://github.com/santa01/usercast",
    plugin_load,
    plugin_unload,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL
};

static void
init_plugin(PurplePlugin *plugin)
{
}

PURPLE_INIT_PLUGIN(usercast, init_plugin, info)

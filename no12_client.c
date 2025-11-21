#include <dbus/dbus.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    DBusMessage* msg;
    DBusMessageIter args;
    DBusConnection* conn;
    DBusError err;
    DBusMessage* reply;

    dbus_error_init(&err);

    conn = dbus_bus_get(DBUS_BUS_SESSION, &err);
    if (dbus_error_is_set(&err)) {
        fprintf(stderr, "Connection Error: %s\n", err.message);
        dbus_error_free(&err);
    }
    if (!conn) exit(1);

    msg = dbus_message_new_method_call("test.server",
                                       "/test/path",
                                       "test.interface",
                                       "SendMessage");
    if (!msg) {
        fprintf(stderr, "Message Null\n");
        exit(1);
    }

    char* messageText = "Hello from Client!";
    dbus_message_iter_init_append(msg, &args);
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &messageText)) {
        fprintf(stderr, "Out Of Memory!\n");
        exit(1);
    }

    reply = dbus_connection_send_with_reply_and_block(conn, msg, -1, &err);

    if (dbus_error_is_set(&err)) {
        fprintf(stderr, "Error: %s\n", err.message);
        dbus_error_free(&err);
    }

    char* replyText;
    dbus_message_iter_init(reply, &args);
    dbus_message_iter_get_basic(&args, &replyText);

    printf("Server Reply: %s\n", replyText);

    dbus_message_unref(msg);
    dbus_message_unref(reply);

    return 0;
}

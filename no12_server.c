#include <dbus/dbus.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main()
{
    DBusError err;
    DBusConnection* conn;
    DBusMessage* msg;
    DBusMessageIter args;
    DBusMessage* reply;

    dbus_error_init(&err);

    conn = dbus_bus_get(DBUS_BUS_SESSION, &err);
    if (dbus_error_is_set(&err)) {
        fprintf(stderr, "Connection Error: %s\n", err.message);
        dbus_error_free(&err);
    }
    if (!conn) exit(1);

    int ret = dbus_bus_request_name(conn, "test.server",
                                    DBUS_NAME_FLAG_REPLACE_EXISTING, &err);

    if (dbus_error_is_set(&err)) {
        fprintf(stderr, "Name Error: %s\n", err.message);
        dbus_error_free(&err);
    }
    if (DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER != ret) exit(1);

    printf("DBus Server Running...\n");

    while (1)
    {
        dbus_connection_read_write(conn, 0);
        msg = dbus_connection_pop_message(conn);

        if (NULL == msg) {
            usleep(100000);
            continue;
        }

        if (dbus_message_is_method_call(msg, "test.interface", "SendMessage"))
        {
            char* text = NULL;
            dbus_message_iter_init(msg, &args);
            dbus_message_iter_get_basic(&args, &text);

            printf("Received: %s\n", text);

            reply = dbus_message_new_method_return(msg);
            DBusMessageIter replyArgs;
            dbus_message_iter_init_append(reply, &replyArgs);
            const char* response = "Server received your message!";
            dbus_message_iter_append_basic(&replyArgs, DBUS_TYPE_STRING, &response);

            dbus_connection_send(conn, reply, NULL);
            dbus_message_unref(reply);
        }

        dbus_message_unref(msg);
    }

    return 0;
}

#include "HTTPServ.h"

static void handleInCall(struct mg_connection *c, int ev, void *ev_data, void *fn_data)
{
    CallCenter *mainCallCenter = static_cast<CallCenter *>(fn_data);

    if (ev != MG_EV_HTTP_MSG)
        return;
    struct mg_http_message *hm = (struct mg_http_message *)ev_data;
    if (!mg_http_match_uri(hm, "/call"))
        return;
    char buf[100] = "";
    int getVarSuccess = mg_http_get_var(&hm->query, "number", buf, sizeof(buf));

    if (!getVarSuccess)
    {
        // CDR
        mg_http_reply(c, BAD_REQUEST, "", "{%m}\n",
                      MG_ESC("Incorrect request"));
        return;
    }
    if (strlen(buf) > 15)
    {
        // CDR
        mg_http_reply(c, BAD_REQUEST, "", "{%m:%s}\n",
                      MG_ESC("Incorrect number"), buf);
        return;
    }
    HttpRequest req;
    req.number = buf;
    // CDR
    switch (mainCallCenter->handleHttpRequest(req))
    {
    case OK:
        mg_http_reply(c, OK, "", "{%m:%s}\n",
                      MG_ESC("Call id"), req.callId);
        break;
    case TOO_MANY_REQUESTS:
        mg_http_reply(c, TOO_MANY_REQUESTS, "", "{%m}\n",
                      MG_ESC("Alredy in queue"));
        break;
    case INTERNAL_SERVER_ERROR:
        /* TODO */
        break;
    case SERVICE_UNAVALIABLE:
        mg_http_reply(c, 503, "", "{%m}\n",
                      MG_ESC("Overload"));
        break;
    default:
        break;
    }
}

void runHTTPServ(QueueConfig &conf)
{
    CallCenter mainCallCenter(conf);
    void *arg = static_cast<void *>(&mainCallCenter);

    struct mg_mgr mgr;
    mg_mgr_init(&mgr);
    mg_http_listen(&mgr, "http://0.0.0.0:8000", handleInCall, arg);
    for (;;)
    {
        mg_mgr_poll(&mgr, 1000);
        mainCallCenter.processQueue();
    }
}

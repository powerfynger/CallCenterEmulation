#include "HTTPServ.h"

bool* writeLogsPtr;

static void handleInCall(struct mg_connection *c, int ev, void *ev_data, void *fn_data)
{
    char buf[100] = "", sender_ip[20] = "";
    HttpRequest req;
    // CDR
    req.record.arrivalTime = std::chrono::system_clock::now();

    CallCenter *mainCallCenter = static_cast<CallCenter *>(fn_data);

    if (ev != MG_EV_HTTP_MSG){

        return;
    }
    mg_snprintf(sender_ip, sizeof(sender_ip), "%M", mg_print_ip, &c->rem.ip);
    struct mg_http_message *hm = (struct mg_http_message *)ev_data;
    if (!mg_http_match_uri(hm, "/call")){
        LOG_IF(*writeLogsPtr, INFO) << "Recieved not call from " << sender_ip;
        return;
    }
    int getVarSuccess = mg_http_get_var(&hm->query, "number", buf, sizeof(buf));

    if (!getVarSuccess)
    {
        mg_http_reply(c, BAD_REQUEST, "", "{%m}\n",
                      MG_ESC("Incorrect request"));
        
        LOG_IF(*writeLogsPtr, INFO) << "Incorrect request from " << sender_ip;
        // CDR
        req.record.endTime = std::chrono::system_clock::now();
        req.record.setStatus(static_cast<int>(BAD_REQUEST));
        req.record.writeCDRToFile();
        return;
    }
    if (strlen(buf) > 15)
    {
        mg_http_reply(c, BAD_REQUEST, "", "{%m:%s}\n",
                      MG_ESC("Incorrect number"), buf);
        
        LOG_IF(*writeLogsPtr, INFO) << "Incorrect number from " << sender_ip;
        // CDR
        req.record.endTime = std::chrono::system_clock::now();
        req.record.setStatus(static_cast<int>(BAD_REQUEST));
        req.record.writeCDRToFile();
        return;
    }
    req.number = buf;
    req.callId = generateRandomString();
    req.record.callId = req.callId;

    switch (mainCallCenter->handleHttpRequest(req))
    {
    case OK:
        mg_http_reply(c, OK, "", "{%m:%s}\n",
                      MG_ESC("Call id"), req.callId.c_str());
        // CDR
        req.record.number = req.number;
        break;
    case TOO_MANY_REQUESTS:
        mg_http_reply(c, TOO_MANY_REQUESTS, "", "{%m}:%s\n",
                      MG_ESC("Alredy in queue"), req.callId.c_str());
        
        // CDR
        req.record.endTime = std::chrono::system_clock::now();
        req.record.number = req.number;
        req.record.setStatus(static_cast<int>(TOO_MANY_REQUESTS));
        req.record.writeCDRToFile();
        break;
    case SERVICE_UNAVALIABLE:
        mg_http_reply(c, SERVICE_UNAVALIABLE, "", "{%m}:%s\n",
                      MG_ESC("Overload"), req.callId.c_str());
        
        // CDR
        req.record.endTime = std::chrono::system_clock::now();
        req.record.number = req.number;
        req.record.setStatus(static_cast<int>(SERVICE_UNAVALIABLE));
        req.record.writeCDRToFile();
        break;
    default:
        mg_http_reply(c, INTERNAL_SERVER_ERROR, "", "{%m}:%s\n",
                      MG_ESC("Error"), req.callId.c_str());
        break;
    }
}

void runHTTPServ(QueueConfig &conf, bool writeLogs)
{
    std::srand(std::time(0));
    *writeLogsPtr = writeLogs;
    CallCenter mainCallCenter(conf, writeLogs);
    void *arg = static_cast<void *>(&mainCallCenter);

    struct mg_mgr mgr;
    mg_mgr_init(&mgr);

    if (&mgr == NULL){
        LOG_IF(writeLogs, FATAL) << "Failed to start listen on port 8000";
        exit(EXIT_FAILURE);
    }
    if(mg_http_listen(&mgr, "http://0.0.0.0:8000", handleInCall, arg) == NULL){
        LOG_IF(writeLogs, FATAL) << "Failed to start listen on port 8000";
        exit(EXIT_FAILURE);
    }
    for (;;)
    {
        mg_mgr_poll(&mgr, 1000);
        mainCallCenter.processQueue();
    }
}

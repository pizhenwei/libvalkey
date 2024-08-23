#include <valkey/valkeycluster.h>

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    UNUSED(argc);
    UNUSED(argv);
    struct timeval timeout = {1, 500000}; // 1.5s

    valkeyClusterContext *cc = valkeyClusterContextInit();
    valkeyClusterSetOptionAddNodes(cc, "127.0.0.1:7000");
    valkeyClusterSetOptionConnectTimeout(cc, timeout);
    valkeyClusterSetOptionRouteUseSlots(cc);
    valkeyClusterConnect2(cc);
    if (!cc) {
        printf("Error: Allocation failure\n");
        exit(-1);
    } else if (cc->err) {
        printf("Error: %s\n", cc->errstr);
        // handle error
        exit(-1);
    }

    valkeyReply *reply = valkeyClusterCommand(cc, "SET %s %s", "key", "value");
    printf("SET: %s\n", reply->str);
    freeReplyObject(reply);

    valkeyReply *reply2 = valkeyClusterCommand(cc, "GET %s", "key");
    printf("GET: %s\n", reply2->str);
    freeReplyObject(reply2);

    valkeyClusterFree(cc);
    return 0;
}

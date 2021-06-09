#include <iostream>
#include <vector>
#include <thread>

#include <cassandra.h>

void assert_future_ok(CassFuture* future, const char* message) {
    if (cass_future_error_code(future) == CASS_OK) {
        return;
    }

    const char *error_msg;
    size_t error_msg_len;
    cass_future_error_message(future, &error_msg, &error_msg_len);
    fprintf(stderr, "ERROR: %s: '%.*s'\n", message, (int)error_msg_len, error_msg);

    std::exit(1);
}

std::pair<CassSession*, CassCluster*> connect() {
    CassCluster* cluster = cass_cluster_new();

    if (cass_cluster_set_protocol_version(cluster, CASS_PROTOCOL_VERSION_V4) != CASS_OK) {
        fprintf(stderr, "ERROR: Failed to set protocol version\n");
        std::exit(1);
    }

    cass_cluster_set_contact_points(cluster, "127.0.0.1");

    if (cass_cluster_set_num_threads_io(cluster, std::thread::hardware_concurrency()) != CASS_OK) {
        fprintf(stderr, "ERROR: Failed to set io threads number\n");
        std::exit(1);
    }

    CassSession* session = cass_session_new();
    CassFuture* connect_future = cass_session_connect(session, cluster);
    cass_future_wait(connect_future);

    assert_future_ok(connect_future, "Unable to connect");
    cass_future_free(connect_future);

    return std::make_pair(session, cluster);
}

void make_simple_request(CassSession* session) {
    const char* query = "SELECT key FROM system.local";
    CassStatement* statement = cass_statement_new(query, 0);

    CassFuture *query_future = cass_session_execute(session, statement);
    cass_future_wait(query_future);

    assert_future_ok(query_future, "Simple request failed");

    cass_statement_free(statement);
    cass_future_free(query_future);
}

int main() {
    auto [session, cluster] = connect();

    std::vector<std::thread> threads;

    for (int t = 0; t < 64; t++) {
        threads.push_back(std::thread(
            [session]() {
                for (int i = 0; i < 1024; i++) {
                    make_simple_request(session);
                }
            }
        ));
    }

    for (auto&& t : threads) {
        t.join();
    }

    std::cout << "DONE!" << std::endl;

    cass_session_free(session);
    cass_cluster_free(cluster);
    return 0;
}
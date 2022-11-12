#include <iostream>
#include <stdexcept>
#include <thread>

#include <curl/curl.h>
#include <MQTTClient.h>

#include "json.hpp"

const std::string API_KEY = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJhdWQiOiJsZW1vbi5tYXJrZXRzIiwiaXNzIjoibGVtb24ubWFya2V0cyIsInN1YiI6InVzcl9xeUhTTHJyZGRHWWRIcThWN0JWeGdIeGc0V3NWU0o4eU1CIiwiZXhwIjoxNjk5NjkwNTUyLCJpYXQiOjE2NjgxNTQ1NTIsImp0aSI6ImFwa19xeVBQTFRURERLZldmRlY4R2RSczhOMkJGUWpxMlduVktkIiwibW9kZSI6Im1hcmtldF9kYXRhIn0.TJeXeJrZLlB9TsUzy7csWzGezr9EWuS8ADWqFdzwlF8";//"YOUR-MARKET-DATA-API-KEY";

class authenticator {

    public:

        authenticator() {

            this->curl = curl_easy_init();
            if(!curl) throw std::runtime_error("Initiating CURL failed!");

            this->header_attribute_list = NULL;

            // Dev Note: Only necessary for using this class in multiple threads
            curl_easy_setopt(this->curl, CURLOPT_NOSIGNAL, 1);

        }

        ~authenticator() {

            this->free_header_attributes();
            curl_easy_cleanup(this->curl);

        }

        void authenticate(std::string* const out_token,
                          std::string* const out_user_id) {

            // Issue the query //

            const std::string header_attribute = std::string("Authorization: Bearer ") + API_KEY;

            this->allocate_header_attribute(
                header_attribute
            );

            const std::string url = "https://realtime.lemon.markets/v1/auth";
            const std::string postfields = "";
                  std::string response = "";

            this->issue_post(
                &url,
                &postfields,
                &response,
                10L // 10 seconds timeout
            );


            // Parse the response //

            std::cout << "authentication response: " << response << std::endl;

            nlohmann::json parsed_response = nlohmann::json::parse(response);

            *out_token = parsed_response["token"];
            *out_user_id = parsed_response["user_id"];

        }

    private:

        CURL* curl;

        void allocate_header_attribute(const std::string& attribute) {

            this->header_attribute_list = curl_slist_append(
                this->header_attribute_list, attribute.c_str()
            );

        }

        void free_header_attributes() {

            curl_slist_free_all(this->header_attribute_list);
            this->header_attribute_list = NULL;

        }

        void issue_post(const std::string* const url,
                        const std::string* const postfields,
                                std::string* const response,
                        const long timeout) {

            curl_easy_setopt(this->curl, CURLOPT_POST, 1);
            //curl_easy_setopt(this->curl, CURLOPT_VERBOSE, 1L);
            curl_easy_setopt(this->curl, CURLOPT_POSTFIELDS, postfields->c_str());
            curl_easy_setopt(this->curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);
            curl_easy_setopt(this->curl, CURLOPT_URL, url->c_str());
            curl_easy_setopt(this->curl, CURLOPT_WRITEFUNCTION, authenticator::write_function);
            curl_easy_setopt(this->curl, CURLOPT_WRITEDATA, response);
            curl_easy_setopt(this->curl, CURLOPT_HTTPHEADER, this->header_attribute_list);
            curl_easy_setopt(this->curl, CURLOPT_HEADEROPT, CURLHEADER_SEPARATE);        
            curl_easy_setopt(this->curl, CURLOPT_TIMEOUT, timeout);

            CURLcode status_code = curl_easy_perform(this->curl);

            curl_easy_setopt(this->curl, CURLOPT_POST, 0);

            if(status_code != CURLE_OK) {

                const std::string curl_error = curl_easy_strerror(status_code);
                throw std::runtime_error(curl_error + ". The query was " + *url);

            }

        }

        struct curl_slist* header_attribute_list;

        static size_t write_function(void* contents,
                                        size_t size,
                                        size_t nmemb,
                                        std::string* s) {

            size_t new_length = size * nmemb;
            s->append((char*) contents, new_length);
            return new_length;

        }

};


void delivered(void *context, MQTTClient_deliveryToken dt) {

    std::cout << "Message with token value " << dt << " delivery confirmed" << std::endl;

}
 
int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message) {

    std::cout << "Message arrived" << std::endl;
    std::cout << "     topic: " << topicName << std::endl;
    std::cout << "   message: " << (char*) message->payload << std::endl;

    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);

    return 1;

}

void connlost(void *context, char *cause) {

    std::cout << "Connection lost" << std::endl;
    std::cout << "     cause: " << cause << std::endl;

}

class mqtt_client {

    private:

        MQTTClient client;

    public:

        mqtt_client(const std::string broker_address,
                    const std::string client_id)

                  : client(NULL) {

            // Create a client instance //

            const int return_code = MQTTClient_create(
                &this->client,
                broker_address.c_str(),
                client_id.c_str(),
                MQTTCLIENT_PERSISTENCE_NONE,
                NULL
            );

            if(return_code != MQTTCLIENT_SUCCESS) {

                throw std::runtime_error("Failed to create client. Return code " + std::to_string(return_code));

            }

        }

        ~mqtt_client() {

            // Destroy the aggregated client instance //

            MQTTClient_destroy(
                &this->client
            );

        }

        void connect(const std::string username) {

            // Connect to a broker //

            MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;

            conn_opts.keepAliveInterval = 15; // ably: values from 15 to 60
            conn_opts.cleansession = 1;
            conn_opts.username = username.c_str();

            const int return_code = MQTTClient_connect(
                this->client,
                &conn_opts
            );

            if(return_code != MQTTCLIENT_SUCCESS) {

                throw std::runtime_error("Failed to connect. Return code " + std::to_string(return_code));

            }

        }

        void set_callbacks() {

            const int return_code = MQTTClient_setCallbacks(
                this->client,
                NULL,
                connlost,
                msgarrvd,
                delivered
            );

            if(return_code != MQTTCLIENT_SUCCESS) {

                throw std::runtime_error("Failed to set callbacks. Return code " + std::to_string(return_code));

            }

        }

        void publish(const std::string topic,
                     const std::string message) {

            MQTTClient_deliveryToken dt;

            MQTTClient_message message_container = MQTTClient_message_initializer;

            message_container.payload = (void*) message.c_str();
            message_container.payloadlen = message.size();
            message_container.qos = 1; // (0 or 1 required by ably for streaming)
            //message_container.retained = 0;

            const int return_code = MQTTClient_publishMessage(
                this->client,
                topic.c_str(),
                &message_container,
                &dt
            );

            if(return_code != MQTTCLIENT_SUCCESS) {

                throw std::runtime_error("Failed to publish message " + message + ". Return code " + std::to_string(return_code));

            }

        }

        void subscribe(const std::string topic) {

            const int return_code = MQTTClient_subscribe(
                this->client,
                topic.c_str(),
                0 // QoS (0 required by ably for streaming)
            );

            if(return_code != MQTTCLIENT_SUCCESS) {

                throw std::runtime_error("Failed to subscribe to topic " + topic + ". Return code " + std::to_string(return_code));

            }

        }

        void unsubscribe(const std::string topic) {

            const int return_code = MQTTClient_unsubscribe(
                this->client,
                topic.c_str()
            );

            if(return_code != MQTTCLIENT_SUCCESS) {

                throw std::runtime_error("Failed to unsubscribe from topic " + topic + ". Return code " + std::to_string(return_code));

            }

        }

        void disconnect() {

            const int return_code = MQTTClient_disconnect(
                this->client,
                10000 // 10 seconds
            );

            if(return_code != MQTTCLIENT_SUCCESS) {

                throw std::runtime_error("Failed to disconnect. Return code " + std::to_string(return_code));

            }

        }

};

int main() {

    // Get a token from the broker //

    std::string token = "";
    std::string user_id = "";

    authenticator().authenticate(
        &token,
        &user_id
    );


    // Subscribe to live market data //

    mqtt_client client(
        "mqtt.ably.io",
        "ably-client"
    );

    client.set_callbacks();


    std::cout << "connecting to mqtt.ably.io" << std::endl;

    client.connect(
        token
    );


    std::cout << "subscribing to live data" << std::endl;

    client.subscribe(
        user_id
    );


    std::cout << "publishing what live data to stream" << std::endl;

    client.publish(
        user_id + ".subscriptions",
        "US82968B1035"
    );

    std::cout << "waiting 30 seconds for any live data to arrive ..." << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(30));


    std::cout << "disconnecting" << std::endl;

    client.disconnect();

    return 0;

}
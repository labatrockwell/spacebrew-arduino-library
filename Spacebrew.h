
#ifndef SPACEBREW_H
#define SPACEBREW_H

#include <WebSocketClient.h>
#include "Arduino.h"

enum SBType { SB_BOOLEAN, SB_STRING, SB_RANGE };
struct PublisherNode {
  char *name;
  char *type;
  char *defaultValue;
  PublisherNode *next;
};
struct SubscriberNode{
  char *name;
  char *type;
  SubscriberNode *next;
};

class Spacebrew{

  public:
    Spacebrew();
    void monitor();
    typedef void (*OnBooleanMessage)(char name[], bool value);
    typedef void (*OnRangeMessage)(char name[], int value);
    typedef void (*OnStringMessage)(char name[], char value[]);
    //typedef void (*OnOtherMessage)(char* name, char* value);//duplicate of OnStringMessage
    typedef void (*OnSBOpen)();
    typedef void (*OnSBClose)(int code, char message[]);
    typedef void (*OnSBError)(char message[]);
    void onOpen(OnSBOpen function);
    void onClose(OnSBClose function);
    void onRangeMessage(OnRangeMessage function);
    void onStringMessage(OnStringMessage function);
    void onBooleanMessage(OnBooleanMessage function);
    void onOtherMessage(OnStringMessage function);
    void onError(OnSBError function);
    void addPublish(char name[], char type[], char defaultValue[]);
    void addPublish(char name[], bool defaultValue){
      addPublish(name, (char*)"boolean", (char*)(defaultValue ? "true" : "false"));
    }
    void addPublish(char name[], char defaultValue[]){
      addPublish(name, "string", defaultValue);
    }
    void addPublish(char name[], int defaultValue);
    void addPublish(char name[], enum SBType type, char defaultValue[]){
      switch(type){
        case SB_STRING:
          addPublish(name, "string", defaultValue);
          break;
        case SB_RANGE:
          addPublish(name, "range", defaultValue);
          break;
        case SB_BOOLEAN:
          addPublish(name, "boolean", defaultValue);
          break;
      }
    }
    void addPublish(char name[], enum SBType type){
      switch(type){
        case SB_STRING:
          addPublish(name, "string", "");
          break;
        case SB_RANGE:
          addPublish(name, "range", "0");
          break;
        case SB_BOOLEAN:
          addPublish(name, "boolean", "false");
          break;
      }
    }
    void addSubscribe(char name[], char* type);
    void addSubscribe(char name[], enum SBType type){
      switch(type){
        case SB_STRING:
          addSubscribe(name, "string");
          break;
        case SB_RANGE:
          addSubscribe(name, "range");
          break;
        case SB_BOOLEAN:
          addSubscribe(name, "boolean");
          break;
      }
    }
    //void addSubscribe(char* name, char* type, OnBooleanMessage function);
    //void addSubscribe(char* name, char* type, OnRangeMessage function);
    //void addSubscribe(char* name, char* type, OnStringMessage function);//also used for OnOtherMessage
    //void addSubscribe(char* name, SBType type, OnBooleanMessage function);
    //void addSubscribe(char* name, SBType type, OnRangeMessage function);
    //void addSubscribe(char* name, SBType type, OnStringMessage function);
    void connect(char hostname[], char clientName[], char description[], int port = 9000);
    void disconnect();
    bool send(char name[], char type[], char value[]);
    bool send(char name[], SBType type, char value[]){
      switch(type){
        case SB_STRING:
          send(name, "string", value);
          break;
        case SB_RANGE:
          send(name, "range", value);
          break;
        case SB_BOOLEAN:
          send(name, "boolean", value);
          break;
      }
    }
    bool send(char name[], char* value){
      send(name, "string", value);
    }
    bool send(char name[], bool value){
      send(name, (char*)"boolean", (char*)(value ? "true" : "false"));
    }
    bool send(char name[], boolean value){
      send(name, (char*)"boolean", (char*)(value ? "true" : "false"));
    }
    bool send(char name[], int value);
    static void onWSError(WebSocketClient client, char* message);//defined in WebSocketClientCallback
    static void onWSOpen(WebSocketClient client);
    static void onWSClose(WebSocketClient client, int code, char* message);
    static void onWSMessage(WebSocketClient client, char* message);

  private:
    uint8_t mac[6];
    WebSocketClient client;
    static bool m_bOpen;
    static bool m_bSendConfig;
    char* m_sClientName;
    char* m_sDescription;
    void sendConfig();
    void onInternalError(char* message);
    /**Output should be at least 5 cells**/
    static void intToString(int input, char* output){
      char val[4] = {input/1000%10+'0', input/100%10+'0', input/10%10+'0', input%10+'0'};
      char i = 0;
      while(val[i] == '0' && i < 3){
        i++;
      }
      while(i < 4){
        *(output++) = val[i++];
      }
      *(output) = NULL;
      return;
    }
    static int stringToInt(char* input){
       int output = 0;
       while(*input != NULL){
         output *= 10;
         output += (*input) - '0';
         input++;
       }
       return output;
    }
    static OnBooleanMessage _onBooleanMessage;
    static OnRangeMessage _onRangeMessage;
    static OnStringMessage _onStringMessage;
    static OnStringMessage _onOtherMessage;
    static OnSBOpen _onOpen;
    static OnSBClose _onClose;
    static OnSBError _onError;
    PublisherNode *publishers;
    SubscriberNode *subscribers;
    static char *cloneString(char *s){
      int n = strlen(s);
      char *out = (char *)malloc(n+1);//new char[n];
      strcpy(out, s);
      return out;
    }
};

#endif
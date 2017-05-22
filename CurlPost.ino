#include <Bridge.h>
#include <BridgeClient.h>
#include <BridgeServer.h>
#include <BridgeSSLClient.h>
#include <BridgeUdp.h>
#include <Console.h>
#include <FileIO.h>
#include <HttpClient.h>
#include <Mailbox.h>
#include <Process.h>
#include <YunClient.h>
#include <YunServer.h>

/*

Tests over tests

*/

#include <Process.h>

//services setups     
String INUMBER_TRIAL = "i841640"; 
String DEVICE_ID = "32b9f043-8569-464e-8bc4-95c567d98d00";
String MESSAGE_TYPE = "e0a6c3bab615e97310f0";
//dummy entries
int CALOR_TEST = 130;
String SENDER = "Arduino Yun";
String URL_TO_SERVICE = "https://iotmms"+INUMBER_TRIAL+"trial.hanatrial.ondemand.com/com.sap.iotservices.mms/v1/api/http/data/"+DEVICE_ID+"/"+MESSAGE_TYPE+"?test="+CALOR_TEST;

//basic 64 -- use postman to generate it
//String USER_AUTHENTICATION = "Basic aTg0MTY0MDpRYVJ1bGV6MjJA";
String USER_AUTHENTICATION = "Bearer b46b9a15c5c0e3e517c9f516e925723";



// Everytime you reset your arduino, you need to reset your proxy again
/*
export http_proxy=http://proxy.wdf.sap.corp:8080
export https_proxy=http://proxy.wdf.sap.corp:8080 
*/


String createCurlCommand (String url, int calor, String sender, String authentication, String messageType) {
    
  String curlCmd = "curl ";
  //header parameters
  curlCmd += "--insecure -H  \"Authorization: "+ authentication +"\" ";
  curlCmd += "-H  \"Content-Type: application/json\" ";
  //type of request
  curlCmd += " -X POST ";
  //body parameters 
  curlCmd += "-d '{";
  curlCmd += " \"mode\": \"async\"";
  //close -d
  curlCmd += "}' ";
  //backend url
  curlCmd += url;
  return curlCmd;
}

void post(){
  String curlCmd = createCurlCommand( URL_TO_SERVICE, CALOR_TEST, SENDER, USER_AUTHENTICATION, MESSAGE_TYPE );
  Serial.println("Curl Cmd Command being saved: ");
  Serial.println(curlCmd);
  Process process;
  process.runShellCommandAsynchronously(curlCmd);

  while (process.running()){
    while (process.available()>0) {
      char c = process.read();
      //Serial.println(c);
    }
    Serial.println("\n");
    Serial.println("########################################");
  }
}

void remove_http_proxy(){
  String remove_http = "export http_proxy=http://proxy.wdf.sap.corp:8080";
  remove_proxy(remove_http);
}


void remove_https_proxy(){
  String remove_https = "export https_proxy=http://proxy.wdf.sap.corp:8080";
  remove_proxy(remove_https);
}

void remove_proxy(String proxyCommand){
  Process p;
  p.runShellCommand(proxyCommand);
  // do nothing until the process finishes
  while (p.running());
  Serial.println("Proxy Removed: "+ proxyCommand);
  
}

void setup(){
  Bridge.begin();
  while(!Serial);
  Serial.println("Starting...");
}

void loop(){
    remove_http_proxy();
    remove_https_proxy();
    CALOR_TEST = random(1,20);
    post();
}

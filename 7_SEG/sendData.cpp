#include "sendData.h"

void SendData::pushAPIData(String json) {

  HTTPClient http;
  http.setConnectTimeout(3000); // 3 second max timeout
  String APIQueryURL = API_URL;

	http.addHeader("Accept",  "application/vnd.github+json");
  http.addHeader("Authorization", "token " + String(API_TOKEN));
  http.begin(APIQueryURL.c_str());
  int httpResponseCode = http.POST(json);

  if (httpResponseCode != 200) {
    // noooooo
  }
  http.end();
}

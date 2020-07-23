#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

const char index_html[] PROGMEM ={"<html>\n<head>\n\n<!-- your webpage info goes here -->\n\n    <title>Test Website</title>\n\t\n\t<meta name=\"author\" content=\"your name\" />\n\t<meta name=\"description\" content=\"\" />\n\n<!-- you should always add your stylesheet (css) in the head tag so that it starts loading before the page html is being displayed -->\t\n\t<link rel=\"stylesheet\" href=\"style.css\" type=\"text/css\" />\n\t\n</head>\n<body>\n\n<!-- webpage content goes here in the body -->\n\n\t<div id=\"page\">\n\t\t<div id=\"logo\">\n\t\t\t<h1><a href=\"/\" id=\"logoLink\">My First Website</a></h1>\n\t\t</div>\n\t\t<div id=\"nav\">\n\t\t\t<ul>\n\t\t\t\t<li><a href=\"#/home.html\">Home</a></li>\n\t\t\t\t<li><a href=\"#/about.html\">About</a></li>\n\t\t\t\t<li><a href=\"#/contact.html\">Contact</a></li>\n\t\t\t</ul>\t\n\t\t</div>\n\t\t<div id=\"content\">\n\t\t\t<h2>Home</h2>\n\t\t\t<p>\n\t\t\t\tThis is my first webpage! I was able to code all the HTML and CSS in order to make it. Watch out world of web design here I come!\n\t\t\t</p>\n\t\t\t<p> \n\t\t\t\tI can use my skills here to create websites for my business, my friends and family, my C.V, blog or articles. As well as any games or more experiment stuff (which is what the web is really all about). \n\t\t\t</p>\n\t\t</div>\n\t\t<div id=\"footer\">\n\t\t\t<p>\n\t\t\t\tWebpage made by <a href=\"/\" target=\"_blank\">[Dhira Negi]</a>\n\t\t\t</p>\n\t\t</div>\n\t</div>\n</body>\n</html>"};
const char style_css[] PROGMEM = {"/*\n* multi-line comment\n*/\np{ line-height: 1em; }\nh1, h2, h3, h4{\n    color: orange;\n\tfont-weight: normal;\n\tline-height: 1.1em;\n\tmargin: 0 0 .5em 0;\n}\nh1{ font-size: 1.7em; }\nh2{ font-size: 1.5em; }\na{\n\tcolor: black;\n\ttext-decoration: none;\n}\n\ta:hover,\n\ta:active{ text-decoration: underline; }\n\n/* you can structure your code's white space so that it is as readable for when you come back in the future or for other people to read and edit quickly */\n\nbody{\n    font-family: arial; font-size: 80%; line-height: 1.2em; width: 100%; margin: 0; background: #eee;\n}\n/* you can put your code all in one line like above */\n#page{ margin: 20px; }\n\n/* or on different lines like below */\n#logo{\n\twidth: 35%;\n\tmargin-top: 5px;\n\tfont-family: georgia;\n\tdisplay: inline-block;\n}\n/* but try and be as concise as possible */\n#nav{\n\twidth: 60%;\n\tdisplay: inline-block;\n\ttext-align: right;\n\tfloat: right;\n}\n\t#nav ul{}\n\t\t#nav ul li{\n\t\t\tdisplay: inline-block;\n\t\t\theight: 62px;\n\t\t}\n\t\t\t#nav ul li a{\n\t\t\t\tpadding: 20px;\n\t\t\t\tbackground: orange;\n\t\t\t\tcolor: white;\n\t\t\t}\n\t\t\t#nav ul li a:hover{\n\t\t\t\tbackground-color: #ffb424;\n\t\t\t\tbox-shadow: 0px 1px 1px #666;\n\t\t\t}\n\t\t\t#nav ul li a:active{ background-color: #ff8f00; }\n\n#content{\n\tmargin: 30px 0;\n\tbackground: white;\n\tpadding: 20px;\n\tclear: both;\n}\n#footer{\n\tborder-bottom: 1px #ccc solid;\n\tmargin-bottom: 10px;\n}\n\t#footer p{\n\t\ttext-align: right;\n\t\ttext-transform: uppercase;\n\t\tfont-size: 80%;\n\t\tcolor: grey;\n\t}\n\n/* multiple styles seperated by a , */\n#content,\nul li a{ box-shadow: 0px 1px 1px #999; }"};

#ifndef STASSID
#define STASSID "Dhira"
#define STAPSK  "Qwerty@123"
#endif

const char* ssid = STASSID;
const char* password = STAPSK; 

ESP8266WebServer server(80);

const int led = 13;

void handleRoot() {
  digitalWrite(led, 1);
  server.send_P(200, "text/html", index_html);
  digitalWrite(led, 0);
}

void handleCss(){
  server.send_P(200,"text/css",style_css);
}
void handleNotFound() {
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}

void setup(void) {
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);
  server.on("/style.css",handleCss);

  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });

  server.on("/gif", []() {
    static const uint8_t gif[] PROGMEM = {
      0x47, 0x49, 0x46, 0x38, 0x37, 0x61, 0x10, 0x00, 0x10, 0x00, 0x80, 0x01,
      0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0x2c, 0x00, 0x00, 0x00, 0x00,
      0x10, 0x00, 0x10, 0x00, 0x00, 0x02, 0x19, 0x8c, 0x8f, 0xa9, 0xcb, 0x9d,
      0x00, 0x5f, 0x74, 0xb4, 0x56, 0xb0, 0xb0, 0xd2, 0xf2, 0x35, 0x1e, 0x4c,
      0x0c, 0x24, 0x5a, 0xe6, 0x89, 0xa6, 0x4d, 0x01, 0x00, 0x3b
    };
    char gif_colored[sizeof(gif)];
    memcpy_P(gif_colored, gif, sizeof(gif));
    // Set the background to a random set of colors
    gif_colored[16] = millis() % 256;
    gif_colored[17] = millis() % 256;
    gif_colored[18] = millis() % 256;
    server.send(200, "image/gif", gif_colored, sizeof(gif_colored));
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
  MDNS.update();
}

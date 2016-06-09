# API

## colors
GET /colors?index=<index>&h=<hue>&s=<saturation>&l=<lightness>

set color at index to the given H,S,L value
index ranges from 0 to NUM_ANIMATIONS. The different colors are used by the effects in various ways

## effects
GET /effect?e=<effectname>

Response
{ "message": "<message from effect>" }

Response (error)
{ "error": "effect not found"}

### solid color
GET /effect?e=solid&speed=500

speed: LEDs are not set all at once. The Color flows from Start to End.

### lauflicht
GET /effect?e=lauflicht&speed=1000&fade=1

## effect info
GET /effects

- list of available effects
- TODO: parameters for each effect

## list files
GET /files
Response
{
  "info": {
    "size": 2949250,
    "used": 753
  },
  "files": [
    {
      "name": "/config.json",
      "size": 51
    }
  ]
}

## list WIFI configs

GET /wifi
Response:
[
  { "id"   : 1,
    "ssid" : "ssid1",
    "pass" : "pass1" },

  { "id"   : 2,
    "ssid" : "ssid2",
    "pass" : "pass2" },
]

## add WIFI config
POST /wifi/add
Request data:
{
  "ssid":"the-ssid",
  "pass":"the-password"
}

## delete WIFI config
GET /wifi/del/:id
deletes the wifi config at :id


GET /:filename
Response:
content of :filename


# TODO

## status
GET /status

- currently running effect
- current colors (done)
- uptime
- battery status
  - voltage
  - percentage (if fuel gauge connected)
- hardware config
  - number of LEDs
  - number of buttons

## change colors
POST /colors

Send up to 5 colors used by the effects
Example POST data:
{ colors: [
    {"h":0,"s":0,"l":0},
    {"h":0,"s":0,"l":0},
    {"h":0,"s":0,"l":0},
    {"h":0,"s":0,"l":0},
    {"h":0,"s":0,"l":0}
]}

Make configurable wia wifi:
- initial Animation
- multiple SSIDs

Control Panel
- set up to 10 colors


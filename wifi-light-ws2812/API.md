# API

## effects
GET /effect?e=<effectname>

Response
{ "message": "<message from effect>" }

Response (error)
{ "error": "effect not found"}

### solid color
GET /effect?e=solid&h=0.9&s=1&l=0.1&speed=500

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


# TODO

## status
GET /status

- currently running effect
- current colors (done)
- connection status
- uptime
- hardware config

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


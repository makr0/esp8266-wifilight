void SSDPSetup() {
  debug_print("Starting SSDP...");
  SSDP.begin();
  SSDP.setSchemaURL("description.xml");
  SSDP.setHTTPPort(80);
  SSDP.setDeviceType("wifilight");
//  SSDP.setName("Philips hue clone");
//  SSDP.setName("Philips hue ("+ ipString + ")");
  SSDP.setName("The eternal Flame");
//  SSDP.setSerialNumber("001788102201");
  SSDP.setSerialNumber("081547112342DEADBEEF");
  SSDP.setURL("index.html");
  SSDP.setModelName("Fnordlicht");
  SSDP.setModelNumber((String)pixelCount);
  SSDP.setModelURL("http://www.meethue.com");
  SSDP.setManufacturer("Unlimited Surprise Systems");
//  SSDP.setManufacturer("Royal Philips Electronics");
  SSDP.setManufacturerURL("http://www.philips.com");
  debug_println("SSDP Started");
}

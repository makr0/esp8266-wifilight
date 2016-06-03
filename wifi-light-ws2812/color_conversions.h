HsbColor hsl2philips_hsb(HslColor color)
{
  HsbColor hsb = HsbColor(color);
  hsb.H = map_range(hsb.H,0,1,0,65535);
  hsb.S = map_range(hsb.S,0,1,0,254);
  hsb.B = map_range(hsb.B,0,1,0,254);
  return (hsb);
}

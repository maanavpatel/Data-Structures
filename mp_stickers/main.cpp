#include "Image.h"
#include "StickerSheet.h"

int main() {

  //
  // Reminder:
  //   Before exiting main, save your creation to disk as myImage.png
  //
  Image alma;

  alma.readFromFile("alma.png");
  
  StickerSheet s(alma, 7);
  Image i;    
  Image j;    
  Image z;
  i.readFromFile("bluehotwheels.png");
  j.readFromFile("blackhotwheels.png");
  z.readFromFile("Zoom-App-Icon-2.png");
  j.grayscale();
  z.scale(0.5);
  i.scale(0.5);
  j.scale(0.5);
  s.addSticker(i, 0, 400);
  s.addSticker(j, 550, 400);
  s.addSticker(z, 300, 100);

  Image sheet = s.render();
  sheet.writeToFile("myImage.png");
  return 0;
}

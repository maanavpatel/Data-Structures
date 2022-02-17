/**
 * @file StickerSheet.h
 * Contains your declaration of the interface for the StickerSheet class.
 */
#pragma once
#include "Image.h"
#include "cs225/PNG.h"
#include <vector>
using std::vector;

struct Coordinate
{
  unsigned x;
  unsigned y;
};

struct Sticker {
  Coordinate pos;
  Image* img;
};

struct Level {
  Sticker s;
  bool hasSticker;
};

class StickerSheet{
    public:
        //picture is the base picture
        //max is the max number of stickers
        StickerSheet (const Image &picture, unsigned max);
        ~StickerSheet();
        StickerSheet(const StickerSheet & other);
        const StickerSheet& operator=(const StickerSheet & other);	

        int addSticker(Image & sticker, unsigned x, unsigned y);	
        void removeSticker(unsigned index);
        void changeMaxStickers(unsigned max);
        Image * getSticker(unsigned index);
        Image render() const;
        bool translate(unsigned index, unsigned x, unsigned y);
        
    private:
        unsigned max_stickers;
        unsigned sticker_count;
        // unsigned top_available_index;
        // Sticker** stickers;
        // std::vector<Image*> stickers;
        // std::vector<Coordinate> positions;
        // std::vector<bool> isLayerAvailable;
        std::vector<Level> levels;
        Image base_img;
        void clear();
        void copy(const StickerSheet & other);

};

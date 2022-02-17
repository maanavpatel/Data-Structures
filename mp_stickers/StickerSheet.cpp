#include "StickerSheet.h"


StickerSheet::StickerSheet(const Image &picture, unsigned max)
{
    base_img = (picture);
    max_stickers = max;
    sticker_count = 0;
    levels.resize(max);
    for (unsigned i = 0; i < levels.size(); i++) {
        levels[i].hasSticker = false;
    }
}

StickerSheet::StickerSheet(const StickerSheet & other)
{
    copy(other);
}

const StickerSheet& StickerSheet::operator=(const StickerSheet & other) {
    if (&other != this) {
        clear();
        copy(other);
    }
    return  *this;
}	

StickerSheet::~StickerSheet() 
{  
    clear();
}

void StickerSheet::clear() {
    levels.clear();
}

void StickerSheet::copy(const StickerSheet & other) {
    max_stickers =  other.max_stickers;
    base_img = (other.base_img);
    sticker_count = other.sticker_count;
    for (unsigned i = 0; i < other.levels.size(); i++) {
        Level l = other.levels[i];
        Image* sticker = &(*(other.levels[i].s.img));
        Coordinate c;
        c.x = l.s.pos.x;
        c.y = l.s.pos.y;
        Sticker s;
        s.img = sticker;
        s.pos = c;
        Level toAdd;
        toAdd.s = s;
        toAdd.hasSticker = l.hasSticker;
        levels.push_back(l);
    }
}

int StickerSheet::addSticker(Image & sticker, unsigned x, unsigned y) {
    if (sticker_count < max_stickers) {
        for (unsigned i = 0; i < levels.size(); i++) {
            //no sticker on current level
            if (!levels[i].hasSticker) {
                    Level& l = levels[i];
                    l.s.pos.x = x;
                    l.s.pos.y = y;
                    l.s.img = &sticker;
                    l.hasSticker = true;
                    sticker_count++;
                    // std::cout << "add sticker at " << i << std::endl;
                    return i;
            }
        }
    }
    //no open slot
    return -1;
}

void StickerSheet::removeSticker(unsigned index) 
{
    // std::cout << "removing sticker at " << index << std::endl;

    if (index < levels.size() && sticker_count != 0 && levels[index].hasSticker) {
        sticker_count--;
        levels[index].hasSticker = false;
        levels[index].s.img = nullptr;
    }
}

void StickerSheet::changeMaxStickers(unsigned max) 
{
    if (max > max_stickers) {
        max_stickers = max;
        levels.resize(max_stickers);
    } else if (max >= 0 && max < max_stickers)
    {
        max_stickers = max;
        levels.resize(max);
        sticker_count = 0;
        //recount how many levels have stickers
        for (unsigned i = 0; i < levels.size(); i++) {
            if (levels[i].hasSticker) {
                sticker_count++;
            }
        }
    }
}

Image* StickerSheet::getSticker(unsigned index) 
{
    if (index >= 0 && index < max_stickers && levels[index].hasSticker) {
        return levels[index].s.img;
    }
    return NULL;
}

Image StickerSheet::render() const
{
    if (sticker_count == 0) {
        // std::cout << "No stickers? sticker count: " << sticker_count << std::endl;
        return base_img;
    }
    // std::cout << "render called" << std::endl;
    Image render = Image();
    // std::cout << "render called" << __LINE__ << std::endl;

    render.resize(base_img.width(), base_img.height());
    //fill in base
    for (unsigned x = 0; x < base_img.width(); x++) {
        for (unsigned y = 0; y < base_img.height(); y++) {
            render.getPixel(x, y) = base_img.getPixel(x, y);    
        }
    }
    if (sticker_count > 0) {
        for (unsigned i = 0; i < levels.size(); i++) {
            //check if there is a sticker at this level
            if (levels[i].hasSticker) {
                Image* sticker = levels[i].s.img;
                Coordinate pos = levels[i].s.pos;
                for (unsigned x = 0; x < sticker->width(); x++) {
                    for (unsigned y = 0; y < sticker->height(); y++) {
                        //inbounds
                        if (x + pos.x < render.width() && y + pos.y < render.height()) {
                            if (sticker->getPixel(x, y).a != 0) {
                                render.getPixel(x + pos.x, y + pos.y) = sticker->getPixel(x, y);
                            }
                        } else { //need to resize the rendering image
                            if (pos.x + sticker->width() > render.width()) {
                                unsigned xboundary = pos.x + sticker->width();
                                render.resize(xboundary,  render.height());
                            }
                            if (pos.y + sticker->height() > render.height()) {
                                unsigned yboundary = pos.y + sticker->height();
                                render.resize(render.width(),  yboundary);
                            }
                            render.getPixel(x + pos.x, y + pos.y) = sticker->getPixel(x, y);
                        } 
                    }
                }
            }
        }
    }
    return render;
}

bool StickerSheet::translate(unsigned index, unsigned x, unsigned y) 
{
    if (index < max_stickers && levels[index].hasSticker) {
        (levels[index].s.pos).x = x;
        (levels[index].s.pos).y = y;
        return true;
    }
    return false;
}


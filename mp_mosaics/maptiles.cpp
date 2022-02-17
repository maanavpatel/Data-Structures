/**
 * @file maptiles.cpp
 * Code for the maptiles function.
 */

#include <iostream>
#include <map>
#include "maptiles.h"
//#include "cs225/RGB_HSL.h"

using namespace std;


Point<3> convertToXYZ(LUVAPixel pixel) {
    return Point<3>( pixel.l, pixel.u, pixel.v );
}

MosaicCanvas* mapTiles(SourceImage const& theSource,
                       vector<TileImage>& theTiles)
{
    MosaicCanvas* image = new MosaicCanvas(theSource.getRows(), theSource.getColumns());
	const int kDim = 3;

	vector<Point<kDim>> treePoints;
	treePoints.resize(theTiles.size()); 

	map<Point<kDim>, int> tiles;

	for (size_t i = 0; i < theTiles.size(); i++)
	{
		LUVAPixel pix = theTiles[i].getAverageColor();
		Point<kDim> p = convertToXYZ(pix);
		treePoints.push_back(p);
		pair<Point<kDim>, int> elem(p,i);
		tiles.insert(elem);
	}

	KDTree<kDim> imageTree(treePoints);

	for (int i = 0; i < theSource.getRows(); i++)
	{
		for (int j = 0; j < theSource.getColumns(); j++) {
			LUVAPixel p = theSource.getRegionColor(i, j);
			Point<kDim> searchFor = convertToXYZ(p);
			Point<kDim> resultTile = imageTree.findNearestNeighbor(searchFor);
			image->setTile(i, j, &(theTiles[tiles[resultTile]]));
		}
	}

	/* return the MosaicCanvas. */
	return image;
}


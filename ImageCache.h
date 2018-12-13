#pragma once

#include "image.h"
#include <vector>
using namespace std;
class CachedImage
{
public:
	int parentID;
	int cacheID;
	Image* img;
	CachedImage(int pid, int ID, Image* theImage);
	~CachedImage();
};

class ImageCache
{
public:
	ImageCache();
	~ImageCache();
	static ImageCache* CacheManager;
	vector<int> IDs;
	int GetNewParentID();
	bool CheckID(int id);
	bool SetCachedImage(int pid, int id, Image* newCache);
	Image* GetCachedImage(int id);
	vector<CachedImage*> imageList; 
};


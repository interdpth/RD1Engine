#include "ImageCache.h"



CachedImage::CachedImage(int pid, int ID, Image* theImage)
{
	img = theImage;
	parentID = pid;
	cacheID = ID;
}


CachedImage::~CachedImage()
{

}


ImageCache::ImageCache()
{

}

bool ImageCache::CheckID(int id)
{
	for each(CachedImage* ci in imageList)
	{
		if (id == ci->cacheID)
		{
			return true;
		}
	}
	return false;
}



bool ImageCache::SetCachedImage(int pid, int id, Image* newCache)
{
	for each(CachedImage* ci in imageList)
	{
		if (id == ci->cacheID)
		{
			return false;
		}
	}

	imageList.push_back(new CachedImage(pid, id, newCache));
	return NULL;
}

Image* ImageCache::GetCachedImage(int id)
{
	for each(CachedImage* ci in imageList)
	{
		if (id == ci->cacheID)
		{
			return ci->img;
		}
	}
	return NULL;
}



ImageCache::~ImageCache()
{
}


int ImageCache::GetNewParentID()
{
	return imageList.size();
}
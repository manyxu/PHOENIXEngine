// PX2ResourceManager.inl

//----------------------------------------------------------------------------
inline CurlObj *ResourceManager::GetCurlObj()
{
	return mCurl;
}
//----------------------------------------------------------------------------
inline void ResourceManager::DDSKeepCompressed (bool keep)
{
	mDDSKeepCompressed = keep;
}
//----------------------------------------------------------------------------
inline bool ResourceManager::IsDDSKeepCompressed ()
{
	return mDDSKeepCompressed;
}
//----------------------------------------------------------------------------
inline bool ResourceManager::IsUseGarbageCollect () const
{
	return mIsUseGarbageCollect;
}
//----------------------------------------------------------------------------
inline void ResourceManager::SetGarbageCollectTime (float seconds)
{
	mGarbageCollectTime = seconds;
}
//----------------------------------------------------------------------------
inline float ResourceManager::GetGarbageCollectTime () const
{
	return mGarbageCollectTime;
}
//----------------------------------------------------------------------------
inline const ResourceVersionItem *ResourceManager::GetDataVersionItem () const
{
	return mDataVersionItem;
}
//----------------------------------------------------------------------------
inline const ResourceVersionItem *ResourceManager::GetDataUpdateVersionItem () const
{
	return mDataUpdateVersionItem;
}
//----------------------------------------------------------------------------
inline const ResourceVersionItem *ResourceManager::GetUpdateVersionItem () const
{
	return mUpdateVersionItem;
}
//----------------------------------------------------------------------------
inline ResourceManager::VersionListTable &ResourceManager::
	GetDataVersionList ()
{
	return mDataVersionList;
}
//----------------------------------------------------------------------------
inline ResourceManager::VersionListTable &ResourceManager::
	GetDataUpdateVersionList ()
{
	return mDataUpdateVersionList;
}
//----------------------------------------------------------------------------
inline ResourceManager::VersionListTable &ResourceManager::
	GetUpdateVersionList ()
{
	return mUpdateVersionList;
}
//----------------------------------------------------------------------------
inline void ResourceManager::SetVersion (const ResourceVersion &version)
{
	mVersion = version;
}
//----------------------------------------------------------------------------
inline const ResourceVersion &ResourceManager::GetVersion () const
{
	return mVersion;
}
//----------------------------------------------------------------------------
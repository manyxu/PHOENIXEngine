// PX2UIProgressBar.inl

//----------------------------------------------------------------------------
inline UIFPicBox *UIProgressBar::GetBackPicBox()
{
	return mBackPicBox;
}
//----------------------------------------------------------------------------
inline UIFPicBox *UIProgressBar::GetProgressPicBox()
{
	return mProgressPicBox;
}
//----------------------------------------------------------------------------
inline UIFPicBox *UIProgressBar::GetOverPicBox()
{
	return mOverPicBox;
}
//----------------------------------------------------------------------------
inline float UIProgressBar::GetProgress() const
{
	return mProgress;
}
//----------------------------------------------------------------------------
inline UIProgressBarCtrl *UIProgressBar::GetProgressBarCtrl()
{
	return mPBCtrl;
}
//----------------------------------------------------------------------------
/*
 Copyright (c) 2012, The Cinder Project, All rights reserved.

 This code is intended for use with the Cinder C++ library: http://libcinder.org

 Redistribution and use in source and binary forms, with or without modification, are permitted provided that
 the following conditions are met:

    * Redistributions of source code must retain the above copyright notice, this list of conditions and
	the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and
	the following disclaimer in the documentation and/or other materials provided with the distribution.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
 WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
*/

#pragma once

#include <windows.h>
#undef min
#undef max

#include "cinder/app/msw/AppImplMsw.h"
#include "cinder/app/msw/AppMsw.h"
#include "cinder/app/Window.h"
#include "cinder/Display.h"

namespace cinder { namespace app {

class WindowImplMswBasic;

class AppImplMswBasic : public AppImplMsw {

private:
	void RenderWindows( int runtimeSyncStage = 0 );
	void SwapBuffers();

  public:
	AppImplMswBasic( AppMsw *app, const AppMsw::Settings &settings );

	void	run();
	//void	runV2();
	void	runV3();

	AppMsw*	getApp() { return mApp; }
	
	void	quit() override;

	void	setFrameRate( float frameRate ) override;
	void syncNewFrame() override;
	void syncSwapFrame() override;
	void	setSyncRole(int nrole) override;
	void	epochReset(float offset = 0.f) override;
	void	enableAutoEpochReset(bool val = true) override;
	void	enableFrameRate();
	void	disableFrameRate();
	void	setDebugFlag( int val );
	void	setBaseFrameNumber(uint32_t n);
	uint32_t	getBaseFrameNumber();
	bool	isFrameRateEnabled() const;

	void	setAppTickNumber(uint32_t n);
	uint32_t	getAppTickNumber();
	void setEngineVSync( bool val );
	bool getEngineVSync();

	size_t		getNumWindows() const;
	WindowRef	getWindowIndex( size_t index );
	WindowRef	getForegroundWindow() const;
	
	void		setupBlankingWindows( DisplayRef fullScreenDisplay );
	void		destroyBlankingWindows();
private:
	std::mutex frameUpdate_mutex;
	std::mutex frameSwap_mutex;
	std::condition_variable frameUpdate_wait;	
	std::condition_variable frameSwap_wait;
private:
	std::vector<HDC> swapGroupHDCs;
	int usingSwapGroupInt = -1;
	unsigned int swapGroupID = 1;
	bool joinSwapGroupNVEx(HDC hdc);
	void leaveSwapGroupNVEx(HDC hdc);
  private:
	void		sleep( double seconds );

	int globalWindowIndex = 0;

	WindowRef		createWindow( Window::Format format );
	RendererRef		findSharedRenderer( const RendererRef &searchRenderer );
	void			closeWindow( class WindowImplMsw *windowImpl ) override;
	void			customCloseWindow(class WindowImplMsw* windowImpl) override;
	void			customWMNCDownEvent(class WindowImplMsw* windowImpl) override;
	void			customWMNCUpEvent(class WindowImplMsw* windowImpl) override;
	void			setForegroundWindow( WindowRef window ) override;
	
	AppMsw*	mApp;
	HINSTANCE		mInstance;
	double			mNextFrameTime;
	bool			mResetFramePacer = false;
	bool			mFrameRateEnabled;
	bool			mAutoOffset = false;
	bool			mEngineVsync = true;
	int				mTestFlag = 0;
	uint32_t		mTriggerFrame = 0;
	uint32_t		mBaseFrameNumber = 0;
	uint32_t		mAppTickNumber = 0;
	bool			mShouldQuit;
	bool			mQuitOnLastWindowClosed;

	std::list<class WindowImplMswBasic*>	mWindows;
	std::list<BlankingWindowRef>			mBlankingWindows;
	WindowRef								mForegroundWindow;

	friend LRESULT CALLBACK WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	friend class AppMsw;
};

class WindowImplMswBasic : public WindowImplMsw {
  public:
	WindowImplMswBasic( const Window::Format &format, RendererRef sharedRenderer, AppImplMswBasic *appImpl )
		: WindowImplMsw( format, sharedRenderer, appImpl ), mAppImplBasic( appImpl ) {}

	void toggleFullScreen( const app::FullScreenOptions &options ) override;

  protected:
	AppImplMswBasic		*mAppImplBasic;
	friend AppImplMswBasic;
};

} } // namespace cinder::app
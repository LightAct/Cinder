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

#include "cinder/app/msw/AppImplMswBasic.h"
#include "cinder/app/msw/RendererImplMsw.h"
#include "cinder/app/msw/PlatformMsw.h"
#include "cinder/Utilities.h"

#include <glad/glad.h>
#include <glad/glad_wgl.h> // For WGL extensions

#include <windowsx.h>
#include <winuser.h>

#ifdef _DEBUG
#include <iostream>
#include <fstream>
#endif // _DEBUG


using std::vector;
using std::string;

namespace cinder { namespace app {

AppImplMswBasic::AppImplMswBasic( AppMsw *app, const AppMsw::Settings &settings )
	: AppImplMsw( app ), mApp( app )
{
	mShouldQuit = false;

	mFrameRate = settings.getFrameRate();
	mFrameRateEnabled = settings.isFrameRateEnabled();
	mQuitOnLastWindowClosed = settings.isQuitOnLastWindowCloseEnabled();

	auto formats = settings.getWindowFormats();
	if( formats.empty() )
		formats.push_back( settings.getDefaultWindowFormat() );

	for( auto &format : formats ) {
		if( ! format.isTitleSpecified() )
			format.setTitle( settings.getTitle() );

		createWindow( format );
	}
}

void AppImplMswBasic::run()
{
//	mApp->privateSetup__();
//	mSetupHasBeenCalled = true;
//
//	// issue initial app activation event
//	mApp->emitDidBecomeActive();
//
//	for( auto &window : mWindows )
//		window->resize();
//
//	// initialize our next frame time
//	mNextFrameTime = getElapsedSeconds();
//	
//	epochResetCounter = 0;
//	int nextFrameCounter = 0;	
//
//	// inner loop
//	while( !mShouldQuit ) {
//
//		// when in sync mode, wait for trigger		
//		if ( mSyncRole == 1 || mSyncRole == 2 ) {
//			std::unique_lock lk(frame_mutex);
//			frame_wait.wait(lk, [this] { return mSyncNextFrame; });
//			// unlock for next frame
//			mSyncNextFrame = false;
//		}
//
//		// calculate time per frame in seconds
//		const double secondsPerFrame = 1.0 / (double)mFrameRate;
//		const unsigned int epochResetter = epochResetCounter;
//		// mApp->privateBeginFrame__();
//
//		// all of our Windows will have marked this as true if the user has unplugged, plugged or modified a Monitor
//		if( mNeedsToRefreshDisplays ) {
//			mNeedsToRefreshDisplays = false;
//			PlatformMsw::get()->refreshDisplays();
//			// if this app is high-DPI aware, we need to issue resizes with possible contentScale changes
//			if( getHighDensityDisplayEnabled() )
//				for( auto &window : mWindows )
//					window->resize();
//		}
//
//		// update and draw		
//		mApp->privateUpdate__();
//
//		double drawTime = mApp->getElapsedSeconds();
//		for( auto &window : mWindows ) {
//			if( ! mShouldQuit ) // test for quit() issued either from update() or prior draw()
//				window->redraw();
//		}
//		mBaseFrameNumber++; // update control point
//		drawTime = mApp->getElapsedSeconds() - drawTime;
//		if (mAutoEpochReset && mFrameRateEnabled) {
//			if (drawTime > secondsPerFrame) {
//				epochResetCounter++;
//			}
//		}
//		//// trigger reset
//		//if (epochResetter != epochResetCounter)
//		//	mEpochReset = true;
//
//		// everything done
//		// everything done
//		// mApp->privateEndSwap__();
//
//		if (mEpochOffset != 0.f) {
//			mNextFrameTime = mApp->getElapsedSeconds();
//			mNextFrameTime += mEpochOffset * 0.001f;
//			mEpochOffset = 0.f;
//		}
//
//		// get current time in seconds
//		double currentSeconds = mApp->getElapsedSeconds();
//
//		// determine if application was frozen for a while and adjust next frame time		
//		double elapsedSeconds = currentSeconds - mNextFrameTime;
//		if( elapsedSeconds > 1.0 ) {
//			int numSkipFrames = (int)(elapsedSeconds / secondsPerFrame);
//			mNextFrameTime += (numSkipFrames * secondsPerFrame);
//		}
//
//		// determine when next frame should be drawn
//		mNextFrameTime += secondsPerFrame;
//		bool makeCinderSleep = mFrameRateEnabled;
//		if (mNextFrameTime > currentSeconds) {
//			if( mSyncRole == 2) {
//				makeCinderSleep = false;
//			}
//		} else {
//			makeCinderSleep = false;
//		}
//		if (makeCinderSleep) {
//			const double cinderSleep = mNextFrameTime - currentSeconds;
//			sleep(cinderSleep);
//		} else {
//			MSG msg;
//			while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
//				::TranslateMessage(&msg);
//				::DispatchMessage(&msg);
//			}
//		}
//		// mApp->privateEndFrame__();
//
//	}
//
////	killWindow( mFullScreen );
//	mApp->emitCleanup();
//	delete mApp;
}
void AppImplMswBasic::RenderWindows( int runtimeSyncStage ) {

	bool redraw = true;
	if ( runtimeSyncStage == 2 ) {
		redraw = mWindows.size() == 1;
	}
	for (auto window : mWindows) {
		if (!mShouldQuit && redraw) {
			window->redraw();
		}
		redraw = true;
	}

	/*if ( runtimeSyncStage == 2 || runtimeSyncStage == 3) {
		bool redraw = mWindows.size() == 1;
		for (auto window : mWindows) {
			if (!mShouldQuit && redraw) {
				window->redraw();
			}
			redraw = true;
		}
	} 
	bool redraw = !skipDefaultWindow;
	if (!redraw)
		redraw = mWindows.size() == 1;

	for (auto window : mWindows) {
		if (!mShouldQuit && redraw) {
			window->redraw();
		}
		redraw = true;
	}*/
	/*
	for (auto window : mWindows) {
		if (!mShouldQuit) {
			window->redraw();
		}		
	}
	*/
	glFinish();
}
void AppImplMswBasic::SwapBuffers() {

	for (auto window : mWindows) {
		if (!mShouldQuit) {
			window->getRenderer()->makeCurrentContext();
			window->getRenderer()->finishDraw();
		}
	}

	return;
	if (mWindows.size() > 1) {
		// output windows first:
		for (auto window : mWindows) {
			if (!mShouldQuit && window != mWindows.front()) {
				window->getRenderer()->makeCurrentContext();
				window->getRenderer()->finishDraw();
			}
		}		
	}	
	{
		// output window:
		if (!mShouldQuit) {
			mWindows.front()->getRenderer()->makeCurrentContext();
			mWindows.front()->getRenderer()->finishDraw();
		}
	}
}

//void AppImplMswBasic::runV2()
//{

//	mApp->privateSetup__();
//	mSetupHasBeenCalled = true;
//
//	// issue initial app activation event
//	mApp->emitDidBecomeActive();
//
//	for (auto& window : mWindows)
//		window->resize();
//
//	// initialize our next frame time
//	mNextFrameTime = getElapsedSeconds();
//	epochResetCounter = 0;
//	size_t mWindowsSize = 1;
//
//	// entire frame drugation (prev.end till current.end)
//	auto fullFrameProfile = std::chrono::high_resolution_clock::now();
//	// inner frame duration
//	auto frameProfiler = std::chrono::high_resolution_clock::now();
//
//	// inner loop
//	while (!mShouldQuit) {
//
//		fullFrameProfile = std::chrono::high_resolution_clock::now();
//
//		// calculate time per frame in seconds
//		const double secondsPerFrame = 1.0 / (double)mFrameRate;
//		const unsigned int epochResetter = epochResetCounter;
//
//		// all of our Windows will have marked this as true if the user has unplugged, plugged or modified a Monitor
//		if (mNeedsToRefreshDisplays) {
//			mNeedsToRefreshDisplays = false;
//			PlatformMsw::get()->refreshDisplays();
//			// if this app is high-DPI aware, we need to issue resizes with possible contentScale changes
//			if (getHighDensityDisplayEnabled())
//				for (auto& window : mWindows)
//					window->resize();
//		}
//
//		//if (mWindowsSize != mWindows.size()) {
//		//	mWindowsSize = mWindows.size();
//		//	/*GLint enable = (mWindowsSize == 1) ? 1 : 0;
//		//	if (WGL_EXT_swap_control) {
//		//		for (auto& window : mWindows) {
//		//			window->getRenderer()->makeCurrentContext();
//		//			::wglSwapIntervalEXT(enable);
//		//			enable = 1;
//		//		}				
//		//	}
//		//	wglDelayBeforeSwapNV(mWindows.front()->getDc(), 0.01f);*/
//		//	// setDebugFlag( 7 );
//		//}
//
//		frameProfiler = std::chrono::high_resolution_clock::now();
//
//		// when in sync mode, wait for trigger		
//		if (mSyncRole == 1 || mSyncRole == 2) {
//			std::unique_lock lk(frame_mutex);
//			frame_wait.wait(lk, [this] { return mSyncNextFrame; });
//			// unlock for next frame
//			mSyncNextFrame = false;
//		}
//
//		mApp->mFrameProfile[3] = (uint32_t)std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - frameProfiler).count();
//		{
//			MSG msg;
//			while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
//				::TranslateMessage(&msg);
//				::DispatchMessage(&msg);
//			}
//		}
//#pragma region "UPDATE"
//		{
//
//			frameProfiler = std::chrono::high_resolution_clock::now();
//			mApp->privateUpdate__(mSyncRole != 2);			
//			mApp->mFrameProfile[0] = (uint32_t)std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - frameProfiler).count();
//		}
//#pragma endregion
//#pragma region "DRAW + SWAP"
//		{ // draw
//			frameProfiler = std::chrono::high_resolution_clock::now();
//			RenderWindows();
//			mApp->mFrameProfile[1] = (uint32_t)std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - frameProfiler).count();
//		}
//#pragma endregion		
//
//		mBaseFrameNumber++;
//
//		// get current time in seconds
//		double currentSeconds = getElapsedSeconds();
//		// for sleep time
//		frameProfiler = std::chrono::high_resolution_clock::now();
//
//		// determine if application was frozen for a while and adjust next frame time				
//		double elapsedSeconds = currentSeconds - mNextFrameTime;
//		if (elapsedSeconds > 1.0) {
//			int numSkipFrames = (int)(elapsedSeconds / secondsPerFrame);
//			mNextFrameTime += (numSkipFrames * secondsPerFrame);
//		}
//		// next frame modification
//
//		bool appendDefault = true;
//		//if(mTestFlag != 0) {
//		//	if(mTriggerFrame == (int)getElapsedFrames()) {
//		//		if(mTestFlag == 3) {
//		//			// reset / same as setting application framerate
//		//			mNextFrameTime = currentSeconds;
//		//		} else if (mTestFlag >= 4 && mTestFlag <= 6) {
//		//			// allow some sleep
//		//			mNextFrameTime = currentSeconds + (0.004 + (mTestFlag - 4) * 0.002);
//		//		} else if (mTestFlag == 7) {
//		//			// move onto vblank only
//		//			mNextFrameTime = currentSeconds - 0.500;
//		//		}
//		//		mTestFlag = 0;
//		//		appendDefault = false;
//		//	}
//		//}
//		//// default workflow
//		//if(appendDefault){
//		//	mNextFrameTime += secondsPerFrame;
//		//}
//
//		mNextFrameTime += secondsPerFrame;
//
//		bool makeCinderSleep = mFrameRateEnabled;
//		if (mNextFrameTime > currentSeconds) {
//			if (mSyncRole == 2) {
//				makeCinderSleep = false;
//			}
//		}
//		else {
//			makeCinderSleep = false;
//		}
//
//		// mWindows.front()->getRenderer()->makeCurrentContext();
//
//		if (makeCinderSleep) {
//
//			// sleep time for frame
//			const double cinderSleep = mNextFrameTime - currentSeconds;
//			mApp->mFrameProfile[2] = (int)(cinderSleep * 1000000.0);
//
//			// sleep(cinderSleep);		
//			const int sleepDuration = (int)(cinderSleep * 1000000.0);
//			std::this_thread::sleep_for(std::chrono::microseconds(sleepDuration));
//
//		}
//		else {
//
//			mApp->mFrameProfile[2] = 0;
//			/*MSG msg;
//			while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
//				::TranslateMessage(&msg);
//				::DispatchMessage(&msg);
//			}*/
//		}
//		mApp->mFrameProfile[4] = (uint32_t)std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - fullFrameProfile).count();
//
//		if (mSyncRole == 2) { /* we are lead by someone else */ }
//		else {
//			mAppTickNumber++;
//		}
//
//		mApp->cinderFrameDone();
//		mApp->privateEndFrame__();
//
//	}
//
//	//	killWindow( mFullScreen );
//	mApp->emitCleanup();
//	delete mApp;
//}

void AppImplMswBasic::runV3() {

	mApp->privateSetup__();
	mSetupHasBeenCalled = true;

	// issue initial app activation event
	mApp->emitDidBecomeActive();

	for (auto& window : mWindows)
		window->resize();

	// initialize our next frame time
	mNextFrameTime = getElapsedSeconds();
	epochResetCounter = 0;

	// inner frame duration
	auto frameProfiler = std::chrono::high_resolution_clock::now();
	auto fullFrameProfiler = std::chrono::high_resolution_clock::now();
	
	// default to primary or standalone run
	int runtimeSyncStage = 0;

	size_t windowsCount = 1;
	bool windowsVsync = mEngineVsync;

	// inner loop
	while (!mShouldQuit) {

		fullFrameProfiler = std::chrono::high_resolution_clock::now();
				
		mResetFramePacer = (runtimeSyncStage != mSyncRole);
		runtimeSyncStage = mSyncRole;

		// calculate time per frame in seconds
		const double secondsPerFrame = 1.0 / (double)mFrameRate;

		// all of our Windows will have marked this as true if the user has unplugged, plugged or modified a Monitor
		if (mNeedsToRefreshDisplays) {
			mNeedsToRefreshDisplays = false;
			PlatformMsw::get()->refreshDisplays();
			// if this app is high-DPI aware, we need to issue resizes with possible contentScale changes
			if (getHighDensityDisplayEnabled())
				for (auto& window : mWindows)
					window->resize();
		}

		// primary mode mode switches
		// either by adding windows or changing vsync mode
		if ( windowsCount != mWindows.size() || windowsVsync != mEngineVsync ) {
			windowsCount = mWindows.size();
			windowsVsync = mEngineVsync;
			if (WGL_EXT_swap_control) {
				// turn all windows vsync off
				for (auto& window : mWindows) {
					window->getRenderer()->makeCurrentContext();
					::wglSwapIntervalEXT((GLint)0);
				}
				// if we are supposed to have vsync on, turn last window on
				if(windowsVsync) {
					mWindows.back()->getRenderer()->makeCurrentContext();
					::wglSwapIntervalEXT((GLint)1);
				}
			}
			mResetFramePacer = true;
		}

		frameProfiler = std::chrono::high_resolution_clock::now();		
		// when in sync mode, wait for trigger		
		if (runtimeSyncStage == 2 || runtimeSyncStage == 3) {
			std::unique_lock lk(frameUpdate_mutex);
			frameUpdate_wait.wait_for(lk, std::chrono::milliseconds(100), [this] { return mSyncNextFrame; });
			mSyncNextFrame = false;
		}
		mApp->mFrameProfile[3] = (uint32_t)std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - frameProfiler).count();

#pragma region "UPDATE"
		{
			mApp->privateUpdate_0__();
			frameProfiler = std::chrono::high_resolution_clock::now();
			mApp->privateUpdate_1__();			
			mApp->privateUpdate_2__();
			mApp->mFrameProfile[0] = (uint32_t)std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - frameProfiler).count();
		}
#pragma endregion
#pragma region "DRAW (without swap)"
		{ 
			// draw
			// frameProfiler = std::chrono::high_resolution_clock::now();
			mApp->mFrameProfile[1] = 0;
			RenderWindows( );
			// mApp->mFrameProfile[1] = (uint32_t)std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - frameProfiler).count();
			for (auto window : mWindows) {
				mApp->mFrameProfile[1] += window->getWindow()->getRenderTime();
			}
		}
#pragma endregion	

		mBaseFrameNumber++;
		mApp->cinderFrameUpdatedAndRendered();

		frameProfiler = std::chrono::high_resolution_clock::now();
		// waits for swap command
		if ( runtimeSyncStage == 2 || runtimeSyncStage == 3) {
			std::unique_lock lk(frameSwap_mutex);
			frameSwap_wait.wait_for(lk, std::chrono::milliseconds(100), [this] { return mSyncSwapFrame; });
			mSyncSwapFrame = false;
		}
		mApp->mFrameProfile[3] += 
			(uint32_t)std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - frameProfiler).count();		

#pragma region "SWAP"
		{
			// frameProfiler = std::chrono::high_resolution_clock::now();
			SwapBuffers();
			// mApp->mFrameProfile[1] += (uint32_t)std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - frameProfiler).count();
		}
#pragma endregion	

		// get current time in seconds
		double currentSeconds = getElapsedSeconds();
		if (mResetFramePacer) {
			mNextFrameTime = currentSeconds - 0.002;
			mResetFramePacer = false;
		}
		// for sleep time
		frameProfiler = std::chrono::high_resolution_clock::now();

		// determine if application was frozen for a while and adjust next frame time				
		/*double elapsedSeconds = currentSeconds - mNextFrameTime;
		if (elapsedSeconds > 1.0) {
			int numSkipFrames = (int)(elapsedSeconds / secondsPerFrame);
			mNextFrameTime += (numSkipFrames * secondsPerFrame);
		}
		mNextFrameTime += secondsPerFrame;*/

		mNextFrameTime += secondsPerFrame;
		bool makeCinderSleep = mFrameRateEnabled;
		if(makeCinderSleep) {
			// next frame time fell behind current seconds
			if (currentSeconds > mNextFrameTime) {
				double frames_behind = floor((currentSeconds - mNextFrameTime) / secondsPerFrame) + 1;
				mNextFrameTime += frames_behind * secondsPerFrame;
			}
		}		
		if (mNextFrameTime > currentSeconds) {
			if(runtimeSyncStage == 0) { 
				if (windowsCount > 1)
					makeCinderSleep = false;
			} else if(runtimeSyncStage == 1 /* as primary in sync */) {
				if (windowsCount > 1)
					makeCinderSleep = false;
			} else if(runtimeSyncStage == 2 /* as secondary in sysnc */) {
				makeCinderSleep = false;
			}
		} else {
			makeCinderSleep = false;
		}

		if (makeCinderSleep) {

			// sleep time for frame
			const double cinderSleep = mNextFrameTime - currentSeconds;
			const int sleepDuration = (int)(cinderSleep * 1000000.0);
			mApp->mFrameProfile[2] = sleepDuration;
			sleep(cinderSleep);					
			// std::this_thread::sleep_for(std::chrono::microseconds(sleepDuration));

		} else {

			mApp->mFrameProfile[2] = 0;
			MSG msg;
			while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}
		}

		if ( runtimeSyncStage == 2 || runtimeSyncStage == 3 ) { /* we are lead by someone else */ }
		else { mAppTickNumber++; }

		// generally not needed
		mBaseFrameNumber++;
		mApp->cinderFrameDone();

		// as primary, wait for swap done command
		// accumulate as wait time
		frameProfiler = std::chrono::high_resolution_clock::now();
		if (runtimeSyncStage == 1) {
			std::unique_lock lk(frameSwap_mutex);
			frameSwap_wait.wait_for(lk, std::chrono::milliseconds(200), [this] { return mSyncSwapFrame; });
			mSyncSwapFrame = false;
		}
		const uint32_t waitTime = (uint32_t)std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - frameProfiler).count();
		mApp->mFrameProfile[3] += waitTime;	

		// push stats out
		mApp->privateEndFrame__();

	}

	//	killWindow( mFullScreen );
	mApp->emitCleanup();
	delete mApp;
}

void AppImplMswBasic::sleep( double seconds )
{
	// create waitable timer
	static HANDLE timer = ::CreateWaitableTimer( NULL, FALSE, NULL );

	// specify relative wait time in units of 100 nanoseconds
	LARGE_INTEGER waitTime;
	waitTime.QuadPart = (LONGLONG)(seconds * -10000000);
	if(waitTime.QuadPart >= 0) return;

	// activate waitable timer
	if ( !::SetWaitableTimer( timer, &waitTime, 0, NULL, NULL, FALSE ) )
		return;

	// handle events until specified time has elapsed
	DWORD result;
	MSG msg;
	while( ! mShouldQuit ) {
		result = ::MsgWaitForMultipleObjects( 1, &timer, false, INFINITE, QS_ALLINPUT );
		if( result == (WAIT_OBJECT_0 + 1) ) {
			// execute messages as soon as they arrive
			while( ::PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) ) {
				::TranslateMessage( &msg );
				::DispatchMessage( &msg );
			}
			// resume waiting
		}
		else return; // time has elapsed
	}
}

RendererRef AppImplMswBasic::findSharedRenderer( const RendererRef &searchRenderer )
{
	if( ! searchRenderer )
		return RendererRef();

	for( const auto &win : mWindows ) {
		RendererRef renderer = win->getRenderer();
		if( renderer && ( typeid(*renderer) == typeid(*searchRenderer) ) )
			return renderer;
	}

	return RendererRef();
}
// Add the HDC to the list
bool AppImplMswBasic::joinSwapGroupNVEx(HDC hdc) {

	// don't add multiple instances
	auto it = std::find(swapGroupHDCs.begin(), swapGroupHDCs.end(), hdc);
	if (it != swapGroupHDCs.end())
		return false;
	
	if ( !wglJoinSwapGroupNV(hdc, swapGroupID) ) {
		// std::cerr << "Failed to join swap group for window!" << std::endl;
		return false;
	}
	swapGroupHDCs.push_back(hdc);
	return true;
}
// Remove the HDC from the list
void AppImplMswBasic::leaveSwapGroupNVEx(HDC hdc) {
	if ( !wglJoinSwapGroupNV(hdc, 0) ) {
		std::cerr << "Failed to leave swap group for window!" << std::endl;
		return;
	}	
	auto it = std::find(swapGroupHDCs.begin(), swapGroupHDCs.end(), hdc);
	if (it != swapGroupHDCs.end()) {
		swapGroupHDCs.erase(it);
	}
}
WindowRef AppImplMswBasic::createWindow( Window::Format format )
{
	if( ! format.getRenderer() )
		format.setRenderer( mApp->getDefaultRenderer()->clone() );

	WindowImplMswBasic* newWindow = new WindowImplMswBasic(format, findSharedRenderer(format.getRenderer()), this);
	newWindow->setWindowIndex(globalWindowIndex++);
	mWindows.push_back(newWindow);

	// emit initial resize if we have fired setup
	if( mSetupHasBeenCalled )
		mWindows.back()->getWindow()->emitResize();
	return mWindows.back()->getWindow();
}

void AppImplMswBasic::customCloseWindow(WindowImplMsw* windowImpl) {
	windowImpl->getWindow()->emitCustomClose();
}

void AppImplMswBasic::customWMNCDownEvent(class WindowImplMsw* windowImpl) {
	windowImpl->getWindow()->emitCustomWMNCDown();
}

void AppImplMswBasic::customWMNCUpEvent(class WindowImplMsw* windowImpl) {
	windowImpl->getWindow()->emitCustomWMNCUp();
}

void AppImplMswBasic::closeWindow( WindowImplMsw *windowImpl )
{

	if(usingSwapGroupInt == 1) {
		leaveSwapGroupNVEx(windowImpl->getDc());
	}

	auto winIt = find( mWindows.begin(), mWindows.end(), windowImpl );
	if( winIt != mWindows.end() ) {
		windowImpl->getWindow()->emitClose();
		windowImpl->privateClose();
		delete windowImpl; // this corresponds to winIt
		mWindows.erase( winIt );
	}

	if( mWindows.empty() && mQuitOnLastWindowClosed )
		mShouldQuit = true;
}

size_t AppImplMswBasic::getNumWindows() const
{
	return mWindows.size();
}

WindowRef AppImplMswBasic::getWindowIndex( size_t index )
{
	if( index >= mWindows.size() )
		return cinder::app::WindowRef();
	
	auto winIt = mWindows.begin();
	std::advance( winIt, index );
	return (*winIt)->mWindowRef;
}

WindowRef AppImplMswBasic::getForegroundWindow() const
{
	return mForegroundWindow;
}

void AppImplMswBasic::setForegroundWindow( WindowRef window )
{
	mForegroundWindow = window;
}

// This creates a full-screen blanking (all black) Window on each display besides 'fullScreenDisplay'
void AppImplMswBasic::setupBlankingWindows( DisplayRef fullScreenDisplay )
{
	destroyBlankingWindows();

	for( auto &display : Display::getDisplays() ) {
		if( display == fullScreenDisplay )
			continue;

		mBlankingWindows.push_back( BlankingWindowRef( new BlankingWindow( display ) ) );
	}
}

void AppImplMswBasic::destroyBlankingWindows()
{
	for( auto &win : mBlankingWindows )
		win->destroy();

	mBlankingWindows.clear();
}

void AppImplMswBasic::quit()
{
	if( ! mApp->privateEmitShouldQuit() )
		return;
	// Always quit, even if ! isQuitOnLastWindowCloseEnabled()
	mShouldQuit = true;
}

void AppImplMswBasic::setFrameRate( float frameRate )
{
	mFrameRate = frameRate;
	// mFrameRateEnabled = true;
	mResetFramePacer = true;
	// mNextFrameTime = mApp->getElapsedSeconds();
}
void AppImplMswBasic::syncNewFrame()
{
	{
		std::lock_guard lk(frameUpdate_mutex);
		mSyncNextFrame = true;		
	}
	frameUpdate_wait.notify_one();
}
void AppImplMswBasic::syncSwapFrame()
{
	{
		std::lock_guard lk(frameSwap_mutex);
		mSyncSwapFrame = true;
	}
	frameSwap_wait.notify_one();
}
void AppImplMswBasic::setSyncRole(int nrole) 
{
	mSyncRole = nrole;
}

void AppImplMswBasic::enableAutoEpochReset(bool val) 
{
	mAutoEpochReset = val;
}
void AppImplMswBasic::epochReset(float offset) 
{
	mEpochOffset = offset;
}
void AppImplMswBasic::setBaseFrameNumber(uint32_t n) 
{
	mBaseFrameNumber = n;
}
uint32_t AppImplMswBasic::getBaseFrameNumber() 
{
	return mBaseFrameNumber;
}
void AppImplMswBasic::setAppTickNumber(uint32_t n)
{
	mAppTickNumber = n;
}
uint32_t AppImplMswBasic::getAppTickNumber()
{
	return mAppTickNumber;
}
void AppImplMswBasic::setEngineVSync(bool val) 
{
	mEngineVsync = val;
}
bool AppImplMswBasic::getEngineVSync() 
{
	return mEngineVsync;
}

void AppImplMswBasic::setDebugFlag( int val ) 
{
	mTestFlag = val;
}
void AppImplMswBasic::enableFrameRate()
{
	mFrameRateEnabled = true;
}
void AppImplMswBasic::disableFrameRate()
{
	mFrameRateEnabled = false;
}

bool AppImplMswBasic::isFrameRateEnabled() const
{
	return mFrameRateEnabled;
}

///////////////////////////////////////////////////////////////////////////////
// WindowImplMswBasic
void WindowImplMswBasic::toggleFullScreen( const app::FullScreenOptions &options )
{
	// if we were full-screen, destroy our blanking windows
	if( mFullScreen )
		mAppImplBasic->destroyBlankingWindows();

	WindowImplMsw::toggleFullScreen( options );

	// if we've entered full-screen, setup our blanking windows if necessary
	if( options.isSecondaryDisplayBlankingEnabled() && mFullScreen )
		mAppImplBasic->setupBlankingWindows( getDisplay() );
}

} } // namespace cinder::app
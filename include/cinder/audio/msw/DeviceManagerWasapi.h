/*
 Copyright (c) 2014, The Cinder Project

 This code is intended to be used with the Cinder C++ library, http://libcinder.org

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

#include "cinder/Cinder.h"
#if defined( CINDER_UWP ) || ( _WIN32_WINNT >= _WIN32_WINNT_VISTA ) // requires Windows Vista+

#include "cinder/audio/Device.h"

#include <map>

struct IMMDevice;

namespace cinder { namespace audio { namespace msw {

class DeviceManagerWasapi : public DeviceManager {
  public:
	  DeviceManagerWasapi();
	~DeviceManagerWasapi();

	DeviceRef getDefaultOutput() override;
	DeviceRef getDefaultInput() override;

	const std::vector<DeviceRef>& getDevices() override;
	const void removeDevice(const char* devKey) override;

	std::string getName( const DeviceRef &device ) override;
	size_t getNumInputChannels( const DeviceRef &device ) override;
	size_t getNumOutputChannels( const DeviceRef &device ) override;
	size_t getSampleRate( const DeviceRef &device ) override;
	size_t getFramesPerBlock( const DeviceRef &device ) override;

	void setSampleRate( const DeviceRef &device, size_t sampleRate ) override;
	void setFramesPerBlock( const DeviceRef &device, size_t framesPerBlock ) override;

	std::shared_ptr<::IMMDevice> getIMMDevice( const DeviceRef &device );

  private:
	  // TODO: fix formatting
	  struct DeviceInfo {
		  std::string mKey;						//! mKey used by Device to get more info from manager
		  std::string mName;						//! friendly mName
		  enum Usage { INPUT, OUTPUT } mUsage;
		  std::wstring			mEndpointId;		//! id used by Wasapi / MMDevice
		  unsigned long			mState;
		  size_t mNumChannels, mSampleRate, mFramesPerBlock;
	  };

	  DeviceInfo& getDeviceInfo( const DeviceRef &device );
	  void rebuildDeviceInfoSet();
	  void rebuildDeviceInfoSetEx();
	  void parseDevices( DeviceInfo::Usage usage );
	  std::vector<std::wstring> parseDeviceIds( DeviceInfo::Usage usage );

	  // devices after being scanned
	  std::vector<DeviceInfo> mDeviceInfoList;
	  // deviceInfoSet + device
	  std::map<DeviceRef, DeviceInfo> mDeviceInfoMap;

	  struct Impl;
	  std::unique_ptr<Impl> mImpl;
};

} } } // namespace cinder::audio::msw

#endif // #if defined( CINDER_UWP ) || ( _WIN32_WINNT >= _WIN32_WINNT_VISTA ) // requires Windows Vista+
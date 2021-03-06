cmake_minimum_required(VERSION 2.8.12)
project(HiTGVoip)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
set (CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-O3 -fpic -finline-functions -ffast-math -fno-strict-aliasing")
set(CMAKE_C_FLAGS "-O3 -fexceptions -fpic -finline-functions -ffast-math")
set(CMAKE_THREAD_PREFER_PTHREAD ON)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
find_package(Opus REQUIRED)
find_package(OpenSSL REQUIRED)
include_directories(include)

if (THREADS_HAVE_PTHREAD_ARG)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
endif()

file(GLOB SOURCES_LIBTGVOIP 3dparts/libtgvoip/VoIPController.cpp 3dparts/libtgvoip/Buffers.cpp 3dparts/libtgvoip/CongestionControl.cpp 3dparts/libtgvoip/EchoCanceller.cpp 3dparts/libtgvoip/JitterBuffer.cpp 3dparts/libtgvoip/logging.cpp 3dparts/libtgvoip/MediaStreamItf.cpp 3dparts/libtgvoip/MessageThread.cpp 3dparts/libtgvoip/NetworkSocket.cpp 3dparts/libtgvoip/OpusDecoder.cpp 3dparts/libtgvoip/OpusEncoder.cpp 3dparts/libtgvoip/PacketReassembler.cpp 3dparts/libtgvoip/VoIPGroupController.cpp 3dparts/libtgvoip/VoIPServerConfig.cpp 3dparts/libtgvoip/audio/AudioInput.cpp 3dparts/libtgvoip/audio/AudioOutput.cpp 3dparts/libtgvoip/audio/AudioIOCallback.cpp 3dparts/libtgvoip/audio/AudioIO.cpp 3dparts/libtgvoip/audio/Resampler.cpp 3dparts/libtgvoip/os/posix/NetworkSocketPosix.cpp 3dparts/libtgvoip/webrtc_dsp/webrtc/base/checks.cc 3dparts/libtgvoip/webrtc_dsp/webrtc/base/stringutils.cc 3dparts/libtgvoip/webrtc_dsp/webrtc/common_audio/audio_util.cc 3dparts/libtgvoip/webrtc_dsp/webrtc/common_audio/channel_buffer.cc 3dparts/libtgvoip/webrtc_dsp/webrtc/common_audio/fft4g.c 3dparts/libtgvoip/webrtc_dsp/webrtc/common_audio/ring_buffer.c 3dparts/libtgvoip/webrtc_dsp/webrtc/common_audio/signal_processing/auto_corr_to_refl_coef.c 3dparts/libtgvoip/webrtc_dsp/webrtc/common_audio/signal_processing/auto_correlation.c 3dparts/libtgvoip/webrtc_dsp/webrtc/common_audio/signal_processing/complex_bit_reverse.c 3dparts/libtgvoip/webrtc_dsp/webrtc/common_audio/signal_processing/complex_fft.c 3dparts/libtgvoip/webrtc_dsp/webrtc/common_audio/signal_processing/copy_set_operations.c 3dparts/libtgvoip/webrtc_dsp/webrtc/common_audio/signal_processing/cross_correlation.c 3dparts/libtgvoip/webrtc_dsp/webrtc/common_audio/signal_processing/division_operations.c 3dparts/libtgvoip/webrtc_dsp/webrtc/common_audio/signal_processing/dot_product_with_scale.c 3dparts/libtgvoip/webrtc_dsp/webrtc/common_audio/signal_processing/downsample_fast.c 3dparts/libtgvoip/webrtc_dsp/webrtc/common_audio/signal_processing/energy.c 3dparts/libtgvoip/webrtc_dsp/webrtc/common_audio/signal_processing/filter_ar.c 3dparts/libtgvoip/webrtc_dsp/webrtc/common_audio/signal_processing/filter_ar_fast_q12.c 3dparts/libtgvoip/webrtc_dsp/webrtc/common_audio/signal_processing/filter_ma_fast_q12.c 3dparts/libtgvoip/webrtc_dsp/webrtc/common_audio/signal_processing/get_hanning_window.c 3dparts/libtgvoip/webrtc_dsp/webrtc/common_audio/signal_processing/get_scaling_square.c 3dparts/libtgvoip/webrtc_dsp/webrtc/common_audio/signal_processing/ilbc_specific_functions.c 3dparts/libtgvoip/webrtc_dsp/webrtc/common_audio/signal_processing/levinson_durbin.c 3dparts/libtgvoip/webrtc_dsp/webrtc/common_audio/signal_processing/lpc_to_refl_coef.c 3dparts/libtgvoip/webrtc_dsp/webrtc/common_audio/signal_processing/min_max_operations.c 3dparts/libtgvoip/webrtc_dsp/webrtc/common_audio/signal_processing/randomization_functions.c 3dparts/libtgvoip/webrtc_dsp/webrtc/common_audio/signal_processing/real_fft.c 3dparts/libtgvoip/webrtc_dsp/webrtc/common_audio/signal_processing/refl_coef_to_lpc.c 3dparts/libtgvoip/webrtc_dsp/webrtc/common_audio/signal_processing/resample.c 3dparts/libtgvoip/webrtc_dsp/webrtc/common_audio/signal_processing/resample_48khz.c 3dparts/libtgvoip/webrtc_dsp/webrtc/common_audio/signal_processing/resample_by_2.c 3dparts/libtgvoip/webrtc_dsp/webrtc/common_audio/signal_processing/resample_by_2_internal.c 3dparts/libtgvoip/webrtc_dsp/webrtc/common_audio/signal_processing/resample_fractional.c 3dparts/libtgvoip/webrtc_dsp/webrtc/common_audio/signal_processing/spl_init.c 3dparts/libtgvoip/webrtc_dsp/webrtc/common_audio/signal_processing/spl_inl.c 3dparts/libtgvoip/webrtc_dsp/webrtc/common_audio/signal_processing/spl_sqrt.c 3dparts/libtgvoip/webrtc_dsp/webrtc/common_audio/signal_processing/spl_sqrt_floor.c 3dparts/libtgvoip/webrtc_dsp/webrtc/common_audio/signal_processing/splitting_filter_impl.c 3dparts/libtgvoip/webrtc_dsp/webrtc/common_audio/signal_processing/sqrt_of_one_minus_x_squared.c 3dparts/libtgvoip/webrtc_dsp/webrtc/common_audio/signal_processing/vector_scaling_operations.c 3dparts/libtgvoip/webrtc_dsp/webrtc/common_audio/sparse_fir_filter.cc 3dparts/libtgvoip/webrtc_dsp/webrtc/common_audio/wav_file.cc 3dparts/libtgvoip/webrtc_dsp/webrtc/common_audio/wav_header.cc 3dparts/libtgvoip/webrtc_dsp/webrtc/modules/audio_processing/aec/aec_core.cc 3dparts/libtgvoip/webrtc_dsp/webrtc/modules/audio_processing/aec/aec_core_sse2.cc 3dparts/libtgvoip/webrtc_dsp/webrtc/modules/audio_processing/aec/aec_resampler.cc 3dparts/libtgvoip/webrtc_dsp/webrtc/modules/audio_processing/aec/echo_cancellation.cc 3dparts/libtgvoip/webrtc_dsp/webrtc/modules/audio_processing/aecm/aecm_core.cc 3dparts/libtgvoip/webrtc_dsp/webrtc/modules/audio_processing/aecm/aecm_core_c.cc 3dparts/libtgvoip/webrtc_dsp/webrtc/modules/audio_processing/aecm/echo_control_mobile.cc 3dparts/libtgvoip/webrtc_dsp/webrtc/modules/audio_processing/agc/legacy/analog_agc.c 3dparts/libtgvoip/webrtc_dsp/webrtc/modules/audio_processing/agc/legacy/digital_agc.c 3dparts/libtgvoip/webrtc_dsp/webrtc/modules/audio_processing/logging/apm_data_dumper.cc 3dparts/libtgvoip/webrtc_dsp/webrtc/modules/audio_processing/ns/noise_suppression.c 3dparts/libtgvoip/webrtc_dsp/webrtc/modules/audio_processing/ns/noise_suppression_x.c 3dparts/libtgvoip/webrtc_dsp/webrtc/modules/audio_processing/ns/ns_core.c 3dparts/libtgvoip/webrtc_dsp/webrtc/modules/audio_processing/ns/nsx_core.c 3dparts/libtgvoip/webrtc_dsp/webrtc/modules/audio_processing/ns/nsx_core_c.c 3dparts/libtgvoip/webrtc_dsp/webrtc/modules/audio_processing/splitting_filter.cc 3dparts/libtgvoip/webrtc_dsp/webrtc/modules/audio_processing/three_band_filter_bank.cc 3dparts/libtgvoip/webrtc_dsp/webrtc/modules/audio_processing/utility/block_mean_calculator.cc 3dparts/libtgvoip/webrtc_dsp/webrtc/modules/audio_processing/utility/delay_estimator.cc 3dparts/libtgvoip/webrtc_dsp/webrtc/modules/audio_processing/utility/delay_estimator_wrapper.cc 3dparts/libtgvoip/webrtc_dsp/webrtc/modules/audio_processing/utility/ooura_fft.cc 3dparts/libtgvoip/webrtc_dsp/webrtc/modules/audio_processing/utility/ooura_fft_sse2.cc 3dparts/libtgvoip/webrtc_dsp/webrtc/system_wrappers/source/cpu_features.cc)
add_library(tgvoip STATIC ${SOURCES_LIBTGVOIP})
target_include_directories(tgvoip PUBLIC 3dparts/libtgvoip/webrtc_dsp)
target_include_directories(tgvoip PUBLIC ${OPUS_INCLUDE_DIRS})
target_compile_definitions(tgvoip PUBLIC TGVOIP_USE_CALLBACK_AUDIO_IO WEBRTC_POSIX TGVOIP_USE_DESKTOP_DSP WEBRTC_APM_DEBUG_DUMP=0 USE_KISS_FFT FIXED_POINT TGVOIP_USE_CXX11_LIB TGVOIP_OTHER)

file(GLOB SOURCES "lib/*.cpp")
file(GLOB HEADERS "include/*.h")

add_library(${PROJECT_NAME} SHARED ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC 3dparts)	
target_link_libraries(${PROJECT_NAME} tgvoip ${OPUS_LIBRARY} Threads::Threads OpenSSL::Crypto OpenSSL::SSL)

install(TARGETS tgvoip
        ARCHIVE DESTINATION lib
)

install(TARGETS ${PROJECT_NAME}
        LIBRARY DESTINATION lib
)

INSTALL(FILES ${HEADERS}
        DESTINATION include
)

#pragma once
#include <chrono>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <cstdint>
#include <iomanip>
#include <sstream>
#include <cstdio>

#include "utils/ini.h"
#include "utils/functions.h"

struct ProfileStat {
	uint64_t totalDurationUs = 0;
	uint64_t peakDurationUs = 0;
	uint32_t callCount = 0;
};

struct NamedProfileStat {
	std::string name;
	uint64_t totalDurationUs = 0;
	uint64_t peakDurationUs = 0;
	uint32_t callCount = 0;
	double avgUsPerFrame = 0.0;
	double avgUsPerCall = 0.0;
};

class Profiler {
private:
	inline static std::unordered_map<std::string, ProfileStat> s_stats;
	inline static uint32_t s_frameCount = 0;
	inline static uint64_t s_frameTotalTimeUs = 0;
	inline static std::chrono::high_resolution_clock::time_point s_frameStartTime;
	inline static bool s_frameStarted = false;
	static constexpr uint32_t REPORT_INTERVAL_FRAMES = 300;

public:
	static void BeginFrame() {
		if (!Ini::EnableDebugProfiler)
			return;

		s_frameStartTime = std::chrono::high_resolution_clock::now();
		s_frameStarted = true;
	}

	static void Record(const std::string& name, uint64_t durationUs) {
		if (!Ini::EnableDebugProfiler)
			return;

		auto& stat = s_stats[name];
		stat.totalDurationUs += durationUs;
		stat.callCount++;
		if (durationUs > stat.peakDurationUs) {
			stat.peakDurationUs = durationUs;
		}
	}

	static void Reset() {
		s_stats.clear();
		s_frameCount = 0;
		s_frameTotalTimeUs = 0;
		s_frameStarted = false;
	}

	static void EndFrame() {
		if (!Ini::EnableDebugProfiler)
			return;

		if (s_frameStarted) {
			const auto frameEnd = std::chrono::high_resolution_clock::now();
			const uint64_t frameDuration = std::chrono::duration_cast<std::chrono::microseconds>(frameEnd - s_frameStartTime).count();
			s_frameTotalTimeUs += frameDuration;
			s_frameStarted = false;
		}

		s_frameCount++;
		if (s_frameCount >= REPORT_INTERVAL_FRAMES) {
			DumpReport();
			s_stats.clear();
			s_frameCount = 0;
			s_frameTotalTimeUs = 0;
		}
	}

	static void DumpReport() {
		if (s_frameCount == 0)
			return;

		std::vector<NamedProfileStat> sorted;
		sorted.reserve(s_stats.size());

		for (const auto& [name, stat] : s_stats) {
			NamedProfileStat item;
			item.name = name;
			item.totalDurationUs = stat.totalDurationUs;
			item.peakDurationUs = stat.peakDurationUs;
			item.callCount = stat.callCount;
			item.avgUsPerFrame = static_cast<double>(stat.totalDurationUs) / s_frameCount;
			item.avgUsPerCall = (stat.callCount > 0) ? (static_cast<double>(stat.totalDurationUs) / stat.callCount) : 0.0;
			sorted.push_back(item);
		}

		std::sort(sorted.begin(), sorted.end(), [](const NamedProfileStat& a, const NamedProfileStat& b) {
			return a.totalDurationUs > b.totalDurationUs;
		});

		const double avgTotalFrameUs = static_cast<double>(s_frameTotalTimeUs) / s_frameCount;
		const double avgTotalFrameMs = avgTotalFrameUs / 1000.0;

		RawLog("Profiler", "==========================================================================================");
		RawLog("Profiler", "[Performance Report - Sampled Over " + std::to_string(s_frameCount) + " Frames]");
		{
			std::ostringstream ss;
			ss << "Total Script Time: " << std::fixed << std::setprecision(2) << (s_frameTotalTimeUs / 1000.0)
			   << " ms | Script Frame Avg: " << std::fixed << std::setprecision(1) << avgTotalFrameUs
			   << " us (" << std::setprecision(2) << avgTotalFrameMs << " ms)";
			RawLog("Profiler", ss.str());
		}
		RawLog("Profiler", "Rank | Function / Option Name         | Avg/Frame   | Peak       | Calls/Frame | Avg/Call");
		RawLog("Profiler", "-----+--------------------------------+-------------+------------+-------------+----------");

		int rank = 1;
		for (const auto& item : sorted) {
			std::ostringstream ss;
			const double callsPerFrame = static_cast<double>(item.callCount) / s_frameCount;
			ss << std::setw(4) << rank++ << " | "
			   << std::left << std::setw(30) << (item.name.length() > 30 ? item.name.substr(0, 27) + "..." : item.name) << " | "
			   << std::right << std::setw(8) << std::fixed << std::setprecision(1) << item.avgUsPerFrame << " us | "
			   << std::setw(7) << item.peakDurationUs << " us | "
			   << std::setw(11) << std::fixed << std::setprecision(1) << callsPerFrame << " | "
			   << std::setw(6) << std::fixed << std::setprecision(1) << item.avgUsPerCall << " us";
			RawLog("Profiler", ss.str());
		}
		RawLog("Profiler", "==========================================================================================");

		if (!sorted.empty()) {
			std::string topName = sorted[0].name;
			if (topName.length() > 22)
				topName = topName.substr(0, 19) + "...";

			char notifyBuf[128];
			snprintf(notifyBuf, sizeof(notifyBuf),
				"GameplayFixesV Profiler:~n~Top: %s (%.0fus/fr)~n~Script Avg: %.2fms",
				topName.c_str(), sorted[0].avgUsPerFrame, avgTotalFrameMs);
			ShowNotification(notifyBuf);
		}
	}
};

class ScopedTimer {
private:
	const char* m_name;
	std::chrono::high_resolution_clock::time_point m_start;

public:
	explicit ScopedTimer(const char* name)
		: m_name(name) {
		if (Ini::EnableDebugProfiler) {
			m_start = std::chrono::high_resolution_clock::now();
		}
	}

	~ScopedTimer() {
		if (Ini::EnableDebugProfiler) {
			const auto end = std::chrono::high_resolution_clock::now();
			const uint64_t duration = std::chrono::duration_cast<std::chrono::microseconds>(end - m_start).count();
			Profiler::Record(m_name, duration);
		}
	}
};

#define PROFILE_SCOPE_CONCAT(a, b) a##b
#define PROFILE_SCOPE_LABEL(a, b) PROFILE_SCOPE_CONCAT(a, b)
#define PROFILE_SCOPE(name) ScopedTimer PROFILE_SCOPE_LABEL(_scoped_prof_, __LINE__)(name)
